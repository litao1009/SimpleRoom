// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Aspect_Driver_HeaderFile
#define _Aspect_Driver_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_Aspect_Driver_HeaderFile
#include <Handle_Aspect_Driver.hxx>
#endif

#ifndef _Handle_Aspect_ColorMap_HeaderFile
#include <Handle_Aspect_ColorMap.hxx>
#endif
#ifndef _Handle_Aspect_TypeMap_HeaderFile
#include <Handle_Aspect_TypeMap.hxx>
#endif
#ifndef _Handle_Aspect_WidthMap_HeaderFile
#include <Handle_Aspect_WidthMap.hxx>
#endif
#ifndef _Handle_Aspect_FontMap_HeaderFile
#include <Handle_Aspect_FontMap.hxx>
#endif
#ifndef _Handle_Aspect_MarkMap_HeaderFile
#include <Handle_Aspect_MarkMap.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _MMgt_TShared_HeaderFile
#include <MMgt_TShared.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Quantity_PlaneAngle_HeaderFile
#include <Quantity_PlaneAngle.hxx>
#endif
#ifndef _Quantity_Factor_HeaderFile
#include <Quantity_Factor.hxx>
#endif
#ifndef _Handle_Standard_Transient_HeaderFile
#include <Handle_Standard_Transient.hxx>
#endif
#ifndef _Standard_CString_HeaderFile
#include <Standard_CString.hxx>
#endif
#ifndef _Standard_ShortReal_HeaderFile
#include <Standard_ShortReal.hxx>
#endif
#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
#ifndef _Aspect_TypeOfText_HeaderFile
#include <Aspect_TypeOfText.hxx>
#endif
#ifndef _Quantity_Ratio_HeaderFile
#include <Quantity_Ratio.hxx>
#endif
#ifndef _Quantity_Length_HeaderFile
#include <Quantity_Length.hxx>
#endif
class Aspect_ColorMap;
class Aspect_TypeMap;
class Aspect_WidthMap;
class Aspect_FontMap;
class Aspect_MarkMap;
class Aspect_DriverDefinitionError;
class Aspect_DriverError;
class Aspect_UndefinedMap;
class Standard_Transient;
class TShort_Array1OfShortReal;
class TCollection_ExtendedString;


//! Defines the common behaviour of the output driver. <br>
//!  Warning: Permits to defines polyline,polygon,marker and text attributes in relation <br>
//!	    with the SINGLE primitives DrawPolyline(),DrawPolygon(),.... <br>
//!	    or the INCREMENTAL primitives BeginPolyline(),BeginPolygon(),... <br>
//!	    or the SET of primitives BeginArcs(),BeginMarkers(),... <br>
//! <br>
//!	    NOTE that : <br>
//!	    The incremental primitives are interesting to used because <br>
//!	    no more arrays are necessary to fill it. <br>
//!	    The set of primitives are interesting to used because this <br>
//!	    increase the drawing performances. <br>
class Aspect_Driver : public MMgt_TShared {

public:

  //! Flush all graphics and Wait after up to date <br>
  Standard_EXPORT   virtual  void EndDraw(const Standard_Boolean Synchronize = Standard_False)  = 0;
  
  Standard_EXPORT     void SetColorMap(const Handle(Aspect_ColorMap)& aColorMap) ;
  
  Standard_EXPORT     void SetTypeMap(const Handle(Aspect_TypeMap)& aTypeMap) ;
  
  Standard_EXPORT     void SetWidthMap(const Handle(Aspect_WidthMap)& aWidthMap) ;
  
//! Sets the current font map to this driver and Enable/Disable <br>
//! this driver to use MDTV fonts instead system fonts. <br>
  Standard_EXPORT     void SetFontMap(const Handle(Aspect_FontMap)& aFontMap) ;
  
  Standard_EXPORT     void SetMarkMap(const Handle(Aspect_MarkMap)& aMarkMap) ;
  
  Standard_EXPORT   virtual  void SetLineAttrib(const Standard_Integer ColorIndex,const Standard_Integer TypeIndex,const Standard_Integer WidthIndex)  = 0;
  
  Standard_EXPORT   virtual  void SetTextAttrib(const Standard_Integer ColorIndex,const Standard_Integer FontIndex)  = 0;
  
  Standard_EXPORT   virtual  void SetTextAttrib(const Standard_Integer ColorIndex,const Standard_Integer FontIndex,const Quantity_PlaneAngle aSlant,const Quantity_Factor aHScale,const Quantity_Factor aWScale,const Standard_Boolean isUnderlined = Standard_False)  = 0;
  
  Standard_EXPORT   virtual  void SetPolyAttrib(const Standard_Integer ColorIndex,const Standard_Integer TileIndex,const Standard_Boolean DrawEdge = Standard_False)  = 0;
  
  Standard_EXPORT   virtual  void SetMarkerAttrib(const Standard_Integer ColorIndex,const Standard_Integer WidthIndex,const Standard_Boolean FillMarker = Standard_False)  = 0;
  
