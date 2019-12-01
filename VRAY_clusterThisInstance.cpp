/* ******************************************************************************
*
*  VRAY_clusterThisInstance
*
*
* Description : Instance an object for the point(s) passed to the method
*
*
***************************************************************************** */

#ifndef __VRAY_clusterThisInstance_cpp__
#define __VRAY_clusterThisInstance_cpp__


/* ******************************************************************************
*  Function Name : instancePoint()
*
*  Description : Instance a point
*
*  Input Arguments : GU_Detail *inst_gdp, GU_Detail *mb_gdp
*
*  Return Value : int
*
***************************************************************************** */
int VRAY_clusterThis::instancePoint(GU_Detail * inst_gdp, GU_Detail * mb_gdp)
{

#ifdef DEBUG
   std::cout << "VRAY_clusterThis::instancePoint()" << std::endl;
#endif

   GEO_Point * ppt;

   ppt = inst_gdp->appendPointElement();
   ppt->setPos((float)myPointAttributes.myNewPos[0],
               (float)myPointAttributes.myNewPos[1],
               (float)myPointAttributes.myNewPos[2], 1.0);

   VRAY_clusterThis::setPointInstanceAttributes(inst_gdp, ppt);

   if(myDoMotionBlur == CLUSTER_MB_DEFORMATION) {
         ppt = mb_gdp->appendPointElement();
         ppt->setPos((float)myPointAttributes.myMBPos[0],
                     (float)myPointAttributes.myMBPos[1],
                     (float)myPointAttributes.myMBPos[2], 1.0);

         VRAY_clusterThis::setPointInstanceAttributes(mb_gdp, ppt);
      }

   if(myCVEX_Exec)
      VRAY_clusterThis::runCVEX(inst_gdp, mb_gdp, myCVEXFname, CLUSTER_CVEX_POINT);

   return 0;
}



/* ******************************************************************************
*  Function Name : instanceSphere()
*
*  Description : Instance a sphere
*
*  Input Arguments : GU_Detail *inst_gdp, GU_Detail *mb_gdp
*
*  Return Value : int
*
***************************************************************************** */
int VRAY_clusterThis::instanceSphere(GU_Detail * inst_gdp, GU_Detail * mb_gdp)
{

#ifdef DEBUG
   std::cout << "VRAY_clusterThis::instanceSphere()" << std::endl;
#endif

   UT_Matrix4 xform(1.0);
   GU_PrimSphere * sphere;
   GU_PrimSphereParms sphere_parms;
   UT_XformOrder xformOrder(UT_XformOrder::SRT,  UT_XformOrder::XYZ);

   xform.xform(xformOrder, myPointAttributes.myNewPos[0], myPointAttributes.myNewPos[1], myPointAttributes.myNewPos[2],
               myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2],
               mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);

//   xform.scale(mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);
//   xform.rotate(myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2], xformOrder);
//   xform.translate(myPointAttributes.myNewPos[0], myPointAttributes.myNewPos[1], myPointAttributes.myNewPos[2]);

   sphere_parms.gdp = inst_gdp;
   sphere_parms.xform = xform;
   sphere = (GU_PrimSphere *) GU_PrimSphere::build(sphere_parms);
   VRAY_clusterThis::setInstanceAttributes(sphere);

   if(myDoMotionBlur == CLUSTER_MB_DEFORMATION) {
//         xform.xform(xformOrder, myPointAttributes.myMBPos[0], myPointAttributes.myMBPos[1], myPointAttributes.myMBPos[2],
//                     myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2],
//                     mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);
         xform.identity();
         xform.scale(mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);
         xform.rotate(myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2], xformOrder);
         xform.translate(myPointAttributes.myMBPos[0], myPointAttributes.myMBPos[1], myPointAttributes.myMBPos[2]);
         sphere_parms.gdp = mb_gdp;
         sphere_parms.xform = xform;
         sphere = (GU_PrimSphere *) GU_PrimSphere::build(sphere_parms);
         VRAY_clusterThis::setInstanceAttributes(sphere);
      }

   if(myCVEX_Exec)
      VRAY_clusterThis::runCVEX(inst_gdp, mb_gdp, myCVEXFname, CLUSTER_CVEX_POINT);

   if(myCVEX_Exec_prim)
      VRAY_clusterThis::runCVEX(inst_gdp, mb_gdp, myCVEXFname_prim, CLUSTER_CVEX_PRIM);

   return 0;
}



