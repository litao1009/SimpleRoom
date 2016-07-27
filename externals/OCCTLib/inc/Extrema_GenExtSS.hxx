// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Extrema_GenExtSS_HeaderFile
#define _Extrema_GenExtSS_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Handle_TColgp_HArray2OfPnt_HeaderFile
#include <Handle_TColgp_HArray2OfPnt.hxx>
#endif
#ifndef _Extrema_FuncExtSS_HeaderFile
#include <Extrema_FuncExtSS.hxx>
#endif
#ifndef _Adaptor3d_SurfacePtr_HeaderFile
#include <Adaptor3d_SurfacePtr.hxx>
#endif
class TColgp_HArray2OfPnt;
class StdFail_NotDone;
class Standard_OutOfRange;
class Standard_TypeMismatch;
class Adaptor3d_Surface;
class Extrema_POnSurf;


//! It calculates all the extremum distances <br>
//!          between two surfaces. <br>
//!          These distances can be minimum or maximum. <br>
class Extrema_GenExtSS  {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   Extrema_GenExtSS();
  //! It calculates all the distances. <br>
//!          The function F(u,v)=distance(S1(u1,v1),S2(u2,v2)) has an <br>
//!          extremum when gradient(F)=0. The algorithm searchs <br>
//!          all the zeros inside the definition ranges of the <br>
//!          surfaces. <br>
//!          NbU and NbV are used to locate the close points <br>
//!          to find the zeros. <br>
  Standard_EXPORT   Extrema_GenExtSS(const Adaptor3d_Surface& S1,const Adaptor3d_Surface& S2,const Standard_Integer NbU,const Standard_Integer NbV,const Standard_Real Tol1,const Standard_Real Tol2);
  //! It calculates all the distances. <br>
//!          The function F(u,v)=distance(P,S(u,v)) has an <br>
//!          extremum when gradient(F)=0. The algorithm searchs <br>
//!          all the zeros inside the definition ranges of the <br>
//!          surface. <br>
//!          NbU and NbV are used to locate the close points <br>
//!          to find the zeros. <br>
  Standard_EXPORT   Extrema_GenExtSS(const Adaptor3d_Surface& S1,const Adaptor3d_Surface& S2,const Standard_Integer NbU,const Standard_Integer NbV,const Standard_Real U1min,const Standard_Real U1sup,const Standard_Real V1min,const Standard_Real V1sup,const Standard_Real U2min,const Standard_Real U2sup,const Standard_Real V2min,const Standard_Real V2sup,const Standard_Real Tol1,const Standard_Real Tol2);
  
  Standard_EXPORT     void Initialize(const Adaptor3d_Surface& S2,const Standard_Integer NbU,const Standard_Integer NbV,const Standard_Real Tol2) ;
  
  Standard_EXPORT     void Initialize(const Adaptor3d_Surface& S2,const Standard_Integer NbU,const Standard_Integer NbV,const Standard_Real U2min,const Standard_Real U2sup,const Standard_Real V2min,const Standard_Real V2sup,const Standard_Real Tol2) ;
  //! the algorithm is done with S1 <br>
//!          An exception is raised if the fields have not <br>
//!          been initialized. <br>
  Standard_EXPORT     void Perform(const Adaptor3d_Surface& S1,const Standard_Real Tol1) ;
  //! the algorithm is done withS1 <br>
//!          An exception is raised if the fields have not <br>
//!          been initialized. <br>
  Standard_EXPORT     void Perform(const Adaptor3d_Surface& S1,const Standard_Real U1min,const Standard_Real U1sup,const Standard_Real V1min,const Standard_Real V1sup,const Standard_Real Tol1) ;
  //! Returns True if the distances are found. <br>
  Standard_EXPORT     Standard_Boolean IsDone() const;
  //! Returns the number of extremum distances. <br>
  Standard_EXPORT     Standard_Integer NbExt() const;
  //! Returns the value of the Nth resulting square distance. <br>
  Standard_EXPORT     Standard_Real SquareDistance(const Standard_Integer N) const;
  //! Returns the point of the Nth resulting distance. <br>
  Standard_EXPORT    const Extrema_POnSurf& PointOnS1(const Standard_Integer N) const;
  //! Returns the point of the Nth resulting distance. <br>
  Standard_EXPORT    const Extrema_POnSurf& PointOnS2(const Standard_Integer N) const;





protected:





private:

  
  Standard_EXPORT     Adaptor3d_SurfacePtr Bidon() const;


Standard_Boolean myDone;
Standard_Boolean myInit;
Standard_Real myu1min;
Standard_Real myu1sup;
Standard_Real myv1min;
Standard_Real myv1sup;
Standard_Real myu2min;
Standard_Real myu2sup;
Standard_Real myv2min;
Standard_Real myv2sup;
Standard_Integer myusample;
Standard_Integer myvsample;
Handle_TColgp_HArray2OfPnt mypoints1;
Handle_TColgp_HArray2OfPnt mypoints2;
Standard_Real mytol1;
Standard_Real mytol2;
Extrema_FuncExtSS myF;
Adaptor3d_SurfacePtr myS2;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
