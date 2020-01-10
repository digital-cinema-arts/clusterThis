/* ******************************************************************************
*
* clusterThis mantra DSO for render time geo instancing/clustering
*
* Description : This DSO  will instance geomtery or volumes during the render of mantra IFD's
*
*    Digital Cinema Arts (C) 2008-2012
*
* This work is licensed under the Creative Commons Attribution-ShareAlike 2.5 License.
* To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/2.5/ or send a letter to
* Creative Commons, 543 Howard Street, 5th Floor, San Francisco, California, 94105, USA.
*
***************************************************************************** */

// Open VDB copyright information can be found here: http://www.openvdb.org

#ifndef __VRAY_clusterThis_cpp__
#define __VRAY_clusterThis_cpp__

#include <GU/GU_Detail.h>
#include <GEO/GEO_PointTree.h>
#include <GEO/GEO_Primitive.h>
#include <GU/GU_PrimCircle.h>
#include <GU/GU_PrimSphere.h>
#include <GU/GU_PrimTube.h>
#include <GU/GU_Grid.h>
#include <GU/GU_PrimNURBCurve.h>
#include <GEO/GEO_PrimSphere.h>
#include <GU/GU_PrimMetaBall.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_XformOrder.h>
#include <UT/UT_Noise.h>
#include <UT/UT_MTwister.h>
#include <UT/UT_BoundingBox.h>
#include <VRAY/VRAY_Procedural.h>
#include <VRAY/VRAY_IO.h>
#include <GEO/GEO_IORib.h>
#include <UT/UT_Exit.h>
#include <UT/UT_Options.h>
#include <UT/UT_IStream.h>
#include <UT/UT_Version.h>
#include <UT/UT_XMLWriter.h>
#include <GA/GA_AttributeRef.h>
#include <GEO/GEO_AttributeHandle.h>
#include <GU/GU_PrimVolume.h>
#include <UT/UT_VoxelArray.h>
#include <SHOP/SHOP_Node.h>
#include <CVEX/CVEX_Context.h>
#include <UT/UT_StringArray.h>
#include <MOT/MOT_Director.h>
#include <SYS/SYS_Math.h>

#include <sys/stat.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <ios>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <exception>
#include <sstream>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>

#include <openvdb/openvdb.h>
#include <openvdb/tools/LevelSetSphere.h>
#include <openvdb/tools/LevelSetUtil.h>
#include <openvdb/tools/ParticlesToLevelSet.h>
#include <openvdb/tools/GridTransformer.h>
#include <openvdb/tools/GridSampling.h>
#include <openvdb/tools/Gradient.h>
#include <openvdb/tools/Filter.h>

#include <houdini_utils/ParmFactory.h>
#include <openvdb_houdini/Utils.h>
#include <openvdb_houdini/SOP_NodeVDB.h>
#include <openvdb_houdini/GU_PrimVDB.h>

namespace hvdb = openvdb_houdini;
namespace hutil = houdini_utils;

#include "version.h"
#include "VRAY_clusterThis.h"
#include "VRAY_clusterThisParms.cpp"
#include "VRAY_clusterThisUtil.cpp"
#include "VRAY_clusterThisRender.cpp"
#include "VRAY_clusterThisInstance.cpp"
#include "VRAY_clusterThisAttributeUtils.cpp"
#include "VRAY_clusterThisCVEXUtil.cpp"
#include "VRAY_clusterThisRunCVEX.cpp"
#include "VRAY_clusterThisPreProcess.cpp"
#include "VRAY_clusterThisPostProcess.cpp"

class VRAY_clusterThis_Exception;



/* ******************************************************************************
*  Function Name : allocProcedural()
*
*  Description :
*
*  Input Arguments : None
*
*  Return Value :
*
***************************************************************************** */
VRAY_Procedural * allocProcedural(const char *)
{
//   std::cout << "VRAY_clusterThis::allocProcedural()" << std::endl;
   return new VRAY_clusterThis();
}



