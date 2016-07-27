// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TopOpeBRepDS_InterferenceTool_HeaderFile
#define _TopOpeBRepDS_InterferenceTool_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _Handle_TopOpeBRepDS_Interference_HeaderFile
#include <Handle_TopOpeBRepDS_Interference.hxx>
#endif
#ifndef _TopOpeBRepDS_Kind_HeaderFile
#include <TopOpeBRepDS_Kind.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Handle_Geom2d_Curve_HeaderFile
#include <Handle_Geom2d_Curve.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _TopOpeBRepDS_Config_HeaderFile
#include <TopOpeBRepDS_Config.hxx>
#endif
class TopOpeBRepDS_Interference;
class TopOpeBRepDS_Transition;
class Geom2d_Curve;



class TopOpeBRepDS_InterferenceTool  {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   static  Handle_TopOpeBRepDS_Interference MakeEdgeInterference(const TopOpeBRepDS_Transition& T,const TopOpeBRepDS_Kind SK,const Standard_Integer SI,const TopOpeBRepDS_Kind GK,const Standard_Integer GI,const Standard_Real P) ;
  
  Standard_EXPORT   static  Handle_TopOpeBRepDS_Interference MakeCurveInterference(const TopOpeBRepDS_Transition& T,const TopOpeBRepDS_Kind SK,const Standard_Integer SI,const TopOpeBRepDS_Kind GK,const Standard_Integer GI,const Standard_Real P) ;
  //!  duplicate I in a new interference with Complement() transition. <br>
  Standard_EXPORT   static  Handle_TopOpeBRepDS_Interference DuplicateCurvePointInterference(const Handle(TopOpeBRepDS_Interference)& I) ;
  
  Standard_EXPORT   static  Handle_TopOpeBRepDS_Interference MakeFaceCurveInterference(const TopOpeBRepDS_Transition& Transition,const Standard_Integer FaceI,const Standard_Integer CurveI,const Handle(Geom2d_Curve)& PC) ;
  
  Standard_EXPORT   static  Handle_TopOpeBRepDS_Interference MakeSolidSurfaceInterference(const TopOpeBRepDS_Transition& Transition,const Standard_Integer SolidI,const Standard_Integer SurfaceI) ;
  
  Standard_EXPORT   static  Handle_TopOpeBRepDS_Interference MakeEdgeVertexInterference(const TopOpeBRepDS_Transition& Transition,const Standard_Integer EdgeI,const Standard_Integer VertexI,const Standard_Boolean VertexIsBound,const TopOpeBRepDS_Config Config,const Standard_Real param) ;
  
  Standard_EXPORT   static  Handle_TopOpeBRepDS_Interference MakeFaceEdgeInterference(const TopOpeBRepDS_Transition& Transition,const Standard_Integer FaceI,const Standard_Integer EdgeI,const Standard_Boolean EdgeIsBound,const TopOpeBRepDS_Config Config) ;
  
  Standard_EXPORT   static  Standard_Real Parameter(const Handle(TopOpeBRepDS_Interference)& CPI) ;
  
  Standard_EXPORT   static  void Parameter(const Handle(TopOpeBRepDS_Interference)& CPI,const Standard_Real Par) ;





protected:





private:





};





// other Inline functions and methods (like "C++: function call" methods)


#endif
