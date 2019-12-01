/* ******************************************************************************
*
*  VRAY_clusterThis.h
*
*
* Description :
*
*
***************************************************************************** */


#ifndef __VRAY_clusterThis_h__
#define __VRAY_clusterThis_h__

//#define DEBUG

static const fpreal CLUSTER_BBOX_MIN = 0.00001;

namespace
{

// This class is required by openvdb::tools::ParticlesToLeveSet
   class ParticleList
   {
      public:
         ParticleList(const GA_Detail * gdp,
                      openvdb::Real radiusMult = 1,
                      openvdb::Real velocityMult = 1) :
            mGdp(gdp),
            mVDBRadiusHandle(gdp, GEO_POINT_DICT, "vdb_radius"),
            mVelHandle(gdp, GEO_POINT_DICT, "v"),
            mHasRadius(mVDBRadiusHandle.isValid()),
            mHasVelocity(mVelHandle.isValid()),
            mRadiusMult(radiusMult),
            mVelocityMult(velocityMult) {
         }

         bool hasRadius()   const {
            return mHasRadius;
         }
         bool hasVelocity() const {
            return mHasVelocity;
         }

         // The public methods below are the only ones required
         // by tools::ParticlesToLevelSet
         size_t size() const {
            return mGdp->getNumPoints();
         }
         openvdb::Vec3R pos(int n) const {
            UT_Vector3 p = mGdp->getPos3(this->offset(n));
            return openvdb::Vec3R(p[0], p[1], p[2]);
         }
         openvdb::Vec3R vel(int n) const {
            if(!mHasVelocity)
               return openvdb::Vec3R(0, 0, 0);
            UT_Vector3 v = mVelHandle.get(this->offset(n));
            return mVelocityMult * openvdb::Vec3R(v[0], v[1], v[2]);
         }
         openvdb::Real radius(int n) const {
            if(!mHasRadius)
               return mRadiusMult;
            return mRadiusMult * mVDBRadiusHandle.get(this->offset(n));
         }

      protected:
         GA_Offset offset(int n) const {
            return mGdp->pointOffset(n);
         }

         const GA_Detail  *  mGdp;
         GA_ROHandleF        mVDBRadiusHandle;
         GA_ROHandleV3       mVelHandle;
         const bool          mHasRadius, mHasVelocity;
         const openvdb::Real mRadiusMult; // multiplier for radius
         const openvdb::Real mVelocityMult; // multiplier for velocity
   };// ParticleList

// Convenient settings struct
   struct Settings {
      Settings(): mRasterizeTrails(false), mFogVolume(false), mDx(1.0), mGradientWidth(-1.0) {}
      bool mRasterizeTrails, mFogVolume;
      float mDx, mGradientWidth;
      float mRadiusMin;
   };

} // unnamed namespace


/* ******************************************************************************
*  Class Name : VRAY_clusterThis_Exception()
*
*  Description :  Exception class for VRAY_clusterThis mantra DSO
*
*
*  Input Arguments : None
*
*  Return Value : None
*
***************************************************************************** */
class VRAY_clusterThis_Exception
{
      std::string e_msg;
      int e_code;

   public:
      VRAY_clusterThis_Exception(std::string msg, int code);
//   ~VRAY_clusterThis_Exception();

      void what() {
         std::cout << "VRAY_clusterThis_Exception::what() - VRAY_clusterThis exception:  " << e_msg << endl;
      }

};


/* ******************************************************************************
*  Class Name : VRAY_clusterThis()
*
*  Description :  Class for VRAY_clusterThis mantra DSO
*
*
*  Input Arguments : None
*
*  Return Value : None
*
***************************************************************************** */
class VRAY_clusterThis : public VRAY_Procedural
{
   public:
      VRAY_clusterThis();
      virtual ~VRAY_clusterThis();

      virtual const char * getClassName();
      virtual int initialize(const UT_BoundingBox * box);
      virtual void getBoundingBox(UT_BoundingBox & box);
      virtual bool hasVolume() {
         return true;
      }
      virtual void render();

      static void exitClusterThis(void * data);
      void exitClusterThisReal(const char * fname);
      void exitClusterThisReal(void * data);

      struct exitData_struct {
         float exitTime;
         int exitCode;
      } exitData;

   private:

      struct pt_attr_ref_struct {

