// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _DBC_VArrayNodeOfVArrayOfCharacter_HeaderFile
#define _DBC_VArrayNodeOfVArrayOfCharacter_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_DBC_VArrayNodeOfVArrayOfCharacter_HeaderFile
#include <Handle_DBC_VArrayNodeOfVArrayOfCharacter.hxx>
#endif

#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
#ifndef _Standard_Character_HeaderFile
#include <Standard_Character.hxx>
#endif
#ifndef _PStandard_ArrayNode_HeaderFile
#include <PStandard_ArrayNode.hxx>
#endif
class DBC_VArrayOfCharacter;
class DBC_VArrayTNodeOfVArrayOfCharacter;


class DBC_VArrayNodeOfVArrayOfCharacter : public PStandard_ArrayNode {

public:

  
  Standard_EXPORT   DBC_VArrayNodeOfVArrayOfCharacter();
  
  Standard_EXPORT   DBC_VArrayNodeOfVArrayOfCharacter(const Standard_Character& aValue);
  
  Standard_EXPORT     void SetValue(const Standard_Character& aValue) ;
  
  Standard_EXPORT     Standard_Address Value() const;

DBC_VArrayNodeOfVArrayOfCharacter(const Storage_stCONSTclCOM& a) : PStandard_ArrayNode(a)
{
  
}
    Standard_Character _CSFDB_GetDBC_VArrayNodeOfVArrayOfCharactermyValue() const { return myValue; }
    void _CSFDB_SetDBC_VArrayNodeOfVArrayOfCharactermyValue(const Standard_Character p) { myValue = p; }



  DEFINE_STANDARD_RTTI(DBC_VArrayNodeOfVArrayOfCharacter)

protected:




private: 


Standard_Character myValue;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