/* ******************************************************************************
*  Function Name : instanceCube()
*
*  Description : Instance a cube
*
*  Input Arguments : GU_Detail *inst_gdp, GU_Detail *mb_gdp
*
*  Return Value : int
*
***************************************************************************** */
int VRAY_clusterThis::instanceCube(GU_Detail * inst_gdp, GU_Detail * mb_gdp)
{
#ifdef DEBUG
   std::cout << "VRAY_clusterThis::instanceCube()" << std::endl;
#endif

   GEO_Primitive * myCube;
   UT_Matrix4 xform(1.0);
   UT_XformOrder xformOrder(UT_XformOrder::TRS,  UT_XformOrder::XYZ);
   xform.rotate(myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2], xformOrder);

   myCube = (GEO_Primitive *) inst_gdp->cube(
               myPointAttributes.myNewPos[0] - ((mySize[0] * myPointAttributes.pscale) / 2),
               myPointAttributes.myNewPos[0] + ((mySize[0] * myPointAttributes.pscale) / 2),
               myPointAttributes.myNewPos[1] - ((mySize[1] * myPointAttributes.pscale) / 2),
               myPointAttributes.myNewPos[1] + ((mySize[1] * myPointAttributes.pscale) / 2),
               myPointAttributes.myNewPos[2] - ((mySize[2] * myPointAttributes.pscale) / 2),
               myPointAttributes.myNewPos[2] + ((mySize[2] * myPointAttributes.pscale) / 2));

   for(int i = 0; i < myCube->getVertexCount(); i++) {
         GEO_Point * ppt = myCube->getVertexElement(i).getPt();
         UT_Vector4  P = ppt->getPos();
         P *= xform;
         ppt->setPos(P);
      }

   VRAY_clusterThis::setInstanceAttributes(myCube);

   if(myDoMotionBlur == CLUSTER_MB_DEFORMATION) {

         myCube = (GEO_Primitive *) mb_gdp->cube(myPointAttributes.myMBPos[0] - ((mySize[0] * myPointAttributes.pscale) / 2),
                                                 myPointAttributes.myMBPos[0] + ((mySize[0] * myPointAttributes.pscale) / 2),
                                                 myPointAttributes.myMBPos[1] - ((mySize[1] * myPointAttributes.pscale) / 2),
                                                 myPointAttributes.myMBPos[1] + ((mySize[1] * myPointAttributes.pscale) / 2),
                                                 myPointAttributes.myMBPos[2] - ((mySize[2] * myPointAttributes.pscale) / 2),
                                                 myPointAttributes.myMBPos[2] + ((mySize[2] * myPointAttributes.pscale) / 2));

         for(int i = 0; i < myCube->getVertexCount(); i++) {
               GEO_Point * ppt = myCube->getVertexElement(i).getPt();
               UT_Vector4  P = ppt->getPos();
               P *= xform;
               ppt->setPos(P);
            }

         VRAY_clusterThis::setInstanceAttributes(myCube);
      }

   if(myCVEX_Exec)
      VRAY_clusterThis::runCVEX(inst_gdp, mb_gdp, myCVEXFname, CLUSTER_CVEX_POINT);

   if(myCVEX_Exec_prim)
      VRAY_clusterThis::runCVEX(inst_gdp, mb_gdp, myCVEXFname_prim, CLUSTER_CVEX_PRIM);

   return 0;
}