         // Required attributes
         GA_RWAttributeRef Cd;
         GA_RWAttributeRef Alpha;
         GA_RWAttributeRef v;
         GA_RWAttributeRef backtrack;
         GA_RWAttributeRef N;
         GA_RWAttributeRef up;
//         GA_RWAttributeRef orient;  // NOTE: using up vector instead of orient, at least for now.
         GA_RWAttributeRef material;
         GA_RWAttributeRef id;
         GA_RWAttributeRef pscale;
         GA_RWAttributeRef radius;
         GA_RWAttributeRef weight;
         GA_RWAttributeRef width;
         GA_RWAttributeRef vdb_radius;
         GA_RWAttributeRef geo_fname;

         // Optional attributes
         /*        GA_RWAttributeRef num_neighbors;
                 GA_RWAttributeRef force;
                 GA_RWAttributeRef vorticity;
                 GA_RWAttributeRef uv;
                 GA_RWAttributeRef age;
                 GA_RWAttributeRef viscosity;
                 GA_RWAttributeRef density;
                 GA_RWAttributeRef pressure;
                 GA_RWAttributeRef mass;
                 GA_RWAttributeRef temperature;*/

      } myPointAttrRefs;


      struct file_attr_ref_struct {

         // Required attributes
         GA_RWAttributeRef Cd;
         GA_RWAttributeRef Alpha;
         GA_RWAttributeRef v;
         GA_RWAttributeRef N;
//      GA_RWAttributeRef orient;
         GA_RWAttributeRef material;
         GA_RWAttributeRef id;
         GA_RWAttributeRef inst_id;
//      GA_RWAttributeRef lod;
         GA_RWAttributeRef pscale;
//      GA_RWAttributeRef up;
//      GA_RWAttributeRef angle;
//      GA_RWAttributeRef offset;
//      GA_RWAttributeRef amp;

//      GA_RWAttributeRef low_bound;
//      GA_RWAttributeRef high_bound;
//      GA_RWAttributeRef deformspace;
//      GA_RWAttributeRef xformobj;

         GA_RWAttributeRef pointCd;
         GA_RWAttributeRef pointAlpha;
         GA_RWAttributeRef pointV;
         GA_RWAttributeRef pointBacktrack;
         GA_RWAttributeRef pointN;
         GA_RWAttributeRef pointMaterial;
         GA_RWAttributeRef pointPscale;
         GA_RWAttributeRef pointId;
         GA_RWAttributeRef pointInstId;
//      GA_RWAttributeRef pointLOD;
//      GA_RWAttributeRef pointUp;
//      GA_RWAttributeRef pointAngle;
//      GA_RWAttributeRef pointOffset;
//      GA_RWAttributeRef pointAmp;

      } myFileAttrRefs;


      struct inst_attr_ref_struct {

         // Required attributes
         GA_RWAttributeRef Cd;
         GA_RWAttributeRef Alpha;
         GA_RWAttributeRef v;
         GA_RWAttributeRef N;
//         GA_RWAttributeRef orient;
         GA_RWAttributeRef material;
         GA_RWAttributeRef id;
         GA_RWAttributeRef inst_id;
         GA_RWAttributeRef pscale;
         GA_RWAttributeRef radius;
         GA_RWAttributeRef weight;
         GA_RWAttributeRef width;

         GA_RWAttributeRef pointCd;
         GA_RWAttributeRef pointAlpha;
         GA_RWAttributeRef pointV;
         GA_RWAttributeRef pointBacktrack;
         GA_RWAttributeRef pointN;
         GA_RWAttributeRef pointUp;
//         GA_RWAttributeRef pointOrient;
         GA_RWAttributeRef pointMaterial;
         GA_RWAttributeRef pointId;
         GA_RWAttributeRef pointWeight;
         GA_RWAttributeRef pointWidth;
         GA_RWAttributeRef pointInstId;
         GA_RWAttributeRef pointPscale;
         GA_RWAttributeRef pointRadius;
         GA_RWAttributeRef pointVDBRadius;

         // Optional attributes
         //GA_RWAttributeRef num_neighbors;
         //GA_RWAttributeRef force;
         //GA_RWAttributeRef vorticity;
         //GA_RWAttributeRef uv;
         //GA_RWAttributeRef age;
         //GA_RWAttributeRef viscosity;
         //GA_RWAttributeRef density;
         //GA_RWAttributeRef pressure;
         //GA_RWAttributeRef mass;
         //GA_RWAttributeRef temperature;

      } myInstAttrRefs;



      struct inst_mb_attr_ref_struct {

