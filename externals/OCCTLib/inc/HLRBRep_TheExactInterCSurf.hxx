// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _HLRBRep_TheExactInterCSurf_HeaderFile
#define _HLRBRep_TheExactInterCSurf_HeaderFile

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
#ifndef _HLRBRep_TheCSFunctionOfInterCSurf_HeaderFile
#include <HLRBRep_TheCSFunctionOfInterCSurf.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
class StdFail_NotDone;
class Standard_DomainError;
class HLRBRep_SurfaceTool;
class gp_Lin;
class HLRBRep_LineTool;
class HLRBRep_TheCSFunctionOfInterCSurf;
class math_FunctionSetRoot;
class gp_Pnt;



class HLRBRep_TheExactInterCSurf  {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   HLRBRep_TheExactInterCSurf(const Standard_Real U,const Standard_Real V,const Standard_Real W,const HLRBRep_TheCSFunctionOfInterCSurf& F,const Standard_Real TolTangency,const Standard_Real MarginCoef = 0.0);
  
  Standard_EXPORT   HLRBRep_TheExactInterCSurf(const HLRBRep_TheCSFunctionOfInterCSurf& F,const Standard_Real TolTangency);
  
  Standard_EXPORT     void Perform(const Standard_Real U,const Standard_Real V,const Standard_Real W,math_FunctionSetRoot& Rsnld,const Standard_Real u0,const Standard_Real v0,const Standard_Real u1,const Standard_Real v1,const Standard_Real w0,const Standard_Real w1) ;
  
  Standard_EXPORT     Standard_Boolean IsDone() const;
  
  Standard_EXPORT     Standard_Boolean IsEmpty() const;
  
  Standard_EXPORT    const gp_Pnt& Point() const;
  
  Standard_EXPORT     Standard_Real ParameterOnCurve() const;
  
  Standard_EXPORT     void ParameterOnSurface(Standard_Real& U,Standard_Real& V) const;
  
  Standard_EXPORT     HLRBRep_TheCSFunctionOfInterCSurf& Function() ;





protected:





private:



Standard_Boolean done;
Standard_Boolean empty;
HLRBRep_TheCSFunctionOfInterCSurf myFunction;
Standard_Real w;
Standard_Real u;
Standard_Real v;
Standard_Real tol;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
