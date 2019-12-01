/* ******************************************************************************
*
*  VRAY_clusterThisUtil
*
*
* Description :
*
*
***************************************************************************** */


#ifndef __VRAY_clusterThisUtil_cpp__
#define __VRAY_clusterThisUtil_cpp__



/* ******************************************************************************
*  Function Name : getBoundingBox
*
*  Description :  Get the bounding box for this VRAY_clusterThis object
*
*  Input Arguments : None
*
*  Return Value : None
*
***************************************************************************** */
void VRAY_clusterThis::getBoundingBox(UT_BoundingBox & box)
{

   box = myVelBox;

#ifdef DEBUG
   std::cout << "VRAY_clusterThis::getBoundingBox() box: " << box << std::endl;
#endif

}


/* ******************************************************************************
*  Function Name : checkRequiredAttributes
*
*  Description :  Check that all the required attributes are in the point cloud
*
*  Input Arguments : None
*
*  Return Value : None
*
***************************************************************************** */
void VRAY_clusterThis::checkRequiredAttributes()
{

   // TODO: Confirm that all required attrs are being checked for each instance type

//   std::cout << "VRAY_clusterThis::checkRequiredAttributes()" << std::endl;
   // Check for required attributes
   if(myPointAttrRefs.Cd.isInvalid()) {
         cout << "Incoming points must have Cd attribute! Throwing exception ..." << endl;
         throw VRAY_clusterThis_Exception("VRAY_clusterThis::checkRequiredAttributes() Incoming points must have Cd attribute! ", 1);
      }

   if(myPointAttrRefs.Alpha.isInvalid()) {
         cout << "Incoming points must have Alpha attribute! Throwing exception ..." << endl;
         throw VRAY_clusterThis_Exception("VRAY_clusterThis::checkRequiredAttributes() Incoming points must have Alpha attribute! ", 1);
      }

   if(myPointAttrRefs.v.isInvalid()) {
         cout << "Incoming points must have v attribute! Throwing exception ..." << endl;
         throw VRAY_clusterThis_Exception("VRAY_clusterThis::checkRequiredAttributes() Incoming points must have v attribute! ", 1);
      }

   if(myPointAttrRefs.N.isInvalid()) {
         cout << "Incoming points must have N attribute! Throwing exception ..." << endl;
         throw VRAY_clusterThis_Exception("VRAY_clusterThis::checkRequiredAttributes() Incoming points must have N attribute! ", 1);
      }

   if(myPointAttrRefs.pscale.isInvalid()) {
         cout << "Incoming points must have pscale attribute! Throwing exception ..." << endl;
         throw VRAY_clusterThis_Exception("VRAY_clusterThis::checkRequiredAttributes() Incoming points must have pscale attribute! ", 1);
      }

   if(myPointAttrRefs.id.isInvalid()) {
         cout << "Incoming points must have id attribute Throwing exception ..." << endl;
         throw VRAY_clusterThis_Exception("VRAY_clusterThis::checkRequiredAttributes() Incoming points must have id attribute! ", 1);
      }

   // Check for weight attribute if the user wants metaballs
   if((myPrimType == CLUSTER_PRIM_METABALL) && (myPointAttrRefs.weight.isInvalid())) {
         cout << "Incoming points must have weight attribute if instancing metaballs! Throwing exception ..." << std::endl;
         throw VRAY_clusterThis_Exception("VRAY_clusterThis::checkRequiredAttributes() Incoming points must have weight attribute if instancing metaballs!", 1);
      }


}



