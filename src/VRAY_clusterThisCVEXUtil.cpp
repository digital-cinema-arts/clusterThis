/* ******************************************************************************
*
*  VRAY_clusterThisCVEXUtils
*
*
* Description :
*
*
***************************************************************************** */

#ifndef __VRAY_clusterThisCVEXUtil_cpp__
#define __VRAY_clusterThisCVEXUtil_cpp__


/*

static void dumpInt(int *v, int n)
{
    int     i;
    printf("%d", v[0]);
    for (i = 1; i < n; i++)
        printf(", %d", v[i]);
}

static void dumpFloat(const fpreal32 *v, int n)
{
    int     i;
    printf("%g", v[0]);
    for (i = 1; i < n; i++)
        printf(", %g", v[i]);
}

static void dumpVector(const UT_Vector3 *v, int n)
{
    int     i;
    printf("{%g,%g,%g}", v[0].x(), v[0].y(), v[0].z());
    for (i = 1; i < n; i++)
        printf(", {%g,%g,%g}", v[i].x(), v[i].y(), v[i].z());
}




static void dumpValue(CVEX_Value *value)
{
    if (!value) {
        cout << "Not a valid CVEX value" << endl;
        return;
    }

//     if (!value->isExport()) {
//         cout << value->getName() << " CVEX value is not exported" << endl;
//         return;
//     }

    printf("%s = [", value->getName());
    switch (value->getType())
    {
    case CVEX_TYPE_INTEGER:
        dumpInt((int *)value->getData(), value->getArraySize());
        break;
    case CVEX_TYPE_FLOAT:
        dumpFloat((fpreal32 *)value->getData(), value->getArraySize());
        break;
    case CVEX_TYPE_VECTOR3:
        dumpVector((UT_Vector3 *)value->getData(), value->getArraySize());
        break;
    default:
        printf("No output supported currently\n");
    }
    printf("]\n");
}

*/



static void dumpValueList(const char * label, CVEX_ValueList & list)
{
   int      i;
   CVEX_Value * value;
   printf("%s:\n", label);
   for(i = 0; i < list.entries(); i++) {
         value = list.getValue(i);
         printf("\t%2d.", i + 1);
         if(value->isExport())
            printf("export ");
         switch(value->getType()) {
               case CVEX_TYPE_INTEGER:
                  printf("int");
                  break;
               case CVEX_TYPE_FLOAT:
                  printf("float");
                  break;
               case CVEX_TYPE_VECTOR3:
                  printf("vector");
                  break;
               case CVEX_TYPE_VECTOR4:
                  printf("vector4");
                  break;
               case CVEX_TYPE_MATRIX3:
                  printf("matrix3");
                  break;
               case CVEX_TYPE_MATRIX4:
                  printf("matrix");
                  break;
               case CVEX_TYPE_STRING:
                  printf("string");
                  break;
               default:
                  printf("unknown");
                  break;
            }
         printf(" %s[%d]\n", value->getName(), value->getArraySize());
      }
}


#endif

