// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _XmlLDrivers_SequenceNodeOfSequenceOfNamespaceDef_HeaderFile
#define _XmlLDrivers_SequenceNodeOfSequenceOfNamespaceDef_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_XmlLDrivers_SequenceNodeOfSequenceOfNamespaceDef_HeaderFile
#include <Handle_XmlLDrivers_SequenceNodeOfSequenceOfNamespaceDef.hxx>
#endif

#ifndef _XmlLDrivers_NamespaceDef_HeaderFile
#include <XmlLDrivers_NamespaceDef.hxx>
#endif
#ifndef _TCollection_SeqNode_HeaderFile
#include <TCollection_SeqNode.hxx>
#endif
#ifndef _TCollection_SeqNodePtr_HeaderFile
#include <TCollection_SeqNodePtr.hxx>
#endif
class XmlLDrivers_NamespaceDef;
class XmlLDrivers_SequenceOfNamespaceDef;



class XmlLDrivers_SequenceNodeOfSequenceOfNamespaceDef : public TCollection_SeqNode {

public:

  
      XmlLDrivers_SequenceNodeOfSequenceOfNamespaceDef(const XmlLDrivers_NamespaceDef& I,const TCollection_SeqNodePtr& n,const TCollection_SeqNodePtr& p);
  
        XmlLDrivers_NamespaceDef& Value() const;




  DEFINE_STANDARD_RTTI(XmlLDrivers_SequenceNodeOfSequenceOfNamespaceDef)

protected:




private: 


XmlLDrivers_NamespaceDef myValue;


};

#define SeqItem XmlLDrivers_NamespaceDef
#define SeqItem_hxx <XmlLDrivers_NamespaceDef.hxx>
#define TCollection_SequenceNode XmlLDrivers_SequenceNodeOfSequenceOfNamespaceDef
#define TCollection_SequenceNode_hxx <XmlLDrivers_SequenceNodeOfSequenceOfNamespaceDef.hxx>
#define Handle_TCollection_SequenceNode Handle_XmlLDrivers_SequenceNodeOfSequenceOfNamespaceDef
#define TCollection_SequenceNode_Type_() XmlLDrivers_SequenceNodeOfSequenceOfNamespaceDef_Type_()
#define TCollection_Sequence XmlLDrivers_SequenceOfNamespaceDef
#define TCollection_Sequence_hxx <XmlLDrivers_SequenceOfNamespaceDef.hxx>

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
