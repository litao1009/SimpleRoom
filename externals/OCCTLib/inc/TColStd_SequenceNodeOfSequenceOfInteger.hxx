// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TColStd_SequenceNodeOfSequenceOfInteger_HeaderFile
#define _TColStd_SequenceNodeOfSequenceOfInteger_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_TColStd_SequenceNodeOfSequenceOfInteger_HeaderFile
#include <Handle_TColStd_SequenceNodeOfSequenceOfInteger.hxx>
#endif

#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _TCollection_SeqNode_HeaderFile
#include <TCollection_SeqNode.hxx>
#endif
#ifndef _TCollection_SeqNodePtr_HeaderFile
#include <TCollection_SeqNodePtr.hxx>
#endif
class TColStd_SequenceOfInteger;



class TColStd_SequenceNodeOfSequenceOfInteger : public TCollection_SeqNode {

public:

  
      TColStd_SequenceNodeOfSequenceOfInteger(const Standard_Integer& I,const TCollection_SeqNodePtr& n,const TCollection_SeqNodePtr& p);
  
        Standard_Integer& Value() const;




  DEFINE_STANDARD_RTTI(TColStd_SequenceNodeOfSequenceOfInteger)

protected:




private: 


Standard_Integer myValue;


};

#define SeqItem Standard_Integer
#define SeqItem_hxx <Standard_Integer.hxx>
#define TCollection_SequenceNode TColStd_SequenceNodeOfSequenceOfInteger
#define TCollection_SequenceNode_hxx <TColStd_SequenceNodeOfSequenceOfInteger.hxx>
#define Handle_TCollection_SequenceNode Handle_TColStd_SequenceNodeOfSequenceOfInteger
#define TCollection_SequenceNode_Type_() TColStd_SequenceNodeOfSequenceOfInteger_Type_()
#define TCollection_Sequence TColStd_SequenceOfInteger
#define TCollection_Sequence_hxx <TColStd_SequenceOfInteger.hxx>

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
