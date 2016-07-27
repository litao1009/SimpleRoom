// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TCollection_SeqNode_HeaderFile
#define _TCollection_SeqNode_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_TCollection_SeqNode_HeaderFile
#include <Handle_TCollection_SeqNode.hxx>
#endif

#ifndef _TCollection_SeqNodePtr_HeaderFile
#include <TCollection_SeqNodePtr.hxx>
#endif
#ifndef _MMgt_TShared_HeaderFile
#include <MMgt_TShared.hxx>
#endif



class TCollection_SeqNode : public MMgt_TShared {

public:

  
      TCollection_SeqNode(const TCollection_SeqNodePtr& n,const TCollection_SeqNodePtr& p);
  
        TCollection_SeqNodePtr& Next() const;
  
        TCollection_SeqNodePtr& Previous() const;




  DEFINE_STANDARD_RTTI(TCollection_SeqNode)

protected:




private: 


TCollection_SeqNodePtr myNext;
TCollection_SeqNodePtr myPrevious;


};


#include <TCollection_SeqNode.lxx>



// other Inline functions and methods (like "C++: function call" methods)


#endif
