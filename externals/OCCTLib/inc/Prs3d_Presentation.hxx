// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Prs3d_Presentation_HeaderFile
#define _Prs3d_Presentation_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_Prs3d_Presentation_HeaderFile
#include <Handle_Prs3d_Presentation.hxx>
#endif

#ifndef _Handle_Graphic3d_Structure_HeaderFile
#include <Handle_Graphic3d_Structure.hxx>
#endif
#ifndef _Handle_Graphic3d_Group_HeaderFile
#include <Handle_Graphic3d_Group.hxx>
#endif
#ifndef _Graphic3d_Structure_HeaderFile
#include <Graphic3d_Structure.hxx>
#endif
#ifndef _Handle_Graphic3d_StructureManager_HeaderFile
#include <Handle_Graphic3d_StructureManager.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Handle_Graphic3d_DataStructureManager_HeaderFile
#include <Handle_Graphic3d_DataStructureManager.hxx>
#endif
#ifndef _Quantity_NameOfColor_HeaderFile
#include <Quantity_NameOfColor.hxx>
#endif
#ifndef _Handle_Prs3d_ShadingAspect_HeaderFile
#include <Handle_Prs3d_ShadingAspect.hxx>
#endif
#ifndef _Handle_Geom_Transformation_HeaderFile
#include <Handle_Geom_Transformation.hxx>
#endif
#ifndef _Quantity_Length_HeaderFile
#include <Quantity_Length.hxx>
#endif
class Graphic3d_Structure;
class Graphic3d_Group;
class Prs3d_Root;
class Graphic3d_StructureManager;
class Graphic3d_DataStructureManager;
class TColStd_Array2OfReal;
class Prs3d_ShadingAspect;
class Geom_Transformation;


//! Defines a presentation object which can be displayed, <br>
//! highlighted or erased. <br>
//! The presentation object stores the results of the <br>
//! presentation algorithms as defined in the StdPrs <br>
//! classes and the Prs3d classes inheriting Prs3d_Root. <br>
//! This presentation object is used to give display <br>
//! attributes defined at this level to <br>
//! ApplicationInteractiveServices classes at the level above. <br>
class Prs3d_Presentation : public Graphic3d_Structure {

public:

  //! Constructs a presentation object <br>
//! if <Init> is false, no color initialization is done. <br>
  Standard_EXPORT   Prs3d_Presentation(const Handle(Graphic3d_StructureManager)& aStructureManager,const Standard_Boolean Init = Standard_True);
  
  Standard_EXPORT   virtual  Handle_Graphic3d_Structure Compute(const Handle(Graphic3d_DataStructureManager)& aProjector) ;
  //! Returns the new Structure defined for the new visualization <br>
  Standard_EXPORT   virtual  Handle_Graphic3d_Structure Compute(const Handle(Graphic3d_DataStructureManager)& aProjector,const TColStd_Array2OfReal& AMatrix) ;
  //! Returns the new Structure defined for the new visualization <br>
  Standard_EXPORT   virtual  void Compute(const Handle(Graphic3d_DataStructureManager)& aProjector,Handle(Graphic3d_Structure)& aStructure) ;
  //! Returns the new Structure defined for the new visualization <br>
  Standard_EXPORT   virtual  void Compute(const Handle(Graphic3d_DataStructureManager)& aProjector,const TColStd_Array2OfReal& AMatrix,Handle(Graphic3d_Structure)& aStructure) ;
  //! displays the whole content of the presentation in white. <br>
  Standard_EXPORT     void Highlight() ;
  //! displays the whole content of the presentation in the specified color. <br>
  Standard_EXPORT     void Color(const Quantity_NameOfColor aColor) ;
  
  Standard_EXPORT     void BoundBox() ;
  
  Standard_EXPORT     void Display() ;
  
  Standard_EXPORT     void SetShadingAspect(const Handle(Prs3d_ShadingAspect)& aShadingAspect) ;
  
  Standard_EXPORT     Standard_Boolean IsPickable() const;
  
  Standard_EXPORT     void Transform(const Handle(Geom_Transformation)& aTransformation) ;
  
  Standard_EXPORT     void Place(const Quantity_Length X,const Quantity_Length Y,const Quantity_Length Z) ;
  
  Standard_EXPORT     void Multiply(const Handle(Geom_Transformation)& aTransformation) ;
  
  Standard_EXPORT     void Move(const Quantity_Length X,const Quantity_Length Y,const Quantity_Length Z) ;
  
  Standard_EXPORT     Handle_Geom_Transformation Transformation() const;
  //! removes the whole content of the presentation. <br>
//!          Does not remove the other connected presentations. <br>
//!	        if WithDestruction == Standard_False then <br>
//!		clears all the groups of primitives in the structure. <br>
  Standard_EXPORT   virtual  void Clear(const Standard_Boolean WithDestruction = Standard_True) ;
  
  Standard_EXPORT     void Connect(const Handle(Prs3d_Presentation)& aPresentation) ;
  
  Standard_EXPORT     void Remove(const Handle(Prs3d_Presentation)& aPresentation) ;
  
  Standard_EXPORT     void RemoveAll() ;
  
  Standard_EXPORT     void SetPickable() ;
  
  Standard_EXPORT     void SetUnPickable() ;


friend class Prs3d_Root;


  DEFINE_STANDARD_RTTI(Prs3d_Presentation)

protected:




private: 

  
  Standard_EXPORT     Handle_Graphic3d_Group CurrentGroup() const;
  
  Standard_EXPORT     Handle_Graphic3d_Group NewGroup() ;

Handle_Graphic3d_Structure myStruct;
Handle_Graphic3d_Group myCurrentGroup;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
