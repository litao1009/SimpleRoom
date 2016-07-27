// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Materials_SequenceNodeOfMtsSequence_HeaderFile
#define _Materials_SequenceNodeOfMtsSequence_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_Materials_SequenceNodeOfMtsSequence_HeaderFile
#include <Handle_Materials_SequenceNodeOfMtsSequence.hxx>
#endif

#ifndef _Handle_Materials_Material_HeaderFile
#include <Handle_Materials_Material.hxx>
#endif
#ifndef _TCollection_SeqNode_HeaderFile
#include <TCollection_SeqNode.hxx>
#endif
#ifndef _TCollection_SeqNodePtr_HeaderFile
#include <TCollection_SeqNodePtr.hxx>
#endif
class Materials_Material;
class Materials_MtsSequence;



class Materials_SequenceNodeOfMtsSequence : public TCollection_SeqNode {

public:

  
      Materials_SequenceNodeOfMtsSequence(const Handle(Materials_Material)& I,const TCollection_SeqNodePtr& n,const TCollection_SeqNodePtr& p);
  
        Handle_Materials_Material& Value() const;




  DEFINE_STANDARD_RTTI(Materials_SequenceNodeOfMtsSequence)

protected:




private: 


Handle_Materials_Material myValue;


};

#define SeqItem Handle_Materials_Material
#define SeqItem_hxx <Materials_Material.hxx>
#define TCollection_SequenceNode Materials_SequenceNodeOfMtsSequence
#define TCollection_SequenceNode_hxx <Materials_SequenceNodeOfMtsSequence.hxx>
#define Handle_TCollection_SequenceNode Handle_Materials_SequenceNodeOfMtsSequence
#define TCollection_SequenceNode_Type_() Materials_SequenceNodeOfMtsSequence_Type_()
#define TCollection_Sequence Materials_MtsSequence
#define TCollection_Sequence_hxx <Materials_MtsSequence.hxx>

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