  Standard_EXPORT   virtual  Standard_Boolean IsKnownImage(const Handle(Standard_Transient)& anImage)  = 0;
  
  Standard_EXPORT   virtual  Standard_Boolean SizeOfImageFile(const Standard_CString anImageFile,Standard_Integer& aWidth,Standard_Integer& aHeight) const = 0;
  
  Standard_EXPORT   virtual  void ClearImage(const Handle(Standard_Transient)& anImageId)  = 0;
  
  Standard_EXPORT   virtual  void ClearImageFile(const Standard_CString anImageFile)  = 0;
  
  Standard_EXPORT   virtual  void DrawImage(const Handle(Standard_Transient)& anImageId,const Standard_ShortReal aX,const Standard_ShortReal aY)  = 0;
  
  Standard_EXPORT   virtual  void DrawImageFile(const Standard_CString anImageFile,const Standard_ShortReal aX,const Standard_ShortReal aY,const Quantity_Factor aScale = 1.0)  = 0;
  //! Fills a complete Image . <br>
  Standard_EXPORT   virtual  void FillAndDrawImage(const Handle(Standard_Transient)& anImageId,const Standard_ShortReal aX,const Standard_ShortReal aY,const Standard_Integer aWidth,const Standard_Integer aHeight,const Standard_Address anArrayOfPixels)  = 0;
  //! Fills a line of the Image . <br>
//!  Warning: 0 <= anIndexOfLine < aHeight <br>
//!	    anIndexOfLine = 0 must be the first call <br>
  Standard_EXPORT   virtual  void FillAndDrawImage(const Handle(Standard_Transient)& anImageId,const Standard_ShortReal aX,const Standard_ShortReal aY,const Standard_Integer anIndexOfLine,const Standard_Integer aWidth,const Standard_Integer aHeight,const Standard_Address anArrayOfPixels)  = 0;
  //! Draw a polyline depending of the SetLineAttrib() attributes. <br>
  Standard_EXPORT   virtual  void DrawPolyline(const TShort_Array1OfShortReal& aListX,const TShort_Array1OfShortReal& aListY)  = 0;
  //! Draw a polygon depending of the SetPolyAttrib() attributes. <br>
  Standard_EXPORT   virtual  void DrawPolygon(const TShort_Array1OfShortReal& aListX,const TShort_Array1OfShortReal& aListY)  = 0;
  //! Draw a segment depending of the SetLineAttrib() attributes. <br>
  Standard_EXPORT   virtual  void DrawSegment(const Standard_ShortReal X1,const Standard_ShortReal Y1,const Standard_ShortReal X2,const Standard_ShortReal Y2)  = 0;
  //! Draws a text depending of the SetTextAttrib() attributes. <br>
  Standard_EXPORT   virtual  void DrawText(const TCollection_ExtendedString& aText,const Standard_ShortReal Xpos,const Standard_ShortReal Ypos,const Standard_ShortReal anAngle = 0.0,const Aspect_TypeOfText aType = Aspect_TOT_SOLID)  = 0;
  //! Draws an framed text depending of the <br>
//! SetTextAttrib() and SetPolyAttrib() attributes. <br>
//!  Warning: Coordinates must be defined in DWU space. <br>
//!	    <aMarge> defines the ratio of the space between the <br>
//!	    polygon borders and the bounding box of the text and <br>
//!	    depending of the height of the text. <br>
  Standard_EXPORT   virtual  void DrawPolyText(const TCollection_ExtendedString& aText,const Standard_ShortReal Xpos,const Standard_ShortReal Ypos,const Quantity_Ratio aMarge = 0.1,const Standard_ShortReal anAngle = 0.0,const Aspect_TypeOfText aType = Aspect_TOT_SOLID)  = 0;
  //! Draws a 1 PIXEL point depending of the SetMarkerAttrib() <br>
  Standard_EXPORT   virtual  void DrawPoint(const Standard_ShortReal X,const Standard_ShortReal Y)  = 0;
  //! Draws the prevously defined marker <aMarker> <br>
  Standard_EXPORT   virtual  void DrawMarker(const Standard_Integer aMarker,const Standard_ShortReal Xpos,const Standard_ShortReal Ypos,const Standard_ShortReal Width,const Standard_ShortReal Height,const Standard_ShortReal Angle = 0.0)  = 0;
  //! Draws an Ellipsoid arc of center <X,Y> and Radius <br>
  Standard_EXPORT   virtual  Standard_Boolean DrawArc(const Standard_ShortReal X,const Standard_ShortReal Y,const Standard_ShortReal anXradius,const Standard_ShortReal anYradius,const Standard_ShortReal aStartAngle = 0.0,const Standard_ShortReal anOpenAngle = 6.283185)  = 0;
  //! Draws an filled Ellipsoid arc of center <X,Y> and Radius <br>
  Standard_EXPORT   virtual  Standard_Boolean DrawPolyArc(const Standard_ShortReal X,const Standard_ShortReal Y,const Standard_ShortReal anXradius,const Standard_ShortReal anYradius,const Standard_ShortReal aStartAngle = 0.0,const Standard_ShortReal anOpenAngle = 6.283185)  = 0;
  //! Begin an incremental polyline primitive of <aNumber> of points . <br>
//!  Warning: Points must be added by the the DrawPoint() method. <br>
  Standard_EXPORT   virtual  void BeginPolyline(const Standard_Integer aNumber)  = 0;
  //! Begin an incremental polygon primitive of <aNumber> of points . <br>
//!  Warning: Points must be added by the the DrawPoint() method. <br>
  Standard_EXPORT   virtual  void BeginPolygon(const Standard_Integer aNumber)  = 0;
  //! Begin a set of segments . <br>
//!  Warning: Segments must be added by the DrawSegment() method. <br>
  Standard_EXPORT   virtual  void BeginSegments()  = 0;
  //! Begin a set of circles or ellips . <br>
//!  Warning: Arcs must be added by the DrawArc() methods. <br>
  Standard_EXPORT   virtual  void BeginArcs()  = 0;
  //! Begin a set of polygon circles or ellips . <br>
//!  Warning: Arcs must be added by the DrawPolyArc() methods. <br>
  Standard_EXPORT   virtual  void BeginPolyArcs()  = 0;
  //! Begin a set of markers . <br>
//!  Warning: Markers must be added by the DrawMarker() method. <br>
  Standard_EXPORT   virtual  void BeginMarkers()  = 0;
  //! Begin a set of points . <br>
//!  Warning: Points must be added by the DrawPoint() method. <br>
  Standard_EXPORT   virtual  void BeginPoints()  = 0;
  //! Close the last Begining primitive <br>
  Standard_EXPORT   virtual  void ClosePrimitive()  = 0;
  
