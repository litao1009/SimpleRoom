// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _GeomLib_Interpolate_HeaderFile
#define _GeomLib_Interpolate_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _Handle_Geom_BSplineCurve_HeaderFile
#include <Handle_Geom_BSplineCurve.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _GeomLib_InterpolationErrors_HeaderFile
#include <GeomLib_InterpolationErrors.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
class Geom_BSplineCurve;
class StdFail_NotDone;
class Standard_OutOfRange;
class TColgp_Array1OfPnt;
class TColStd_Array1OfReal;


//! this class is used to construct a BSpline curve by <br>
//!          interpolation  of points  at given parameters  The <br>
//!          continuity   of the curve   is degree -  1 and the <br>
//!          method used when boundary  condition are not given <br>
//!          is to use odd degrees  and null the derivatives on <br>
//!          both sides from degree -1 down to (degree+1) / 2 <br>
//!          When even degree is given the returned curve is of <br>
//!          degree - 1 so that the degree of the curve is odd <br>
class GeomLib_Interpolate  {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   GeomLib_Interpolate(const Standard_Integer Degree,const Standard_Integer NumPoints,const TColgp_Array1OfPnt& Points,const TColStd_Array1OfReal& Parameters);
  
//! returns if everything went OK <br>
        Standard_Boolean IsDone() const;
  //! returns the error type if any <br>
        GeomLib_InterpolationErrors Error() const;
  //!  returns the interpolated curve of the requested degree <br>
  Standard_EXPORT     Handle_Geom_BSplineCurve Curve() const;





protected:





private:



Handle_Geom_BSplineCurve myCurve;
Standard_Boolean myIsDone;
GeomLib_InterpolationErrors myError;


};


#include <GeomLib_Interpolate.lxx>



// other Inline functions and methods (like "C++: function call" methods)


#endif
