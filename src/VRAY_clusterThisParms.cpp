/* ******************************************************************************
*
*  VRAY_clusterThisParms
*
*
* Description :
*
*
***************************************************************************** */

#ifndef __VRAY_clusterThisParms_cpp__
#define __VRAY_clusterThisParms_cpp__



/* ******************************************************************************
*  Function Name : theArgs()
*
*  Description : Data for the arguments...
*
*  Input Arguments : None
*
*  Return Value :
*
***************************************************************************** */
static VRAY_ProceduralArg theArgs[] = {

   VRAY_ProceduralArg("prim_type", "integer", "0"),
   VRAY_ProceduralArg("use_geo_file", "integer", "0"),
   VRAY_ProceduralArg("src_geo_file", "string", "default.bgeo"),
   VRAY_ProceduralArg("num_copy", "integer", "10"),
   VRAY_ProceduralArg("recursion", "integer", "2"),
   VRAY_ProceduralArg("radius",    "real",  "0.1"),
   VRAY_ProceduralArg("size",  "real", "0.01 0.01 0.01"),
   VRAY_ProceduralArg("freq",    "real",  "1.0 1.0 1.0"),
   VRAY_ProceduralArg("offset",    "real",  "0.0 0.0 0.0"),
   VRAY_ProceduralArg("birth_prob", "real",  "0.5"),
   VRAY_ProceduralArg("motion_blur", "integer", "0"),
   VRAY_ProceduralArg("backtrack_mb", "integer", "0"),
   VRAY_ProceduralArg("mb_shutter", "real", "0.1"),
   VRAY_ProceduralArg("mb_shutter2", "real", "0.9"),
   VRAY_ProceduralArg("verbose", "integer", "0"),
   VRAY_ProceduralArg("time", "real",  "0.0"),
   VRAY_ProceduralArg("frame", "real",  "0.0"),
   VRAY_ProceduralArg("noise_type", "integer", "0"),
   VRAY_ProceduralArg("noise_amp", "real", "0.1"),
   VRAY_ProceduralArg("noise_rough", "real", "0.1"),
   VRAY_ProceduralArg("noise_atten", "real", "0.0"),
   VRAY_ProceduralArg("noise_seed", "integer", "7"),
   VRAY_ProceduralArg("noise_fractal_depth", "integer", "3"),
   VRAY_ProceduralArg("geo_file", "string", "default.bgeo"),
   VRAY_ProceduralArg("filter_type", "integer", "0"),
   VRAY_ProceduralArg("filter_amp", "real", "0.0"),
   VRAY_ProceduralArg("temp_file_path", "string", "/tmp/geo/"),
   VRAY_ProceduralArg("temp_file", "integer", "0"),
   VRAY_ProceduralArg("write_stats", "integer", "0"),
   VRAY_ProceduralArg("stats_file_path", "string", "/tmp/render_stats.xml"),
   VRAY_ProceduralArg("save_temp_file", "integer", "0"),
   VRAY_ProceduralArg("bbox_fudge_factor", "real",  "0.001"),
   VRAY_ProceduralArg("otl_version", "string", "DCA_VERSION"),

   VRAY_ProceduralArg("CVEX_shader", "string", "default.vex"),
   VRAY_ProceduralArg("CVEX_exec", "integer", "0"),
   VRAY_ProceduralArg("CVEX_shader_prim", "string", "default.vex"),
   VRAY_ProceduralArg("CVEX_exec_prim", "integer", "0"),
   VRAY_ProceduralArg("CVEX_shader_pre", "string", "default.vex"),
   VRAY_ProceduralArg("CVEX_exec_pre", "integer", "0"),
   VRAY_ProceduralArg("CVEX_shader_post", "string", "default.vex"),
   VRAY_ProceduralArg("CVEX_exec_post", "integer", "0"),

   VRAY_ProceduralArg("Cd_pt", "integer", "0"),
   VRAY_ProceduralArg("Alpha_pt", "integer", "0"),
   VRAY_ProceduralArg("v_pt", "integer", "0"),
   VRAY_ProceduralArg("N_pt", "integer", "0"),
   VRAY_ProceduralArg("pscale_pt", "integer", "0"),

   VRAY_ProceduralArg("Cd_prim", "integer", "0"),
   VRAY_ProceduralArg("Alpha_prim", "integer", "0"),
   VRAY_ProceduralArg("v_prim", "integer", "0"),
   VRAY_ProceduralArg("N_prim", "integer", "0"),
   VRAY_ProceduralArg("pscale_prim", "integer", "0"),
   VRAY_ProceduralArg("weight_prim", "integer", "0"),
   VRAY_ProceduralArg("width_prim", "integer", "0"),

   VRAY_ProceduralArg("cvex_Cd_pt", "integer", "0"),
   VRAY_ProceduralArg("cvex_Alpha_pt", "integer", "0"),
   VRAY_ProceduralArg("cvex_v_pt", "integer", "0"),
   VRAY_ProceduralArg("cvex_N_pt", "integer", "0"),
   VRAY_ProceduralArg("cvex_pscale_pt", "integer", "0"),

   VRAY_ProceduralArg("cvex_Cd_prim", "integer", "0"),
   VRAY_ProceduralArg("cvex_Alpha_prim", "integer", "0"),
   VRAY_ProceduralArg("cvex_v_prim", "integer", "0"),
   VRAY_ProceduralArg("cvex_N_prim", "integer", "0"),
   VRAY_ProceduralArg("cvex_pscale_prim", "integer", "0"),
   VRAY_ProceduralArg("cvex_weight_prim", "integer", "0"),
   VRAY_ProceduralArg("cvex_width_prim", "integer", "0"),

   VRAY_ProceduralArg("post_process", "integer", "1"),
   VRAY_ProceduralArg("nn_post_process", "integer", "1"),
   VRAY_ProceduralArg("nn_post_pos_influence", "real", "0.1"),
   VRAY_ProceduralArg("nn_post_vel_influence", "real", "0.1"),
   VRAY_ProceduralArg("vdb_post_process", "integer", "1"),
   VRAY_ProceduralArg("post_use_vdb_file", "integer", "0"),
   VRAY_ProceduralArg("vdb_input_filename", "string", "default.vdb"),
   VRAY_ProceduralArg("vdb_source_grid_name", "string", "vdb_gradient"),
   VRAY_ProceduralArg("vdb_post_raster_type", "integer", "0"),
   VRAY_ProceduralArg("vdb_post_ws_units", "integer", "1"),
   VRAY_ProceduralArg("vdb_post_fog_volume", "integer", "0"),
   VRAY_ProceduralArg("vdb_post_dx", "real", "1.0"),
   VRAY_ProceduralArg("vdb_post_gradient_width", "real", "0.5"),
   VRAY_ProceduralArg("vdb_post_voxel_size", "real", "0.025"),
   VRAY_ProceduralArg("vdb_post_radius_min", "real", "1.5"),
   VRAY_ProceduralArg("vdb_post_radius_mult", "real", "1.0"),
   VRAY_ProceduralArg("vdb_post_velocity_mult", "real", "1.0"),
   VRAY_ProceduralArg("vdb_post_bandwidth", "real", "0.2"),
   VRAY_ProceduralArg("vdb_post_falloff", "real", "0.5"),
   VRAY_ProceduralArg("vdb_post_pos_influence", "real", "0.1"),
   VRAY_ProceduralArg("vdb_post_vel_influence", "real", "0.1"),
   VRAY_ProceduralArg("vdb_post_normal_influence", "real", "0.1"),
   VRAY_ProceduralArg("vdb_post_median_filter", "integer", "0"),
   VRAY_ProceduralArg("vdb_post_mean_filter", "integer", "0"),
   VRAY_ProceduralArg("vdb_post_mean_curvature_filter", "integer", "0"),
   VRAY_ProceduralArg("vdb_post_laplacian_filter", "integer", "0"),
   VRAY_ProceduralArg("vdb_post_median_iterations", "integer", "4"),
   VRAY_ProceduralArg("vdb_post_mean_iterations", "integer", "4"),
   VRAY_ProceduralArg("vdb_post_mean_curvature_iterations", "integer", "4"),
   VRAY_ProceduralArg("vdb_post_laplacian_iterations", "integer", "4"),
   VRAY_ProceduralArg("vdb_post_offset_filter", "integer", "0"),
   VRAY_ProceduralArg("vdb_post_offset_filter_amount", "real", "0.1"),
   VRAY_ProceduralArg("vdb_post_renormalize_filter", "integer", "0"),
   VRAY_ProceduralArg("vdb_post_write_vdb_files", "integer", "0"),
   VRAY_ProceduralArg("vdb_file_base_name", "string", "default.vdb"),

   VRAY_ProceduralArg()
};