         // Required attributes
         GA_RWAttributeRef Cd;
         GA_RWAttributeRef Alpha;
         GA_RWAttributeRef v;
         GA_RWAttributeRef N;
//         GA_RWAttributeRef orient;
         GA_RWAttributeRef material;
         GA_RWAttributeRef id;
         GA_RWAttributeRef inst_id;
         GA_RWAttributeRef pscale;
         GA_RWAttributeRef radius;
         GA_RWAttributeRef weight;
         GA_RWAttributeRef width;

         GA_RWAttributeRef pointCd;
         GA_RWAttributeRef pointAlpha;
         GA_RWAttributeRef pointV;
         GA_RWAttributeRef pointBacktrack;
         GA_RWAttributeRef pointN;
         GA_RWAttributeRef pointUp;
//         GA_RWAttributeRef pointOrient;
         GA_RWAttributeRef pointMaterial;
         GA_RWAttributeRef pointWeight;
         GA_RWAttributeRef pointWidth;
         GA_RWAttributeRef pointId;
         GA_RWAttributeRef pointInstId;
         GA_RWAttributeRef pointPscale;
         GA_RWAttributeRef pointRadius;
         GA_RWAttributeRef pointVDBRadius;

         // Optional attributes
         //GA_RWAttributeRef num_neighbors;
         //GA_RWAttributeRef force;
         //GA_RWAttributeRef vorticity;
         //GA_RWAttributeRef uv;
         //GA_RWAttributeRef age;
         //GA_RWAttributeRef viscosity;
         //GA_RWAttributeRef density;
         //GA_RWAttributeRef pressure;
         //GA_RWAttributeRef mass;
         //GA_RWAttributeRef temperature;

      } myInstMBAttrRefs;


      struct pt_attr_struct {
         // Required attributes
         UT_Vector4 myPos;
         UT_Vector4 myNewPos;
         UT_Vector4 myMBPos;

         UT_Vector3 Cd;
         fpreal Alpha;
         UT_Vector3 v;
         UT_Vector4 backtrack;
         UT_Vector3 N;
         UT_Vector3 up;
//         UT_Vector4 orient;
         uint32 id;
         fpreal radius;
         fpreal vdb_radius;
         fpreal pscale;
         fpreal weight;
         fpreal width;
         UT_String material;
         UT_String geo_fname;

//         fpreal theta;

         // Optional attributes
         /*        uint32 num_neighbors;
                 UT_Vector3 force;
                 UT_Vector3 vorticity;
                 UT_Vector3 uv;
                 fpreal age;
                 fpreal viscosity;
                 fpreal density;
                 fpreal pressure;
                 fpreal mass;
                 fpreal temperature;*/
      } myPointAttributes;


      void  calculateNewPosition(fpreal theta, uint32 i, uint32 j);
      void  dumpParameters();
      int   preLoadGeoFile(GU_Detail * file_gdp);
      void  createAttributeRefs(GU_Detail * inst_gdp, GU_Detail * mb_gdp);
      int   getAttributeRefs(GU_Detail * gdp);
      int   getAttributes(GEO_Point * ppt);
      void  checkRequiredAttributes();
      int   addFileAttributeRefs(GU_Detail * gdp);
      void  setInstanceAttributes(GEO_Primitive * myGeoPrim);
      void  setPointInstanceAttributes(GU_Detail * gdp, GEO_Point * ppt);
      int   setFileAttributes(GU_Detail * gdp);
      int   getOTLParameters();
      int   runCVEX(GU_Detail * inst_gdp, GU_Detail * mb_gdp, UT_String theCVEXFname, uint method);

      void postNNProcess(GU_Detail * gdp, GU_Detail * inst_gdp, GU_Detail * mb_gdp);

      // voxel processing
      int   convertVDBUnits();
      void  convert(openvdb::ScalarGrid::Ptr, ParticleList&, const Settings&, hvdb::Interrupter &);
      void  convertVector(openvdb::VectorGrid::Ptr, ParticleList&, const Settings&, hvdb::Interrupter &);
      void  postProcess(GU_Detail * gdp, GU_Detail * inst_gdp, GU_Detail * mb_gdp);
      void  buildVDBGrids(GU_Detail * gdp);
      void  preProcess(GU_Detail * gdp);

