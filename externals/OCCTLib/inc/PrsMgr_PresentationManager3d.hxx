// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _PrsMgr_PresentationManager3d_HeaderFile
#define _PrsMgr_PresentationManager3d_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_PrsMgr_PresentationManager3d_HeaderFile
#include <Handle_PrsMgr_PresentationManager3d.hxx>
#endif

#ifndef _Handle_Graphic3d_StructureManager_HeaderFile
#include <Handle_Graphic3d_StructureManager.hxx>
#endif
#ifndef _TColStd_ListOfTransient_HeaderFile
#include <TColStd_ListOfTransient.hxx>
#endif
#ifndef _PrsMgr_PresentationManager_HeaderFile
#include <PrsMgr_PresentationManager.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Handle_PrsMgr_PresentableObject_HeaderFile
#include <Handle_PrsMgr_PresentableObject.hxx>
#endif
#ifndef _Quantity_NameOfColor_HeaderFile
#include <Quantity_NameOfColor.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Handle_Prs3d_Presentation_HeaderFile
#include <Handle_Prs3d_Presentation.hxx>
#endif
#ifndef _Handle_V3d_View_HeaderFile
#include <Handle_V3d_View.hxx>
#endif
#ifndef _Handle_Geom_Transformation_HeaderFile
#include <Handle_Geom_Transformation.hxx>
#endif
#ifndef _Quantity_Length_HeaderFile
#include <Quantity_Length.hxx>
#endif
#ifndef _Graphic3d_NameOfMaterial_HeaderFile
#include <Graphic3d_NameOfMaterial.hxx>
#endif
#ifndef _Handle_Prs3d_ShadingAspect_HeaderFile
#include <Handle_Prs3d_ShadingAspect.hxx>
#endif
#ifndef _Handle_PrsMgr_Presentation3d_HeaderFile
#include <Handle_PrsMgr_Presentation3d.hxx>
#endif
#ifndef _Handle_PrsMgr_Presentation_HeaderFile
#include <Handle_PrsMgr_Presentation.hxx>
#endif
class Graphic3d_StructureManager;
class PrsMgr_PresentableObject;
class Prs3d_Presentation;
class V3d_View;
class Geom_Transformation;
class Prs3d_ShadingAspect;
class PrsMgr_Presentation3d;
class PrsMgr_Presentation;


//! A framework to manage 3D displays, graphic entities <br>
//! and their updates. <br>
//! Used in the AIS package (Application Interactive <br>
//! Services), to enable the advanced user to define the <br>
//! default display mode of a new interactive object which <br>
//! extends the list of signatures and types. <br>
//! Definition of new display types is handled by calling <br>
//! the presentation algorithms provided by the StdPrs package. <br>
class PrsMgr_PresentationManager3d : public PrsMgr_PresentationManager {

public:

  
//! Creates a framework to manage displays and graphic <br>
//! entities with the 3D view aStructureManager. <br>
  Standard_EXPORT   PrsMgr_PresentationManager3d(const Handle(Graphic3d_StructureManager)& aStructureManager);
  
  Standard_EXPORT   virtual  Standard_Boolean Is3D() const;
  //! Highlights the graphic object aPresentableObject in <br>
//! the color aColor. <br>
//! aPresentableObject has the display mode aMode; <br>
//! this has the default value of 0, that is, the wireframe display mode. <br>
  Standard_EXPORT     void Color(const Handle(PrsMgr_PresentableObject)& aPresentableObject,const Quantity_NameOfColor aColor = Quantity_NOC_YELLOW,const Standard_Integer aMode = 0) ;
  //! highlights the boundbox of the presentation <br>
  Standard_EXPORT     void BoundBox(const Handle(PrsMgr_PresentableObject)& aPresentableObject,const Standard_Integer aMode = 0) ;
  
  Standard_EXPORT     void BeginDraw() ;
  //! stores <aPrs> in a list of structure to be displayed <br>
//!          in immediate mode. will be taken in account in EndDraw Method. <br>
  Standard_EXPORT     void AddToImmediateList(const Handle(Prs3d_Presentation)& aPrs) ;
  
