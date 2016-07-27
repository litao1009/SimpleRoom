// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _PTopoDS_TShell_HeaderFile
#define _PTopoDS_TShell_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_PTopoDS_TShell_HeaderFile
#include <Handle_PTopoDS_TShell.hxx>
#endif

#ifndef _TopAbs_ShapeEnum_HeaderFile
#include <TopAbs_ShapeEnum.hxx>
#endif
#ifndef _PTopoDS_TShape_HeaderFile
#include <PTopoDS_TShape.hxx>
#endif


class PTopoDS_TShell : public PTopoDS_TShape {

public:

  
  Standard_EXPORT   PTopoDS_TShell();
  
  Standard_EXPORT     TopAbs_ShapeEnum ShapeType() const;

PTopoDS_TShell(const Storage_stCONSTclCOM& a) : PTopoDS_TShape(a)
{
  
}



  DEFINE_STANDARD_RTTI(PTopoDS_TShell)

protected:




private: 




};





// other Inline functions and methods (like "C++: function call" methods)


#endif
