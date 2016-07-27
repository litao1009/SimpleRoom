// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TDocStd_DataMapNodeOfLabelIDMapDataMap_HeaderFile
#define _TDocStd_DataMapNodeOfLabelIDMapDataMap_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_TDocStd_DataMapNodeOfLabelIDMapDataMap_HeaderFile
#include <Handle_TDocStd_DataMapNodeOfLabelIDMapDataMap.hxx>
#endif

#ifndef _TDF_Label_HeaderFile
#include <TDF_Label.hxx>
#endif
#ifndef _TDF_IDMap_HeaderFile
#include <TDF_IDMap.hxx>
#endif
#ifndef _TCollection_MapNode_HeaderFile
#include <TCollection_MapNode.hxx>
#endif
#ifndef _TCollection_MapNodePtr_HeaderFile
#include <TCollection_MapNodePtr.hxx>
#endif
class TDF_Label;
class TDF_IDMap;
class TDF_LabelMapHasher;
class TDocStd_LabelIDMapDataMap;
class TDocStd_DataMapIteratorOfLabelIDMapDataMap;



class TDocStd_DataMapNodeOfLabelIDMapDataMap : public TCollection_MapNode {

public:

  
      TDocStd_DataMapNodeOfLabelIDMapDataMap(const TDF_Label& K,const TDF_IDMap& I,const TCollection_MapNodePtr& n);
  
        TDF_Label& Key() const;
  
        TDF_IDMap& Value() const;




  DEFINE_STANDARD_RTTI(TDocStd_DataMapNodeOfLabelIDMapDataMap)

protected:




private: 


TDF_Label myKey;
TDF_IDMap myValue;


};

#define TheKey TDF_Label
#define TheKey_hxx <TDF_Label.hxx>
#define TheItem TDF_IDMap
#define TheItem_hxx <TDF_IDMap.hxx>
#define Hasher TDF_LabelMapHasher
#define Hasher_hxx <TDF_LabelMapHasher.hxx>
#define TCollection_DataMapNode TDocStd_DataMapNodeOfLabelIDMapDataMap
#define TCollection_DataMapNode_hxx <TDocStd_DataMapNodeOfLabelIDMapDataMap.hxx>
#define TCollection_DataMapIterator TDocStd_DataMapIteratorOfLabelIDMapDataMap
#define TCollection_DataMapIterator_hxx <TDocStd_DataMapIteratorOfLabelIDMapDataMap.hxx>
#define Handle_TCollection_DataMapNode Handle_TDocStd_DataMapNodeOfLabelIDMapDataMap
#define TCollection_DataMapNode_Type_() TDocStd_DataMapNodeOfLabelIDMapDataMap_Type_()
#define TCollection_DataMap TDocStd_LabelIDMapDataMap
#define TCollection_DataMap_hxx <TDocStd_LabelIDMapDataMap.hxx>

#include <TCollection_DataMapNode.lxx>

#undef TheKey
#undef TheKey_hxx
#undef TheItem
#undef TheItem_hxx
#undef Hasher
#undef Hasher_hxx
#undef TCollection_DataMapNode
#undef TCollection_DataMapNode_hxx
#undef TCollection_DataMapIterator
#undef TCollection_DataMapIterator_hxx
#undef Handle_TCollection_DataMapNode
#undef TCollection_DataMapNode_Type_
#undef TCollection_DataMap
#undef TCollection_DataMap_hxx


// other Inline functions and methods (like "C++: function call" methods)


#endif
