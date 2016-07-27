// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _GeomLib_DenominatorMultiplier_HeaderFile
#define _GeomLib_DenominatorMultiplier_HeaderFile

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
#ifndef _TColStd_Array1OfReal_HeaderFile
#include <TColStd_Array1OfReal.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
class Geom_BSplineSurface;
class Standard_OutOfRange;
class Standard_ConstructionError;
class TColStd_Array1OfReal;


//! this class is used to  construct the BSpline curve <br>
//!          from an Approximation ( ApproxAFunction from AdvApprox). <br>
class GeomLib_DenominatorMultiplier  {
public:

  DEFINE_STANDARD_ALLOC

  //! if the surface is rational this will define the evaluator <br>
//!          of a real function of 2 variables a(u,v) such that <br>
//!          if we define a new surface by : <br>
//!                       a(u,v) * N(u,v) <br>
//!          NewF(u,v) = ---------------- <br>
//!                       a(u,v) * D(u,v) <br>
  Standard_EXPORT   GeomLib_DenominatorMultiplier(const Handle(Geom_BSplineSurface)& Surface,const TColStd_Array1OfReal& KnotVector);
  //! Returns the value of <br>
//!          a(UParameter,VParameter)= <br>
//! <br>
//!            H0(UParameter)/Denominator(Umin,Vparameter) <br>
//! <br>
//!            D Denominator(Umin,Vparameter) <br>
//!          - ------------------------------[H1(u)]/(Denominator(Umin,Vparameter)^2) <br>
//!            D U <br>
//! <br>
//!          + H3(UParameter)/Denominator(Umax,Vparameter) <br>
//! <br>
//!            D Denominator(Umax,Vparameter) <br>
//!          - ------------------------------[H2(u)]/(Denominator(Umax,Vparameter)^2) <br>
//!            D U <br>
  Standard_EXPORT     Standard_Real Value(const Standard_Real UParameter,const Standard_Real VParameter) const;





protected:





private:



Handle_Geom_BSplineSurface mySurface;
TColStd_Array1OfReal myKnotFlatVector;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