/* ******************************************************************************
*  Function Name : getOTLParameters
*
*  Description : Get OTL parameters
*
*  Input Arguments : None
*
*  Return Value : None
*
***************************************************************************** */
int VRAY_clusterThis::getOTLParameters()
{

//   UT_String geo_fname, temp_fname, cvex_fname, otl_version;
   const int * int_ptr;
   const fpreal * flt_ptr;
   const char ** char_handle;

   if(int_ptr = VRAY_Procedural::getIParm("prim_type"))
      myPrimType = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("num_copy"))
      myNumCopies = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("recursion"))
      myRecursion = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("motion_blur"))
      myDoMotionBlur = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("backtrack_mb"))
      myUseBacktrackMB = *int_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("mb_shutter"))
      myShutter = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("mb_shutter2"))
      myShutter2 = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("radius"))
      myRadius = *flt_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("noise_type"))
      myNoiseType = *int_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("freq")) {
         myFreqX = *flt_ptr++;
         myFreqY = *flt_ptr++;
         myFreqZ = *flt_ptr;
      }

   if(flt_ptr = VRAY_Procedural::getFParm("offset")) {
         myOffsetX = *flt_ptr++;
         myOffsetY = *flt_ptr++;
         myOffsetZ = *flt_ptr;
      }

   if(flt_ptr = VRAY_Procedural::getFParm("noise_rough"))
      myRough = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("size")) {
         mySize[0] = *flt_ptr++;
         mySize[1] = *flt_ptr++;
         mySize[2] = *flt_ptr;
      }

   if(flt_ptr = VRAY_Procedural::getFParm("birth_prob"))
      myBirthProb = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("time"))
      myCurrentTime = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("frame"))
      myCurrentFrame = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("bbox_fudge_factor"))
      myBBoxFudgeFactor = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("noise_amp"))
      myNoiseAmp = *flt_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("filter_type"))
      myFilterType = *int_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("filter_amp"))
      myFilterAmp = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("noise_atten"))
      myNoiseAtten = *flt_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("noise_seed"))
      myNoiseSeed = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("noise_fractal_depth"))
      myFractalDepth = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("use_geo_file"))
      myUseGeoFile = *int_ptr;

   if(char_handle = VRAY_Procedural::getSParm("src_geo_file")) {
         mySrcGeoFname = (UT_String)(*char_handle);
         mySrcGeoFname.harden();
      }


   if(char_handle = VRAY_Procedural::getSParm("geo_file")) {
         myGeoFile = (UT_String)(*char_handle);
         myGeoFile.harden();
      }

   if(char_handle = VRAY_Procedural::getSParm("temp_file_path")) {
         myTempFname = (UT_String)(*char_handle);
         myTempFname.harden();
      }


   if(int_ptr = VRAY_Procedural::getIParm("temp_file"))
      myUseTempFile = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("save_temp_file"))
      mySaveTempFile =  *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("write_stats"))
      mySaveXMLFile = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("verbose"))
      myVerbose = *int_ptr;

   if(char_handle = VRAY_Procedural::getSParm("stats_file_path")) {
         myXMLFileName = (UT_String)(*char_handle);
         myXMLFileName.harden();
      }