/* ******************************************************************************
*  Function Name : convert()
*
*  Description : convert point cloud to VDB level set or fog volume
*
*  Input Arguments : None
*
*  Return Value :
*
* ***************************************************************************** */
void VRAY_clusterThis::convert(
   openvdb::ScalarGrid::Ptr outputGrid,
   ParticleList & paList,
   const Settings & settings,
   hvdb::Interrupter & boss)
{

   openvdb::tools::ParticlesToLevelSet<openvdb::ScalarGrid, ParticleList, hvdb::Interrupter> raster(*outputGrid, boss);

   if(myVerbose == CLUSTER_MSG_DEBUG)
      std::cout << "VRAY_clusterThis::convert() " << std::endl;

   raster.setRmin(settings.mRadiusMin);

   if(myVerbose == CLUSTER_MSG_DEBUG) {
         std::cout << "VRAY_clusterThis::convert(): raster.getVoxelSize(): " << raster.getVoxelSize() << std::endl;
         std::cout << "VRAY_clusterThis::convert(): raster.getRmin(): " << raster.getRmin() << std::endl;
         std::cout << "VRAY_clusterThis::convert(): raster.getHalfWidth(): " << raster.getHalfWidth() << std::endl;
      }

   if(raster.getHalfWidth() < openvdb::Real(2)) {
         std::cout << "VRAY_clusterThis::convert(): Half width of narrow-band < 2 voxels which creates holes when meshed!" << std::endl;
      }
   else
      if(raster.getHalfWidth() > openvdb::Real(1000)) {
            throw std::runtime_error(
               "VRAY_clusterThis::convert(): Half width of narrow-band > 1000 voxels which exceeds memory limitations!");
         }

   if(settings.mRasterizeTrails && paList.hasVelocity()) {
         if(myVerbose == CLUSTER_MSG_DEBUG)
            std::cout << "VRAY_clusterThis::convert(): rasterizing trails"  << std::endl;
         raster.rasterizeTrails(paList, settings.mDx);
      }
   else {
         if(myVerbose == CLUSTER_MSG_DEBUG)
            std::cout << "VRAY_clusterThis::convert(): rasterizing spheres"  << std::endl;
         raster.rasterizeSpheres(paList);
      }

   if(boss.wasInterrupted()) {
         if(myVerbose == CLUSTER_MSG_DEBUG)
            std::cout << "VRAY_clusterThis::convert(): Process was interrupted"  << std::endl;
         return;
      }

   // Convert the level-set into a fog volume.
   if(settings.mFogVolume) {
         if(myVerbose == CLUSTER_MSG_DEBUG)
            std::cout << "VRAY_clusterThis::convert(): converting to fog volume"  << std::endl;
         float cutOffDist = std::numeric_limits<float>::max();
         if(settings.mGradientWidth > 1e-6)
            cutOffDist = settings.mGradientWidth;
         openvdb::tools::levelSetToFogVolume(*outputGrid, cutOffDist, false);
      }

// print stats of the vdb grid
   if(myVerbose == CLUSTER_MSG_DEBUG)
      outputGrid->print();

}



/* ******************************************************************************
*  Function Name : convertVector()
*
*  Description : convert point cloud to VDB vector level set or fog volume
*
*  Input Arguments : None
*
*  Return Value :
*
* ***************************************************************************** */
void VRAY_clusterThis::convertVector(
   openvdb::VectorGrid::Ptr outputGrid,
   ParticleList & paList,
   const Settings & settings,
   hvdb::Interrupter & boss)
{

   openvdb::tools::ParticlesToLevelSet<openvdb::VectorGrid, ParticleList, hvdb::Interrupter> raster(*outputGrid, boss);

   if(myVerbose == CLUSTER_MSG_DEBUG)
      std::cout << "VRAY_clusterThis::convert() " << std::endl;

//   raster.setRmin(settings.mRadiusMin);

   if(myVerbose == CLUSTER_MSG_DEBUG) {
         std::cout << "VRAY_clusterThis::convert(): raster.getVoxelSize(): " << raster.getVoxelSize() << std::endl;
         std::cout << "VRAY_clusterThis::convert(): raster.getRmin(): " << raster.getRmin() << std::endl;
         std::cout << "VRAY_clusterThis::convert(): raster.getHalfWidth(): " << raster.getHalfWidth() << std::endl;
      }

//   if(raster.getHalfWidth() < openvdb::Real(2)) {
//         std::cout << "VRAY_clusterThis::convert(): Half width of narrow-band < 2 voxels which creates holes when meshed!" << std::endl;
//      }
//   else
//      if(raster.getHalfWidth() > openvdb::Real(1000)) {
//            throw std::runtime_error(
//               "VRAY_clusterThis::convert(): Half width of narrow-band > 1000 voxels which exceeds memory limitations!");
//         }

   if(settings.mRasterizeTrails && paList.hasVelocity()) {
         if(myVerbose == CLUSTER_MSG_DEBUG)
            std::cout << "VRAY_clusterThis::convert(): rasterizing trails"  << std::endl;
//         raster.rasterizeTrails(paList, settings.mDx);
      }
   else {
         if(myVerbose == CLUSTER_MSG_DEBUG)
            std::cout << "VRAY_clusterThis::convert(): rasterizing spheres"  << std::endl;
//         raster.rasterizeSpheres(paList);
      }

   if(boss.wasInterrupted()) {
         if(myVerbose == CLUSTER_MSG_DEBUG)
            std::cout << "VRAY_clusterThis::convert(): Process was interrupted"  << std::endl;
         return;
      }

   // Convert the level-set into a fog volume.
   if(settings.mFogVolume) {
         if(myVerbose == CLUSTER_MSG_DEBUG)
            std::cout << "VRAY_clusterThis::convert(): converting to fog volume"  << std::endl;
         float cutOffDist = std::numeric_limits<float>::max();
         if(settings.mGradientWidth > 1e-6)
            cutOffDist = settings.mGradientWidth;
//         openvdb::tools::levelSetToFogVolume(*outputGrid, cutOffDist, false);
      }

// print stats of the vdb grid
   if(myVerbose == CLUSTER_MSG_DEBUG)
      outputGrid->print();

}




