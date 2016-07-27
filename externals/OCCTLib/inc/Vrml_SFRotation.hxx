// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Vrml_SFRotation_HeaderFile
#define _Vrml_SFRotation_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif


//! defines SFRotation type of VRML field types. <br>
//!          The  4  values  represent  an  axis  of  rotation  followed  by  amount  of <br>
//!          right-handed  rotation  about  the  that  axis, in  radians. <br>
class Vrml_SFRotation  {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   Vrml_SFRotation();
  
  Standard_EXPORT   Vrml_SFRotation(const Standard_Real aRotationX,const Standard_Real aRotationY,const Standard_Real aRotationZ,const Standard_Real anAngle);
  
  Standard_EXPORT     void SetRotationX(const Standard_Real aRotationX) ;
  
  Standard_EXPORT     Standard_Real RotationX() const;
  
  Standard_EXPORT     void SetRotationY(const Standard_Real aRotationY) ;
  
  Standard_EXPORT     Standard_Real RotationY() const;
  
  Standard_EXPORT     void SetRotationZ(const Standard_Real aRotationZ) ;
  
  Standard_EXPORT     Standard_Real RotationZ() const;
  
  Standard_EXPORT     void SetAngle(const Standard_Real anAngle) ;
  
  Standard_EXPORT     Standard_Real Angle() const;





protected:





private:



Standard_Real myRotationX;
Standard_Real myRotationY;
Standard_Real myRotationZ;
Standard_Real myAngle;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif