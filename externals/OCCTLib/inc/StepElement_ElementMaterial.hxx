// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StepElement_ElementMaterial_HeaderFile
#define _StepElement_ElementMaterial_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_StepElement_ElementMaterial_HeaderFile
#include <Handle_StepElement_ElementMaterial.hxx>
#endif

#ifndef _Handle_TCollection_HAsciiString_HeaderFile
#include <Handle_TCollection_HAsciiString.hxx>
#endif
#ifndef _Handle_StepRepr_HArray1OfMaterialPropertyRepresentation_HeaderFile
#include <Handle_StepRepr_HArray1OfMaterialPropertyRepresentation.hxx>
#endif
#ifndef _MMgt_TShared_HeaderFile
#include <MMgt_TShared.hxx>
#endif
class TCollection_HAsciiString;
class StepRepr_HArray1OfMaterialPropertyRepresentation;


//! Representation of STEP entity ElementMaterial <br>
class StepElement_ElementMaterial : public MMgt_TShared {

public:

  //! Empty constructor <br>
  Standard_EXPORT   StepElement_ElementMaterial();
  //! Initialize all fields (own and inherited) <br>
  Standard_EXPORT     void Init(const Handle(TCollection_HAsciiString)& aMaterialId,const Handle(TCollection_HAsciiString)& aDescription,const Handle(StepRepr_HArray1OfMaterialPropertyRepresentation)& aProperties) ;
  //! Returns field MaterialId <br>
  Standard_EXPORT     Handle_TCollection_HAsciiString MaterialId() const;
  //! Set field MaterialId <br>
  Standard_EXPORT     void SetMaterialId(const Handle(TCollection_HAsciiString)& MaterialId) ;
  //! Returns field Description <br>
  Standard_EXPORT     Handle_TCollection_HAsciiString Description() const;
  //! Set field Description <br>
  Standard_EXPORT     void SetDescription(const Handle(TCollection_HAsciiString)& Description) ;
  //! Returns field Properties <br>
  Standard_EXPORT     Handle_StepRepr_HArray1OfMaterialPropertyRepresentation Properties() const;
  //! Set field Properties <br>
  Standard_EXPORT     void SetProperties(const Handle(StepRepr_HArray1OfMaterialPropertyRepresentation)& Properties) ;




  DEFINE_STANDARD_RTTI(StepElement_ElementMaterial)

protected:




private: 


Handle_TCollection_HAsciiString theMaterialId;
Handle_TCollection_HAsciiString theDescription;
Handle_StepRepr_HArray1OfMaterialPropertyRepresentation theProperties;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