      // Instancing methods
      int instancePoint(GU_Detail * inst_gdp, GU_Detail * mb_gdp);
      int instanceSphere(GU_Detail * inst_gdp, GU_Detail * mb_gdp);
      int instanceCube(GU_Detail * inst_gdp, GU_Detail * mb_gdp);
      int instanceGrid(GU_Detail * inst_gdp, GU_Detail * mb_gdp);
      int instanceTube(GU_Detail * inst_gdp, GU_Detail * mb_gdp);
      int instanceCircle(GU_Detail * inst_gdp, GU_Detail * mb_gdp);
      int instanceCurve(GU_Detail * inst_gdp, GU_Detail * mb_gdp, fpreal theta, long int point_num);
      int instanceMetaball(GU_Detail * inst_gdp, GU_Detail * mb_gdp);
      int instanceFile(GU_Detail * file_gdp, GU_Detail * inst_gdp, GU_Detail * mb_gdp);

      // member variables
      GU_Detail * myGdp;
      UT_BoundingBox myBox;
      UT_BoundingBox myVelBox;
      long int myNumSourcePoints;
      UT_String myMaterial;
      UT_Matrix3 myXformInverse;
      bool tempFileDeleted;
      UT_Noise myNoise;
      UT_MersenneTwister myMersenneTwister;
      uint32 myUsePointRadius;
//      bool myUsePointGeoFname;
      UT_String myObjectName;
//      UT_String myOTLVersion;
      fpreal   myVelocityScale;
      long int myInstanceNum;
      fpreal   myLOD;
      static const fpreal myFPS = 24.0;
      fpreal myTimeScale;
//      UT_IntArray mySRCPointList;
      GEO_PointTree mySRCPointTree;
      clock_t myInitTime;
      clock_t myPreProcTime;
      clock_t myPostProcTime;
      clock_t myRenderTime;
      clock_t myInitExecTime;
      clock_t myPreProcExecTime;
      clock_t myPostProcExecTime;
      clock_t myRenderExecTime;

      time_t myInitStartTime;
      time_t myPreProcStartTime;
      time_t myPostProcStartTime;
      time_t myRenderStartTime;
      time_t myInitEndTime;
      time_t myPreProcEndTime;
      time_t myPostProcEndTime;
      time_t myRenderEndTime;

      fpreal myPointTreeMemUsage;
      fpreal mySourceGDPMemUsage;
      fpreal myInstGDPMemUsage;
      fpreal myInstMbGDPMemUsage;

      GU_Detail * myFileGDP;
      openvdb::ScalarGrid::Ptr mySourceGeoGrid;
      openvdb::VectorGrid::Ptr mySourceGradientGrid;

      openvdb::Index64 mySourceGeoGridMemUsage;
      openvdb::Index64 mySourceGradientGridMemUsage;


      // Parameters
      uint32   myNumCopies;
      int     myUseGeoFile;
      UT_String myGeoFile;
      UT_String mySrcGeoFname;
      uint32   myPrimType;
      uint32   myMethod;  // Instance now or deferred
      uint32   myGridPointLimit;
      fpreal   mySize[3];
      uint32   myRecursion;
      fpreal   myFreqX, myFreqY, myFreqZ;
      fpreal   myOffsetX, myOffsetY, myOffsetZ;
      fpreal   myRadius;
      fpreal   myBirthProb;
      uint32   myDoMotionBlur;
      uint32   myUseBacktrackMB;
      fpreal   myShutter;
      fpreal   myShutter2;
      int      myNoiseType;
      fpreal   myRough;
      fpreal   myCurrentTime;
      fpreal   myCurrentFrame;
      fpreal   myBBoxFudgeFactor;
      int      myVerbose;

      fpreal   myNoiseAmp;
      fpreal   myNoiseAtten;
      int      myNoiseSeed;
      int      myFractalDepth;
      uint32   myFilterType;
      fpreal   myFilterAmp;

      UT_String myCVEXFname;
      int     myCVEX_Exec;
      UT_String myCVEXFname_prim;
      int     myCVEX_Exec_prim;
      UT_String myCVEXFname_pre;
      int     myCVEX_Exec_pre;
      UT_String myCVEXFname_post;
      int      myCVEX_Exec_post;

      UT_String myTempFname;
      int      myUseTempFile;
      int      mySaveTempFile;
      int      mySaveXMLFile;
      UT_String myXMLFileName;