/* ******************************************************************************
*  Function Name : getProceduralArgs()
*
*  Description :
*
*  Input Arguments : None
*
*  Return Value :
*
***************************************************************************** */
const VRAY_ProceduralArg * getProceduralArgs(const char *)
{
//   std::cout << "VRAY_clusterThis::getProceduralArgs()" << std::endl;
   return theArgs;
}



/* ******************************************************************************
*  Function Name : VRAY_clusterThis_Exception()
*
*  Description : Constructor for a "VRAY_clusterThis_Exception Exception" object
*
*  Input Arguments : None
*
*  Return Value :
*
***************************************************************************** */
VRAY_clusterThis_Exception::VRAY_clusterThis_Exception(std::string msg, int code)
{

   e_msg = msg;
   e_code = code;

};


//VRAY_clusterThis_Exception::~VRAY_clusterThis_Exception() {


//   };





/* ******************************************************************************
*  Function Name : VRAY_clusterThis()
*
*  Description :  Constructor for a  VRAY_clusterThis object
*
*  Input Arguments : None
*
*  Return Value :
*
***************************************************************************** */
VRAY_clusterThis::VRAY_clusterThis()
{

#ifdef DEBUG
   std::cout << "VRAY_clusterThis::VRAY_clusterThis() - Constructor" << std::endl;
#endif

   if(myVerbose > CLUSTER_MSG_INFO)
      std::cout << "VRAY_clusterThis - Constructor" << std::endl;

   // Init member variables
   myBox.initBounds(0, 0, 0);
   myVelBox.initBounds(0, 0, 0);

   myPrimType = CLUSTER_PRIM_SPHERE;
   myUseGeoFile = 0;
   mySrcGeoFname = "";
   myNumCopies = 0;
   myNoiseType = 0;
   myFreqX = 0.0;
   myFreqY = 0.0;
   myFreqZ = 0.0;
   myOffsetX = 0.0;
   myOffsetY = 0.0;
   myOffsetZ = 0.0;
   myRadius = 0.0;
   myRough = 0.0;
   myBirthProb = 0.0;
   mySize[0] = 0.0;
   mySize[1] = 0.0;
   mySize[2] = 0.0;
   myDoMotionBlur = CLUSTER_MB_NONE;
   myShutter = 0.1;
   myNoiseAmp = 0.0;
   myFilterType = 0;
   myFilterAmp = 0.0;
   myNoiseAtten = 0.0;
   myNoiseSeed = 0;
   myFractalDepth = 0;
   myRecursion = 0;
   myBBoxFudgeFactor = 0.0;

   myVerbose = CLUSTER_MSG_QUIET;

   myUseTempFile = 0;
   mySaveTempFile = 0;
   mySaveXMLFile = 0;
   myXMLFileName = "";

   myUsePointRadius = 0;
   myUseBacktrackMB = 0;

   // CVEX parms
   myCVEX_Exec = 0;
   myCVEX_Exec_prim = 0;
   myCVEX_Exec_pre = 0;
   myCVEX_Exec_post = 0;
   myCVEXFname = "";
   myCVEXFname_prim = "";
   myCVEXFname_pre = "";
   myCVEXFname_post = "";

   myCVEXPointVars.cvex_Cd_pt = 0;
   myCVEXPointVars.cvex_Alpha_pt = 0;
   myCVEXPointVars.cvex_v_pt = 0;
   myCVEXPointVars.cvex_N_pt = 0;
   myCVEXPointVars.cvex_pscale_pt = 0;

   myCVEXPrimVars.cvex_Cd_prim = 0;
   myCVEXPrimVars.cvex_Alpha_prim = 0;
   myCVEXPrimVars.cvex_N_prim = 0;
   myCVEXPrimVars.cvex_pscale_prim = 0;
   myCVEXPrimVars.cvex_weight_prim = 0;
   myCVEXPrimVars.cvex_width_prim = 0;


   // VDB pre processing parms
   myVDBPreProcess = 0;
   myPreRasterType = 0;
   myPreDx = 1.0;
   myPreFogVolume = 0;
   myPreGradientWidth = 0.5;
   myPreVoxelSize = 0.025;
   myPreRadiusMin = 1.5;
   myPreBandWidth = 0.2;
   myPreWSUnits = 1;
   myPreVDBRadiusMult = 1.0;
   myPreVDBVelocityMult = 1.0;
   myPreFalloff = 0.5;
   myPrePosInfluence = 0.1;
   myPreNormalInfluence = 0.1;
   myPreVelInfluence = 0.1;
   myPreVDBMedianFilter = 0;
   myPreVDBMeanFilter = 0;
   myPreVDBMeanCurvatureFilter = 0;
   myPreVDBLaplacianFilter = 0;
   myPreVDBOffsetFilter = 0;
   myPreVDBOffsetFilterAmount = 0.1;
   myPreVDBReNormalizeFilter = 0;
   myPreVDBWriteDebugFiles = 0;
   myPreVDBMedianIterations = 4;
   myPreVDBMeanIterations = 4;
   myPreVDBMeanCurvatureIterations = 4;
   myPreVDBLaplacianIterations = 4;

   // Post processing parms
   myPostProcess = 0;

   // Nearest neighbor post processing parms
   myNNPostProcess = 0;
   myNNPostPosInfluence = 0.1;
   myNNPostVelInfluence = 0.1;

   // VDB post processing parms
   myVDBPostProcess = 0;
   myVDBSourceFile = "";
   myVDBSourceGridName = "";
   myPostRasterType = 0;
   myPostDx = 1.0;
   myPostFogVolume = 0;
   myPostGradientWidth = 0.5;
   myPostVoxelSize = 0.025;
   myPostRadiusMin = 1.5;
   myPostBandWidth = 0.2;
   myPostWSUnits = 1;
   myPostVDBRadiusMult = 1.0;
   myPostVDBVelocityMult = 1.0;
   myPostFalloff = 0.5;
   myPostPosInfluence = 0.1;
   myPostNormalInfluence = 0.1;
   myPostVelInfluence = 0.1;
   myPostVDBMedianFilter = 0;
   myPostVDBMeanFilter = 0;
   myPostVDBMeanCurvatureFilter = 0;
   myPostVDBLaplacianFilter = 0;
   myPostVDBOffsetFilter = 0;
   myPostVDBOffsetFilterAmount = 0.1;
   myPostVDBReNormalizeFilter = 0;
   myPostVDBMedianIterations = 4;
   myPostVDBMeanIterations = 4;
   myPostVDBMeanCurvatureIterations = 4;
   myPostVDBLaplacianIterations = 4;
   myPostVDBWriteVDBFiles = 0;

   VRAY_clusterThis::exitData.exitTime = 3.333;
   VRAY_clusterThis::exitData.exitCode = 3;

   myNumSourcePoints = 0;
   myCurrentTime = 0.0;
   myCurrentFrame = 0.0;
   myInstanceNum = 0;
   myTimeScale = 0.5F / myFPS;

   int exitCallBackStatus = -1;
   exitCallBackStatus = UT_Exit::addExitCallback(VRAY_clusterThis::exitClusterThis, (void *)this);

   if((exitCallBackStatus != 1) && exitCallBackStatus != 0) {
         std::cout << "VRAY_clusterThis::VRAY_clusterThis() - error adding ExitCallback() - exitCallBackStatus = " << std::hex
                   << exitCallBackStatus << std::dec << std::endl;
      }


}


