// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _CDM_StackIteratorOfStackOfDocument_HeaderFile
#define _CDM_StackIteratorOfStackOfDocument_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
#ifndef _Handle_CDM_Document_HeaderFile
#include <Handle_CDM_Document.hxx>
#endif
#ifndef _Handle_CDM_StackNodeOfStackOfDocument_HeaderFile
#include <Handle_CDM_StackNodeOfStackOfDocument.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class Standard_NoSuchObject;
class CDM_Document;
class CDM_StackOfDocument;
class CDM_StackNodeOfStackOfDocument;



class CDM_StackIteratorOfStackOfDocument  {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   CDM_StackIteratorOfStackOfDocument();
  
  Standard_EXPORT   CDM_StackIteratorOfStackOfDocument(const CDM_StackOfDocument& S);
  
  Standard_EXPORT     void Initialize(const CDM_StackOfDocument& S) ;
  
        Standard_Boolean More() const;
  
  Standard_EXPORT     void Next() ;
  
  Standard_EXPORT    const Handle_CDM_Document& Value() const;





protected:





private:



Standard_Address current;


};

#define Item Handle_CDM_Document
#define Item_hxx <CDM_Document.hxx>
#define TCollection_StackNode CDM_StackNodeOfStackOfDocument
#define TCollection_StackNode_hxx <CDM_StackNodeOfStackOfDocument.hxx>
#define TCollection_StackIterator CDM_StackIteratorOfStackOfDocument
#define TCollection_StackIterator_hxx <CDM_StackIteratorOfStackOfDocument.hxx>
#define Handle_TCollection_StackNode Handle_CDM_StackNodeOfStackOfDocument
#define TCollection_StackNode_Type_() CDM_StackNodeOfStackOfDocument_Type_()
#define TCollection_Stack CDM_StackOfDocument
#define TCollection_Stack_hxx <CDM_StackOfDocument.hxx>

#include <TCollection_StackIterator.lxx>

#undef Item
#undef Item_hxx
#undef TCollection_StackNode
#undef TCollection_StackNode_hxx
#undef TCollection_StackIterator
#undef TCollection_StackIterator_hxx
#undef Handle_TCollection_StackNode
#undef TCollection_StackNode_Type_
#undef TCollection_Stack
#undef TCollection_Stack_hxx


// other Inline functions and methods (like "C++: function call" methods)


#endif
