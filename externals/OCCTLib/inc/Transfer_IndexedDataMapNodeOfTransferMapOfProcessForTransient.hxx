// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Transfer_IndexedDataMapNodeOfTransferMapOfProcessForTransient_HeaderFile
#define _Transfer_IndexedDataMapNodeOfTransferMapOfProcessForTransient_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_Transfer_IndexedDataMapNodeOfTransferMapOfProcessForTransient_HeaderFile
#include <Handle_Transfer_IndexedDataMapNodeOfTransferMapOfProcessForTransient.hxx>
#endif

#ifndef _Handle_Standard_Transient_HeaderFile
#include <Handle_Standard_Transient.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Handle_Transfer_Binder_HeaderFile
#include <Handle_Transfer_Binder.hxx>
#endif
#ifndef _TCollection_MapNodePtr_HeaderFile
#include <TCollection_MapNodePtr.hxx>
#endif
#ifndef _TCollection_MapNode_HeaderFile
#include <TCollection_MapNode.hxx>
#endif
class Standard_Transient;
class Transfer_Binder;
class TColStd_MapTransientHasher;
class Transfer_TransferMapOfProcessForTransient;



class Transfer_IndexedDataMapNodeOfTransferMapOfProcessForTransient : public TCollection_MapNode {

public:

  
      Transfer_IndexedDataMapNodeOfTransferMapOfProcessForTransient(const Handle(Standard_Transient)& K1,const Standard_Integer K2,const Handle(Transfer_Binder)& I,const TCollection_MapNodePtr& n1,const TCollection_MapNodePtr& n2);
  
        Handle_Standard_Transient& Key1() const;
  
        Standard_Integer& Key2() const;
  
        TCollection_MapNodePtr& Next2() const;
  
        Handle_Transfer_Binder& Value() const;




  DEFINE_STANDARD_RTTI(Transfer_IndexedDataMapNodeOfTransferMapOfProcessForTransient)

protected:




private: 


Handle_Standard_Transient myKey1;
Standard_Integer myKey2;
Handle_Transfer_Binder myValue;
TCollection_MapNodePtr myNext2;


};

#define TheKey Handle_Standard_Transient
#define TheKey_hxx <Standard_Transient.hxx>
#define TheItem Handle_Transfer_Binder
#define TheItem_hxx <Transfer_Binder.hxx>
#define Hasher TColStd_MapTransientHasher
#define Hasher_hxx <TColStd_MapTransientHasher.hxx>
#define TCollection_IndexedDataMapNode Transfer_IndexedDataMapNodeOfTransferMapOfProcessForTransient
#define TCollection_IndexedDataMapNode_hxx <Transfer_IndexedDataMapNodeOfTransferMapOfProcessForTransient.hxx>
#define Handle_TCollection_IndexedDataMapNode Handle_Transfer_IndexedDataMapNodeOfTransferMapOfProcessForTransient
#define TCollection_IndexedDataMapNode_Type_() Transfer_IndexedDataMapNodeOfTransferMapOfProcessForTransient_Type_()
#define TCollection_IndexedDataMap Transfer_TransferMapOfProcessForTransient
#define TCollection_IndexedDataMap_hxx <Transfer_TransferMapOfProcessForTransient.hxx>

#include <TCollection_IndexedDataMapNode.lxx>

#undef TheKey
#undef TheKey_hxx
#undef TheItem
#undef TheItem_hxx
#undef Hasher
#undef Hasher_hxx
#undef TCollection_IndexedDataMapNode
#undef TCollection_IndexedDataMapNode_hxx
#undef Handle_TCollection_IndexedDataMapNode
#undef TCollection_IndexedDataMapNode_Type_
#undef TCollection_IndexedDataMap
#undef TCollection_IndexedDataMap_hxx


// other Inline functions and methods (like "C++: function call" methods)


#endif