int VRAY_clusterThis::convertVDBUnits()
{
//     const bool toWSUnits = static_cast<bool>(evalInt("worldSpaceUnits", 0, 0));
//
//     if (toWSUnits) {
//         setFloat("bandWidthWS", 0, 0, evalFloat("bandWidth", 0, 0) * mVoxelSize);
//         return 1;
//     }
//
//     setFloat("bandWidth", 0, 0, evalFloat("bandWidthWS", 0, 0) / mVoxelSize);
//
   return 1;
}




/* ******************************************************************************
*  Function Name : exitClusterThis()
*
*  Description :  Start the exit process
*
*  Input Arguments : void *data
*
*  Return Value :
*
***************************************************************************** */
void VRAY_clusterThis::exitClusterThis(void * data)
{
   VRAY_clusterThis * me = (VRAY_clusterThis *)data;

   if(me->myVerbose == CLUSTER_MSG_DEBUG)
      std::cout << std::endl << std::endl << "VRAY_clusterThis::exitClusterThis() - Preparing to exit!" << std::endl;


   const char * fname = me->myTempFname;

   me->exitClusterThisReal(fname);
//         me->exitClusterThisReal(data);

}



/* ******************************************************************************
*  Function Name : exitClusterThisReal()
*
*  Description : Clean up temp file if used, save stats to file and DB
*
*  Input Arguments : None
*
*  Return Value :
*
* ***************************************************************************** */
//void VRAY_clusterThis::exitClusterThisReal(void * data)
void VRAY_clusterThis::exitClusterThisReal(const char * fname)
{
   struct stat fileResults;
//   VRAY_clusterThis * me = (VRAY_clusterThis *)data;

   if(myVerbose == CLUSTER_MSG_DEBUG)
      cout << "VRAY_clusterThis::exitClusterThisReal() - Running exit processing" << endl;



   if(mySaveXMLFile) {

         std::ostringstream oss(std::ostringstream::out);
         UT_XMLWriter xml_writer;

         xml_writer.beginWritingToFile(myXMLFileName);
         xml_writer.setIndentation(3);
         xml_writer.writeRawString("\n");

         xml_writer.startElement("render_stats");
         xml_writer.writeComment("*********** clusterThis render stats ***********");

         xml_writer.startElement("parameters");
         xml_writer.writeComment("*********** clusterThis parameters ***********");

         oss.str(std::string());
         oss << myPrimType;
         xml_writer.writeElement("myPrimType", oss.str().c_str());
         oss.str(std::string());
         oss << myUseGeoFile;
         xml_writer.writeElement("myUseGeoFile", oss.str().c_str());
         xml_writer.writeElement("mySrcGeoFname", mySrcGeoFname);
         oss.str(std::string());
         oss << myNumCopies;
         xml_writer.writeElement("myNumCopies", oss.str().c_str());
         oss.str(std::string());
         oss << myNoiseType;
         xml_writer.writeElement("myNoiseType", oss.str().c_str());
         oss.str(std::string());
         oss << myFreqX << " " << myFreqY << " " << myFreqZ;
         xml_writer.writeElement("myFreq", oss.str().c_str());
         oss.str(std::string());
         oss << myOffsetX << " " << myOffsetY << " " << myOffsetZ;
         xml_writer.writeElement("myOffset", oss.str().c_str());
         oss.str(std::string());
         oss << myRadius;
         xml_writer.writeElement("myRadius", oss.str().c_str());
         oss.str(std::string());
         oss << myRough;
         xml_writer.writeElement("myRough", oss.str().c_str());
         oss.str(std::string());
         oss << myBirthProb;
         xml_writer.writeElement("myBirthProb", oss.str().c_str());
         oss.str(std::string());
         oss << mySize[0] << " " <<  mySize[1] << " " <<  mySize[2];
         xml_writer.writeElement("mySize", oss.str().c_str());
         oss.str(std::string());
         oss << myDoMotionBlur;
         xml_writer.writeElement("myDoMotionBlur", oss.str().c_str());
         oss.str(std::string());
         oss << myCurrentTime;
         xml_writer.writeElement("myCurrentTime", oss.str().c_str());
         oss.str(std::string());
         oss << myCurrentFrame;
         xml_writer.writeElement("myCurrentFrame", oss.str().c_str());
         oss.str(std::string());
         oss << myShutter << " " << myShutter2;
         xml_writer.writeElement("myShutter", oss.str().c_str());
         oss.str(std::string());
         oss << myNoiseAmp;
         xml_writer.writeElement("myNoiseAmp", oss.str().c_str());
         oss.str(std::string());
         oss << myFilterType;
         xml_writer.writeElement("myFilterType", oss.str().c_str());
         oss.str(std::string());
         oss << myNoiseAtten;
         xml_writer.writeElement("myNoiseAtten", oss.str().c_str());
         oss.str(std::string());
         oss << myNoiseSeed;
         xml_writer.writeElement("myNoiseSeed", oss.str().c_str());
         oss.str(std::string());
         oss << myFractalDepth;
         xml_writer.writeElement("myFractalDepth", oss.str().c_str());
         oss.str(std::string());
         oss << myRecursion;
         xml_writer.writeElement("myRecursion", oss.str().c_str());
         oss.str(std::string());
         oss << myBBoxFudgeFactor;
         xml_writer.writeElement("myBBoxFudgeFactor", oss.str().c_str());
         oss.str(std::string());
         oss << myVerbose;
         xml_writer.writeElement("myVerbose", oss.str().c_str());
         oss.str(std::string());
         oss << myUseTempFile;
         xml_writer.writeElement("myUseTempFile", oss.str().c_str());
         oss.str(std::string());
         oss << mySaveTempFile;
         xml_writer.writeElement("mySaveTempFile", oss.str().c_str());
         oss.str(std::string());
         oss << myUsePointRadius;
         xml_writer.writeElement("myUsePointRadius", oss.str().c_str());
         oss.str(std::string());
         oss << myUseBacktrackMB;
         xml_writer.writeElement("myUseBacktrackMB", oss.str().c_str());
         oss.str(std::string());

         oss << myCVEX_Exec;
         xml_writer.writeElement("myCVEX_Exec", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEX_Exec_prim;
         xml_writer.writeElement("myCVEX_Exec_prim", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEX_Exec_pre;
         xml_writer.writeElement("myCVEX_Exec_pre", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEX_Exec_post;
         xml_writer.writeElement("myCVEX_Exec_post", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEXFname;
         xml_writer.writeElement("myCVEXFname", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEXFname_prim;
         xml_writer.writeElement("myCVEXFname_prim", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEXFname_pre;
         xml_writer.writeElement("myCVEXFname_pre", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEXFname_post;
         xml_writer.writeElement("myCVEXFname_post", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEXPointVars.cvex_Cd_pt;
         xml_writer.writeElement("myCVEXPointVars.cvex_Cd_pt", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEXPointVars.cvex_Alpha_pt;
         xml_writer.writeElement("myCVEXPointVars.cvex_Alpha_pt", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEXPointVars.cvex_v_pt;
         xml_writer.writeElement("myCVEXPointVars.cvex_v_pt", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEXPointVars.cvex_N_pt;
         xml_writer.writeElement("myCVEXPointVars.cvex_N_pt", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEXPointVars.cvex_pscale_pt;
         xml_writer.writeElement("myCVEXPointVars.cvex_pscale_pt", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEXPrimVars.cvex_Cd_prim;
         xml_writer.writeElement("myCVEXPrimVars.cvex_Cd_prim", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEXPrimVars.cvex_Alpha_prim;
         xml_writer.writeElement("myCVEXPrimVars.cvex_Alpha_prim", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEXPrimVars.cvex_N_prim;
         xml_writer.writeElement("myCVEXPrimVars.cvex_N_prim", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEXPrimVars.cvex_pscale_prim;
         xml_writer.writeElement("myCVEXPrimVars.cvex_pscale_prim", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEXPrimVars.cvex_weight_prim;
         xml_writer.writeElement("myCVEXPrimVars.cvex_weight_prim", oss.str().c_str());
         oss.str(std::string());
         oss << myCVEXPrimVars.cvex_width_prim;
         xml_writer.writeElement("myCVEXPrimVars.cvex_width_prim", oss.str().c_str());

         oss.str(std::string());
         oss << myPostProcess;
         xml_writer.writeElement("myPostProcess", oss.str().c_str());
         oss.str(std::string());
         oss << myNNPostProcess;
         xml_writer.writeElement("myNNPostProcess", oss.str().c_str());
         oss.str(std::string());
         oss << myNNPostPosInfluence;
         xml_writer.writeElement("myNNPostPosInfluence", oss.str().c_str());
         oss.str(std::string());
         oss << myNNPostVelInfluence;
         xml_writer.writeElement("myNNPostVelInfluence", oss.str().c_str());
         oss << myVDBPostProcess;
         xml_writer.writeElement("myVDBPostProcess", oss.str().c_str());
         oss.str(std::string());
         oss << myPostRasterType;
         xml_writer.writeElement("myPostRasterType", oss.str().c_str());
         oss.str(std::string());
         oss << myPostDx;
         xml_writer.writeElement("myPostDx", oss.str().c_str());
         oss.str(std::string());
         oss << myPostFogVolume;
         xml_writer.writeElement("myPostFogVolume", oss.str().c_str());
         oss.str(std::string());
         oss << myPostGradientWidth;
         xml_writer.writeElement("myPostGradientWidth", oss.str().c_str());
         oss.str(std::string());
         oss << myPostVoxelSize;
         xml_writer.writeElement("myPostVoxelSize", oss.str().c_str());
         oss.str(std::string());
         oss << myPostRadiusMin;
         xml_writer.writeElement("myPostRadiusMin", oss.str().c_str());
         oss.str(std::string());
         oss << myPostBandWidth;
         xml_writer.writeElement("myPostBandWidth", oss.str().c_str());
         oss.str(std::string());
         oss << myPostWSUnits;
         xml_writer.writeElement("myPostWSUnits", oss.str().c_str());
         oss.str(std::string());
         oss << myPostVDBRadiusMult;
         xml_writer.writeElement("myPostVDBRadiusMult", oss.str().c_str());
         oss.str(std::string());
         oss << myPostVDBVelocityMult;
         xml_writer.writeElement("myPostVDBVelocityMult", oss.str().c_str());
         oss.str(std::string());
         oss << myPostFalloff;
         xml_writer.writeElement("myPostFalloff", oss.str().c_str());
         oss.str(std::string());
         oss << myPostPosInfluence;
         xml_writer.writeElement("myPostPosInfluence", oss.str().c_str());
         oss.str(std::string());
         oss << myPostNormalInfluence;
         xml_writer.writeElement("myPostNormalInfluence", oss.str().c_str());
         oss.str(std::string());
         oss << myPostVelInfluence;
         xml_writer.writeElement("myPostVelInfluence", oss.str().c_str());
         oss.str(std::string());
         oss << myPostVDBMedianFilter;
         xml_writer.writeElement("myPostVDBMedianFilter", oss.str().c_str());
         oss.str(std::string());
         oss << myPostVDBMeanFilter;
         xml_writer.writeElement("myPostVDBMeanFilter", oss.str().c_str());
         oss.str(std::string());
         oss << myPostVDBMeanCurvatureFilter;
         xml_writer.writeElement("myPostVDBMeanCurvatureFilter", oss.str().c_str());
         oss.str(std::string());
         oss << myPostVDBLaplacianFilter;
         xml_writer.writeElement("myPostVDBLaplacianFilter", oss.str().c_str());
         oss.str(std::string());
         oss << myPostVDBOffsetFilter;
         xml_writer.writeElement("myPostVDBOffsetFilter", oss.str().c_str());
         oss.str(std::string());
         oss << myPostVDBOffsetFilterAmount;
         xml_writer.writeElement("myPostVDBOffsetFilterAmount", oss.str().c_str());
         oss.str(std::string());
         oss << myPostVDBReNormalizeFilter;
         xml_writer.writeElement("myPostVDBReNormalizeFilter", oss.str().c_str());
         oss.str(std::string());
         oss << myPostVDBWriteVDBFiles;
         xml_writer.writeElement("myPostVDBWriteVDBFiles", oss.str().c_str());
         oss.str(std::string());
         oss << myVDBBaseFileName;
         xml_writer.writeElement("myVDBBaseFileName", oss.str().c_str());
         oss.str(std::string());
         oss << myPostVDBMedianIterations;
         xml_writer.writeElement("myPostVDBMedianIterations", oss.str().c_str());
         oss.str(std::string());
         oss << myPostVDBMeanIterations;
         xml_writer.writeElement("myPostVDBMeanIterations", oss.str().c_str());
         oss.str(std::string());
         oss << myPostVDBMeanCurvatureIterations;
         xml_writer.writeElement("myPostVDBMeanCurvatureIterations", oss.str().c_str());
         oss.str(std::string());
         oss << myPostVDBLaplacianIterations;
         xml_writer.writeElement("myPostVDBLaplacianIterations", oss.str().c_str());

         xml_writer.endElement();  // parameters

         xml_writer.startElement("timing_stats");
         xml_writer.writeComment("*********** clusterThis timing stats ***********");

         std::string tmp_str = asctime(localtime(&myInitStartTime));
         tmp_str.replace(tmp_str.length() - 1, tmp_str.length(), "");
         xml_writer.writeElement("myInitStartTime", tmp_str.c_str());

         oss.str(std::string());
         oss << (((float)myInitTime) / CLOCKS_PER_SEC);
         xml_writer.writeElement("myInitTime", oss.str().c_str());

         oss.str(std::string());
         oss << (((float)myInitExecTime) / CLOCKS_PER_SEC);
         xml_writer.writeElement("myInitExecTime", oss.str().c_str());

         tmp_str = asctime(localtime(&myInitEndTime));
         tmp_str.replace(tmp_str.length() - 1, tmp_str.length(), "");
         xml_writer.writeElement("myInitEndTime", tmp_str.c_str());

//   xml_writer.writeAttribute("foo", "123");
//   xml_writer.writeAttribute("bar", "456");

         tmp_str = asctime(localtime(&myPreProcStartTime));
         tmp_str.replace(tmp_str.length() - 1, tmp_str.length(), "");
         xml_writer.writeElement("myPreProcStartTime", tmp_str.c_str());

         oss.str(std::string());
         oss << (((float)myPreProcTime) / CLOCKS_PER_SEC);
         xml_writer.writeElement("myPreProcTime", oss.str().c_str());

         oss.str(std::string());
         oss << (((float)myPreProcExecTime) / CLOCKS_PER_SEC);
         xml_writer.writeElement("myPreProcExecTime", oss.str().c_str());

         tmp_str = asctime(localtime(&myPreProcEndTime));
         tmp_str.replace(tmp_str.length() - 1, tmp_str.length(), "");
         xml_writer.writeElement("myPreProcEndTime", tmp_str.c_str());

         tmp_str = asctime(localtime(&myPostProcStartTime));
         tmp_str.replace(tmp_str.length() - 1, tmp_str.length(), "");
         xml_writer.writeElement("myPostProcStartTime", tmp_str.c_str());

         oss.str(std::string());
         oss << (((float)myPostProcTime) / CLOCKS_PER_SEC);
         xml_writer.writeElement("myPostProcTime", oss.str().c_str());

         oss.str(std::string());
         oss << (((float)myPostProcExecTime) / CLOCKS_PER_SEC);
         xml_writer.writeElement("myPostProcExecTime", oss.str().c_str());

         tmp_str = asctime(localtime(&myPostProcEndTime));
         tmp_str.replace(tmp_str.length() - 1, tmp_str.length(), "");
         xml_writer.writeElement("myPostProcEndTime", tmp_str.c_str());

         tmp_str = asctime(localtime(&myRenderStartTime));
         tmp_str.replace(tmp_str.length() - 1, tmp_str.length(), "");
         xml_writer.writeElement("myRenderStartTime", tmp_str.c_str());

         oss.str(std::string());
         oss << (((float)myRenderTime) / CLOCKS_PER_SEC);
         xml_writer.writeElement("myRenderTime", oss.str().c_str());

         oss.str(std::string());
         oss << (((float)myRenderExecTime) / CLOCKS_PER_SEC);
         xml_writer.writeElement("myRenderExecTime", oss.str().c_str());

         tmp_str = asctime(localtime(&myRenderEndTime));
         tmp_str.replace(tmp_str.length() - 1, tmp_str.length(), "");
         xml_writer.writeElement("myRenderEndTime", tmp_str.c_str());

         xml_writer.endElement();   // timing_stats

         xml_writer.startElement("geometry_stats");
         xml_writer.writeComment("***********  clusterThis geometry stats ***********");

         oss.str(std::string());
         oss << myNumSourcePoints;
         xml_writer.writeElement("myNumSourcePoints", oss.str().c_str());
         oss.str(std::string());
         oss << myInstanceNum;
         xml_writer.writeElement("myInstanceNum", oss.str().c_str());

         oss.str(std::string());
         oss  <<  myBox;
//         oss  << "(" << myBox.vals[0][1] << " , "  << myBox.vals[0][1] << " , " << myBox.vals[0][2] << ") - ("
//         << myBox.vals[1][0] << " , "  << myBox.vals[1][1] << " , "  << myBox.vals[1][2] << ")";
//         oss.str().replace(oss.str().find("\n") - 1, oss.str().find("\n"), "");
//         oss.str().erase(std::remove(oss.str().begin(), oss.str().end(), '\n'), oss.str().end());
         xml_writer.writeElement("myBox", oss.str().c_str());

         oss.str(std::string());
         oss  <<  myVelBox;
//         oss  << "(" << myVelBox.vals[0][1] << " , "  << myVelBox.vals[0][1] << " , " << myVelBox.vals[0][2] << ") - ("
//         << myVelBox.vals[1][0] << " , "  << myVelBox.vals[1][1] << " , "  << myVelBox.vals[1][2] << ")";
         oss.str().replace(oss.str().length() - 1, oss.str().length(), "");
         xml_writer.writeElement("myVelBox", oss.str().c_str());

         oss.str(std::string());
         oss << myLOD;
         xml_writer.writeElement("myLOD", oss.str().c_str());
         oss.str(std::string());
         oss << mySourceGDPMemUsage;
         xml_writer.writeElement("mySourceGDPMemUsage", oss.str().c_str());
         oss.str(std::string());
         oss << myInstGDPMemUsage;
         xml_writer.writeElement("myInstGDPMemUsage", oss.str().c_str());
         oss.str(std::string());
         oss << myInstMbGDPMemUsage;
         xml_writer.writeElement("myInstMbGDPMemUsage", oss.str().c_str());
         oss.str(std::string());
         oss << myPointTreeMemUsage;
         xml_writer.writeElement("myPointTreeMemUsage", oss.str().c_str());
         oss.str(std::string());
         oss << mySourceGeoGridMemUsage;
         xml_writer.writeElement("mySourceGeoGridMemUsage", oss.str().c_str());
         oss.str(std::string());
         oss << mySourceGradientGridMemUsage;
         xml_writer.writeElement("mySourceGradientGridMemUsage", oss.str().c_str());

         xml_writer.endElement(); //geometry_stats

         xml_writer.startElement("misc");
         xml_writer.writeComment("*********** misc *********** ");

         oss.str(std::string());
         oss << myMaterial;
         xml_writer.writeElement("myMaterial", oss.str().c_str());
         oss.str(std::string());
         oss << tempFileDeleted;
         xml_writer.writeElement("tempFileDeleted", oss.str().c_str());

         xml_writer.endElement(); // misc.

         xml_writer.endElement();  // render_stats

         xml_writer.endWriting();

      }

//bool   beginWritingToFile (const char *file)
//bool   beginWritingToMemory (UT_String &memory)
//bool   endWriting ()
//bool   setIndentation (int spaces_count)
//bool   writeElement (const char *tag, const char *string)
//bool   startElement (const char *tag)
//bool   endElement ()
//bool   writeAttribute (const char *name, const char *value)
//bool   writeString (const char *string)
//bool   writeComment (const char *string)
//bool   writeRawString (const char *string)
//bool   writeCDataElement (const char *data)


//   cout << "VRAY_clusterThis::exitClusterThisReal(): " << tempFileDeleted << endl;
//   cout << "VRAY_clusterThis::exitClusterThisReal() - temp filename " << myTempFname << endl;
//   const char * fname = me->myTempFname;
//   ofstream myStream;
//   myStream.open("exit_data.txt", ios_base::app);
//   myStream << this->exitData.exitTime << std::endl;
//   myStream << this->exitData.exitCode << std::endl;
//   myStream.flush();
//   myStream.close();
//   cout << "VRAY_clusterThis::exitClusterThisReal() : " << this->exitData.exitTime << endl;


   if(this->myUseTempFile && !this->mySaveTempFile) {
         if((UT_String(fname)).isstring() && stat(fname, &fileResults) == 0) {
               if(myVerbose > CLUSTER_MSG_INFO)
                  cout << "VRAY_clusterThis::exitClusterThisReal() - Found temp file " << fname << endl;
               if(!remove(fname) && (myVerbose > CLUSTER_MSG_INFO))
                  cout << "VRAY_clusterThis::exitClusterThisReal() - Removed geometry temp file: " << fname << endl;
            }
         else
            if(myVerbose > CLUSTER_MSG_INFO)
               cout << "VRAY_clusterThis::exitClusterThisReal() - Did not find temp file " << endl << endl;
//               cout << "VRAY_clusterThis::exitClusterThisReal() - Did not find temp file " << fname << endl << endl;
      }



   if(myVerbose > CLUSTER_MSG_INFO)
      std::cout << "VRAY_clusterThis::exitClusterThisReal() - Exiting" << std::endl;
}





/// ********************** MISC. STATIC FUNCTIONS *************************************




static void getRoughBBox(UT_BoundingBox & box, UT_BoundingBox & vbox,
                         const GEO_Point * point, const fpreal scale,
//                         const GEO_Point * point, const UT_Vector3 & scale,
                         const GA_ROAttributeRef & voff,
                         fpreal tscale, const UT_Matrix4 & xform)
{
   fpreal     maxradius;
//   static fpreal isin45 = 1.0F / SYSsin(M_PI / 4);  // isin45 = 1.41421
   UT_Vector3    pt;

   maxradius = scale * 0.5F;
//   maxradius = SYSmax(scale.x(), scale.y()) * isin45 * 0.5F;

   pt = UT_Vector3(-maxradius, -maxradius, 0) * xform;
   box.initBounds(pt);
   pt = UT_Vector3(-maxradius,  maxradius, 0) * xform;
   box.enlargeBounds(pt);
   pt = UT_Vector3(maxradius, -maxradius, 0) * xform;
   box.enlargeBounds(pt);
   pt = UT_Vector3(maxradius,  maxradius, 0) * xform;
   box.enlargeBounds(pt);

   box.translate(point->getPos());
   vbox = box;

   UT_Vector3  vel;
   int      i;
   fpreal      amount;

   vel = point->getValue<UT_Vector3>(voff);
//         std::cout << "getRoughBBox() vel: " << vel << std::endl;
   for(i = 0; i < 3; i++) {
         amount = vel(i) * tscale;
         if(amount < 0)
            vbox.vals[i][1] -= amount;
         else
            vbox.vals[i][0] -= amount;

      }
//         std::cout << "getRoughBBox() amount: " << amount << std::endl;
//   std::cout << "getRoughBBox() \nbox: " << box << "vbox: " << vbox << std::endl;


}

static inline void clampBox(UT_BoundingBox & from, const UT_BoundingBox & to)
{
   if(from.vals[0][0] < to.vals[0][0])
      from.vals[0][0] = to.vals[0][0];
   if(from.vals[1][0] < to.vals[1][0])
      from.vals[1][0] = to.vals[1][0];
   if(from.vals[2][0] < to.vals[2][0])
      from.vals[2][0] = to.vals[2][0];
   if(from.vals[0][1] > to.vals[0][1])
      from.vals[0][1] = to.vals[0][1];
   if(from.vals[1][1] > to.vals[1][1])
      from.vals[1][1] = to.vals[1][1];
   if(from.vals[2][1] > to.vals[2][1])
      from.vals[2][1] = to.vals[2][1];
}

static inline int testClampBox(const UT_BoundingBox & from, const UT_BoundingBox & to)
{
   if(from.vals[0][0] < to.vals[0][0])
      return 1;
   if(from.vals[1][0] < to.vals[1][0])
      return 1;
   if(from.vals[2][0] < to.vals[2][0])
      return 1;
   if(from.vals[0][1] > to.vals[0][1])
      return 1;
   if(from.vals[1][1] > to.vals[1][1])
      return 1;
   if(from.vals[2][1] > to.vals[2][1])
      return 1;
   return 0;
}


static short int myPasses(int mode)
{
   static short int num_passes; // keep track of how many times the DSO gets called
   if(mode)
      return num_passes;
   else
      num_passes++;
   return 0;
}



static inline int calculateNewInstPosition(fpreal theta, uint32 i, uint32 j)
{
#ifdef DEBUG
   cout << "VRAY_clusterThis::calculateNewInstPosition() i: " << i << " j: " << j << endl;
#endif

   // Calculate a new position for the object ...
//   fpreal delta = theta * i;
//   fpreal dx, dy, dz = 0.0;
//   dx = SYSsin(delta * myFreqX + myOffsetX);
//   dy = SYScos(delta * myFreqY + myOffsetY);
//   dz = SYScos(delta * myFreqZ + myOffsetZ);

//#ifdef DEBUG
//   cout << "VRAY_clusterThis::calculateNewInstPosition() " << "delta: " << delta << endl;
//   cout << "VRAY_clusterThis::calculateNewInstPosition() " << "dx: " << dx << " dy: " << dy << " dz: " << dz << endl;
//#endif

//   myNoise.setSeed(myPointAttributes.id);

   // Calculate a bit of noise to add to the new position ...
   // TODO:
//   fpreal noise_bias = (myNoise.turbulence(myPointAttributes.myPos, myFractalDepth, myRough, myNoiseAtten) * myNoiseAmp) + 1.0;

//      UT_Vector3 myNoiseVec;
   // myNoise.turbulence(myPos, myFractalDepth, myNoiseVec, myRough, myNoiseAtten);
   // cout << "VRAY_clusterThis::render() " << "myNoiseVec: " << myNoiseVec.x() << " " << myNoiseVec.x() << " " << myNoiseVec.x() << endl;

//#ifdef DEBUG
//   cout << "VRAY_clusterThis::calculateNewInstPosition() " << "noise_bias: " << noise_bias << endl;
//#endif

//   // Calculate the new object's position
//   myPointAttributes.myNewPos[0] = (fpreal) myPointAttributes.myPos.x() +
//                                   ((dx * myRadius) * noise_bias * SYSsin(static_cast<fpreal>(j + i)));
//   myPointAttributes.myNewPos[1] = (fpreal) myPointAttributes.myPos.y() +
//                                   ((dy * myRadius) * noise_bias * SYScos(static_cast<fpreal>(j + i)));
//   myPointAttributes.myNewPos[2] = (fpreal) myPointAttributes.myPos.z() +
//                                   ((dz * myRadius) * noise_bias * (SYSsin(static_cast<fpreal>(j + i)) + SYScos(static_cast<fpreal>(j + i))));
////   myPointAttributes.myNewPos[2] = ( fpreal ) myPointAttributes.myPos.z() +
////                                    ( ( dz * myRadius ) * noise_bias * ( SYScos ( static_cast<fpreal>(j + i)) ) );
//
//   if (myDoMotionBlur == CLUSTER_MB_DEFORMATION) {
//      myPointAttributes.myMBPos[0] = myPointAttributes.myNewPos[0] - myPointAttributes.v.x();
//      myPointAttributes.myMBPos[1] = myPointAttributes.myNewPos[1] - myPointAttributes.v.y();
//      myPointAttributes.myMBPos[2] = myPointAttributes.myNewPos[2] - myPointAttributes.v.z();
//   }

//#ifdef DEBUG
//   cout << "VRAY_clusterThis::calculateNewInstPosition() myPos:   "
//        << myPointAttributes.myPos.x() << " " << myPointAttributes.myPos.y() << " " << myPointAttributes.myPos.z() << endl;
//   cout << "VRAY_clusterThis::calculateNewInstPosition() newPos: "
//        << myPointAttributes.myNewPos[0] << " " << myPointAttributes.myNewPos[1] << " " << myPointAttributes.myNewPos[2] << endl;
//#endif

   return 0;

}


static inline int computeDivs(fpreal inc, fpreal min)
{
   int  divs = (int)SYSceil(inc / min);
   if(divs < 1)
      divs = 1;
   else
      if(divs > 4)
         divs = 4;
   return divs;
}



#endif


