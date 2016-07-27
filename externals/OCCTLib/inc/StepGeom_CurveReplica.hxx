// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StepGeom_CurveReplica_HeaderFile
#define _StepGeom_CurveReplica_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_StepGeom_CurveReplica_HeaderFile
#include <Handle_StepGeom_CurveReplica.hxx>
#endif

#ifndef _Handle_StepGeom_Curve_HeaderFile
#include <Handle_StepGeom_Curve.hxx>
#endif
#ifndef _Handle_StepGeom_CartesianTransformationOperator_HeaderFile
#include <Handle_StepGeom_CartesianTransformationOperator.hxx>
#endif
#ifndef _StepGeom_Curve_HeaderFile
#include <StepGeom_Curve.hxx>
#endif
#ifndef _Handle_TCollection_HAsciiString_HeaderFile
#include <Handle_TCollection_HAsciiString.hxx>
#endif
class StepGeom_Curve;
class StepGeom_CartesianTransformationOperator;
class TCollection_HAsciiString;



class StepGeom_CurveReplica : public StepGeom_Curve {

public:

  //! Returns a CurveReplica <br>
  Standard_EXPORT   StepGeom_CurveReplica();
  
  Standard_EXPORT   virtual  void Init(const Handle(TCollection_HAsciiString)& aName) ;
  
  Standard_EXPORT   virtual  void Init(const Handle(TCollection_HAsciiString)& aName,const Handle(StepGeom_Curve)& aParentCurve,const Handle(StepGeom_CartesianTransformationOperator)& aTransformation) ;
  
  Standard_EXPORT     void SetParentCurve(const Handle(StepGeom_Curve)& aParentCurve) ;
  
  Standard_EXPORT     Handle_StepGeom_Curve ParentCurve() const;
  
  Standard_EXPORT     void SetTransformation(const Handle(StepGeom_CartesianTransformationOperator)& aTransformation) ;
  
  Standard_EXPORT     Handle_StepGeom_CartesianTransformationOperator Transformation() const;




  DEFINE_STANDARD_RTTI(StepGeom_CurveReplica)

protected:




private: 


Handle_StepGeom_Curve parentCurve;
Handle_StepGeom_CartesianTransformationOperator transformation;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