/* ******************************************************************************
*  Function Name : instanceGrid()
*
*  Description : Instance a grid
*
*  Input Arguments : GU_Detail *inst_gdp, GU_Detail *mb_gdp
*
*  Return Value : int
*
***************************************************************************** */
int VRAY_clusterThis::instanceGrid(GU_Detail * inst_gdp, GU_Detail * mb_gdp)
{
#ifdef DEBUG
   std::cout << "VRAY_clusterThis::instanceGrid()" << std::endl;
#endif

   GEO_Primitive * myGrid;
   GU_GridParms grid_parms;
   UT_XformOrder xformOrder(UT_XformOrder::TRS,  UT_XformOrder::XYZ);
   UT_Matrix4 xform(1.0);

   xform.rotate(myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2], xformOrder);

   grid_parms.rows = 2;
   grid_parms.cols = 2;
   grid_parms.xsize = mySize[0] * myPointAttributes.pscale;
   grid_parms.ysize = mySize[1] * myPointAttributes.pscale;
   grid_parms.xcenter = myPointAttributes.myNewPos[0];
   grid_parms.ycenter = myPointAttributes.myNewPos[1];
   grid_parms.zcenter = myPointAttributes.myNewPos[2];
   grid_parms.plane = GU_PLANE_XY;
   myGrid = inst_gdp->buildGrid(grid_parms, GU_GRID_POLY);

   for(int i = 0; i < myGrid->getVertexCount(); i++) {
         GEO_Point * ppt = myGrid->getVertexElement(i).getPt();
         UT_Vector4  P = ppt->getPos();
         P *= xform;
         ppt->setPos(P);
      }

   VRAY_clusterThis::setInstanceAttributes(myGrid);

   if(myDoMotionBlur == CLUSTER_MB_DEFORMATION) {

         grid_parms.xcenter = myPointAttributes.myMBPos[0];
         grid_parms.ycenter = myPointAttributes.myMBPos[1];
         grid_parms.zcenter = myPointAttributes.myMBPos[2];
         myGrid = mb_gdp->buildGrid(grid_parms, GU_GRID_POLY);

         for(int i = 0; i < myGrid->getVertexCount(); i++) {
               GEO_Point * ppt = myGrid->getVertexElement(i).getPt();
               UT_Vector4  P = ppt->getPos();
               P *= xform;
               ppt->setPos(P);
            }

         VRAY_clusterThis::setInstanceAttributes(myGrid);
      }

   if(myCVEX_Exec)
      VRAY_clusterThis::runCVEX(inst_gdp, mb_gdp, myCVEXFname, CLUSTER_CVEX_POINT);

   if(myCVEX_Exec_prim)
      VRAY_clusterThis::runCVEX(inst_gdp, mb_gdp, myCVEXFname_prim, CLUSTER_CVEX_PRIM);

   return 0;
}


/* ******************************************************************************
*  Function Name : instanceTube()
*
*  Description : Instance a tube
*
*  Input Arguments : GU_Detail *inst_gdp, GU_Detail *mb_gdp
*
*  Return Value : int
*
***************************************************************************** */
int VRAY_clusterThis::instanceTube(GU_Detail * inst_gdp, GU_Detail * mb_gdp)
{
#ifdef DEBUG
   std::cout << "VRAY_clusterThis::instanceTube()" << std::endl;
#endif

   GU_PrimTube * tube;
   GU_PrimTubeParms tube_parms;
   GU_CapOptions tube_cap_options;
   UT_Matrix4 xform(1.0);
   UT_XformOrder xformOrder(UT_XformOrder::SRT,  UT_XformOrder::XYZ);

//    GU_CapOptions:
//GU_CapType   firstUCap
//GU_CapType   lastUCap
//GU_CapType   firstVCap
//GU_CapType   lastVCap
//int    pshapeU
//int    numfirstUCaps
//int    numlastUCaps
//int    numfirstVCaps
//int    numlastVCaps
//int    pshapeV
//float  firstUScale
//float  lastUScale
//float  firstVScale
//float  lastVScale

// enum     GU_CapType:
//GU_CAP_NONE = 0, GU_CAP_FACETED = 1, GU_CAP_SHARED = 2, GU_CAP_ROUNDED = 3, GU_CAP_TANGENT = 4
//
// GU_PrimTubeParms:
//int    rows
//int    cols
//int    orderu
//int    orderv
//int    imperfect
//float  taper
//GEO_SurfaceType    type
//
// GEO_SurfaceType:
//GEO_PATCH_ROWS
//GEO_PATCH_COLS
//GEO_PATCH_ROWCOL
//GEO_PATCH_TRIANGLE
//GEO_PATCH_QUADS
//GEO_PATCH_ALTTRIANGLE
//GEO_PATCH_REVTRIANGLE

   tube_cap_options.numfirstUCaps = 1;
   tube_cap_options.numlastUCaps = 1;
   tube_cap_options.firstUCap = GU_CAP_FACETED;
   tube_cap_options.lastUCap = GU_CAP_FACETED;
   tube_cap_options.numfirstVCaps = 1;
   tube_cap_options.numlastVCaps = 1;
   tube_cap_options.firstVCap = GU_CAP_FACETED;
   tube_cap_options.lastVCap = GU_CAP_FACETED;
   tube_parms.rows = 4;
   tube_parms.cols = 10;
   tube_parms.taper = 1.0;
   tube_parms.type = GEO_PATCH_TRIANGLE;

   xform.xform(xformOrder, myPointAttributes.myNewPos[0], myPointAttributes.myNewPos[1], myPointAttributes.myNewPos[2],
               myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2],
               mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);

