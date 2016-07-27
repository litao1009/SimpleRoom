// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _AdvApprox_DichoCutting_HeaderFile
#define _AdvApprox_DichoCutting_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _AdvApprox_Cutting_HeaderFile
#include <AdvApprox_Cutting.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif



//! if Cutting is necessary in [a,b], we cut at (a+b) / 2. <br>
class AdvApprox_DichoCutting  : public AdvApprox_Cutting {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   AdvApprox_DichoCutting();
  
  Standard_EXPORT   virtual  Standard_Boolean Value(const Standard_Real a,const Standard_Real b,Standard_Real& cuttingvalue) const;





protected:





private:





};





// other Inline functions and methods (like "C++: function call" methods)


#endif
