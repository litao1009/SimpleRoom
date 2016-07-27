// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _BinTools_HeaderFile
#define _BinTools_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _Standard_OStream_HeaderFile
#include <Standard_OStream.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Standard_ExtCharacter_HeaderFile
#include <Standard_ExtCharacter.hxx>
#endif
#ifndef _Standard_IStream_HeaderFile
#include <Standard_IStream.hxx>
#endif
class BinTools_ShapeSet;
class BinTools_Curve2dSet;
class BinTools_CurveSet;
class BinTools_SurfaceSet;
class BinTools_LocationSet;


//! Tool to keep shapes in binary format <br>
class BinTools  {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   static  Standard_OStream& PutReal(Standard_OStream& OS,const Standard_Real theValue) ;
  
  Standard_EXPORT   static  Standard_OStream& PutInteger(Standard_OStream& OS,const Standard_Integer theValue) ;
  
  Standard_EXPORT   static  Standard_OStream& PutBool(Standard_OStream& OS,const Standard_Boolean theValue) ;
  
  Standard_EXPORT   static  Standard_OStream& PutExtChar(Standard_OStream& OS,const Standard_ExtCharacter theValue) ;
  
  Standard_EXPORT   static  Standard_IStream& GetReal(Standard_IStream& IS,Standard_Real& theValue) ;
  
  Standard_EXPORT   static  Standard_IStream& GetInteger(Standard_IStream& IS,Standard_Integer& theValue) ;
  
  Standard_EXPORT   static  Standard_IStream& GetBool(Standard_IStream& IS,Standard_Boolean& theValue) ;
  
  Standard_EXPORT   static  Standard_IStream& GetExtChar(Standard_IStream& IS,Standard_ExtCharacter& theValue) ;





protected:





private:




friend class BinTools_ShapeSet;
friend class BinTools_Curve2dSet;
friend class BinTools_CurveSet;
friend class BinTools_SurfaceSet;
friend class BinTools_LocationSet;

};





// other Inline functions and methods (like "C++: function call" methods)


#endif