  Standard_EXPORT     void EndDraw(const Handle(V3d_View)& theView,const Standard_Boolean DoubleBuffer = Standard_False) ;
  
  Standard_EXPORT     void Connect(const Handle(PrsMgr_PresentableObject)& aPresentableObject,const Handle(PrsMgr_PresentableObject)& anOtherObject,const Standard_Integer aMode = 0,const Standard_Integer anOtherMode = 0) ;
  
//! Sets the transformation aTransformation for the <br>
//! presentable object aPresentableObject. <br>
//! aPresentableObject has the display mode aMode; <br>
//! this has the default value of 0, that is, the wireframe <br>
//! display mode. <br>
  Standard_EXPORT     void Transform(const Handle(PrsMgr_PresentableObject)& aPresentableObject,const Handle(Geom_Transformation)& aTransformation,const Standard_Integer aMode = 0) ;
  
//! Sets a position to move the presentable object <br>
//! aPresentableObject to. This position is defined by the <br>
//! lengths along the x, y and z axes: X, Y and Z respectively. <br>
//! aPresentableObject has the display mode aMode; <br>
//! this has the default value of 0, that is, the wireframe display mode. <br>
  Standard_EXPORT     void Place(const Handle(PrsMgr_PresentableObject)& aPresentableObject,const Quantity_Length X,const Quantity_Length Y,const Quantity_Length Z,const Standard_Integer aMode = 0) ;
  
//! Defines the transformation aTransformation for the <br>
//! presentable object aPresentableObject. <br>
//! aPresentableObject has the display mode aMode; <br>
//! this has the default value of 0, that is, the wireframe <br>
//! display mode. <br>
  Standard_EXPORT     void Multiply(const Handle(PrsMgr_PresentableObject)& aPresentableObject,const Handle(Geom_Transformation)& aTransformation,const Standard_Integer aMode = 0) ;
  
//! Sets a position to move the presentable object <br>
//! aPresentableObject to. This position is defined by the <br>
//! lengths along the x, y and z axes: X, Y and Z respectively. <br>
//! aPresentableObject has the display mode aMode; <br>
//! this has the default value of 0, that is, the wireframe <br>
//! display mode. <br>
  Standard_EXPORT     void Move(const Handle(PrsMgr_PresentableObject)& aPresentableObject,const Quantity_Length X,const Quantity_Length Y,const Quantity_Length Z,const Standard_Integer aMode = 0) ;
  //! Returns the structure manager. <br>
       const Handle_Graphic3d_StructureManager& StructureManager() const;
  //! this method will change the color and the aspect <br>
//!          of the presentations containg shaded structures. <br>
  Standard_EXPORT     void SetShadingAspect(const Handle(PrsMgr_PresentableObject)& aPresentableObject,const Quantity_NameOfColor aColor,const Graphic3d_NameOfMaterial aMaterial,const Standard_Integer aMode = 0) ;
  //! this method will change the color and the aspect <br>
//!          of the presentations containg shaded structures. <br>
  Standard_EXPORT     void SetShadingAspect(const Handle(PrsMgr_PresentableObject)& aPresentableObject,const Handle(Prs3d_ShadingAspect)& aShadingAspect,const Standard_Integer aMode = 0) ;
  
  Standard_EXPORT     Handle_PrsMgr_Presentation3d CastPresentation(const Handle(PrsMgr_PresentableObject)& aPresentableObject,const Standard_Integer aMode = 0) const;




  DEFINE_STANDARD_RTTI(PrsMgr_PresentationManager3d)

protected:




private: 

  //! Creates a new presentation in the presentation manager. <br>
  Standard_EXPORT     Handle_PrsMgr_Presentation newPresentation(const Handle(PrsMgr_PresentableObject)& aPresentableObject) ;

Handle_Graphic3d_StructureManager myStructureManager;
TColStd_ListOfTransient myStrList;


};


#include <PrsMgr_PresentationManager3d.lxx>



// other Inline functions and methods (like "C++: function call" methods)


#endif
