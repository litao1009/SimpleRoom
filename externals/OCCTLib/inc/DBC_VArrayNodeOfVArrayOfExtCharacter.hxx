// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _DBC_VArrayNodeOfVArrayOfExtCharacter_HeaderFile
#define _DBC_VArrayNodeOfVArrayOfExtCharacter_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_DBC_VArrayNodeOfVArrayOfExtCharacter_HeaderFile
#include <Handle_DBC_VArrayNodeOfVArrayOfExtCharacter.hxx>
#endif

#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
#ifndef _Standard_ExtCharacter_HeaderFile
#include <Standard_ExtCharacter.hxx>
#endif
#ifndef _PStandard_ArrayNode_HeaderFile
#include <PStandard_ArrayNode.hxx>
#endif
class DBC_VArrayOfExtCharacter;
class DBC_VArrayTNodeOfVArrayOfExtCharacter;


class DBC_VArrayNodeOfVArrayOfExtCharacter : public PStandard_ArrayNode {

public:

  
  Standard_EXPORT   DBC_VArrayNodeOfVArrayOfExtCharacter();
  
  Standard_EXPORT   DBC_VArrayNodeOfVArrayOfExtCharacter(const Standard_ExtCharacter& aValue);
  
  Standard_EXPORT     void SetValue(const Standard_ExtCharacter& aValue) ;
  
  Standard_EXPORT     Standard_Address Value() const;

DBC_VArrayNodeOfVArrayOfExtCharacter(const Storage_stCONSTclCOM& a) : PStandard_ArrayNode(a)
{
  
}
    Standard_ExtCharacter _CSFDB_GetDBC_VArrayNodeOfVArrayOfExtCharactermyValue() const { return myValue; }
    void _CSFDB_SetDBC_VArrayNodeOfVArrayOfExtCharactermyValue(const Standard_ExtCharacter p) { myValue = p; }



  DEFINE_STANDARD_RTTI(DBC_VArrayNodeOfVArrayOfExtCharacter)

protected:




private: 


Standard_ExtCharacter myValue;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
