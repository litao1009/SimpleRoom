// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _IGESSolid_Array1OfVertexList_HeaderFile
#define _IGESSolid_Array1OfVertexList_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Handle_IGESSolid_VertexList_HeaderFile
#include <Handle_IGESSolid_VertexList.hxx>
#endif
class Standard_RangeError;
class Standard_DimensionMismatch;
class Standard_OutOfRange;
class Standard_OutOfMemory;
class IGESSolid_VertexList;



class IGESSolid_Array1OfVertexList  {
public:

  DEFINE_STANDARD_ALLOC

  
      IGESSolid_Array1OfVertexList(const Standard_Integer Low,const Standard_Integer Up);
  
      IGESSolid_Array1OfVertexList(const Handle(IGESSolid_VertexList)& Item,const Standard_Integer Low,const Standard_Integer Up);
  
  Standard_EXPORT     void Init(const Handle(IGESSolid_VertexList)& V) ;
  
        void Destroy() ;
~IGESSolid_Array1OfVertexList()
{
  Destroy();
}
  
        Standard_Boolean IsAllocated() const;
  
  Standard_EXPORT    const IGESSolid_Array1OfVertexList& Assign(const IGESSolid_Array1OfVertexList& Other) ;
   const IGESSolid_Array1OfVertexList& operator =(const IGESSolid_Array1OfVertexList& Other) 
{
  return Assign(Other);
}
  
        Standard_Integer Length() const;
  
        Standard_Integer Lower() const;
  
        Standard_Integer Upper() const;
  
        void SetValue(const Standard_Integer Index,const Handle(IGESSolid_VertexList)& Value) ;
  
       const Handle_IGESSolid_VertexList& Value(const Standard_Integer Index) const;
     const Handle_IGESSolid_VertexList& operator ()(const Standard_Integer Index) const
{
  return Value(Index);
}
  
        Handle_IGESSolid_VertexList& ChangeValue(const Standard_Integer Index) ;
      Handle_IGESSolid_VertexList& operator ()(const Standard_Integer Index) 
{
  return ChangeValue(Index);
}





protected:





private:

  
  Standard_EXPORT   IGESSolid_Array1OfVertexList(const IGESSolid_Array1OfVertexList& AnArray);


Standard_Integer myLowerBound;
Standard_Integer myUpperBound;
Standard_Address myStart;
Standard_Boolean isAllocated;


};

#define Array1Item Handle_IGESSolid_VertexList
#define Array1Item_hxx <IGESSolid_VertexList.hxx>
#define TCollection_Array1 IGESSolid_Array1OfVertexList
#define TCollection_Array1_hxx <IGESSolid_Array1OfVertexList.hxx>

#include <TCollection_Array1.lxx>

#undef Array1Item
#undef Array1Item_hxx
#undef TCollection_Array1
#undef TCollection_Array1_hxx


// other Inline functions and methods (like "C++: function call" methods)


#endif
