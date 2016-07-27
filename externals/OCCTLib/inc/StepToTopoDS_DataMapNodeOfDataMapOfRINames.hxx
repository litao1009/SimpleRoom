// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StepToTopoDS_DataMapNodeOfDataMapOfRINames_HeaderFile
#define _StepToTopoDS_DataMapNodeOfDataMapOfRINames_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_StepToTopoDS_DataMapNodeOfDataMapOfRINames_HeaderFile
#include <Handle_StepToTopoDS_DataMapNodeOfDataMapOfRINames.hxx>
#endif

#ifndef _TCollection_AsciiString_HeaderFile
#include <TCollection_AsciiString.hxx>
#endif
#ifndef _TopoDS_Shape_HeaderFile
#include <TopoDS_Shape.hxx>
#endif
#ifndef _TCollection_MapNode_HeaderFile
#include <TCollection_MapNode.hxx>
#endif
#ifndef _TCollection_MapNodePtr_HeaderFile
#include <TCollection_MapNodePtr.hxx>
#endif
class TCollection_AsciiString;
class TopoDS_Shape;
class StepToTopoDS_DataMapOfRINames;
class StepToTopoDS_DataMapIteratorOfDataMapOfRINames;



class StepToTopoDS_DataMapNodeOfDataMapOfRINames : public TCollection_MapNode {

public:

  
      StepToTopoDS_DataMapNodeOfDataMapOfRINames(const TCollection_AsciiString& K,const TopoDS_Shape& I,const TCollection_MapNodePtr& n);
  
        TCollection_AsciiString& Key() const;
  
        TopoDS_Shape& Value() const;




  DEFINE_STANDARD_RTTI(StepToTopoDS_DataMapNodeOfDataMapOfRINames)

protected:




private: 


TCollection_AsciiString myKey;
TopoDS_Shape myValue;


};

#define TheKey TCollection_AsciiString
#define TheKey_hxx <TCollection_AsciiString.hxx>
#define TheItem TopoDS_Shape
#define TheItem_hxx <TopoDS_Shape.hxx>
#define Hasher TCollection_AsciiString
#define Hasher_hxx <TCollection_AsciiString.hxx>
#define TCollection_DataMapNode StepToTopoDS_DataMapNodeOfDataMapOfRINames
#define TCollection_DataMapNode_hxx <StepToTopoDS_DataMapNodeOfDataMapOfRINames.hxx>
#define TCollection_DataMapIterator StepToTopoDS_DataMapIteratorOfDataMapOfRINames
#define TCollection_DataMapIterator_hxx <StepToTopoDS_DataMapIteratorOfDataMapOfRINames.hxx>
#define Handle_TCollection_DataMapNode Handle_StepToTopoDS_DataMapNodeOfDataMapOfRINames
#define TCollection_DataMapNode_Type_() StepToTopoDS_DataMapNodeOfDataMapOfRINames_Type_()
#define TCollection_DataMap StepToTopoDS_DataMapOfRINames
#define TCollection_DataMap_hxx <StepToTopoDS_DataMapOfRINames.hxx>

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
