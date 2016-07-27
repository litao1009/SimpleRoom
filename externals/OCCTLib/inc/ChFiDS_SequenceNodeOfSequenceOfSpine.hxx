// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _ChFiDS_SequenceNodeOfSequenceOfSpine_HeaderFile
#define _ChFiDS_SequenceNodeOfSequenceOfSpine_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_ChFiDS_SequenceNodeOfSequenceOfSpine_HeaderFile
#include <Handle_ChFiDS_SequenceNodeOfSequenceOfSpine.hxx>
#endif

#ifndef _Handle_ChFiDS_Spine_HeaderFile
#include <Handle_ChFiDS_Spine.hxx>
#endif
#ifndef _TCollection_SeqNode_HeaderFile
#include <TCollection_SeqNode.hxx>
#endif
#ifndef _TCollection_SeqNodePtr_HeaderFile
#include <TCollection_SeqNodePtr.hxx>
#endif
class ChFiDS_Spine;
class ChFiDS_SequenceOfSpine;



class ChFiDS_SequenceNodeOfSequenceOfSpine : public TCollection_SeqNode {

public:

  
      ChFiDS_SequenceNodeOfSequenceOfSpine(const Handle(ChFiDS_Spine)& I,const TCollection_SeqNodePtr& n,const TCollection_SeqNodePtr& p);
  
        Handle_ChFiDS_Spine& Value() const;




  DEFINE_STANDARD_RTTI(ChFiDS_SequenceNodeOfSequenceOfSpine)

protected:




private: 


Handle_ChFiDS_Spine myValue;


};

#define SeqItem Handle_ChFiDS_Spine
#define SeqItem_hxx <ChFiDS_Spine.hxx>
#define TCollection_SequenceNode ChFiDS_SequenceNodeOfSequenceOfSpine
#define TCollection_SequenceNode_hxx <ChFiDS_SequenceNodeOfSequenceOfSpine.hxx>
#define Handle_TCollection_SequenceNode Handle_ChFiDS_SequenceNodeOfSequenceOfSpine
#define TCollection_SequenceNode_Type_() ChFiDS_SequenceNodeOfSequenceOfSpine_Type_()
#define TCollection_Sequence ChFiDS_SequenceOfSpine
#define TCollection_Sequence_hxx <ChFiDS_SequenceOfSpine.hxx>

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
