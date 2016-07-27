// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _PGeom_Vector_HeaderFile
#define _PGeom_Vector_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_PGeom_Vector_HeaderFile
#include <Handle_PGeom_Vector.hxx>
#endif

#ifndef _gp_Vec_HeaderFile
#include <gp_Vec.hxx>
#endif
#ifndef _PGeom_Geometry_HeaderFile
#include <PGeom_Geometry.hxx>
#endif
class gp_Vec;


class PGeom_Vector : public PGeom_Geometry {

public:

  //! Set the field vec. <br>
  Standard_EXPORT     void Vec(const gp_Vec& aVec) ;
  //! Returns the value of the field vec. <br>
  Standard_EXPORT     gp_Vec Vec() const;

PGeom_Vector(const Storage_stCONSTclCOM& a) : PGeom_Geometry(a)
{
  
}
    const gp_Vec& _CSFDB_GetPGeom_Vectorvec() const { return vec; }



  DEFINE_STANDARD_RTTI(PGeom_Vector)

protected:

  //! Initializes the field(s) with default value(s). <br>
  Standard_EXPORT   PGeom_Vector();
  //! Initializes the field vec with <aVec>. <br>
  Standard_EXPORT   PGeom_Vector(const gp_Vec& aVec);

gp_Vec vec;


private: 




};





// other Inline functions and methods (like "C++: function call" methods)


#endif
