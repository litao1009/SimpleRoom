// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TopOpeBRepTool_REGUS_HeaderFile
#define _TopOpeBRepTool_REGUS_HeaderFile

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
#ifndef _TopTools_DataMapOfShapeListOfShape_HeaderFile
#include <TopTools_DataMapOfShapeListOfShape.hxx>
#endif
#ifndef _TopoDS_Shape_HeaderFile
#include <TopoDS_Shape.hxx>
#endif
#ifndef _TopTools_IndexedMapOfShape_HeaderFile
#include <TopTools_IndexedMapOfShape.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _TopTools_MapOfShape_HeaderFile
#include <TopTools_MapOfShape.hxx>
#endif
#ifndef _TopTools_ListOfShape_HeaderFile
#include <TopTools_ListOfShape.hxx>
#endif
class TopoDS_Shape;
class TopoDS_Face;
class TopTools_ListOfShape;
class TopTools_DataMapOfShapeListOfShape;
class TopoDS_Edge;



class TopOpeBRepTool_REGUS  {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   TopOpeBRepTool_REGUS();
  
  Standard_EXPORT     void Init(const TopoDS_Shape& S) ;
  
  Standard_EXPORT    const TopoDS_Shape& S() const;
  
  Standard_EXPORT     Standard_Boolean MapS() ;
  
  Standard_EXPORT   static  Standard_Boolean WireToFace(const TopoDS_Face& Fanc,const TopTools_ListOfShape& nWs,TopTools_ListOfShape& nFs) ;
  
  Standard_EXPORT   static  Standard_Boolean SplitF(const TopoDS_Face& Fanc,TopTools_ListOfShape& FSplits) ;
  
  Standard_EXPORT     Standard_Boolean SplitFaces() ;
  
  Standard_EXPORT     Standard_Boolean REGU() ;
  
  Standard_EXPORT     void SetFsplits(TopTools_DataMapOfShapeListOfShape& Fsplits) ;
  
  Standard_EXPORT     void GetFsplits(TopTools_DataMapOfShapeListOfShape& Fsplits) const;
  
  Standard_EXPORT     void SetOshNsh(TopTools_DataMapOfShapeListOfShape& OshNsh) ;
  
  Standard_EXPORT     void GetOshNsh(TopTools_DataMapOfShapeListOfShape& OshNsh) const;
  
  Standard_EXPORT     Standard_Boolean InitBlock() ;
  
  Standard_EXPORT     Standard_Boolean NextinBlock() ;
  
  Standard_EXPORT     Standard_Boolean NearestF(const TopoDS_Edge& e,const TopTools_ListOfShape& lof,TopoDS_Face& ffound) const;





protected:





private:



Standard_Boolean hasnewsplits;
TopTools_DataMapOfShapeListOfShape myFsplits;
TopTools_DataMapOfShapeListOfShape myOshNsh;
TopoDS_Shape myS;
TopTools_DataMapOfShapeListOfShape mymapeFsstatic;
TopTools_DataMapOfShapeListOfShape mymapeFs;
TopTools_IndexedMapOfShape mymapemult;
Standard_Integer mynF;
Standard_Integer myoldnF;
TopoDS_Shape myf;
TopTools_MapOfShape myedstoconnect;
TopTools_ListOfShape mylFinBlock;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