//   xform.scale(mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);
//   xform.rotate(myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2], xformOrder);
//   xform.translate(myPointAttributes.myNewPos[0], myPointAttributes.myNewPos[1], myPointAttributes.myNewPos[2]);

//   tube_parms.xform.scale(mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);
//   tube_parms.xform.rotate(myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2], xformOrder);
//   tube_parms.xform.translate(myPointAttributes.myNewPos[0], myPointAttributes.myNewPos[1], myPointAttributes.myNewPos[2]);

   tube_parms.gdp = inst_gdp;
   tube_parms.xform = xform;
   tube = (GU_PrimTube *) GU_PrimTube::build(tube_parms, tube_cap_options);

   VRAY_clusterThis::setInstanceAttributes(tube);

   if(myDoMotionBlur == CLUSTER_MB_DEFORMATION) {
         xform.identity();
         xform.xform(xformOrder, myPointAttributes.myMBPos[0], myPointAttributes.myMBPos[1], myPointAttributes.myMBPos[2],
                     myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2],
                     mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);
//         xform.scale(mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);
//         xform.rotate(myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2], xformOrder);
//         xform.translate(myPointAttributes.myMBPos[0], myPointAttributes.myMBPos[1], myPointAttributes.myMBPos[2]);

//         tube_parms.xform.scale(mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);
//         tube_parms.xform.rotate(myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2], xformOrder);
//         tube_parms.xform.translate(myPointAttributes.myMBPos[0], myPointAttributes.myMBPos[1], myPointAttributes.myMBPos[2]);

         tube_parms.gdp = mb_gdp;
         tube = (GU_PrimTube *) GU_PrimTube::build(tube_parms, tube_cap_options);

         VRAY_clusterThis::setInstanceAttributes(tube);
      }

   if(myCVEX_Exec)
      VRAY_clusterThis::runCVEX(inst_gdp, mb_gdp, myCVEXFname, CLUSTER_CVEX_POINT);

   if(myCVEX_Exec_prim)
      VRAY_clusterThis::runCVEX(inst_gdp, mb_gdp, myCVEXFname_prim, CLUSTER_CVEX_PRIM);

   return 0;
}


/* ******************************************************************************
*  Function Name : instanceCircle()
*
*  Description :
*
*  Input Arguments : GU_Detail *inst_gdp, GU_Detail *mb_gdp
*
*  Return Value : int
*
***************************************************************************** */
int VRAY_clusterThis::instanceCircle(GU_Detail * inst_gdp, GU_Detail * mb_gdp)
{
#ifdef DEBUG
   std::cout << "VRAY_clusterThis::instanceCircle()" << std::endl;
#endif


   UT_Matrix4 xform(1.0);
   GU_PrimCircle * circle;
   GU_PrimCircleParms circle_parms;
   UT_XformOrder xformOrder(UT_XformOrder::SRT,  UT_XformOrder::XYZ);

//   xform.xform(xformOrder, myPointAttributes.myNewPos[0], myPointAttributes.myNewPos[1], myPointAttributes.myNewPos[2],
//               myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2],
//               mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);

   xform.scale(mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);
   xform.rotate(myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2], xformOrder);
   xform.translate(myPointAttributes.myNewPos[0], myPointAttributes.myNewPos[1], myPointAttributes.myNewPos[2]);

   circle_parms.gdp = inst_gdp;
   circle_parms.xform = xform;
   circle = (GU_PrimCircle *) GU_PrimCircle::build(circle_parms);
   VRAY_clusterThis::setInstanceAttributes(circle);

   if(myDoMotionBlur == CLUSTER_MB_DEFORMATION) {
         xform.identity();
         xform.scale(mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);
         xform.rotate(myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2], xformOrder);
         xform.translate(myPointAttributes.myMBPos[0], myPointAttributes.myMBPos[1], myPointAttributes.myMBPos[2]);
