// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Contap_TheSearchInsideOfContour_HeaderFile
#define _Contap_TheSearchInsideOfContour_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _IntSurf_SequenceOfInteriorPoint_HeaderFile
#include <IntSurf_SequenceOfInteriorPoint.hxx>
#endif
#ifndef _Handle_Adaptor3d_HSurface_HeaderFile
#include <Handle_Adaptor3d_HSurface.hxx>
#endif
#ifndef _Handle_Adaptor3d_TopolTool_HeaderFile
#include <Handle_Adaptor3d_TopolTool.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
class StdFail_NotDone;
class Standard_OutOfRange;
class Adaptor3d_HSurface;
class Adaptor3d_HSurfaceTool;
class Adaptor3d_TopolTool;
class Contap_HContTool;
class Contap_TheSurfFunctionOfContour;
class IntSurf_InteriorPoint;



class Contap_TheSearchInsideOfContour  {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   Contap_TheSearchInsideOfContour();
  
  Standard_EXPORT   Contap_TheSearchInsideOfContour(Contap_TheSurfFunctionOfContour& F,const Handle(Adaptor3d_HSurface)& Surf,const Handle(Adaptor3d_TopolTool)& T,const Standard_Real Epsilon);
  
  Standard_EXPORT     void Perform(Contap_TheSurfFunctionOfContour& F,const Handle(Adaptor3d_HSurface)& Surf,const Handle(Adaptor3d_TopolTool)& T,const Standard_Real Epsilon) ;
  
  Standard_EXPORT     void Perform(Contap_TheSurfFunctionOfContour& F,const Handle(Adaptor3d_HSurface)& Surf,const Standard_Real UStart,const Standard_Real VStart) ;
  
        Standard_Boolean IsDone() const;
  
        Standard_Integer NbPoints() const;
  
       const IntSurf_InteriorPoint& Value(const Standard_Integer Index) const;





protected:





private:



Standard_Boolean done;
IntSurf_SequenceOfInteriorPoint list;


};

#define ThePSurface Handle_Adaptor3d_HSurface
#define ThePSurface_hxx <Adaptor3d_HSurface.hxx>
#define ThePSurfaceTool Adaptor3d_HSurfaceTool
#define ThePSurfaceTool_hxx <Adaptor3d_HSurfaceTool.hxx>
#define Handle_TheTopolTool Handle_Adaptor3d_TopolTool
#define TheTopolTool Adaptor3d_TopolTool
#define TheTopolTool_hxx <Adaptor3d_TopolTool.hxx>
#define TheSITool Contap_HContTool
#define TheSITool_hxx <Contap_HContTool.hxx>
#define TheFunction Contap_TheSurfFunctionOfContour
#define TheFunction_hxx <Contap_TheSurfFunctionOfContour.hxx>
#define IntStart_SearchInside Contap_TheSearchInsideOfContour
#define IntStart_SearchInside_hxx <Contap_TheSearchInsideOfContour.hxx>

#include <IntStart_SearchInside.lxx>

#undef ThePSurface
#undef ThePSurface_hxx
#undef ThePSurfaceTool
#undef ThePSurfaceTool_hxx
#undef Handle_TheTopolTool
#undef TheTopolTool
#undef TheTopolTool_hxx
#undef TheSITool
#undef TheSITool_hxx
#undef TheFunction
#undef TheFunction_hxx
#undef IntStart_SearchInside
#undef IntStart_SearchInside_hxx


// other Inline functions and methods (like "C++: function call" methods)


#endif