//   if(char_handle = VRAY_Procedural::getSParm("otl_version")) {
//         myOTLVersion = (UT_String)(*char_handle);
//         myOTLVersion.harden();
//      }




   if(char_handle = VRAY_Procedural::getSParm("CVEX_shader")) {
         myCVEXFname = (UT_String)(*char_handle);
         myCVEXFname.harden();
      }


   if(int_ptr = VRAY_Procedural::getIParm("CVEX_exec"))
      myCVEX_Exec =  *int_ptr;


   if(char_handle = VRAY_Procedural::getSParm("CVEX_shader_prim")) {
         myCVEXFname_prim = (UT_String)(*char_handle);
         myCVEXFname_prim.harden();
      }

   if(int_ptr = VRAY_Procedural::getIParm("CVEX_exec_prim"))
      myCVEX_Exec_prim = *int_ptr;


   if(char_handle = VRAY_Procedural::getSParm("CVEX_shader_pre")) {
         myCVEXFname_pre = (UT_String)(*char_handle);
         myCVEXFname_pre.harden();
      }

   if(int_ptr = VRAY_Procedural::getIParm("CVEX_exec_pre"))
      myCVEX_Exec_pre = *int_ptr;

   if(char_handle = VRAY_Procedural::getSParm("CVEX_shader_post")) {
         myCVEXFname_post = (UT_String)(*char_handle);
         myCVEXFname_post.harden();
      }

   if(int_ptr = VRAY_Procedural::getIParm("CVEX_exec_post"))
      myCVEX_Exec_post = *int_ptr;


   if(int_ptr = VRAY_Procedural::getIParm("cvex_Cd_pt"))
      myCVEXPointVars.cvex_Cd_pt = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("cvex_Alpha_pt"))
      myCVEXPointVars.cvex_Alpha_pt = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("cvex_v_pt"))
      myCVEXPointVars.cvex_v_pt = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("cvex_N_pt"))
      myCVEXPointVars.cvex_N_pt = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("cvex_pscale_pt"))
      myCVEXPointVars.cvex_pscale_pt = *int_ptr;


   if(int_ptr = VRAY_Procedural::getIParm("cvex_Cd_prim"))
      myCVEXPrimVars.cvex_Cd_prim = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("cvex_Alpha_prim"))
      myCVEXPrimVars.cvex_Alpha_prim = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("cvex_v_prim"))
      myCVEXPrimVars.cvex_v_prim = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("cvex_N_prim"))
      myCVEXPrimVars.cvex_N_prim = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("cvex_pscale_prim"))
      myCVEXPrimVars.cvex_pscale_prim = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("cvex_weight_prim"))
      myCVEXPrimVars.cvex_weight_prim = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("cvex_width_prim"))
      myCVEXPrimVars.cvex_width_prim = *int_ptr;


   // Post processing parms
   if(int_ptr = VRAY_Procedural::getIParm("post_process"))
      myPostProcess = *int_ptr;

   // Nearest neighbor post processing parms
   if(int_ptr = VRAY_Procedural::getIParm("nn_post_process"))
      myNNPostProcess = *int_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("nn_post_pos_influence"))
      myNNPostPosInfluence = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("nn_post_vel_influence"))
      myNNPostVelInfluence = *flt_ptr;

   // VDB pre processing parms
   if(int_ptr = VRAY_Procedural::getIParm("vdb_post_process"))
      myVDBPostProcess = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("post_use_vdb_file"))
      myUseVDBSourceFile = *int_ptr;

   if(char_handle = VRAY_Procedural::getSParm("vdb_input_filename")) {
         myVDBSourceFile = (UT_String)(*char_handle);
         myVDBSourceFile.harden();
      }

   if(char_handle = VRAY_Procedural::getSParm("vdb_source_grid_name")) {
         myVDBSourceGridName = (UT_String)(*char_handle);
         myVDBSourceGridName.harden();
      }

   if(int_ptr = VRAY_Procedural::getIParm("vdb_post_raster_type"))
      myPostRasterType = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("vdb_post_ws_units"))
      myPostWSUnits = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("vdb_post_fog_volume"))
      myPostFogVolume = *int_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("vdb_post_dx"))
      myPostDx = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("vdb_post_gradient_width"))
      myPostGradientWidth = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("vdb_post_voxel_size"))
      myPostVoxelSize = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("vdb_post_radius_min"))
      myPostRadiusMin = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("vdb_post_radius_mult"))
      myPostVDBRadiusMult = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("vdb_post_velocity_mult"))
      myPostVDBVelocityMult = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("vdb_post_bandwidth"))
      myPostBandWidth = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("vdb_post_falloff"))
      myPostFalloff = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("vdb_post_pos_influence"))
      myPostPosInfluence = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("vdb_post_vel_influence"))
      myPostVelInfluence = *flt_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("vdb_post_normal_influence"))
      myPostNormalInfluence = *flt_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("vdb_post_median_filter"))
      myPostVDBMedianFilter = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("vdb_post_mean_filter"))
      myPostVDBMeanFilter = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("vdb_post_mean_curvature_filter"))
      myPostVDBMeanCurvatureFilter = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("vdb_post_laplacian_filter"))
      myPostVDBLaplacianFilter = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("vdb_post_median_iterations"))
      myPostVDBMedianIterations = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("vdb_post_mean_iterations"))
      myPostVDBMeanIterations = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("vdb_post_mean_curvature_iterations"))
      myPostVDBMeanCurvatureIterations = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("vdb_post_laplacian_iterations"))
      myPostVDBLaplacianIterations = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("vdb_post_offset_filter"))
      myPostVDBOffsetFilter = *int_ptr;

   if(flt_ptr = VRAY_Procedural::getFParm("vdb_post_offset_filter_amount"))
      myPostVDBOffsetFilterAmount = *flt_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("vdb_post_renormalize_filter"))
      myPostVDBReNormalizeFilter = *int_ptr;

   if(int_ptr = VRAY_Procedural::getIParm("vdb_post_write_vdb_files"))
      myPostVDBWriteVDBFiles = *int_ptr;

   if(char_handle = VRAY_Procedural::getSParm("vdb_file_base_name")) {
         myVDBBaseFileName = (UT_String)(*char_handle);
         myVDBBaseFileName.harden();
      }



   return 0;

}





