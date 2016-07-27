// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StepAP203_PersonOrganizationItem_HeaderFile
#define _StepAP203_PersonOrganizationItem_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _StepData_SelectType_HeaderFile
#include <StepData_SelectType.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Handle_Standard_Transient_HeaderFile
#include <Handle_Standard_Transient.hxx>
#endif
#ifndef _Handle_StepAP203_Change_HeaderFile
#include <Handle_StepAP203_Change.hxx>
#endif
#ifndef _Handle_StepAP203_StartWork_HeaderFile
#include <Handle_StepAP203_StartWork.hxx>
#endif
#ifndef _Handle_StepAP203_ChangeRequest_HeaderFile
#include <Handle_StepAP203_ChangeRequest.hxx>
#endif
#ifndef _Handle_StepAP203_StartRequest_HeaderFile
#include <Handle_StepAP203_StartRequest.hxx>
#endif
#ifndef _Handle_StepRepr_ConfigurationItem_HeaderFile
#include <Handle_StepRepr_ConfigurationItem.hxx>
#endif
#ifndef _Handle_StepBasic_Product_HeaderFile
#include <Handle_StepBasic_Product.hxx>
#endif
#ifndef _Handle_StepBasic_ProductDefinitionFormation_HeaderFile
#include <Handle_StepBasic_ProductDefinitionFormation.hxx>
#endif
#ifndef _Handle_StepBasic_ProductDefinition_HeaderFile
#include <Handle_StepBasic_ProductDefinition.hxx>
#endif
#ifndef _Handle_StepBasic_Contract_HeaderFile
#include <Handle_StepBasic_Contract.hxx>
#endif
#ifndef _Handle_StepBasic_SecurityClassification_HeaderFile
#include <Handle_StepBasic_SecurityClassification.hxx>
#endif
class Standard_Transient;
class StepAP203_Change;
class StepAP203_StartWork;
class StepAP203_ChangeRequest;
class StepAP203_StartRequest;
class StepRepr_ConfigurationItem;
class StepBasic_Product;
class StepBasic_ProductDefinitionFormation;
class StepBasic_ProductDefinition;
class StepBasic_Contract;
class StepBasic_SecurityClassification;


//! Representation of STEP SELECT type PersonOrganizationItem <br>
class StepAP203_PersonOrganizationItem  : public StepData_SelectType {
public:

  DEFINE_STANDARD_ALLOC

  //! Empty constructor <br>
  Standard_EXPORT   StepAP203_PersonOrganizationItem();
  //! Recognizes a kind of PersonOrganizationItem select type <br>
//!          1 -> Change from StepAP203 <br>
//!          2 -> StartWork from StepAP203 <br>
//!          3 -> ChangeRequest from StepAP203 <br>
//!          4 -> StartRequest from StepAP203 <br>
//!          5 -> ConfigurationItem from StepRepr <br>
//!          6 -> Product from StepBasic <br>
//!          7 -> ProductDefinitionFormation from StepBasic <br>
//!          8 -> ProductDefinition from StepBasic <br>
//!          9 -> Contract from StepBasic <br>
//!          10 -> SecurityClassification from StepBasic <br>
//!          0 else <br>
  Standard_EXPORT     Standard_Integer CaseNum(const Handle(Standard_Transient)& ent) const;
  //! Returns Value as Change (or Null if another type) <br>
  Standard_EXPORT     Handle_StepAP203_Change Change() const;
  //! Returns Value as StartWork (or Null if another type) <br>
  Standard_EXPORT     Handle_StepAP203_StartWork StartWork() const;
  //! Returns Value as ChangeRequest (or Null if another type) <br>
  Standard_EXPORT     Handle_StepAP203_ChangeRequest ChangeRequest() const;
  //! Returns Value as StartRequest (or Null if another type) <br>
  Standard_EXPORT     Handle_StepAP203_StartRequest StartRequest() const;
  //! Returns Value as ConfigurationItem (or Null if another type) <br>
  Standard_EXPORT     Handle_StepRepr_ConfigurationItem ConfigurationItem() const;
  //! Returns Value as Product (or Null if another type) <br>
  Standard_EXPORT     Handle_StepBasic_Product Product() const;
  //! Returns Value as ProductDefinitionFormation (or Null if another type) <br>
  Standard_EXPORT     Handle_StepBasic_ProductDefinitionFormation ProductDefinitionFormation() const;
  //! Returns Value as ProductDefinition (or Null if another type) <br>
  Standard_EXPORT     Handle_StepBasic_ProductDefinition ProductDefinition() const;
  //! Returns Value as Contract (or Null if another type) <br>
  Standard_EXPORT     Handle_StepBasic_Contract Contract() const;
  //! Returns Value as SecurityClassification (or Null if another type) <br>
  Standard_EXPORT     Handle_StepBasic_SecurityClassification SecurityClassification() const;





protected:





private:





};





// other Inline functions and methods (like "C++: function call" methods)


#endif
