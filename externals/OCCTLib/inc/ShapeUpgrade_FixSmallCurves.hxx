// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _ShapeUpgrade_FixSmallCurves_HeaderFile
#define _ShapeUpgrade_FixSmallCurves_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_ShapeUpgrade_FixSmallCurves_HeaderFile
#include <Handle_ShapeUpgrade_FixSmallCurves.hxx>
#endif

#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Handle_ShapeUpgrade_SplitCurve3d_HeaderFile
#include <Handle_ShapeUpgrade_SplitCurve3d.hxx>
#endif
#ifndef _Handle_ShapeUpgrade_SplitCurve2d_HeaderFile
#include <Handle_ShapeUpgrade_SplitCurve2d.hxx>
#endif
#ifndef _TopoDS_Edge_HeaderFile
#include <TopoDS_Edge.hxx>
#endif
#ifndef _TopoDS_Face_HeaderFile
#include <TopoDS_Face.hxx>
#endif
#ifndef _ShapeUpgrade_Tool_HeaderFile
#include <ShapeUpgrade_Tool.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Handle_Geom_Curve_HeaderFile
#include <Handle_Geom_Curve.hxx>
#endif
#ifndef _Handle_Geom2d_Curve_HeaderFile
#include <Handle_Geom2d_Curve.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _ShapeExtend_Status_HeaderFile
#include <ShapeExtend_Status.hxx>
#endif
class ShapeUpgrade_SplitCurve3d;
class ShapeUpgrade_SplitCurve2d;
class TopoDS_Edge;
class TopoDS_Face;
class Geom_Curve;
class Geom2d_Curve;



class ShapeUpgrade_FixSmallCurves : public ShapeUpgrade_Tool {

public:

  
  Standard_EXPORT   ShapeUpgrade_FixSmallCurves();
  
  Standard_EXPORT     void Init(const TopoDS_Edge& theEdge,const TopoDS_Face& theFace) ;
  
  Standard_EXPORT   virtual  Standard_Boolean Approx(Handle(Geom_Curve)& Curve3d,Handle(Geom2d_Curve)& Curve2d,Handle(Geom2d_Curve)& Curve2dR,Standard_Real& First,Standard_Real& Last) ;
  //! Sets the tool for splitting 3D curves. <br>
  Standard_EXPORT     void SetSplitCurve3dTool(const Handle(ShapeUpgrade_SplitCurve3d)& splitCurve3dTool) ;
  //! Sets the tool for splitting pcurves. <br>
  Standard_EXPORT     void SetSplitCurve2dTool(const Handle(ShapeUpgrade_SplitCurve2d)& splitCurve2dTool) ;
  //! Queries the status of last call to Perform <br>
//! OK   : <br>
//! DONE1: <br>
//! DONE2: <br>
//! FAIL1: <br>
  Standard_EXPORT     Standard_Boolean Status(const ShapeExtend_Status status) const;




  DEFINE_STANDARD_RTTI(ShapeUpgrade_FixSmallCurves)

protected:

  
  Standard_EXPORT   virtual  Handle_ShapeUpgrade_SplitCurve3d GetSplitCurve3dTool() const;
  //! Returns the tool for splitting pcurves. <br>
  Standard_EXPORT   virtual  Handle_ShapeUpgrade_SplitCurve2d GetSplitCurve2dTool() const;

Standard_Integer myStatus;
Handle_ShapeUpgrade_SplitCurve3d mySplitCurve3dTool;
Handle_ShapeUpgrade_SplitCurve2d mySplitCurve2dTool;
TopoDS_Edge myEdge;
TopoDS_Face myFace;


private: 




};





// other Inline functions and methods (like "C++: function call" methods)


#endif
