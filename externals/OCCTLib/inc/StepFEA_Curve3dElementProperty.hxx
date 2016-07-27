// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StepFEA_Curve3dElementProperty_HeaderFile
#define _StepFEA_Curve3dElementProperty_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_StepFEA_Curve3dElementProperty_HeaderFile
#include <Handle_StepFEA_Curve3dElementProperty.hxx>
#endif

#ifndef _Handle_TCollection_HAsciiString_HeaderFile
#include <Handle_TCollection_HAsciiString.hxx>
#endif
#ifndef _Handle_StepFEA_HArray1OfCurveElementInterval_HeaderFile
#include <Handle_StepFEA_HArray1OfCurveElementInterval.hxx>
#endif
#ifndef _Handle_StepFEA_HArray1OfCurveElementEndOffset_HeaderFile
#include <Handle_StepFEA_HArray1OfCurveElementEndOffset.hxx>
#endif
#ifndef _Handle_StepFEA_HArray1OfCurveElementEndRelease_HeaderFile
#include <Handle_StepFEA_HArray1OfCurveElementEndRelease.hxx>
#endif
#ifndef _MMgt_TShared_HeaderFile
#include <MMgt_TShared.hxx>
#endif
class TCollection_HAsciiString;
class StepFEA_HArray1OfCurveElementInterval;
class StepFEA_HArray1OfCurveElementEndOffset;
class StepFEA_HArray1OfCurveElementEndRelease;


//! Representation of STEP entity Curve3dElementProperty <br>
class StepFEA_Curve3dElementProperty : public MMgt_TShared {

public:

  //! Empty constructor <br>
  Standard_EXPORT   StepFEA_Curve3dElementProperty();
  //! Initialize all fields (own and inherited) <br>
  Standard_EXPORT     void Init(const Handle(TCollection_HAsciiString)& aPropertyId,const Handle(TCollection_HAsciiString)& aDescription,const Handle(StepFEA_HArray1OfCurveElementInterval)& aIntervalDefinitions,const Handle(StepFEA_HArray1OfCurveElementEndOffset)& aEndOffsets,const Handle(StepFEA_HArray1OfCurveElementEndRelease)& aEndReleases) ;
  //! Returns field PropertyId <br>
  Standard_EXPORT     Handle_TCollection_HAsciiString PropertyId() const;
  //! Set field PropertyId <br>
  Standard_EXPORT     void SetPropertyId(const Handle(TCollection_HAsciiString)& PropertyId) ;
  //! Returns field Description <br>
  Standard_EXPORT     Handle_TCollection_HAsciiString Description() const;
  //! Set field Description <br>
  Standard_EXPORT     void SetDescription(const Handle(TCollection_HAsciiString)& Description) ;
  //! Returns field IntervalDefinitions <br>
  Standard_EXPORT     Handle_StepFEA_HArray1OfCurveElementInterval IntervalDefinitions() const;
  //! Set field IntervalDefinitions <br>
  Standard_EXPORT     void SetIntervalDefinitions(const Handle(StepFEA_HArray1OfCurveElementInterval)& IntervalDefinitions) ;
  //! Returns field EndOffsets <br>
  Standard_EXPORT     Handle_StepFEA_HArray1OfCurveElementEndOffset EndOffsets() const;
  //! Set field EndOffsets <br>
  Standard_EXPORT     void SetEndOffsets(const Handle(StepFEA_HArray1OfCurveElementEndOffset)& EndOffsets) ;
  //! Returns field EndReleases <br>
  Standard_EXPORT     Handle_StepFEA_HArray1OfCurveElementEndRelease EndReleases() const;
  //! Set field EndReleases <br>
  Standard_EXPORT     void SetEndReleases(const Handle(StepFEA_HArray1OfCurveElementEndRelease)& EndReleases) ;




  DEFINE_STANDARD_RTTI(StepFEA_Curve3dElementProperty)

protected:




private: 


Handle_TCollection_HAsciiString thePropertyId;
Handle_TCollection_HAsciiString theDescription;
Handle_StepFEA_HArray1OfCurveElementInterval theIntervalDefinitions;
Handle_StepFEA_HArray1OfCurveElementEndOffset theEndOffsets;
Handle_StepFEA_HArray1OfCurveElementEndRelease theEndReleases;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
