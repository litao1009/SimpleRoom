// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Aspect_SequenceNodeOfSequenceOfTypeMapEntry_HeaderFile
#define _Aspect_SequenceNodeOfSequenceOfTypeMapEntry_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_Aspect_SequenceNodeOfSequenceOfTypeMapEntry_HeaderFile
#include <Handle_Aspect_SequenceNodeOfSequenceOfTypeMapEntry.hxx>
#endif

#ifndef _Aspect_TypeMapEntry_HeaderFile
#include <Aspect_TypeMapEntry.hxx>
#endif
#ifndef _TCollection_SeqNode_HeaderFile
#include <TCollection_SeqNode.hxx>
#endif
#ifndef _TCollection_SeqNodePtr_HeaderFile
#include <TCollection_SeqNodePtr.hxx>
#endif
class Aspect_TypeMapEntry;
class Aspect_SequenceOfTypeMapEntry;



class Aspect_SequenceNodeOfSequenceOfTypeMapEntry : public TCollection_SeqNode {

public:

  
      Aspect_SequenceNodeOfSequenceOfTypeMapEntry(const Aspect_TypeMapEntry& I,const TCollection_SeqNodePtr& n,const TCollection_SeqNodePtr& p);
  
        Aspect_TypeMapEntry& Value() const;




  DEFINE_STANDARD_RTTI(Aspect_SequenceNodeOfSequenceOfTypeMapEntry)

protected:




private: 


Aspect_TypeMapEntry myValue;


};

#define SeqItem Aspect_TypeMapEntry
#define SeqItem_hxx <Aspect_TypeMapEntry.hxx>
#define TCollection_SequenceNode Aspect_SequenceNodeOfSequenceOfTypeMapEntry
#define TCollection_SequenceNode_hxx <Aspect_SequenceNodeOfSequenceOfTypeMapEntry.hxx>
#define Handle_TCollection_SequenceNode Handle_Aspect_SequenceNodeOfSequenceOfTypeMapEntry
#define TCollection_SequenceNode_Type_() Aspect_SequenceNodeOfSequenceOfTypeMapEntry_Type_()
#define TCollection_Sequence Aspect_SequenceOfTypeMapEntry
#define TCollection_Sequence_hxx <Aspect_SequenceOfTypeMapEntry.hxx>

#include <TCollection_SequenceNode.lxx>

#undef SeqItem
#undef SeqItem_hxx
#undef TCollection_SequenceNode
#undef TCollection_SequenceNode_hxx
#undef Handle_TCollection_SequenceNode
#undef TCollection_SequenceNode_Type_
#undef TCollection_Sequence
#undef TCollection_Sequence_hxx


// other Inline functions and methods (like "C++: function call" methods)


#endif
