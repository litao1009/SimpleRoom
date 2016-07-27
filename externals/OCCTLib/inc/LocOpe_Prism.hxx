// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _LocOpe_Prism_HeaderFile
#define _LocOpe_Prism_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _TopoDS_Shape_HeaderFile
#include <TopoDS_Shape.hxx>
#endif
#ifndef _gp_Vec_HeaderFile
#include <gp_Vec.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _TopTools_DataMapOfShapeListOfShape_HeaderFile
#include <TopTools_DataMapOfShapeListOfShape.hxx>
#endif
#ifndef _Handle_Geom_Curve_HeaderFile
#include <Handle_Geom_Curve.hxx>
#endif
class Standard_NoSuchObject;
class StdFail_NotDone;
class TopoDS_Shape;
class gp_Vec;
class TopTools_ListOfShape;
class TColGeom_SequenceOfCurve;
class Geom_Curve;


//! Defines a prism (using Prism from BRepSweep) <br>
//!          with modifications provided for the Prism feature. <br>
class LocOpe_Prism  {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   LocOpe_Prism();
  
  Standard_EXPORT   LocOpe_Prism(const TopoDS_Shape& Base,const gp_Vec& V);
  
  Standard_EXPORT   LocOpe_Prism(const TopoDS_Shape& Base,const gp_Vec& V,const gp_Vec& Vectra);
  
  Standard_EXPORT     void Perform(const TopoDS_Shape& Base,const gp_Vec& V) ;
  
  Standard_EXPORT     void Perform(const TopoDS_Shape& Base,const gp_Vec& V,const gp_Vec& Vtra) ;
  
  Standard_EXPORT    const TopoDS_Shape& FirstShape() const;
  
  Standard_EXPORT    const TopoDS_Shape& LastShape() const;
  
  Standard_EXPORT    const TopoDS_Shape& Shape() const;
  
  Standard_EXPORT    const TopTools_ListOfShape& Shapes(const TopoDS_Shape& S) const;
  
  Standard_EXPORT     void Curves(TColGeom_SequenceOfCurve& SCurves) const;
  
  Standard_EXPORT     Handle_Geom_Curve BarycCurve() const;





protected:





private:

  
  Standard_EXPORT     void IntPerf() ;


TopoDS_Shape myBase;
gp_Vec myVec;
gp_Vec myTra;
Standard_Boolean myIsTrans;
Standard_Boolean myDone;
TopoDS_Shape myRes;
TopoDS_Shape myFirstShape;
TopoDS_Shape myLastShape;
TopTools_DataMapOfShapeListOfShape myMap;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