/* ******************************************************************************
*  Function Name : dumpParameters()
*
*  Description :   Dump the user parameters to the console
*
*  Input Arguments : None
*
*  Return Value : None
*
***************************************************************************** */
void VRAY_clusterThis::dumpParameters()
{

   std::cout << std::endl;

   std::cout << "VRAY_clusterThis::dumpParameters() **** SETUP PARAMETERS ****" << std::endl;

   std::cout << "VRAY_clusterThis::dumpParameters() myPrimType: " << myPrimType << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myNumCopies: " << myNumCopies << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myUseGeoFile: " << myUseGeoFile << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() mySrcGeoFname: " << mySrcGeoFname << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myRecursion: " << myRecursion << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myRadius: " << myRadius << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myRough: " << myRough << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() mySize: " << mySize[0] << " " << mySize[1] << " " << mySize[2] << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myFreq: " << myFreqX << " " << myFreqY << " " << myFreqZ << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myOffset: " << myOffsetX << " " << myOffsetY << " " << myOffsetZ << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myBirthProb: " << myBirthProb << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCurrentTime: " << myCurrentTime << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myDoMotionBlur: " << myDoMotionBlur << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myShutter: " << myShutter << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myVerbose: " << myVerbose << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myGeoFile: " << myGeoFile << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myTempFname: " << myTempFname << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myUseTempFile: " << myUseTempFile << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() mySaveTempFile: " << mySaveTempFile << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCurrentFrame: " << myCurrentFrame << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCurrentTime: " << myCurrentTime << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myBBoxFudgeFactor: " << myBBoxFudgeFactor << std::endl;

   std::cout << "VRAY_clusterThis::dumpParameters() **** NOISE PARAMETERS ****" << std::endl;

   std::cout << "VRAY_clusterThis::dumpParameters() myNoiseType: " << myNoiseType << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myNoiseAmp: " << myNoiseAmp << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myNoiseAtten: " << myNoiseAtten << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myNoiseSeed: " << myNoiseSeed << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myFractalDepth: " << myFractalDepth << std::endl;


   std::cout << "VRAY_clusterThis::dumpParameters() **** CVEX PARAMETERS ****" << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEXFname: " << myCVEXFname << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEX_Exec: " << myCVEX_Exec << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEXFname_prim: " << myCVEXFname_prim << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEX_Exec_prim: " << myCVEX_Exec_prim << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEXFname_pre: " << myCVEXFname_pre << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEX_Exec_pre: " << myCVEX_Exec_pre << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEXFname_post: " << myCVEXFname_post << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEX_Exec_post: " << myCVEX_Exec_post << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEXPointVars.cvex_Cd_pt: " << myCVEXPointVars.cvex_Cd_pt << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEXPointVars.cvex_Alpha_pt: " << myCVEXPointVars.cvex_Alpha_pt << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEXPointVars.cvex_N_pt: " << myCVEXPointVars.cvex_Alpha_pt << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEXPointVars.cvex_v_pt: " << myCVEXPointVars.cvex_v_pt << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEXPointVars.cvex_pscale_pt: " << myCVEXPointVars.cvex_pscale_pt << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEXPrimVars.cvex_Cd_prim: " << myCVEXPrimVars.cvex_Cd_prim << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEXPrimVars.cvex_Alpha_prim: " << myCVEXPrimVars.cvex_Alpha_prim << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEXPrimVars.cvex_N_prim: " << myCVEXPrimVars.cvex_N_prim << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEXPrimVars.cvex_v_prim: " << myCVEXPrimVars.cvex_v_prim << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEXPrimVars.cvex_pscale_prim: " << myCVEXPrimVars.cvex_pscale_prim << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEXPrimVars.cvex_weight_prim: " << myCVEXPrimVars.cvex_weight_prim << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myCVEXPrimVars.cvex_width_prim: " << myCVEXPrimVars.cvex_width_prim << std::endl;

   std::cout << "VRAY_clusterThis::dumpParameters() **** POST PROCESS PARAMETERS ****" << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostProcess: " << myPostProcess << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myNNPostProcess: " << myNNPostProcess << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myNNPostPosInfluence: " << myNNPostPosInfluence << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myNNPostVelInfluence: " << myNNPostVelInfluence << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myVDBPostProcess: " << myVDBPostProcess << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myUseVDBSourceFile: " << myUseVDBSourceFile << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myVDBSourceFile: " << myVDBSourceFile << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myVDBSourceGridName: " << myVDBSourceGridName << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostRasterType: " << myPostRasterType << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostDx: " << myPostDx << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostFogVolume: " << myPostFogVolume << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostRadiusMin: " << myPostRadiusMin << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostVDBVelocityMult: " << myPostVDBVelocityMult << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostVDBRadiusMult: " << myPostVDBRadiusMult << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostGradientWidth: " << myPostGradientWidth << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostVoxelSize: " << myPostVoxelSize << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostBandWidth: " << myPostBandWidth << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostWSUnits: " << myPostWSUnits << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostFalloff: " << myPostFalloff << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostPosInfluence: " << myPostPosInfluence << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostVelInfluence: " << myPostVelInfluence << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostNormalInfluence: " << myPostNormalInfluence << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostVDBMedianFilter: " << myPostVDBMedianFilter << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostVDBMeanFilter: " << myPostVDBMeanFilter << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostVDBMeanCurvatureFilter: " << myPostVDBMeanCurvatureFilter << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostVDBLaplacianFilter: " << myPostVDBLaplacianFilter << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostVDBMedianIterations: " << myPostVDBMedianIterations << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostVDBMeanIterations: " << myPostVDBMeanIterations << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostVDBMeanCurvatureIterations: " << myPostVDBMeanCurvatureIterations << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostVDBLaplacianIterations: " << myPostVDBLaplacianIterations << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostVDBOffsetFilter: " << myPostVDBOffsetFilter << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostVDBOffsetFilterAmount: " << myPostVDBOffsetFilterAmount << std::endl;
//   std::cout << "VRAY_clusterThis::dumpParameters() myVDBReNormalizeFilter: " << myVDBReNormalizeFilter << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myPostVDBWriteVDBFiles: " << myPostVDBWriteVDBFiles << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myVDBBaseFileName: " << myVDBBaseFileName << std::endl;

   std::cout << "VRAY_clusterThis::dumpParameters() **** MISC PARAMETERS ****" << std::endl;
//   std::cout << "VRAY_clusterThis::dumpParameters() myOTLVersion: " << myOTLVersion << std::endl;
   std::cout << "VRAY_clusterThis::dumpParameters() myVelocityScale: " << myVelocityScale << std::endl;
//   std::cout << "VRAY_clusterThis::dumpParameters() myFilterType: " << myFilterType << std::endl;
//   std::cout << "VRAY_clusterThis::dumpParameters() myFilterAmp: " << myFilterAmp << std::endl;
//   std::cout << "VRAY_clusterThis::dumpParameters() myCopyAttrs: " << myCopyAttrs << std::endl;
//   std::cout << "VRAY_clusterThis::dumpParameters() myBlendAttrs: " << myBlendAttrs << std::endl;

   std::cout << std::endl;


}





#endif