/* ******************************************************************************
*  Function Name : ~VRAY_clusterThis()
*
*  Description :  Destructor for a  VRAY_clusterThis object
*
*  Input Arguments : None
*
*  Return Value :
*
***************************************************************************** */
VRAY_clusterThis::~VRAY_clusterThis()
{
//   std::cout << "VRAY_clusterThis::~VRAY_clusterThis() - Destructor" << std::endl;
}



/* ******************************************************************************
*  Function Name : getClassName()
*
*  Description : Get the class name for this VRAY_clusterThis object
*
*  Input Arguments : None
*
*  Return Value : const char *
*
***************************************************************************** */
const char * VRAY_clusterThis::getClassName()
{
//   std::cout << "VRAY_clusterThis::getClassName()" << std::endl;
   return "VRAY_clusterThis";
}


/* ******************************************************************************
*  Function Name : initialize
*
*  Description :  Initialize the VRAY_clusterThis object
*
*  Input Arguments : const UT_BoundingBox *
*
*  Return Value : int
*
***************************************************************************** */
int VRAY_clusterThis::initialize(const UT_BoundingBox * box)
{
   if(myVerbose > CLUSTER_MSG_INFO)
      std::cout << "VRAY_clusterThis::initialize()" << std::endl;

   void   *   handle;
   const char  *  name;
   UT_BoundingBox tbox, tvbox;
   UT_Matrix4     xform;
   UT_String      str;

   myInitTime = std::clock();
   std::time(&myInitStartTime);

   // Get the OTL parameters
   VRAY_clusterThis::getOTLParameters();

   if(myVerbose > CLUSTER_MSG_QUIET) {
         std::cout << std::endl << "VRAY_clusterThis::initialize() - Version: "
                   << MAJOR_VER << "." << MINOR_VER << "." << BUILD_VER << std::endl;
         std::cout << "VRAY_clusterThis::initialize() - Built for Houdini Version: "
                   << UT_MAJOR_VERSION << "." << UT_MINOR_VERSION << "." << UT_BUILD_VERSION_INT << std::endl;
         std::cout << "VRAY_clusterThis::initialize() - Initializing ..." <<  std::endl;
      }

//          cout << "VM_GEO_clusterThis OTL version: " <<  myOTLVersion << std::endl;
//       if(myOTLVersion != DCA_VERSION) {
//          cout << "VM_GEO_clusterThis OTL is wrong version: " <<  myOTLVersion << ", should be version: " << DCA_VERSION << ", please install correct version." << std::endl;
//          throw VRAY_clusterThis_Exception ( "VRAY_clusterThis::initialize() VM_GEO_clusterThis OTL is wrong version!", 1 );
//       }


   // Dump the user parameters to the console
   if(myVerbose == CLUSTER_MSG_DEBUG)
      VRAY_clusterThis::dumpParameters();


   // Find the geometry object to render
//   name = 0;
//   if(VRAY_Procedural::import("object", str))
//      name = str.isstring() ? (const char *)str : 0;
////   handle = VRAY_Procedural::queryObject(name);
//   handle = VRAY_Procedural::queryObject(0);
//   if(!handle) {
//         VRAYerror("%s couldn't find object '%s'", VRAY_Procedural::getClassName(), name);
//         return 0;
//      }
//   name = VRAY_Procedural::queryObjectName(handle);
//
////   std::cout << "VRAY_clusterThis::initialize() name: " << name << std::endl;
//
//   myGdp = (GU_Detail *)VRAY_Procedural::queryGeometry(handle, 0);
//   if(!myGdp) {
//         VRAYerror("%s object '%s' has no geometry", VRAY_Procedural::getClassName(), name);
//         return 0;
//      }



   handle = VRAY_Procedural::queryObject(0);
   myGdp = VRAY_Procedural::allocateGeometry();

   if(myUseGeoFile) {
         // If the file failed to load, throw an exception
         if(!(myGdp->load((const char *)mySrcGeoFname).success()))
            throw VRAY_clusterThis_Exception("VRAY_clusterThis::initialize() - Failed to read source geometry file ", 1);

         if(myVerbose > CLUSTER_MSG_INFO)
            cout << "VRAY_clusterThis::initialize() - Successfully loaded source geo file: " << mySrcGeoFname << endl;
      }
   else {
         myGdp->copy(*VRAY_Procedural::queryGeometry(handle, 0));
         if(myVerbose > CLUSTER_MSG_INFO)
            cout << "VRAY_clusterThis::initialize() - Copied incoming geometry" << endl;
      }


   // Get the point's attribute references
   VRAY_clusterThis::getAttributeRefs(myGdp);

   // Check for required attributes
   VRAY_clusterThis::checkRequiredAttributes();

   myNumSourcePoints = (long int) myGdp->points().entries();

   VRAY_Procedural::querySurfaceShader(handle, myMaterial);
   myMaterial.harden();
//         myPointAttributes.material = myMaterial;

#ifdef DEBUG
   cout << "VRAY_clusterThis::initialize() myMaterial: " << myMaterial << std::endl;
#endif

   myObjectName = VRAY_Procedural::queryObjectName(handle);

//      cout << "VRAY_clusterThis::initialize() Object Name: " << myObjectName << std::endl;
//      cout << "VRAY_clusterThis::initialize() Root Name: " << queryRootName() << std::endl;

#ifdef DEBUG
   cout << "Geometry Samples: " << queryGeometrySamples(handle) << std::endl;
#endif


//   if(import("object:name", str)) {
//         cout << "object:name: " << str << std::endl;
//      }
//
//   if(import("object:surface", str)) {
//         cout << "object:surface: " << str << std::endl;
//      }
//
//   if(import("plane:variable", str)) {
//         cout << "plane:variable: " << str << std::endl;
//      }
//
//   if(import("image:resolution", str)) {
//         cout << "image:resolution: " << str << std::endl;
//      }
//
//   if(import("object:categories", str)) {
//         cout << "object:categories: " << str << std::endl;
//      }
//
//   if(import("object:renderpoints", str)) {
//         cout << "object:renderpoints: " << str << std::endl;
//      }



//   changeSetting("object:geo_velocityblur", "on");

//   int     vblur = 0;
//   import("object:velocityblur", &vblur, 0);
//
//   if(vblur) {
//         str = 0;
//         import("velocity", str);
//         if(str.isstring()) {
////               const char  *  name;
////               name = queryObjectName(handle);
//               VRAYwarning("%s[%s] cannot get %s",
//                           VRAY_Procedural::getClassName(), (const char *)myObjectName, " motion blur attr");
//
//            }
//      }



   myXformInverse = queryTransform(handle, 0);
   myXformInverse.invert();


   if(myPostProcess && myVDBPostProcess)
      VRAY_clusterThis::buildVDBGrids(myGdp);

   // find how much noise is being generated to help with BBox calculations
   fpreal noise_bias;

   if(myNoiseType < 4) {
         myNoise.setSeed(myPointAttributes.id);
         noise_bias = (myNoise.turbulence(myPointAttributes.myPos, myFractalDepth, myRough, myNoiseAtten) * myNoiseAmp) + 1.0;
      }
   else {
         myMersenneTwister.setSeed(myPointAttributes.id);
         noise_bias = (myMersenneTwister.frandom() * myNoiseAmp) + 1.0;
      }

#ifdef DEBUG
   cout << "VRAY_clusterThis::initialize() " << "noise_bias: " << noise_bias << endl;
#endif


   // Calculate our BBox for the incoming point cloud and add noise, radius, etc. to enlarge the BBox to
   // accomodate the instanced geometry
   // Build the geo point tree to be used for nearest neighbor caculations
   fpreal scale;
   fpreal radius;
   fpreal pscale;
   int first = 1;
   xform = myXformInverse;

   for(uint32 i = myGdp->points().entries(); i-- > 0;) {
         GEO_Point * ppt = myGdp->points()(i);

//         // Append to our list of points to be used for various tasks,
//         // like breaking up the point cloud into regular grids, etc.
//         mySRCPointList.append(i);

         if(myUsePointRadius)
            radius = static_cast<fpreal>(ppt->getValue<fpreal>(myPointAttrRefs.radius, 0));
         else
            radius = myRadius;

         pscale = static_cast<fpreal>(ppt->getValue<fpreal>(myPointAttrRefs.pscale, 0));

         if(myUsePointRadius)
            scale = (radius + noise_bias) * pscale;
         else
            scale = (myRadius + noise_bias) * pscale;

         mySRCPointTree.appendPtRadius(myGdp, ppt, radius);

         getRoughBBox(tbox, tvbox, ppt, scale, myPointAttrRefs.v, myTimeScale, xform);
         if(first) {
               myBox = tbox;
               myVelBox = tvbox;
               first = 0;
            }
         else {
               myBox.enlargeBounds(tbox);
               myVelBox.enlargeBounds(tvbox);
            }

      }

   if(first) {
         std::cout << "VRAY_clusterThis::initialize() " << getClassName() << " found no points in: " << name << std::endl;
         VRAYwarning("VRAY_clusterThis::initialize() %s found no points in %s", getClassName(), name);
         return 0;
      }


//   std::cout << "VRAY_clusterThis::initialize() *** \nmyBox: " << myBox << "myVelBox: " << myVelBox << std::endl;

   // NOTE: Enlarge the bbox another x% because we won't be able to give mantra the actual bbox size via render() method.
   // (mantra calls getBoundingBox() only once, and it's before the render() method is called by mantra).
   myBox.enlargeBounds(myBBoxFudgeFactor);
   myVelBox.enlargeBounds(myBBoxFudgeFactor);

//   myBox.enlargeFloats();
//   myVelBox.enlargeFloats();

   for(int i = 0; i < 3; i++)
      for(int j = 0; j < 2; j++) {
            if(myBox.vals[i][j] <= CLUSTER_BBOX_MIN) {
//            if(SYSisNan(myBox.vals[i][j])) {
//            if(myBox.vals[i][j] <= std::numeric_limits<float>::min()) {
                  myBox.vals[i][j] = CLUSTER_BBOX_MIN;
                  if(myVerbose == CLUSTER_MSG_DEBUG)
                     std::cout << "VRAY_clusterThis::initialize() found NAN: " << myBox.vals[i][j] << std::endl;
               }
         }


   if(myVerbose == CLUSTER_MSG_DEBUG)
      std::cout << "VRAY_clusterThis::initialize()\nmyBox: " << myBox << "myVelBox: " << myVelBox << std::endl;


   if(box) {
         if(myPointAttrRefs.v.isValid()) {
               if(testClampBox(myBox, *box) || testClampBox(myVelBox, *box))
                  VRAYwarning("%s[%s] cannot render a partial box %s", getClassName(), name, "with motion blur");
               std::cout << "VRAY_clusterThis::initialize() " << getClassName() << " WARNING: cannot render a partial box " << name << std::endl;
            }
         else {
               clampBox(myBox, *box);
               clampBox(myVelBox, *box);
            }
      }

//   std::cout << "VRAY_clusterThis::initialize() 3 \nmyBox: " << myBox << "myVelBox: " << myVelBox << std::endl;
//   if(box) {
//         std::cout << "VRAY_clusterThis::initialize() box min: " << box->xmin() << " " << box->ymin() << " " << box->zmin() << std::endl;
//         std::cout << "VRAY_clusterThis::initialize() box max: " << box->xmax() << " " << box->ymax() << " " << box->zmax() << std::endl;
//      }


   myPointTreeMemUsage = mySRCPointTree.getMemoryUsage();

   if(myVerbose == CLUSTER_MSG_DEBUG)
      std::cout << "VRAY_clusterThis::initialize() myPointTreeMemUsage: " << myPointTreeMemUsage << std::endl;


   std::time(&myInitEndTime);
   myInitExecTime = std::clock() - myInitTime;
   cout << "VRAY_clusterThis::initialize() " << "myInitExecTime: " << myInitExecTime << endl;


   return 1;
}