//         xform.xform(xformOrder, myPointAttributes.myMBPos[0], myPointAttributes.myMBPos[1], myPointAttributes.myMBPos[2],
//                     myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2],
//                     mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);
         circle_parms.gdp = mb_gdp;
         circle_parms.xform = xform;
         circle = (GU_PrimCircle *) GU_PrimCircle::build(circle_parms);
         VRAY_clusterThis::setInstanceAttributes(circle);
      }

   if(myCVEX_Exec)
      VRAY_clusterThis::runCVEX(inst_gdp, mb_gdp, myCVEXFname, CLUSTER_CVEX_POINT);

   if(myCVEX_Exec_prim)
      VRAY_clusterThis::runCVEX(inst_gdp, mb_gdp, myCVEXFname_prim, CLUSTER_CVEX_PRIM);

   return 0;
}



/* ******************************************************************************
*  Function Name : instanceCurve()
*
*  Description : Instance a curve
*
*  Input Arguments : GU_Detail *inst_gdp, GU_Detail *mb_gdp
*
*  Return Value : int
*
***************************************************************************** */
int VRAY_clusterThis::instanceCurve(GU_Detail * inst_gdp, GU_Detail * mb_gdp, fpreal theta, long int point_num)
{
#ifdef DEBUG
   std::cout << "VRAY_clusterThis::instanceCurve()" << std::endl;
#endif

   GEO_Point * ppt;
   GU_PrimNURBCurve * myCurve, * myMBCurve;
   uint32 num_vtx;
   int myCurvePointNum = 0;
   UT_Vector4 pt_pos;
   GU_Detail * curve_gdp, * curve_mb_gdp;

   curve_gdp = allocateGeometry();

   GA_RWAttributeRef prim_Cd = curve_gdp->addDiffuseAttribute(GEO_PRIMITIVE_DICT);
   GA_RWAttributeRef prim_Alpha = curve_gdp->addAlphaAttribute(GEO_PRIMITIVE_DICT);
   GA_RWAttributeRef prim_v = curve_gdp->addVelocityAttribute(GEO_PRIMITIVE_DICT);
   GA_RWAttributeRef prim_N = curve_gdp->addNormalAttribute(GEO_PRIMITIVE_DICT);
   GA_RWAttributeRef prim_material = curve_gdp->addStringTuple(GEO_PRIMITIVE_DICT, "shop_materialpath", 1);

   GA_RWAttributeRef pt_Cd = curve_gdp->addDiffuseAttribute(GEO_POINT_DICT);
   GA_RWAttributeRef pt_Alpha = curve_gdp->addAlphaAttribute(GEO_POINT_DICT);
   GA_RWAttributeRef pt_v = curve_gdp->addVelocityAttribute(GEO_POINT_DICT);
   GA_RWAttributeRef pt_N = curve_gdp->addNormalAttribute(GEO_POINT_DICT);
   GA_RWAttributeRef pt_pscale = curve_gdp->addFloatTuple(GA_ATTRIB_POINT, "pscale", 1);
   GA_RWAttributeRef pt_width = curve_gdp->addFloatTuple(GA_ATTRIB_POINT, "width", 1);
   GA_RWAttributeRef pt_id = curve_gdp->addIntTuple(GA_ATTRIB_POINT, "id", 1);
   GA_RWAttributeRef pt_instId = curve_gdp->addIntTuple(GA_ATTRIB_POINT, "inst_id", 1);
   GA_RWAttributeRef pt_material = curve_gdp->addStringTuple(GA_ATTRIB_POINT, "shop_materialpath", 1);

   GA_RWAttributeRef pt_mb_Cd;
   GA_RWAttributeRef pt_mb_Alpha;
   GA_RWAttributeRef pt_mb_v;
   GA_RWAttributeRef pt_mb_N;
   GA_RWAttributeRef pt_mb_pscale;
   GA_RWAttributeRef pt_mb_width;
   GA_RWAttributeRef pt_mb_id;
   GA_RWAttributeRef pt_mb_instId;
   GA_RWAttributeRef pt_mb_material;

   num_vtx = ((myNumCopies * myRecursion) > 4) ? (myNumCopies * myRecursion) : 4;
   myCurve = (GU_PrimNURBCurve *)GU_PrimNURBCurve::build((GU_Detail *)curve_gdp, num_vtx, 4, 0, 1, 1);

   myCurve->setValue<UT_Vector3>(prim_Cd, (const UT_Vector3)myPointAttributes.Cd);
   myCurve->setValue<fpreal>(prim_Alpha, (const fpreal)myPointAttributes.Alpha);
   myCurve->setString(prim_material, myPointAttributes.material);

//   cout << "VRAY_clusterThis::instanceCurve() - num vertices: " << myCurve->getVertexCount() << endl;;
//   cout << "id: "  << myCurve->getPrimitiveId() << endl;
//   cout << "breakCount: "  << myCurve->breakCount() << endl;

   if(myDoMotionBlur == CLUSTER_MB_DEFORMATION) {
         curve_mb_gdp = allocateGeometry();
         myMBCurve = (GU_PrimNURBCurve *)GU_PrimNURBCurve::build((GU_Detail *)curve_mb_gdp, num_vtx, 4, 0, 1, 1);
         pt_mb_Cd = curve_gdp->addDiffuseAttribute(GEO_POINT_DICT);
         pt_mb_Alpha = curve_gdp->addAlphaAttribute(GEO_POINT_DICT);
         pt_mb_v = curve_gdp->addVelocityAttribute(GEO_POINT_DICT);
         pt_mb_N = curve_gdp->addNormalAttribute(GEO_POINT_DICT);
         pt_mb_pscale = curve_gdp->addFloatTuple(GA_ATTRIB_POINT, "pscale", 1);
         pt_mb_width = curve_gdp->addFloatTuple(GA_ATTRIB_POINT, "width", 1);
         pt_mb_id = curve_gdp->addIntTuple(GA_ATTRIB_POINT, "id", 1);
         pt_mb_instId = curve_gdp->addIntTuple(GA_ATTRIB_POINT, "inst_id", 1);
         pt_mb_material = curve_gdp->addStringTuple(GA_ATTRIB_POINT, "shop_materialpath", 1);
      }


   for(int copyNum = 0; copyNum < myNumCopies; copyNum++)
      for(int recursionNum = 0; recursionNum < myRecursion; recursionNum++) {

            VRAY_clusterThis::calculateNewPosition(theta, copyNum, recursionNum);
            ppt = myCurve->getVertexElement(myCurvePointNum).getPt();
            ppt->setPos((float)myPointAttributes.myNewPos[0],
                        (float)myPointAttributes.myNewPos[1],
                        (float)myPointAttributes.myNewPos[2], 1.0);

            // Assign attributes to each point
            ppt->setValue<UT_Vector3>(pt_Cd, (const UT_Vector3)myPointAttributes.Cd);
            ppt->setValue<fpreal>(pt_Alpha, (const fpreal)myPointAttributes.Alpha);
            ppt->setValue<UT_Vector3>(pt_v, (const UT_Vector3)myPointAttributes.v);
            ppt->setValue<UT_Vector3>(pt_N, (const UT_Vector3)myPointAttributes.N);
            ppt->setValue<fpreal>(pt_pscale, (const fpreal)myPointAttributes.pscale);
            ppt->setValue<fpreal>(pt_width, (const fpreal)myPointAttributes.width);
            ppt->setValue<int>(pt_id, (const int)myPointAttributes.id);
            ppt->setValue<int>(pt_instId, (const int)myCurvePointNum);
            ppt->setString(pt_material, myPointAttributes.material);

            if(myDoMotionBlur == CLUSTER_MB_DEFORMATION) {
                  ppt = myMBCurve->getVertexElement(myCurvePointNum).getPt();
                  ppt->setPos((float)myPointAttributes.myNewPos[0],
                              (float)myPointAttributes.myMBPos[1],
                              (float)myPointAttributes.myMBPos[2], 1.0);

                  // Assign attributes to each point
                  ppt->setValue<UT_Vector3>(pt_mb_Cd, (const UT_Vector3)myPointAttributes.Cd);
                  ppt->setValue<fpreal>(pt_mb_Alpha, (const fpreal)myPointAttributes.Alpha);
                  ppt->setValue<UT_Vector3>(pt_mb_v, (const UT_Vector3)myPointAttributes.v);
                  ppt->setValue<UT_Vector3>(pt_mb_N, (const UT_Vector3)myPointAttributes.N);
                  ppt->setValue<fpreal>(pt_mb_pscale, (const fpreal)myPointAttributes.pscale);
                  ppt->setValue<fpreal>(pt_mb_width, (const fpreal)myPointAttributes.width);
                  ppt->setValue<int>(pt_mb_id, (const int)myPointAttributes.id);
                  ppt->setValue<int>(pt_mb_instId, (const int)myInstanceNum);
                  ppt->setString(pt_mb_material, myPointAttributes.material);
               }
//            std::cout << "VRAY_clusterThis::instanceCurve() myCurvePointNum: " << myCurvePointNum << std::endl;
            myCurvePointNum++;
         }

//   myCurve->close();
//   myMBCurve->close ();

   if(myCVEX_Exec)
      VRAY_clusterThis::runCVEX(curve_gdp, curve_mb_gdp, myCVEXFname, CLUSTER_CVEX_POINT);

   if(myCVEX_Exec_prim)
      VRAY_clusterThis::runCVEX(curve_gdp, curve_mb_gdp, myCVEXFname_prim, CLUSTER_CVEX_PRIM);

   inst_gdp->merge(*curve_gdp);
   VRAY_Procedural::freeGeometry(curve_gdp);
   if(myDoMotionBlur == CLUSTER_MB_DEFORMATION) {
         mb_gdp->merge(*curve_mb_gdp);
         VRAY_Procedural::freeGeometry(curve_mb_gdp);
      }

   return 0;
}



