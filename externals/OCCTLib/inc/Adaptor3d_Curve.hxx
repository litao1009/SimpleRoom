// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Adaptor3d_Curve_HeaderFile
#define _Adaptor3d_Curve_HeaderFile

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
#ifndef _GeomAbs_Shape_HeaderFile
#include <GeomAbs_Shape.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Handle_Adaptor3d_HCurve_HeaderFile
#include <Handle_Adaptor3d_HCurve.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _GeomAbs_CurveType_HeaderFile
#include <GeomAbs_CurveType.hxx>
#endif
#ifndef _Handle_Geom_BezierCurve_HeaderFile
#include <Handle_Geom_BezierCurve.hxx>
#endif
#ifndef _Handle_Geom_BSplineCurve_HeaderFile
#include <Handle_Geom_BSplineCurve.hxx>
#endif
class Standard_OutOfRange;
class Standard_NoSuchObject;
class Standard_DomainError;
class TColStd_Array1OfReal;
class Adaptor3d_HCurve;
class gp_Pnt;
class gp_Vec;
class gp_Lin;
class gp_Circ;
class gp_Elips;
class gp_Hypr;
class gp_Parab;
class Geom_BezierCurve;
class Geom_BSplineCurve;


//! Root class for 3D curves on which geometric <br>
//! algorithms work. <br>
//! An adapted curve is an interface between the <br>
//! services provided by a curve and those required of <br>
//! the curve by algorithms which use it. <br>
//! Two derived concrete classes are provided: <br>
//! - GeomAdaptor_Curve for a curve from the Geom package <br>
//! - Adaptor3d_CurveOnSurface for a curve lying on <br>
//!   a surface from the Geom package. <br>
class Adaptor3d_Curve  {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   virtual  void Delete() ;
Standard_EXPORT virtual ~Adaptor3d_Curve(){Delete();}
  
  Standard_EXPORT   virtual  Standard_Real FirstParameter() const;
  
  Standard_EXPORT   virtual  Standard_Real LastParameter() const;
  
  Standard_EXPORT   virtual  GeomAbs_Shape Continuity() const;
  //! Returns  the number  of  intervals for  continuity <br>
//!          <S>. May be one if Continuity(me) >= <S> <br>
  Standard_EXPORT   virtual  Standard_Integer NbIntervals(const GeomAbs_Shape S) ;
  //! Stores in <T> the  parameters bounding the intervals <br>
//!          of continuity <S>. <br>
//! <br>
//!          The array must provide  enough room to  accomodate <br>
//!          for the parameters. i.e. T.Length() > NbIntervals() <br>
  Standard_EXPORT   virtual  void Intervals(TColStd_Array1OfReal& T,const GeomAbs_Shape S) ;
  //! Returns    a  curve equivalent   of  <me>  between <br>
//!          parameters <First>  and <Last>. <Tol>  is used  to <br>
//!          test for 3d points confusion. <br>//! If <First> >= <Last> <br>
  Standard_EXPORT   virtual  Handle_Adaptor3d_HCurve Trim(const Standard_Real First,const Standard_Real Last,const Standard_Real Tol) const;
  
  Standard_EXPORT   virtual  Standard_Boolean IsClosed() const;
  
  Standard_EXPORT   virtual  Standard_Boolean IsPeriodic() const;
  
  Standard_EXPORT   virtual  Standard_Real Period() const;
  //! Computes the point of parameter U on the curve. <br>
  Standard_EXPORT   virtual  gp_Pnt Value(const Standard_Real U) const;
  //! Computes the point of parameter U on the curve. <br>
  Standard_EXPORT   virtual  void D0(const Standard_Real U,gp_Pnt& P) const;
  //! Computes the point of parameter U on the curve with its <br>
//!  first derivative. <br>//! Raised if the continuity of the current interval <br>
//!  is not C1. <br>
  Standard_EXPORT   virtual  void D1(const Standard_Real U,gp_Pnt& P,gp_Vec& V) const;
  
//!  Returns the point P of parameter U, the first and second <br>
//!  derivatives V1 and V2. <br>//! Raised if the continuity of the current interval <br>
//!  is not C2. <br>
  Standard_EXPORT   virtual  void D2(const Standard_Real U,gp_Pnt& P,gp_Vec& V1,gp_Vec& V2) const;
  
//!  Returns the point P of parameter U, the first, the second <br>
//!  and the third derivative. <br>//! Raised if the continuity of the current interval <br>
//!  is not C3. <br>
  Standard_EXPORT   virtual  void D3(const Standard_Real U,gp_Pnt& P,gp_Vec& V1,gp_Vec& V2,gp_Vec& V3) const;
  
//!  The returned vector gives the value of the derivative for the <br>
//!  order of derivation N. <br>//! Raised if the continuity of the current interval <br>
//!  is not CN. <br>//! Raised if N < 1. <br>
  Standard_EXPORT   virtual  gp_Vec DN(const Standard_Real U,const Standard_Integer N) const;
  //!  Returns the parametric  resolution corresponding <br>
//!         to the real space resolution <R3d>. <br>
  Standard_EXPORT   virtual  Standard_Real Resolution(const Standard_Real R3d) const;
  //! Returns  the  type of the   curve  in the  current <br>
//!          interval :   Line,   Circle,   Ellipse, Hyperbola, <br>
//!          Parabola, BezierCurve, BSplineCurve, OtherCurve. <br>
  Standard_EXPORT   virtual  GeomAbs_CurveType GetType() const;
  
  Standard_EXPORT   virtual  gp_Lin Line() const;
  
  Standard_EXPORT   virtual  gp_Circ Circle() const;
  
  Standard_EXPORT   virtual  gp_Elips Ellipse() const;
  
  Standard_EXPORT   virtual  gp_Hypr Hyperbola() const;
  
  Standard_EXPORT   virtual  gp_Parab Parabola() const;
  
  Standard_EXPORT   virtual  Standard_Integer Degree() const;
  
  Standard_EXPORT   virtual  Standard_Boolean IsRational() const;
  
  Standard_EXPORT   virtual  Standard_Integer NbPoles() const;
  
  Standard_EXPORT   virtual  Standard_Integer NbKnots() const;
  
  Standard_EXPORT   virtual  Handle_Geom_BezierCurve Bezier() const;
  
  Standard_EXPORT   virtual  Handle_Geom_BSplineCurve BSpline() const;





protected:





private:





};





// other Inline functions and methods (like "C++: function call" methods)


#endif
