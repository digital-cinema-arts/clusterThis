/* ******************************************************************************
*
*  VRAY_clusterThisPreProcess
*
*
* Description :
*
*
***************************************************************************** */

#ifndef __VRAY_clusterThisPreProcess_cpp__
#define __VRAY_clusterThisPreProcess_cpp__


/* ******************************************************************************
*  Function Name : buildVDBGrids()
*
*  Description :   Build the VDB grids to be used for pre & post voxel processing
*
*  Input Arguments : GU_Detail * gdp
*
*  Return Value : None
*
***************************************************************************** */
void VRAY_clusterThis::buildVDBGrids(GU_Detail * gdp)
{

   UT_Vector3 tmp_v, new_v;

   if(myVerbose > CLUSTER_MSG_INFO)
      cout << "VRAY_clusterThis::buildVDBGrids() Processing Voxels" << std::endl;

   // init VDB
   openvdb::initialize();


   // If the user wants to build VDB grids form the source point cloud
   if(!myUseVDBSourceFile) {

         ParticleList paList(gdp, myPostVDBRadiusMult, myPostVDBVelocityMult);
         if(myVerbose == CLUSTER_MSG_DEBUG)
            std::cout << "VRAY_clusterThis::buildVDBGrids() paList.size() ... "  << paList.size() << std::endl;

         if(paList.size() != 0) {

               hvdb::Interrupter boss("VRAY_clusterThis::buildVDBGrids() Converting particles to level set");

               Settings settings;
               settings.mRadiusMin = myPostRadiusMin;
               settings.mRasterizeTrails = myPostRasterType;
               settings.mDx = myPostDx;  // only used for rasterizeTrails()
//               settings.mFogVolume = myPostFogVolume;
               settings.mFogVolume = 1;
               settings.mGradientWidth = myPostGradientWidth;  // only used for fog volume

               float background;

               // background in WS units
               if(myPostWSUnits)
                  background = myPostBandWidth;
               // background NOT in WS units
               else
                  background = myPostVoxelSize * myPostBandWidth;

               // Construct a new scalar grid with the specified background value.
               openvdb::math::Transform::Ptr transform =
                  openvdb::math::Transform::createLinearTransform(myPostVoxelSize);
               mySourceGeoGrid = openvdb::ScalarGrid::create(background);
               mySourceGeoGrid->setTransform(transform);
               mySourceGeoGrid->setGridClass(openvdb::GRID_LEVEL_SET);

               // Perform the particle conversion.
               this->convert(mySourceGeoGrid, paList, settings, boss);

               if(myVerbose == CLUSTER_MSG_DEBUG) {
                     std::cout << "VRAY_clusterThis::buildVDBGrids() - activeVoxelCount(): "
                               << mySourceGeoGrid->activeVoxelCount() << std::endl;
                     std::cout << "VRAY_clusterThis::buildVDBGrids() - background: "
                               << mySourceGeoGrid->background() << std::endl;
                  }

               UT_String gridNameStr = "ClusterGrid";
               mySourceGeoGrid->insertMeta("float type", openvdb::StringMetadata("averaged_velocity"));
               mySourceGeoGrid->insertMeta("name", openvdb::StringMetadata((const char *)gridNameStr));
               mySourceGeoGrid->insertMeta("voxel_size", openvdb::FloatMetadata(myPostVoxelSize));
               mySourceGeoGrid->insertMeta("background", openvdb::FloatMetadata(background));

               if(myVerbose > CLUSTER_MSG_INFO)
                  std::cout << "VRAY_clusterThis::buildVDBGrids() - Building grids ... " << std::endl;


               openvdb::FloatTree::ConstPtr myTreePtr;
               openvdb::VectorTree::ConstPtr myGradTreePtr;
//         openvdb::FloatTree::ValueType sampleResult;
//         openvdb::VectorGrid::ValueType gradResult;

               openvdb::tools::Filter<openvdb::FloatGrid> preFilter(*mySourceGeoGrid);

               if(myPostVDBMedianFilter) {
                     if(myVerbose > CLUSTER_MSG_INFO)
                        std::cout << "VRAY_clusterThis::buildVDBGrids() - Filtering with median filter ... " << std::endl;
                     for(int n = 0; n < myPostVDBMedianIterations && !boss.wasInterrupted(); ++n)
                        preFilter.median();
                  }

               if(myPostVDBMeanFilter) {
                     std::cout << "VRAY_clusterThis::buildVDBGrids() - Filtering with mean filter ... " << std::endl;
                     for(int n = 0; n < myPostVDBMeanIterations && !boss.wasInterrupted(); ++n)
                        preFilter.mean();
                  }

               if(myPostVDBMeanCurvatureFilter) {
                     std::cout << "VRAY_clusterThis::buildVDBGrids() - Filtering with mean curvature filter ... " << std::endl;
                     for(int n = 0; n < myPostVDBMeanCurvatureIterations && !boss.wasInterrupted(); ++n)
                        preFilter.meanCurvature();
                  }

               if(myPostVDBLaplacianFilter) {
                     std::cout << "VRAY_clusterThis::buildVDBGrids() - Filtering with laplacian filter ... " << std::endl;
                     for(int n = 0; n < myPostVDBLaplacianIterations && !boss.wasInterrupted(); ++n)
                        preFilter.laplacian();
                  }

//                           if(myVDBReNormalizeFilter)
//                              float r = preFilter.renormalize(3, 0.1);

               if(myPostVDBOffsetFilter) {
                     std::cout << "VRAY_clusterThis::buildVDBGrids() - Filtering with offset filter ... " << std::endl;
//00841         std::cout << "Morphological " << (offset>0 ? "erosion" : "dilation")
//00842             << " by the offset " << offset << std::endl;
                     preFilter.offset(myPostVDBOffsetFilterAmount);
                  }


               mySourceGeoGrid->pruneGrid();

               mySourceGeoGridMemUsage = mySourceGeoGrid->memUsage();
               if(myVerbose == CLUSTER_MSG_DEBUG)
                  std::cout << "VRAY_clusterThis::buildVDBGrids() - Scalar grid memory usage: "
                            << mySourceGeoGridMemUsage << std::endl;


               if(myVerbose == CLUSTER_MSG_DEBUG)
                  std::cout << "VRAY_clusterThis::buildVDBGrids() - Creating the gradient grid ... " << std::endl;

               mySourceGradientGrid = openvdb::VectorGrid::create();
               mySourceGradientGrid->setTransform(transform);
               mySourceGradientGrid->setGridClass(openvdb::GRID_FOG_VOLUME);
//               mySourceGradientGrid->setGridClass(openvdb::GRID_LEVEL_SET);

               openvdb::tools::Gradient<openvdb::ScalarGrid> myGradient(*mySourceGeoGrid);
               mySourceGradientGrid = myGradient.process();

               mySourceGradientGridMemUsage = mySourceGradientGrid->memUsage();
               if(myVerbose == CLUSTER_MSG_DEBUG)
                  std::cout << "VRAY_clusterThis::postProcess() - Gradient grid memory usage: "
                            << mySourceGradientGridMemUsage << std::endl;

               gridNameStr = "ClusterGradientGrid";
               mySourceGradientGrid->insertMeta("vector type", openvdb::StringMetadata("covariant (gradient)"));
               mySourceGradientGrid->insertMeta("name", openvdb::StringMetadata((const char *)gridNameStr));
               mySourceGradientGrid->insertMeta("VoxelSize", openvdb::FloatMetadata(myPostVoxelSize));
               mySourceGradientGrid->insertMeta("background", openvdb::FloatMetadata(background));

               if(myVerbose == CLUSTER_MSG_DEBUG)
                  std::cout << "VRAY_clusterThis::buildVDBGrids() - Finished creating/processing the gradient grid ... " << std::endl;

               // Write out the VDB files if the user wants them
               if(myPostVDBWriteVDBFiles) {

                     UT_String gridFileName;

                     if(myVerbose > CLUSTER_MSG_INFO)
                        std::cout << "VRAY_clusterThis::postProcess() - Writing grids to disk ... " << std::endl;
                     openvdb::GridPtrVec scalarGrids;
                     openvdb::GridPtrVec gradGrids;

                     gridFileName = myVDBBaseFileName;
                     gridFileName.insert(myVDBBaseFileName.length(), "_scalar.vdb");

                     openvdb::io::File outFile(static_cast<char *>(gridFileName));
                     scalarGrids.push_back(mySourceGeoGrid);
                     outFile.write(scalarGrids);
                     outFile.close();

                     gridFileName = myVDBBaseFileName;
                     gridFileName.insert(myVDBBaseFileName.length(), "_gradient.vdb");

                     openvdb::io::File gradientFile(static_cast<char *>(gridFileName));
                     gradGrids.push_back(mySourceGradientGrid);
                     gradientFile.write(gradGrids);
                     gradientFile.close();
                     if(myVerbose > CLUSTER_MSG_INFO)
                        std::cout << "VRAY_clusterThis::postProcess() - Finished writing grids to disk ... " << std::endl;
                  }


               // Clear the scalar grid to free memory
               mySourceGeoGrid->clear();

            }   //  if(paList.size() != 0)


      }

   // Read the VDB grids from disk
   else {


         if(myVerbose > CLUSTER_MSG_INFO)
            std::cout << "VRAY_clusterThis::buildVDBGrids() - Reading gradient grids from disk ... " << std::endl;

         openvdb::GridBase::Ptr baseGrid;
         openvdb::io::File VDBSourceFile(static_cast<char *>(myVDBSourceFile));
         VDBSourceFile.open();

         for(openvdb::io::File::NameIterator nameIter = VDBSourceFile.beginName();
               nameIter != VDBSourceFile.endName(); ++nameIter) {
               // Read in only the grid we are interested in.
               if(nameIter.gridName() == static_cast<char *>(myVDBSourceGridName)) {
                     std::cout << "gridName: " << nameIter.gridName() << std::endl;
                     baseGrid = VDBSourceFile.readGrid(nameIter.gridName());
                  }
               else {
                     std::cout << "skipping grid " << nameIter.gridName() << std::endl;
                  }
            }

         VDBSourceFile.close();
         mySourceGradientGrid = openvdb::gridPtrCast<openvdb::VectorGrid>(baseGrid);

         if(myVerbose > CLUSTER_MSG_INFO)
            std::cout << "VRAY_clusterThis::buildVDBGrids() - Finished reading gradient grids from disk ... " << std::endl;

      }


}



/* ******************************************************************************
*  Function Name : preProcess()
*
*  Description :   pre process the source geo
*
*  Input Arguments : GU_Detail * gdp
*
*  Return Value : None
*
***************************************************************************** */

void VRAY_clusterThis::preProcess(GU_Detail * gdp)
{

   myPreProcTime = std::clock();
   std::time(&myPreProcStartTime);

}


#endif


