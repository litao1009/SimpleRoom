// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _gce_MakeRotation2d_HeaderFile
#define _gce_MakeRotation2d_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _gp_Trsf2d_HeaderFile
#include <gp_Trsf2d.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
class gp_Pnt2d;
class gp_Trsf2d;


//! Implements an elementary construction algorithm for <br>
//! a rotation in 2D space. The result is a gp_Trsf2d transformation. <br>
//! A MakeRotation2d object provides a framework for: <br>
//! -   defining the construction of the transformation, <br>
//! -   implementing the construction algorithm, and <br>
//! -   consulting the result. <br>
class gce_MakeRotation2d  {
public:

  DEFINE_STANDARD_ALLOC

  //! Constructs a rotation through angle Angle about the center Point. <br>
  Standard_EXPORT   gce_MakeRotation2d(const gp_Pnt2d& Point,const Standard_Real Angle);
  //! Returns the constructed transformation. <br>
  Standard_EXPORT    const gp_Trsf2d& Value() const;
  
  Standard_EXPORT    const gp_Trsf2d& Operator() const;
Standard_EXPORT operator gp_Trsf2d() const;





protected:





private:



gp_Trsf2d TheRotation2d;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
