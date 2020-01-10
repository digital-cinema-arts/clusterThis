/* ******************************************************************************
*
*  VRAY_clusterThisPostProcess
*
*
* Description :
*
*
***************************************************************************** */

#ifndef __VRAY_clusterThisPostProcess_cpp__
#define __VRAY_clusterThisPostProcess_cpp__



/* ******************************************************************************
*  Function Name : postNNProcess()
*
*  Description :   nearest neighbor post process the geo
*
*  Input Arguments : GU_Detail * gdp, GU_Detail * inst_gdp, GU_Detail * mb_gdp
*
*  Return Value : None
*
***************************************************************************** */

void VRAY_clusterThis::postNNProcess(GU_Detail * gdp, GU_Detail * inst_gdp, GU_Detail * mb_gdp)

{
   long int stat_interval = (long int)(myNumSourcePoints * 0.10) + 1;

   GEO_Point * src_ppt, * inst_ppt;
   GEO_PointPtrArray src_list;
   UT_Vector3 tmp_v, new_v;
   fpreal dist;
   fpreal inst_radius;
   uint32 num_passes;
   long int pt_counter = 0;
   UT_Vector3 v;

   if(myVerbose > CLUSTER_MSG_INFO)
      cout << "VRAY_clusterThis::postNNProcess() Performing nearest neighbor processing " << std::endl;

   GU_Detail * temp_gdp = inst_gdp;

   if(myDoMotionBlur == CLUSTER_MB_DEFORMATION)
      num_passes = 2;
   else
      num_passes = 1;

   while(num_passes) {
         if((myDoMotionBlur == CLUSTER_MB_DEFORMATION) && num_passes == 1)
            temp_gdp = mb_gdp;
         num_passes--;

         if(myVerbose == CLUSTER_MSG_DEBUG)
            cout << "VRAY_clusterThis::postNNProcess() num_passes: " << num_passes << std::endl;

         pt_counter = 0;

         GA_FOR_ALL_GPOINTS(temp_gdp, inst_ppt) {

            UT_Vector3 inst_pos = inst_ppt->getPos();

            if(myDoMotionBlur != CLUSTER_MB_DEFORMATION) {
                  inst_radius = static_cast<fpreal>(inst_ppt->getValue<fpreal>(myInstAttrRefs.pointRadius, 0));
                  UT_Vector3 inst_v = static_cast<UT_Vector3>(inst_ppt->getValue<UT_Vector3>(myInstAttrRefs.pointV, 0));
               }
            else {
                  inst_radius = static_cast<fpreal>(inst_ppt->getValue<fpreal>(myInstMBAttrRefs.pointRadius, 0));
                  UT_Vector3 inst_v = static_cast<UT_Vector3>(inst_ppt->getValue<UT_Vector3>(myInstMBAttrRefs.pointV, 0));
               }
            int num_src_pts_found = mySRCPointTree.findAllClosePt(inst_pos, inst_radius, src_list);
//      cout << "VRAY_clusterThis::postNNProcess() num_src_pts_found: " << num_src_pts_found << " inst_radius: " << inst_radius << std::endl;

            new_v = 0.0;
            if(num_src_pts_found > 0) {

                  for(uint i = 0; i < src_list.entries(); i++) {
                        src_ppt = src_list(i);
                        tmp_v = static_cast<UT_Vector3>(src_ppt->getValue<UT_Vector3>(myPointAttrRefs.v, 0));
                        dist = distance2(inst_pos, static_cast<UT_Vector3>(src_ppt->getPos()));
//                     dist = distance3d(inst_pos, static_cast<UT_Vector3>(src_ppt->getPos()));
//                     new_v = new_v + (tmp_v * (1 + (inst_radius - dist)));
//                     new_v = new_v + (tmp_v * (1 + SYSsqrt((inst_radius * inst_radius) - dist)));
                        new_v = new_v + (tmp_v * (1 + (inst_radius * inst_radius) - dist));
//                     new_v = new_v + (tmp_v * (1 + inst_radius - dist));
//                     cout << "VRAY_clusterThis::postNNProcess() new_v: " << new_v << std::endl;
                     }

                  v = static_cast<UT_Vector3>((new_v / static_cast<float>(num_src_pts_found)));
//            v = static_cast<const UT_Vector3>(((new_v / myFPS) / static_cast<float>(num_src_pts_found)));

                  inst_ppt->setValue<UT_Vector3>(myInstAttrRefs.pointV,
                                                 static_cast<const UT_Vector3>(v * myNNPostVelInfluence));

                  inst_ppt->setPos((inst_pos + static_cast<const UT_Vector3>(v * myNNPostPosInfluence)));
               }

            if(myVerbose > CLUSTER_MSG_INFO) {
                  pt_counter++;
                  if((long int)(pt_counter % (stat_interval * myNumCopies * myRecursion)) == 0) {
                        std::cout << "VRAY_clusterThis::postNNProcess() Number of points post processed: "
                                  << pt_counter << std::endl;
                     }
               }

         }
      }

}

//class FOO
//{
//public:
//    THREADED_METHOD2(                   // Construct two parameter threaded method
//                    FOO,                // Name of class
//                    myLength > 100,     // Evaluated to see if we should multithread.
//                    bar,                // Name of function
//                    int, p1,            // An integer parameter named p1
//                    float, p2)          // A float parameter named p2
//    void barPartial(int p1, float p2, const UT_JobInfo &info);
//
//    int myLength;
//    int *myData;
//};
//
//void
//FOO::barPartial(int p1, float p2, const UT_JobInfo &info)
//{
//    int         i, n;
//    for (info.divideWork(myLength, i, n); i < n; i++)
//    {
//        myData[i] += p1 * p2;
//    }
//}
//





/* ******************************************************************************
*  Function Name : postProcess()
*
*  Description :   post process the geo
*
*  Input Arguments : GU_Detail * gdp, GU_Detail * inst_gdp, GU_Detail * mb_gdp
*
*  Return Value : None
*
***************************************************************************** */

void VRAY_clusterThis::postProcess(GU_Detail * gdp, GU_Detail * inst_gdp, GU_Detail * mb_gdp)

{
   myPostProcTime = std::clock();
   std::time(&myPostProcStartTime);

   long int stat_interval = (long int)(myNumSourcePoints * 0.10) + 1;

   GEO_PointPtrArray src_list;
   UT_Vector3 tmp_v, new_v;
   uint32 num_passes = 0;
   UT_Vector3 inst_pos, seed_pos, currVel;
   long int pt_counter = 0;
   long int pointsFound = 0;

   // Perform "nearest neighbor" post processing
   if(myNNPostProcess) {
         postNNProcess(gdp, inst_gdp, mb_gdp);
      }


   // Perform Open VDB post processing
   if(myVDBPostProcess) {

         GU_Detail * temp_gdp = inst_gdp;

         if(myDoMotionBlur == CLUSTER_MB_DEFORMATION)
            num_passes = 2;
         else
            num_passes = 1;


         if(myVerbose > CLUSTER_MSG_INFO)
            cout << "VRAY_clusterThis::postProcess() Processing Voxels" << std::endl;
//            VRAYwarning("%s VRAY_clusterThis::postProcess() Processing Voxels", getClassName());


         while(num_passes) {
               if((myDoMotionBlur == CLUSTER_MB_DEFORMATION) && num_passes == 1)
                  temp_gdp = mb_gdp;
               num_passes--;

               if(myVerbose == CLUSTER_MSG_DEBUG)
                  cout << "VRAY_clusterThis::postProcess() num_passes: " << num_passes << std::endl;

               GEO_AttributeHandle inst_vel_gah = temp_gdp->getPointAttribute("v");
               GEO_AttributeHandle source_vel_gah = gdp->getPointAttribute("v");
               GEO_AttributeHandle inst_N_gah = temp_gdp->getPointAttribute("N");
               GEO_AttributeHandle source_N_gah = gdp->getPointAttribute("N");
               GEO_AttributeHandle inst_Cd_gah = temp_gdp->getPointAttribute("Cd");
               GEO_AttributeHandle source_Cd_gah = gdp->getPointAttribute("Cd");
               GEO_AttributeHandle inst_Alpha_gah = temp_gdp->getPointAttribute("Alpha");
               GEO_AttributeHandle source_Alpha_gah = gdp->getPointAttribute("Alpha");

               if(!inst_vel_gah.isAttributeValid())
                  throw VRAY_clusterThis_Exception("VRAY_clusterThis::postProcess() Instance velocity handle invalid, exiting ...", 1);
               if(!source_vel_gah.isAttributeValid())
                  throw VRAY_clusterThis_Exception("VRAY_clusterThis::postProcess() Source velocity handle invalid, exiting ...", 1);
               if(!inst_N_gah.isAttributeValid())
                  throw VRAY_clusterThis_Exception("VRAY_clusterThis::postProcess() Instance normal handle invalid, exiting ...", 1);
               if(!source_N_gah.isAttributeValid())
                  throw VRAY_clusterThis_Exception("VRAY_clusterThis::postProcess() Source normal handle invalid, exiting ...", 1);
               if(!inst_Cd_gah.isAttributeValid())
                  throw VRAY_clusterThis_Exception("VRAY_clusterThis::postProcess() Instance color handle invalid, exiting ...", 1);
               if(!source_Cd_gah.isAttributeValid())
                  throw VRAY_clusterThis_Exception("VRAY_clusterThis::postProcess() Source color handle invalid, exiting ...", 1);
               if(!inst_Alpha_gah.isAttributeValid())
                  throw VRAY_clusterThis_Exception("VRAY_clusterThis::postProcess() Instance alpha handle invalid, exiting ...", 1);
               if(!source_Alpha_gah.isAttributeValid())
                  throw VRAY_clusterThis_Exception("VRAY_clusterThis::postProcess() Source alpha handle invalid, exiting ...", 1);

               openvdb::FloatTree::ValueType sampleResult;
               openvdb::VectorGrid::ValueType gradResult;

//         openvdb::tools::PointSampler mySampler;
               openvdb::tools::PointSampler gradSampler;
               // Get the reference to the gradient grid's tree to be used in sampling
               openvdb::VectorTree & myGradTree = mySourceGradientGrid->treeRW();

               GEO_Point * ppt;

               if(myVerbose > CLUSTER_MSG_INFO)
                  std::cout << "VRAY_clusterThis::postProcess() - Sampling instanced geometry ... " << std::endl;

               pt_counter = 0;

               GA_FOR_ALL_GPOINTS(temp_gdp, ppt) {
//            int     myCurrPtOff = ppt->getMapOffset();
//                              std::cout << "myCurrPtOff: " << myCurrPtOff << std::endl;
//   for(uint32 i = temp_gdp->points().entries(); i-- > 0;) {
//         GEO_Point * ppt = temp_gdp->points()(i);

                  inst_pos = ppt->getPos();

                  openvdb::Vec3R theIndex =
                     mySourceGradientGrid->worldToIndex(openvdb::Vec3R(inst_pos[0], inst_pos[1], inst_pos[2]));
//                  openvdb::Vec3R theIndex =
//                     outputGrid->worldToIndex(openvdb::Vec3R(inst_pos[0], inst_pos[1], inst_pos[2]));

                  const openvdb::Vec3R  inst_sample_pos(theIndex[0], theIndex[1], theIndex[2]);

//                  bool success = mySampler.sample(myTree, inst_sample_pos, sampleResult);
                  bool grad_success = gradSampler.sample(myGradTree, inst_sample_pos, gradResult);

                  // if the instanced point is within the vdb volume
//            if(success) {
//                  if(success && grad_success) {
                  if(grad_success) {
                        pointsFound++;

                        inst_vel_gah.setElement(ppt);
                        currVel = inst_vel_gah.getV3();

                        // Set the position of the instanced point based on the gradient to determine direction,
                        // and the distance from the sampled point, then scaled by the user's amount of influence.
                        UT_Vector3 gradVect = UT_Vector3(gradResult[0], gradResult[1], gradResult[2]);

                        sampleResult = gradVect.length();

                        ppt->setPos(inst_pos + (myPostPosInfluence *(sampleResult * gradVect)));
//                       ppt->setPos(inst_pos + (sampleResult * myPosInfluence *(currVel / myFPS)));

                        // Set the velocity of the instanced point based on the gradient to determine direction,
                        // and the distance from the sampled point, then scaled by the user's amount of influence.

//                       inst_vel_gah.setV3(currVel * ((1 / sampleResult) * radius));
                        inst_vel_gah.setV3(currVel + (myPostVelInfluence *(sampleResult * gradVect)));

//                        std::cout << "inst_pos: " << inst_pos << " inst_sample_pos: "
//                                  << inst_sample_pos << " sampleResult: " << sampleResult
//                                  << " gradResult: " << gradResult << " currVel: " << currVel
//                                  << " new vel: " <<  currVel + (myPostVelInfluence *(sampleResult * gradVect)) << std::endl;

                        inst_N_gah.setV3(inst_N_gah.getV3() + (myPostNormalInfluence *(sampleResult * gradVect)));

//                        inst_Cd_gah.setElement(ppt);
//                        inst_Cd_gah.setV3(inst_Cd_gah.getV3() * abs(sampleResult));
//
//                        inst_Alpha_gah.setElement(ppt);
//                        inst_Alpha_gah.setF(inst_Alpha_gah.getF() * abs(sampleResult));

                     } // if the instanced point is within the vdb volume


                  if(myVerbose > CLUSTER_MSG_INFO) {
                        pt_counter++;
                        if((long int)(pt_counter % (stat_interval * myNumCopies * myRecursion)) == 0) {
                              std::cout << "VRAY_clusterThis::postProcess() Number of points post processed: " << pt_counter
                                        << "\t - Number of points found in vdb grid: " << pointsFound << std::endl;
                           }
                     }

               }



            }




         if(myVerbose > CLUSTER_MSG_INFO) {
               if(!pointsFound)
                  cout << "VRAY_clusterThis::postProcess() NO POINTS POST PROCESSED!!! " << std::endl;
               else
                  cout << "VRAY_clusterThis::postProcess() Average instanced points post processed: "
                       << float((float(pointsFound) / float(pt_counter) * 100.0f)) << "%" << std::endl;
            }


//         if(myPostVDBWriteVDBFiles) {
//               if(myVerbose > CLUSTER_MSG_INFO)
//                  std::cout << "VRAY_clusterThis::postProcess() - Writing grids to disk ... " << std::endl;
////               openvdb::GridPtrVec scalargrids;
//               openvdb::GridPtrVec gradgrids;
//
//               openvdb::io::File gradientFile(static_cast<char *>(myVDBBaseFileName));
//               gradgrids.push_back(mySourceGradientGrid);
//               gradientFile.write(gradgrids);
//               gradientFile.close();
//               if(myVerbose > CLUSTER_MSG_INFO)
//                  std::cout << "VRAY_clusterThis::postProcess() - Finished writing grids to disk ... " << std::endl;
//            }



         std::time(&myPostProcEndTime);
         myPostProcExecTime = std::clock() - myPostProcTime;

      } // if(myVDBPostProcess)


}


#endif











