// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StepBasic_Address_HeaderFile
#define _StepBasic_Address_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_StepBasic_Address_HeaderFile
#include <Handle_StepBasic_Address.hxx>
#endif

#ifndef _Handle_TCollection_HAsciiString_HeaderFile
#include <Handle_TCollection_HAsciiString.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _MMgt_TShared_HeaderFile
#include <MMgt_TShared.hxx>
#endif
class TCollection_HAsciiString;



class StepBasic_Address : public MMgt_TShared {

public:

  //! Returns a Address <br>
  Standard_EXPORT   StepBasic_Address();
  
  Standard_EXPORT   virtual  void Init(const Standard_Boolean hasAinternalLocation,const Handle(TCollection_HAsciiString)& aInternalLocation,const Standard_Boolean hasAstreetNumber,const Handle(TCollection_HAsciiString)& aStreetNumber,const Standard_Boolean hasAstreet,const Handle(TCollection_HAsciiString)& aStreet,const Standard_Boolean hasApostalBox,const Handle(TCollection_HAsciiString)& aPostalBox,const Standard_Boolean hasAtown,const Handle(TCollection_HAsciiString)& aTown,const Standard_Boolean hasAregion,const Handle(TCollection_HAsciiString)& aRegion,const Standard_Boolean hasApostalCode,const Handle(TCollection_HAsciiString)& aPostalCode,const Standard_Boolean hasAcountry,const Handle(TCollection_HAsciiString)& aCountry,const Standard_Boolean hasAfacsimileNumber,const Handle(TCollection_HAsciiString)& aFacsimileNumber,const Standard_Boolean hasAtelephoneNumber,const Handle(TCollection_HAsciiString)& aTelephoneNumber,const Standard_Boolean hasAelectronicMailAddress,const Handle(TCollection_HAsciiString)& aElectronicMailAddress,const Standard_Boolean hasAtelexNumber,const Handle(TCollection_HAsciiString)& aTelexNumber) ;
  
  Standard_EXPORT     void SetInternalLocation(const Handle(TCollection_HAsciiString)& aInternalLocation) ;
  
  Standard_EXPORT     void UnSetInternalLocation() ;
  
  Standard_EXPORT     Handle_TCollection_HAsciiString InternalLocation() const;
  
  Standard_EXPORT     Standard_Boolean HasInternalLocation() const;
  
  Standard_EXPORT     void SetStreetNumber(const Handle(TCollection_HAsciiString)& aStreetNumber) ;
  
  Standard_EXPORT     void UnSetStreetNumber() ;
  
  Standard_EXPORT     Handle_TCollection_HAsciiString StreetNumber() const;
  
  Standard_EXPORT     Standard_Boolean HasStreetNumber() const;
  
  Standard_EXPORT     void SetStreet(const Handle(TCollection_HAsciiString)& aStreet) ;
  
  Standard_EXPORT     void UnSetStreet() ;
  
  Standard_EXPORT     Handle_TCollection_HAsciiString Street() const;
  
  Standard_EXPORT     Standard_Boolean HasStreet() const;
  
  Standard_EXPORT     void SetPostalBox(const Handle(TCollection_HAsciiString)& aPostalBox) ;
  
  Standard_EXPORT     void UnSetPostalBox() ;
  
  Standard_EXPORT     Handle_TCollection_HAsciiString PostalBox() const;
  
  Standard_EXPORT     Standard_Boolean HasPostalBox() const;
  
  Standard_EXPORT     void SetTown(const Handle(TCollection_HAsciiString)& aTown) ;
  
  Standard_EXPORT     void UnSetTown() ;
  
  Standard_EXPORT     Handle_TCollection_HAsciiString Town() const;
  
  Standard_EXPORT     Standard_Boolean HasTown() const;
  
  Standard_EXPORT     void SetRegion(const Handle(TCollection_HAsciiString)& aRegion) ;
  
  Standard_EXPORT     void UnSetRegion() ;
  
  Standard_EXPORT     Handle_TCollection_HAsciiString Region() const;
  
  Standard_EXPORT     Standard_Boolean HasRegion() const;
  
  Standard_EXPORT     void SetPostalCode(const Handle(TCollection_HAsciiString)& aPostalCode) ;
  
  Standard_EXPORT     void UnSetPostalCode() ;
  
  Standard_EXPORT     Handle_TCollection_HAsciiString PostalCode() const;
  
  Standard_EXPORT     Standard_Boolean HasPostalCode() const;
  
  Standard_EXPORT     void SetCountry(const Handle(TCollection_HAsciiString)& aCountry) ;
  
  Standard_EXPORT     void UnSetCountry() ;
  
  Standard_EXPORT     Handle_TCollection_HAsciiString Country() const;
  
  Standard_EXPORT     Standard_Boolean HasCountry() const;
  
  Standard_EXPORT     void SetFacsimileNumber(const Handle(TCollection_HAsciiString)& aFacsimileNumber) ;
  
  Standard_EXPORT     void UnSetFacsimileNumber() ;
  
  Standard_EXPORT     Handle_TCollection_HAsciiString FacsimileNumber() const;
  
  Standard_EXPORT     Standard_Boolean HasFacsimileNumber() const;
  
  Standard_EXPORT     void SetTelephoneNumber(const Handle(TCollection_HAsciiString)& aTelephoneNumber) ;
  
  Standard_EXPORT     void UnSetTelephoneNumber() ;
  
  Standard_EXPORT     Handle_TCollection_HAsciiString TelephoneNumber() const;
  
  Standard_EXPORT     Standard_Boolean HasTelephoneNumber() const;
  
  Standard_EXPORT     void SetElectronicMailAddress(const Handle(TCollection_HAsciiString)& aElectronicMailAddress) ;
  
  Standard_EXPORT     void UnSetElectronicMailAddress() ;
  
  Standard_EXPORT     Handle_TCollection_HAsciiString ElectronicMailAddress() const;
  
  Standard_EXPORT     Standard_Boolean HasElectronicMailAddress() const;
  
  Standard_EXPORT     void SetTelexNumber(const Handle(TCollection_HAsciiString)& aTelexNumber) ;
  
  Standard_EXPORT     void UnSetTelexNumber() ;
  
  Standard_EXPORT     Handle_TCollection_HAsciiString TelexNumber() const;
  
  Standard_EXPORT     Standard_Boolean HasTelexNumber() const;




  DEFINE_STANDARD_RTTI(StepBasic_Address)

protected:




private: 


Handle_TCollection_HAsciiString internalLocation;
Handle_TCollection_HAsciiString streetNumber;
Handle_TCollection_HAsciiString street;
Handle_TCollection_HAsciiString postalBox;
Handle_TCollection_HAsciiString town;
Handle_TCollection_HAsciiString region;
Handle_TCollection_HAsciiString postalCode;
Handle_TCollection_HAsciiString country;
Handle_TCollection_HAsciiString facsimileNumber;
Handle_TCollection_HAsciiString telephoneNumber;
Handle_TCollection_HAsciiString electronicMailAddress;
Handle_TCollection_HAsciiString telexNumber;
Standard_Boolean hasInternalLocation;
Standard_Boolean hasStreetNumber;
Standard_Boolean hasStreet;
Standard_Boolean hasPostalBox;
Standard_Boolean hasTown;
Standard_Boolean hasRegion;
Standard_Boolean hasPostalCode;
Standard_Boolean hasCountry;
Standard_Boolean hasFacsimileNumber;
Standard_Boolean hasTelephoneNumber;
Standard_Boolean hasElectronicMailAddress;
Standard_Boolean hasTelexNumber;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
