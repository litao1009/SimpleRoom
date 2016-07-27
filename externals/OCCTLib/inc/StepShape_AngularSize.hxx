// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StepShape_AngularSize_HeaderFile
#define _StepShape_AngularSize_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_StepShape_AngularSize_HeaderFile
#include <Handle_StepShape_AngularSize.hxx>
#endif

#ifndef _StepShape_AngleRelator_HeaderFile
#include <StepShape_AngleRelator.hxx>
#endif
#ifndef _StepShape_DimensionalSize_HeaderFile
#include <StepShape_DimensionalSize.hxx>
#endif
#ifndef _Handle_StepRepr_ShapeAspect_HeaderFile
#include <Handle_StepRepr_ShapeAspect.hxx>
#endif
#ifndef _Handle_TCollection_HAsciiString_HeaderFile
#include <Handle_TCollection_HAsciiString.hxx>
#endif
class StepRepr_ShapeAspect;
class TCollection_HAsciiString;


//! Representation of STEP entity AngularSize <br>
class StepShape_AngularSize : public StepShape_DimensionalSize {

public:

  //! Empty constructor <br>
  Standard_EXPORT   StepShape_AngularSize();
  //! Initialize all fields (own and inherited) <br>
  Standard_EXPORT     void Init(const Handle(StepRepr_ShapeAspect)& aDimensionalSize_AppliesTo,const Handle(TCollection_HAsciiString)& aDimensionalSize_Name,const StepShape_AngleRelator aAngleSelection) ;
  //! Returns field AngleSelection <br>
  Standard_EXPORT     StepShape_AngleRelator AngleSelection() const;
  //! Set field AngleSelection <br>
  Standard_EXPORT     void SetAngleSelection(const StepShape_AngleRelator AngleSelection) ;




  DEFINE_STANDARD_RTTI(StepShape_AngularSize)

protected:




private: 


StepShape_AngleRelator theAngleSelection;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