      // VDB pre processing parms
      int      myVDBPreProcess;
      int      myPreRasterType;
      fpreal   myPreDx;
      int      myPreFogVolume;
      fpreal   myPreGradientWidth;
      fpreal   myPreVoxelSize;
      fpreal   myPreRadiusMin;
      fpreal   myPreBandWidth;
      int      myPreWSUnits;
      fpreal   myPreVDBVelocityMult;
      fpreal   myPreVDBRadiusMult;
      fpreal   myPreFalloff;
      fpreal   myPrePosInfluence;
      fpreal   myPreNormalInfluence;
      fpreal   myPreVelInfluence;
      int      myPreVDBMedianFilter;
      int      myPreVDBMeanFilter;
      int      myPreVDBMeanCurvatureFilter;
      int      myPreVDBLaplacianFilter;
      int      myPreVDBMedianIterations;
      int      myPreVDBMeanIterations;
      int      myPreVDBMeanCurvatureIterations;
      int      myPreVDBLaplacianIterations;
      int      myPreVDBOffsetFilter;
      fpreal   myPreVDBOffsetFilterAmount;
      int      myPreVDBReNormalizeFilter;
      int      myPreVDBWriteDebugFiles;

      int      myPostProcess;
      int      myNNPostProcess;
      fpreal   myNNPostPosInfluence;
      fpreal   myNNPostVelInfluence;

      // VDB post processing parms
      int      myVDBPostProcess;
      int      myUseVDBSourceFile;
      UT_String myVDBSourceFile;
      UT_String myVDBSourceGridName;
      int      myPostRasterType;
      fpreal   myPostDx;
      int      myPostFogVolume;
      fpreal   myPostGradientWidth;
      fpreal   myPostVoxelSize;
      fpreal   myPostRadiusMin;
      fpreal   myPostBandWidth;
      int      myPostWSUnits;
      fpreal   myPostVDBVelocityMult;
      fpreal   myPostVDBRadiusMult;
      fpreal   myPostFalloff;
      fpreal   myPostPosInfluence;
      fpreal   myPostNormalInfluence;
      fpreal   myPostVelInfluence;
      int      myPostVDBMedianFilter;
      int      myPostVDBMeanFilter;
      int      myPostVDBMeanCurvatureFilter;
      int      myPostVDBLaplacianFilter;
      int      myPostVDBMedianIterations;
      int      myPostVDBMeanIterations;
      int      myPostVDBMeanCurvatureIterations;
      int      myPostVDBLaplacianIterations;
      int      myPostVDBOffsetFilter;
      fpreal   myPostVDBOffsetFilterAmount;
      int      myPostVDBReNormalizeFilter;
      int      myPostVDBWriteVDBFiles;
      UT_String myVDBBaseFileName;

      // A struct to keep track os CVEX vars to pass to the CVEX code
      struct cvex_pt_vars_struct {
      uint  cvex_Cd_pt:
         1;
      uint  cvex_Alpha_pt:
         1;
      uint  cvex_v_pt:
         1;
      uint  cvex_N_pt:
         1;
      uint  cvex_pscale_pt:
         1;
      } myCVEXPointVars;

      struct cvex_prim_vars_struct {
      uint  cvex_Cd_prim:
         1;
      uint  cvex_Alpha_prim:
         1;
      uint  cvex_v_prim:
         1;
      uint  cvex_N_prim:
         1;
      uint  cvex_pscale_prim:
         1;
      uint  cvex_weight_prim:
         1;
      uint  cvex_width_prim:
         1;
      } myCVEXPrimVars;


      enum clusterPrimTypeEnum {
         CLUSTER_POINT = 0,
         CLUSTER_PRIM_SPHERE,
         CLUSTER_PRIM_CUBE,
         CLUSTER_PRIM_GRID,
         CLUSTER_PRIM_TUBE,
         CLUSTER_PRIM_CIRCLE,
         CLUSTER_PRIM_CURVE,
         CLUSTER_PRIM_METABALL,
         CLUSTER_FILE
      };

      enum clusterMotionBlurTypeEnum {
         CLUSTER_MB_NONE = 0,
         CLUSTER_MB_VELOCITY,
         CLUSTER_MB_DEFORMATION
      };

      enum clusterVerboseTypeEnum {
         CLUSTER_MSG_QUIET = 0,
         CLUSTER_MSG_INFO,
         CLUSTER_MSG_VERBOSE,
         CLUSTER_MSG_DEBUG
      };

      enum clusterCVEXMethod {
         CLUSTER_CVEX_POINT = 0,
         CLUSTER_CVEX_PRIM
      };

};



#endif