/* ******************************************************************************
*  Function Name : instanceMetaball()
*
*  Description : Instance a metaball
*
*  Input Arguments : GU_Detail *inst_gdp, GU_Detail *mb_gdp
*
*  Return Value : int
*
***************************************************************************** */
int VRAY_clusterThis::instanceMetaball(GU_Detail * inst_gdp, GU_Detail * mb_gdp)
{

#ifdef DEBUG
   std::cout << "VRAY_clusterThis::instanceMetaball()" << std::endl;
#endif

   UT_Matrix4 xform(1.0);
   UT_XformOrder xformOrder(UT_XformOrder::SRT,  UT_XformOrder::XYZ);
   GU_PrimMetaBall * metaball;
   GU_PrimMetaBallParms metaball_parms;

   xform.xform(xformOrder, myPointAttributes.myNewPos[0], myPointAttributes.myNewPos[1], myPointAttributes.myNewPos[2],
               myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2],
               mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);

   metaball_parms.gdp = inst_gdp;
   metaball_parms.xform = xform;
   metaball_parms.weight = myPointAttributes.weight;
   metaball = (GU_PrimMetaBall *) GU_PrimMetaBall::build(metaball_parms);
   VRAY_clusterThis::setInstanceAttributes(metaball);

   if(myDoMotionBlur == CLUSTER_MB_DEFORMATION) {
         metaball_parms.weight = myPointAttributes.weight;
         xform.identity();
         xform.xform(xformOrder, myPointAttributes.myMBPos[0], myPointAttributes.myMBPos[1], myPointAttributes.myMBPos[2],
                     myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2],
                     mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);
         metaball_parms.gdp = mb_gdp;
         metaball_parms.xform = xform;
         metaball = (GU_PrimMetaBall *) GU_PrimMetaBall::build(metaball_parms);
         VRAY_clusterThis::setInstanceAttributes(metaball);
      }

   if(myCVEX_Exec)
      VRAY_clusterThis::runCVEX(inst_gdp, mb_gdp, myCVEXFname, CLUSTER_CVEX_POINT);

   if(myCVEX_Exec_prim)
      VRAY_clusterThis::runCVEX(inst_gdp, mb_gdp, myCVEXFname_prim, CLUSTER_CVEX_PRIM);

   return 0;
}


