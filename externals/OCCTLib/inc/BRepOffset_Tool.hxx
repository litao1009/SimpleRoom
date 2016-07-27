// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _BRepOffset_Tool_HeaderFile
#define _BRepOffset_Tool_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _TopAbs_Orientation_HeaderFile
#include <TopAbs_Orientation.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _TopAbs_State_HeaderFile
#include <TopAbs_State.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Handle_BRepAlgo_AsDes_HeaderFile
#include <Handle_BRepAlgo_AsDes.hxx>
#endif
#ifndef _Handle_Geom_Curve_HeaderFile
#include <Handle_Geom_Curve.hxx>
#endif
class TopoDS_Edge;
class TopoDS_Vertex;
class TopoDS_Face;
class TopTools_ListOfShape;
class BRepOffset_Analyse;
class TopTools_DataMapOfShapeShape;
class TopoDS_Wire;
class TopoDS_Shape;
class TopTools_DataMapOfShapeListOfShape;
class TopTools_MapOfShape;
class TopTools_IndexedMapOfShape;
class BRepAlgo_AsDes;
class BRepAlgo_Image;
class Geom_Curve;



class BRepOffset_Tool  {
public:

  DEFINE_STANDARD_ALLOC

  //! <V1> is the FirstVertex ,<V2> is the Last Vertex of <Edge> <br>
//!          taking account the orientation of Edge. <br>
  Standard_EXPORT   static  void EdgeVertices(const TopoDS_Edge& E,TopoDS_Vertex& V1,TopoDS_Vertex& V2) ;
  //! returns the cumul  of the orientation  of <Edge> <br>
//!          and thc containing wire in <Face> <br>
  Standard_EXPORT   static  TopAbs_Orientation OriEdgeInFace(const TopoDS_Edge& E,const TopoDS_Face& F) ;
  //! <E> is a section  between <F1> and <F2>.  Computes <br>
//!          <O1> the orientation of <E> in <F1> influenced by <F2>. <br>
//!          idem for <O2>. <br>
  Standard_EXPORT   static  void OrientSection(const TopoDS_Edge& E,const TopoDS_Face& F1,const TopoDS_Face& F2,TopAbs_Orientation& O1,TopAbs_Orientation& O2) ;
  //! Returns True if  <F1> and <F2> has common Vertices <br>
//!          or edges , <LE> contains the common edges. <LV> the <br>
//!          common vertices. <br>
  Standard_EXPORT   static  Standard_Boolean HasCommonShapes(const TopoDS_Face& F1,const TopoDS_Face& F2,TopTools_ListOfShape& LE,TopTools_ListOfShape& LV) ;
  //! Computes the   Section betwwen  <F1> and  <F2> the <br>
//!          edges solution   are  stored in <LInt1>  with  the <br>
//!          orientation on <F1>, the sames edges are stored in <br>
//!          <Lint2> with the orientation on <F2>. <br>
  Standard_EXPORT   static  void Inter3D(const TopoDS_Face& F1,const TopoDS_Face& F2,TopTools_ListOfShape& LInt1,TopTools_ListOfShape& LInt2,const TopAbs_State Side,const TopoDS_Edge& RefEdge,const Standard_Boolean IsRefEdgeDefined = Standard_False) ;
  //! Find if the edges <Edges> of the face <F2> are on <br>
//!          the face <F1>. <br>
//!          Set in <LInt1> <LInt2> the updated edges. <br>
//!          If all the edges are computed, returns true. <br>
//! <br>
  Standard_EXPORT   static  Standard_Boolean TryProject(const TopoDS_Face& F1,const TopoDS_Face& F2,const TopTools_ListOfShape& Edges,TopTools_ListOfShape& LInt1,TopTools_ListOfShape& LInt2,const TopAbs_State Side,const Standard_Real TolConf) ;
  
  Standard_EXPORT   static  void PipeInter(const TopoDS_Face& F1,const TopoDS_Face& F2,TopTools_ListOfShape& LInt1,TopTools_ListOfShape& LInt2,const TopAbs_State Side) ;
  
  Standard_EXPORT   static  void Inter2d(const TopoDS_Face& F,const TopoDS_Edge& E1,const TopoDS_Edge& E2,TopTools_ListOfShape& LV,const Standard_Real Tol) ;
  
  Standard_EXPORT   static  void InterOrExtent(const TopoDS_Face& F1,const TopoDS_Face& F2,TopTools_ListOfShape& LInt1,TopTools_ListOfShape& LInt2,const TopAbs_State Side) ;
  
  Standard_EXPORT   static  void CheckBounds(const TopoDS_Face& F,const BRepOffset_Analyse& Analyse,Standard_Boolean& enlargeU,Standard_Boolean& enlargeVfirst,Standard_Boolean& enlargeVlast) ;
  //! if <ChangeGeom> is TRUE  ,   the surface  can  be <br>
//!          changed . <br>
//!          if <UpdatePCurve>  is  TRUE, update the  pcurves of the <br>
//!          edges of <F> on   the new surface.if the surface has  been changed, <br>
//!          Returns  True if The Surface of  <NF> has changed. <br>
//! <br>
  Standard_EXPORT   static  Standard_Boolean EnLargeFace(const TopoDS_Face& F,TopoDS_Face& NF,const Standard_Boolean ChangeGeom,const Standard_Boolean UpDatePCurve = Standard_False,const Standard_Boolean enlargeU = Standard_True,const Standard_Boolean enlargeVfirst = Standard_True,const Standard_Boolean enlargeVlast = Standard_True) ;
  
  Standard_EXPORT   static  void ExtentFace(const TopoDS_Face& F,TopTools_DataMapOfShapeShape& ConstShapes,TopTools_DataMapOfShapeShape& ToBuild,const TopAbs_State Side,const Standard_Real TolConf,TopoDS_Face& NF) ;
  //! Via the wire explorer store in <NOnV1> for <br>
//!          an Edge <E> of <W> his Edge neighbour on the first <br>
//!          vertex <V1> of <E>. <br>
//!          Store in NOnV2 the Neighbour of <E>on the last <br>
//!          vertex <V2> of <E>. <br>
  Standard_EXPORT   static  void BuildNeighbour(const TopoDS_Wire& W,const TopoDS_Face& F,TopTools_DataMapOfShapeShape& NOnV1,TopTools_DataMapOfShapeShape& NOnV2) ;
  //! Store in MVE for a vertex <V>  in <S> the incident <br>
//!          edges <E> in <S>. <br>
//!          An Edge is Store only one Time for a vertex. <br>
  Standard_EXPORT   static  void MapVertexEdges(const TopoDS_Shape& S,TopTools_DataMapOfShapeListOfShape& MVE) ;
  //! Remove the non valid   part of an offsetshape <br>
//!          1 - Remove all the free boundary  and the faces <br>
//!          connex to such edges. <br>
//!          2 - Remove all the shapes not  valid in the result <br>
//!          (according to the side of offseting) <br>
//!   in this verion only the first point is implemented. <br>
  Standard_EXPORT   static  TopoDS_Shape Deboucle3D(const TopoDS_Shape& S,const TopTools_MapOfShape& Boundary) ;
  
  Standard_EXPORT   static  void CorrectOrientation(const TopoDS_Shape& SI,const TopTools_IndexedMapOfShape& NewEdges,Handle(BRepAlgo_AsDes)& AsDes,BRepAlgo_Image& InitOffset,const Standard_Real Offset) ;
  
  Standard_EXPORT   static  Standard_Real Gabarit(const Handle(Geom_Curve)& aCurve) ;





protected:





private:





};





// other Inline functions and methods (like "C++: function call" methods)


#endif