/* ******************************************************************************
*  Function Name : calculateNewPosition()
*
*  Description :   Calculate the position of the new instance
*
*  Input Arguments : int i, int j
*
*  Return Value : None
*
***************************************************************************** */
inline void VRAY_clusterThis::calculateNewPosition(fpreal theta, uint32 i, uint32 j)
{
#ifdef DEBUG
   cout << "VRAY_clusterThis::calculateNewPosition() i: " << i << " j: " << j << endl;
#endif

   // Calculate a new position for the object ...
   fpreal delta = theta * i;
   fpreal noise_bias;
   fpreal dx, dy, dz = 0.0;
   fpreal radius;
   dx = SYSsin(delta * myFreqX + myOffsetX);
   dy = SYScos(delta * myFreqY + myOffsetY);
   dz = SYScos(delta * myFreqZ + myOffsetZ);

#ifdef DEBUG
   cout << "VRAY_clusterThis::calculateNewPosition() " << "delta: " << delta << endl;
   cout << "VRAY_clusterThis::calculateNewPosition() " << "dx: " << dx << " dy: " << dy << " dz: " << dz << endl;
#endif

   if(myNoiseType < 4) {
         myNoise.setSeed(myPointAttributes.id);
         noise_bias = (myNoise.turbulence(myPointAttributes.myPos, myFractalDepth, myRough, myNoiseAtten) * myNoiseAmp) + 1.0;
//         cout << "VRAY_clusterThis::calculateNewPosition() turbulence: " << "noise_bias: " << noise_bias << endl;
      }
   else {
         myMersenneTwister.setSeed(myPointAttributes.id);
         noise_bias = (myMersenneTwister.frandom() * myNoiseAmp) + 1.0;
//         cout << "VRAY_clusterThis::calculateNewPosition() myMersenneTwister: " << "noise_bias: " << noise_bias << endl;
      }


#ifdef DEBUG
   cout << "VRAY_clusterThis::calculateNewPosition() " << "noise_bias: " << noise_bias << endl;
#endif

   if(myUsePointRadius)
      radius = myPointAttributes.radius;
   else
      radius = myRadius;


   // Calculate the new object's position
   myPointAttributes.myNewPos[0] = (fpreal) myPointAttributes.myPos.x() +
                                   ((dx * radius) * noise_bias * SYSsin(static_cast<fpreal>(j + i)));
   myPointAttributes.myNewPos[1] = (fpreal) myPointAttributes.myPos.y() +
                                   ((dy * radius) * noise_bias * SYScos(static_cast<fpreal>(j + i)));
   myPointAttributes.myNewPos[2] = (fpreal) myPointAttributes.myPos.z() +
                                   ((dz * radius) * noise_bias * (SYSsin(static_cast<fpreal>(j + i)) + SYScos(static_cast<fpreal>(j + i))));
//   myPointAttributes.myNewPos[2] = ( fpreal ) myPointAttributes.myPos.z() +
//                                    ( ( dz * radius ) * noise_bias * ( SYScos ( static_cast<fpreal>(j + i)) ) );

   if(myDoMotionBlur == CLUSTER_MB_DEFORMATION) {
//         myPointAttributes.myMBPos[0] = myPointAttributes.myNewPos[0] + myPointAttributes.v.x();
//         myPointAttributes.myMBPos[1] = myPointAttributes.myNewPos[1] + myPointAttributes.v.y();
//         myPointAttributes.myMBPos[2] = myPointAttributes.myNewPos[2] + myPointAttributes.v.z();

         if(myUseBacktrackMB) {
               myPointAttributes.myMBPos[0] = myPointAttributes.myNewPos[0] - myPointAttributes.backtrack.x();
               myPointAttributes.myMBPos[1] = myPointAttributes.myNewPos[1] - myPointAttributes.backtrack.y();
               myPointAttributes.myMBPos[2] = myPointAttributes.myNewPos[2] - myPointAttributes.backtrack.z();
            }
         else {
               myPointAttributes.myMBPos[0] = myPointAttributes.myNewPos[0] - myPointAttributes.v.x();
               myPointAttributes.myMBPos[1] = myPointAttributes.myNewPos[1] - myPointAttributes.v.y();
               myPointAttributes.myMBPos[2] = myPointAttributes.myNewPos[2] - myPointAttributes.v.z();
            }
      }

#ifdef DEBUG
   cout << "VRAY_clusterThis::calculateNewPosition() myPos:   "
        << myPointAttributes.myPos.x() << " " << myPointAttributes.myPos.y() << " " << myPointAttributes.myPos.z() << endl;
   cout << "VRAY_clusterThis::calculateNewPosition() newPos: "
        << myPointAttributes.myNewPos[0] << " " << myPointAttributes.myNewPos[1] << " " << myPointAttributes.myNewPos[2] << endl;
#endif

}


/* ******************************************************************************
*  Function Name : preLoadGeoFile()
*
*  Description :
*
*  Input Arguments : GU_Detail *file_gdp
*
*  Return Value : file load status
*
***************************************************************************** */
int VRAY_clusterThis::preLoadGeoFile(GU_Detail * file_gdp)
{
//     UT_Options myOptions;

   if(file_gdp->load((const char *)myGeoFile).success())
      return 0;
   else
      return 1;

}


#endif









