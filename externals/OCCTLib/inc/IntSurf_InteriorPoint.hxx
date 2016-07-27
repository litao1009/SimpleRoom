// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _IntSurf_InteriorPoint_HeaderFile
#define _IntSurf_InteriorPoint_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _gp_Pnt_HeaderFile
#include <gp_Pnt.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _gp_Vec_HeaderFile
#include <gp_Vec.hxx>
#endif
#ifndef _gp_Vec2d_HeaderFile
#include <gp_Vec2d.hxx>
#endif
class gp_Pnt;
class gp_Vec;
class gp_Vec2d;


//! Definition of a point solution of the <br>
//!          intersection between an implicit an a <br>
//!          parametrised surface. These points are <br>
//!          passing points on the intersection lines, <br>
//!          or starting points for the closed lines <br>
//!          on the parametrised surface. <br>
class IntSurf_InteriorPoint  {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   IntSurf_InteriorPoint();
  
  Standard_EXPORT   IntSurf_InteriorPoint(const gp_Pnt& P,const Standard_Real U,const Standard_Real V,const gp_Vec& Direc,const gp_Vec2d& Direc2d);
  
  Standard_EXPORT     void SetValue(const gp_Pnt& P,const Standard_Real U,const Standard_Real V,const gp_Vec& Direc,const gp_Vec2d& Direc2d) ;
  //! Returns the 3d coordinates of the interior point. <br>
       const gp_Pnt& Value() const;
  //! Returns the parameters of the interior point on the <br>
//!          parametric surface. <br>
        void Parameters(Standard_Real& U,Standard_Real& V) const;
  //! Returns the first parameter of the interior point on the <br>
//!          parametric surface. <br>
        Standard_Real UParameter() const;
  //! Returns the second parameter of the interior point on the <br>
//!          parametric surface. <br>
        Standard_Real VParameter() const;
  //! Returns the tangent at the intersection in 3d space <br>
//!          associated to the interior point. <br>
       const gp_Vec& Direction() const;
  //! Returns the tangent at the intersection in the parametric <br>
//!          space of the parametric surface. <br>
       const gp_Vec2d& Direction2d() const;





protected:





private:



gp_Pnt point;
Standard_Real paramu;
Standard_Real paramv;
gp_Vec direc;
gp_Vec2d direc2d;


};


#include <IntSurf_InteriorPoint.lxx>



// other Inline functions and methods (like "C++: function call" methods)


#endif