  Standard_EXPORT     Handle_Aspect_ColorMap ColorMap() const;
  
  Standard_EXPORT     Handle_Aspect_TypeMap TypeMap() const;
  
  Standard_EXPORT     Handle_Aspect_WidthMap WidthMap() const;
  
  Standard_EXPORT     Handle_Aspect_FontMap FontMap() const;
  
  Standard_EXPORT     Handle_Aspect_MarkMap MarkMap() const;
  //! Returns the Available WorkSpace in DWU coordinates <br>
  Standard_EXPORT   virtual  void WorkSpace(Quantity_Length& Width,Quantity_Length& Heigth) const = 0;
  //! Returns the DWU value depending of <br>
//!          the PIXEL value. <br>
  Standard_EXPORT   virtual  Quantity_Length Convert(const Standard_Integer PV) const = 0;
  //! Returns the PIXEL value depending of <br>
//!          the DWU value. <br>
  Standard_EXPORT   virtual  Standard_Integer Convert(const Quantity_Length DV) const = 0;
  //! Returns the DWU position depending of <br>
//!          the PIXEL position . <br>
  Standard_EXPORT   virtual  void Convert(const Standard_Integer PX,const Standard_Integer PY,Quantity_Length& DX,Quantity_Length& DY) const = 0;
  //! Returns the PIXEL position depending of <br>
//!          the DWU position . <br>//! Returns TRUE when the driver must use MDTV fonts <br>
//!	instead system fonts. <br>
  Standard_EXPORT   virtual  void Convert(const Quantity_Length DX,const Quantity_Length DY,Standard_Integer& PX,Standard_Integer& PY) const = 0;




  DEFINE_STANDARD_RTTI(Aspect_Driver)

protected:

  
  Standard_EXPORT   Aspect_Driver();
  
  Standard_EXPORT   virtual  void InitializeColorMap(const Handle(Aspect_ColorMap)& aColorMap)  = 0;
  
  Standard_EXPORT   virtual  void InitializeTypeMap(const Handle(Aspect_TypeMap)& aTypeMap)  = 0;
  
  Standard_EXPORT   virtual  void InitializeWidthMap(const Handle(Aspect_WidthMap)& aWidthMap)  = 0;
  
  Standard_EXPORT   virtual  void InitializeFontMap(const Handle(Aspect_FontMap)& aFontMap)  = 0;
  
  Standard_EXPORT   virtual  void InitializeMarkMap(const Handle(Aspect_MarkMap)& aMarkMap)  = 0;



private: 


Handle_Aspect_ColorMap myColorMap;
Handle_Aspect_TypeMap myTypeMap;
Handle_Aspect_WidthMap myWidthMap;
Handle_Aspect_FontMap myFontMap;
Handle_Aspect_MarkMap myMarkMap;
Standard_Boolean myColorMapIsDefined;
Standard_Boolean myWidthMapIsDefined;
Standard_Boolean myTypeMapIsDefined;
Standard_Boolean myFontMapIsDefined;
Standard_Boolean myMarkMapIsDefined;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
