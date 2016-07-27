// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _HLRTopoBRep_VData_HeaderFile
#define _HLRTopoBRep_VData_HeaderFile

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
#ifndef _TopoDS_Shape_HeaderFile
#include <TopoDS_Shape.hxx>
#endif
class TopoDS_Shape;



class HLRTopoBRep_VData  {
public:

  DEFINE_STANDARD_ALLOC

  
      HLRTopoBRep_VData();
  
  Standard_EXPORT   HLRTopoBRep_VData(const Standard_Real P,const TopoDS_Shape& V);
  
        Standard_Real Parameter() const;
  
       const TopoDS_Shape& Vertex() const;





protected:





private:



Standard_Real myParameter;
TopoDS_Shape myVertex;


};


#include <HLRTopoBRep_VData.lxx>



// other Inline functions and methods (like "C++: function call" methods)


#endif
