// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _GeomConvert_BSplineSurfaceToBezierSurface_HeaderFile
#define _GeomConvert_BSplineSurfaceToBezierSurface_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _Handle_Geom_BSplineSurface_HeaderFile
#include <Handle_Geom_BSplineSurface.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Handle_Geom_BezierSurface_HeaderFile
#include <Handle_Geom_BezierSurface.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
class Geom_BSplineSurface;
class Standard_DimensionError;
class Standard_DomainError;
class Standard_OutOfRange;
class Geom_BezierSurface;
class TColGeom_Array2OfBezierSurface;
class TColStd_Array1OfReal;



//!  This algorithm converts a B-spline surface into several <br>
//!  Bezier surfaces. It uses an algorithm of knot insertion. <br>
//! A BSplineSurfaceToBezierSurface object provides a framework for: <br>
//! -   defining the BSpline surface to be converted, <br>
//! -   implementing the construction algorithm, and <br>
//! -   consulting the results. <br>
//!  References : <br>
//!  Generating the Bezier points of B-spline curves and surfaces <br>
//!  (Wolfgang Bohm) CAD volume 13 number 6 november 1981 <br>
class GeomConvert_BSplineSurfaceToBezierSurface  {
public:

  DEFINE_STANDARD_ALLOC

  //! Computes all the data needed to convert <br>
//! -   the BSpline surface BasisSurface into a series of adjacent Bezier surfaces. <br>
//! The result consists of a grid of BasisSurface patches <br>
//! limited by isoparametric curves corresponding to knot <br>
//! values, both in the u and v parametric directions of <br>
//! the surface. A row in the grid corresponds to a series <br>
//! of adjacent patches, all limited by the same two <br>
//! u-isoparametric curves. A column in the grid <br>
//! corresponds to a series of adjacent patches, all <br>
//! limited by the same two v-isoparametric curves. <br>
//! Use the available interrogation functions to ascertain <br>
//! the number of computed Bezier patches, and then to <br>
//! construct each individual Bezier surface (or all Bezier surfaces). <br>
//! Note: ParametricTolerance is not used. <br>
  Standard_EXPORT   GeomConvert_BSplineSurfaceToBezierSurface(const Handle(Geom_BSplineSurface)& BasisSurface);
  //! Computes all the data needed to convert <br>
//!   the patch of the BSpline surface BasisSurface <br>
//!   limited by the two parameter values U1 and U2 in <br>
//!   the u parametric direction, and by the two <br>
//!   parameter values V1 and V2 in the v parametric <br>
//!   direction, into a series of adjacent Bezier surfaces. <br>
//! The result consists of a grid of BasisSurface patches <br>
//! limited by isoparametric curves corresponding to knot <br>
//! values, both in the u and v parametric directions of <br>
//! the surface. A row in the grid corresponds to a series <br>
//! of adjacent patches, all limited by the same two <br>
//! u-isoparametric curves. A column in the grid <br>
//! corresponds to a series of adjacent patches, all <br>
//! limited by the same two v-isoparametric curves. <br>
//! Use the available interrogation functions to ascertain <br>
//! the number of computed Bezier patches, and then to <br>
//! construct each individual Bezier surface (or all Bezier surfaces). <br>
//! Note: ParametricTolerance is not used.  Raises DomainError <br>
//! if U1 or U2 or V1 or V2 are out of the parametric bounds <br>
//!  of the basis surface [FirstUKnotIndex, LastUKnotIndex] , <br>
//!  [FirstVKnotIndex, LastVKnotIndex] The tolerance criterion is <br>
//!  ParametricTolerance. <br>
//!  Raised if U2 - U1 <= ParametricTolerance or <br>
//!  V2 - V1 <= ParametricTolerance. <br>
  Standard_EXPORT   GeomConvert_BSplineSurfaceToBezierSurface(const Handle(Geom_BSplineSurface)& BasisSurface,const Standard_Real U1,const Standard_Real U2,const Standard_Real V1,const Standard_Real V2,const Standard_Real ParametricTolerance);
  //! Constructs and returns the Bezier surface of indices <br>
//! (UIndex, VIndex) to the patch grid computed on the <br>
//! BSpline surface analyzed by this algorithm. <br>
//! This Bezier surface has the same orientation as the <br>
//! BSpline surface analyzed in this framework. <br>
//! UIndex is an index common to a row in the patch <br>
//! grid. A row in the grid corresponds to a series of <br>
//! adjacent patches, all limited by the same two <br>
//! u-isoparametric curves of the surface. VIndex is an <br>
//! index common to a column in the patch grid. A column <br>
//! in the grid corresponds to a series of adjacent <br>
//! patches, all limited by the same two v-isoparametric <br>
//! curves of the surface. <br>
//! Exceptions <br>
//! Standard_OutOfRange if: <br>
//! -   UIndex is less than 1 or greater than the number <br>
//!   of rows in the patch grid computed on the BSpline <br>
//!   surface analyzed by this algorithm (as returned by <br>
//!   the function NbUPatches); or if <br>
//! -   VIndex is less than 1 or greater than the number <br>
//!   of columns in the patch grid computed on the <br>
//!   BSpline surface analyzed by this algorithm (as <br>
//!   returned by the function NbVPatches). <br>
  Standard_EXPORT     Handle_Geom_BezierSurface Patch(const Standard_Integer UIndex,const Standard_Integer VIndex) ;
  //! Constructs all the Bezier surfaces whose data is <br>
//! computed by this algorithm, and loads them into the Surfaces table. <br>
//! These Bezier surfaces have the same orientation as <br>
//! the BSpline surface analyzed in this framework. <br>
//! The Surfaces array is organised in the same way as <br>
//! the patch grid computed on the BSpline surface <br>
//! analyzed by this algorithm. A row in the array <br>
//! corresponds to a series of adjacent patches, all <br>
//! limited by the same two u-isoparametric curves of <br>
//! the surface. A column in the array corresponds to a <br>
//! series of adjacent patches, all limited by the same two <br>
//! v-isoparametric curves of the surface. <br>
//! Exceptions <br>
//! Standard_DimensionError if the Surfaces array <br>
//! was not created with the following bounds: <br>
//! -   1, and the number of adjacent patch series in the <br>
//!   u parametric direction of the patch grid computed <br>
//!   on the BSpline surface, analyzed by this algorithm <br>
//!   (as given by the function NbUPatches) as row bounds, <br>
//! -   1, and the number of adjacent patch series in the <br>
//!   v parametric direction of the patch grid computed <br>
//!   on the BSpline surface, analyzed by this algorithm <br>
//!   (as given by the function NbVPatches) as column bounds. <br>
  Standard_EXPORT     void Patches(TColGeom_Array2OfBezierSurface& Surfaces) ;
  //! This methode returns the bspline's u-knots associated to <br>
//!          the converted Patches <br>//! Raised  if the length  of Curves is not equal to <br>
//!  NbUPatches +  1. <br>
  Standard_EXPORT     void UKnots(TColStd_Array1OfReal& TKnots) const;
  //! This methode returns the bspline's v-knots associated to <br>
//!          the converted Patches <br>//! Raised  if the length  of Curves is not equal to <br>
//!  NbVPatches +  1. <br>
  Standard_EXPORT     void VKnots(TColStd_Array1OfReal& TKnots) const;
  
//!  Returns the number of Bezier surfaces in the U direction. <br>
//!  If at the creation time you have decomposed the basis Surface <br>
//!  between the parametric values UFirst, ULast the number of <br>
//!  Bezier surfaces in the U direction depends on the number of <br>
//!  knots included inside the interval [UFirst, ULast]. <br>
//!  If you have decomposed the whole basis B-spline surface the <br>
//!  number of Bezier surfaces NbUPatches is equal to the number of <br>
//!  UKnots less one. <br>
  Standard_EXPORT     Standard_Integer NbUPatches() const;
  
//!  Returns the number of Bezier surfaces in the V direction. <br>
//!  If at the creation time you have decomposed the basis surface <br>
//!  between the parametric values VFirst, VLast the number of <br>
//!  Bezier surfaces in the V direction depends on the number of <br>
//!  knots included inside the interval [VFirst, VLast]. <br>
//!  If you have decomposed the whole basis B-spline surface the <br>
//!  number of Bezier surfaces NbVPatches is equal to the number of <br>
//!  VKnots less one. <br>
  Standard_EXPORT     Standard_Integer NbVPatches() const;





protected:





private:



Handle_Geom_BSplineSurface mySurface;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
