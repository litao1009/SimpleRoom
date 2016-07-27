// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _IGESGeom_CircularArc_HeaderFile
#define _IGESGeom_CircularArc_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_IGESGeom_CircularArc_HeaderFile
#include <Handle_IGESGeom_CircularArc.hxx>
#endif

#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _gp_XY_HeaderFile
#include <gp_XY.hxx>
#endif
#ifndef _IGESData_IGESEntity_HeaderFile
#include <IGESData_IGESEntity.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class gp_XY;
class gp_Pnt2d;
class gp_Pnt;
class gp_Dir;


//! defines IGESCircularArc, Type <100> Form <0> <br>
//!          in package IGESGeom <br>
//!          A circular arc is a connected portion of a parent circle <br>
//!          which consists of more than one point. The definition space <br>
//!          coordinate system is always chosen so that the circular arc <br>
//!          remains in a plane either coincident with or parallel to <br>
//!          the XT, YT plane. <br>
class IGESGeom_CircularArc : public IGESData_IGESEntity {

public:

  
  Standard_EXPORT   IGESGeom_CircularArc();
  //! This method is used to set the fields of the class <br>
//!           CircularArc <br>
//!       - aZT     : Shift above the Z plane <br>
//!       - aCenter : Center of the circle of which the arc forms a part <br>
//!       - aStart  : Starting point of the circular arc <br>
//!       - anEnd   : Ending point of the circular arc <br>
  Standard_EXPORT     void Init(const Standard_Real aZT,const gp_XY& aCenter,const gp_XY& aStart,const gp_XY& anEnd) ;
  //! returns the center of the circle of which arc forms a part <br>
  Standard_EXPORT     gp_Pnt2d Center() const;
  //! returns the center of the circle of which arc forms a part <br>
//! after applying Transf. Matrix <br>
  Standard_EXPORT     gp_Pnt TransformedCenter() const;
  //! returns the start point of the arc <br>
  Standard_EXPORT     gp_Pnt2d StartPoint() const;
  //! returns the start point of the arc after applying Transf. Matrix <br>
  Standard_EXPORT     gp_Pnt TransformedStartPoint() const;
  //! returns the parallel displacement of the plane containing the <br>
//! arc from the XT, YT plane <br>
  Standard_EXPORT     Standard_Real ZPlane() const;
  //! returns the end point of the arc <br>
  Standard_EXPORT     gp_Pnt2d EndPoint() const;
  //! returns the end point of the arc after applying Transf. Matrix <br>
  Standard_EXPORT     gp_Pnt TransformedEndPoint() const;
  //! returns the radius of the circle of which arc forms a part <br>
  Standard_EXPORT     Standard_Real Radius() const;
  //! returns the angle subtended by the arc at the center in radians <br>
  Standard_EXPORT     Standard_Real Angle() const;
  //! Z-Axis of circle (i.e. [0,0,1]) <br>
  Standard_EXPORT     gp_Dir Axis() const;
  //! Z-Axis after applying Trans. Matrix <br>
  Standard_EXPORT     gp_Dir TransformedAxis() const;
  //! True if StartPoint = EndPoint <br>
  Standard_EXPORT     Standard_Boolean IsClosed() const;




  DEFINE_STANDARD_RTTI(IGESGeom_CircularArc)

protected:




private: 


Standard_Real theZT;
gp_XY theCenter;
gp_XY theStart;
gp_XY theEnd;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
