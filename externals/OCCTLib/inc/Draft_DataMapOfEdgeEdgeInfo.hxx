// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Draft_DataMapOfEdgeEdgeInfo_HeaderFile
#define _Draft_DataMapOfEdgeEdgeInfo_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _TCollection_BasicMap_HeaderFile
#include <TCollection_BasicMap.hxx>
#endif
#ifndef _Handle_Draft_DataMapNodeOfDataMapOfEdgeEdgeInfo_HeaderFile
#include <Handle_Draft_DataMapNodeOfDataMapOfEdgeEdgeInfo.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
class Standard_DomainError;
class Standard_NoSuchObject;
class TopoDS_Edge;
class Draft_EdgeInfo;
class TopTools_ShapeMapHasher;
class Draft_DataMapNodeOfDataMapOfEdgeEdgeInfo;
class Draft_DataMapIteratorOfDataMapOfEdgeEdgeInfo;



class Draft_DataMapOfEdgeEdgeInfo  : public TCollection_BasicMap {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   Draft_DataMapOfEdgeEdgeInfo(const Standard_Integer NbBuckets = 1);
  
  Standard_EXPORT     Draft_DataMapOfEdgeEdgeInfo& Assign(const Draft_DataMapOfEdgeEdgeInfo& Other) ;
    Draft_DataMapOfEdgeEdgeInfo& operator =(const Draft_DataMapOfEdgeEdgeInfo& Other) 
{
  return Assign(Other);
}
  
  Standard_EXPORT     void ReSize(const Standard_Integer NbBuckets) ;
  
  Standard_EXPORT     void Clear() ;
~Draft_DataMapOfEdgeEdgeInfo()
{
  Clear();
}
  
  Standard_EXPORT     Standard_Boolean Bind(const TopoDS_Edge& K,const Draft_EdgeInfo& I) ;
  
  Standard_EXPORT     Standard_Boolean IsBound(const TopoDS_Edge& K) const;
  
  Standard_EXPORT     Standard_Boolean UnBind(const TopoDS_Edge& K) ;
  
  Standard_EXPORT    const Draft_EdgeInfo& Find(const TopoDS_Edge& K) const;
   const Draft_EdgeInfo& operator()(const TopoDS_Edge& K) const
{
  return Find(K);
}
  
  Standard_EXPORT     Draft_EdgeInfo& ChangeFind(const TopoDS_Edge& K) ;
    Draft_EdgeInfo& operator()(const TopoDS_Edge& K) 
{
  return ChangeFind(K);
}
  
  Standard_EXPORT     Standard_Address Find1(const TopoDS_Edge& K) const;
  
  Standard_EXPORT     Standard_Address ChangeFind1(const TopoDS_Edge& K) ;





protected:





private:

  
  Standard_EXPORT   Draft_DataMapOfEdgeEdgeInfo(const Draft_DataMapOfEdgeEdgeInfo& Other);




};





// other Inline functions and methods (like "C++: function call" methods)


#endif
