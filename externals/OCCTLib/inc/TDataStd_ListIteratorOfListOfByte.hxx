// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TDataStd_ListIteratorOfListOfByte_HeaderFile
#define _TDataStd_ListIteratorOfListOfByte_HeaderFile

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
#ifndef _Standard_Byte_HeaderFile
#include <Standard_Byte.hxx>
#endif
#ifndef _Handle_TDataStd_ListNodeOfListOfByte_HeaderFile
#include <Handle_TDataStd_ListNodeOfListOfByte.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class Standard_NoMoreObject;
class Standard_NoSuchObject;
class TDataStd_ListOfByte;
class TDataStd_ListNodeOfListOfByte;



class TDataStd_ListIteratorOfListOfByte  {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   TDataStd_ListIteratorOfListOfByte();
  
  Standard_EXPORT   TDataStd_ListIteratorOfListOfByte(const TDataStd_ListOfByte& L);
  
  Standard_EXPORT     void Initialize(const TDataStd_ListOfByte& L) ;
  
        Standard_Boolean More() const;
  
  Standard_EXPORT     void Next() ;
  
  Standard_EXPORT     Standard_Byte& Value() const;


friend class TDataStd_ListOfByte;



protected:





private:



Standard_Address current;
Standard_Address previous;


};

#define Item Standard_Byte
#define Item_hxx <Standard_Byte.hxx>
#define TCollection_ListNode TDataStd_ListNodeOfListOfByte
#define TCollection_ListNode_hxx <TDataStd_ListNodeOfListOfByte.hxx>
#define TCollection_ListIterator TDataStd_ListIteratorOfListOfByte
#define TCollection_ListIterator_hxx <TDataStd_ListIteratorOfListOfByte.hxx>
#define Handle_TCollection_ListNode Handle_TDataStd_ListNodeOfListOfByte
#define TCollection_ListNode_Type_() TDataStd_ListNodeOfListOfByte_Type_()
#define TCollection_List TDataStd_ListOfByte
#define TCollection_List_hxx <TDataStd_ListOfByte.hxx>

#include <TCollection_ListIterator.lxx>

#undef Item
#undef Item_hxx
#undef TCollection_ListNode
#undef TCollection_ListNode_hxx
#undef TCollection_ListIterator
#undef TCollection_ListIterator_hxx
#undef Handle_TCollection_ListNode
#undef TCollection_ListNode_Type_
#undef TCollection_List
#undef TCollection_List_hxx


// other Inline functions and methods (like "C++: function call" methods)


#endif