/* ******************************************************************************
*  Function Name : instanceFile()
*
*  Description :  Instance the geometry from a BGEO file
*
*  Input Arguments : GU_Detail *file_gdp, GU_Detail *inst_gdp, GU_Detail *mb_gdp
*
*  Return Value : int
*
***************************************************************************** */
int VRAY_clusterThis::instanceFile(GU_Detail * file_gdp, GU_Detail * inst_gdp, GU_Detail * mb_gdp)
{
#ifdef DEBUG
   std::cout << "VRAY_clusterThis::instanceFile()" << std::endl;
   cout << "VRAY_clusterThis::instanceFile() myPointAttributes.geo_fname: " << myPointAttributes.geo_fname << endl;
#endif

#define USE_POINT_FNAME 1

   GU_Detail null_gdp;
   UT_Matrix4 xform(1.0);
   UT_Matrix3 rot_xform(1.0);
//   UT_XformOrder xformOrder(UT_XformOrder::SRT,  UT_XformOrder::XYZ);

#ifdef USE_POINT_FNAME
   GU_Detail * file_geo_gdp;
   file_geo_gdp = VRAY_Procedural::allocateGeometry();
   if(!file_geo_gdp->load((const char *)myPointAttributes.geo_fname).success())
      throw VRAY_clusterThis_Exception("VRAY_clusterThis::instanceFile() Failed to load geometry file ", 1);
   GU_Detail temp_gdp(file_geo_gdp);
#else
   GU_Detail temp_gdp(file_gdp);
#endif

   UT_Vector3 myDir = myPointAttributes.N;
   UT_Vector3 myUp = UT_Vector3(0, 1, 0);

// Transform the geo to the new position
   rot_xform.orient(myDir, myUp);
   xform = rot_xform;

   xform.scale(mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);
//    xform.rotate(myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2], xformOrder);
   xform.translate(myPointAttributes.myNewPos[0], myPointAttributes.myNewPos[1], myPointAttributes.myNewPos[2]);
//   xform.xform(xformOrder, myPointAttributes.myNewPos[0], myPointAttributes.myNewPos[1], myPointAttributes.myNewPos[2],
//               myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2],
//               mySize[0], mySize[1], mySize[2]);

   temp_gdp.transform(xform);

//   GU_Detail theFileGDP(theFiles[myPointAttributes.lod][myPointAttributes.anim_frame]);
   addFileAttributeRefs(&temp_gdp);
   setFileAttributes(&temp_gdp);

   // Run CVEX function on this instance
   if(myCVEX_Exec)
      VRAY_clusterThis::runCVEX(&temp_gdp, &null_gdp, myCVEXFname, CLUSTER_CVEX_POINT);
   if(myCVEX_Exec_prim)
      VRAY_clusterThis::runCVEX(&temp_gdp, &null_gdp, myCVEXFname_prim, CLUSTER_CVEX_PRIM);

   inst_gdp->merge(temp_gdp);


   if(myDoMotionBlur == CLUSTER_MB_DEFORMATION) {
#ifdef USE_POINT_FNAME
         GU_Detail temp_gdp(file_geo_gdp);
#else
         GU_Detail temp_gdp(file_gdp);
#endif

         xform.identity();
         rot_xform.identity();
         rot_xform.orient(myDir, myUp);
         xform = rot_xform;

         xform.scale(mySize[0] * myPointAttributes.pscale, mySize[1] * myPointAttributes.pscale, mySize[2] * myPointAttributes.pscale);
//        xform.rotate(myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2], xformOrder);
         xform.translate(myPointAttributes.myMBPos[0], myPointAttributes.myMBPos[1], myPointAttributes.myMBPos[2]);
         //   xform.xform(xformOrder, myPointAttributes.myMBPos[0], myPointAttributes.myMBPos[1], myPointAttributes.myMBPos[2],
         //               myPointAttributes.N[0], myPointAttributes.N[1], myPointAttributes.N[2],
         //               mySize[0], mySize[1], mySize[2]);

         // Run CVEX function on this instance
         if(myCVEX_Exec)
            VRAY_clusterThis::runCVEX(&temp_gdp, &null_gdp, myCVEXFname, CLUSTER_CVEX_POINT);
         if(myCVEX_Exec_prim)
            VRAY_clusterThis::runCVEX(&temp_gdp, &null_gdp, myCVEXFname_prim, CLUSTER_CVEX_PRIM);

         temp_gdp.transform(xform);
         mb_gdp->merge(temp_gdp);
      }


#ifdef USE_POINT_FNAME
   VRAY_Procedural::freeGeometry(file_geo_gdp);
#endif

   return 0;
}


#endif

