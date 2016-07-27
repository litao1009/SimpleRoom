// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _IGESDraw_CircArraySubfigure_HeaderFile
#define _IGESDraw_CircArraySubfigure_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_IGESDraw_CircArraySubfigure_HeaderFile
#include <Handle_IGESDraw_CircArraySubfigure.hxx>
#endif

#ifndef _Handle_IGESData_IGESEntity_HeaderFile
#include <Handle_IGESData_IGESEntity.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _gp_XYZ_HeaderFile
#include <gp_XYZ.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Handle_TColStd_HArray1OfInteger_HeaderFile
#include <Handle_TColStd_HArray1OfInteger.hxx>
#endif
#ifndef _IGESData_IGESEntity_HeaderFile
#include <IGESData_IGESEntity.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class IGESData_IGESEntity;
class TColStd_HArray1OfInteger;
class Standard_OutOfRange;
class gp_XYZ;
class gp_Pnt;


//! Defines IGES Circular Array Subfigure Instance Entity, <br>
//!            Type <414> Form Number <0> in package IGESDraw <br>
//! <br>
//!            Used to produce copies of object called the base entity, <br>
//!            arranging them around the edge of an imaginary circle <br>
//!            whose center and radius are specified <br>
class IGESDraw_CircArraySubfigure : public IGESData_IGESEntity {

public:

  
  Standard_EXPORT   IGESDraw_CircArraySubfigure();
  //! This method is used to set the fields of the class <br>
//!           CircArraySubfigure <br>
//!       - aBase     : Base entity <br>
//!       - aNumLocs  : Total number of possible instance locations <br>
//!       - aCenter   : Coordinates of Center of imaginary circle <br>
//!       - aRadius   : Radius of imaginary circle <br>
//!       - aStAngle  : Start angle in radians <br>
//!       - aDelAngle : Delta angle in radians <br>
//!       - aFlag     : DO-DON'T flag to control which portion to <br>
//!                     display <br>
//!       - allNumPos : All position to be or not to be processed <br>
  Standard_EXPORT     void Init(const Handle(IGESData_IGESEntity)& aBase,const Standard_Integer aNumLocs,const gp_XYZ& aCenter,const Standard_Real aRadius,const Standard_Real aStAngle,const Standard_Real aDelAngle,const Standard_Integer aFlag,const Handle(TColStd_HArray1OfInteger)& allNumPos) ;
  //! returns the base entity, copies of which are produced <br>
  Standard_EXPORT     Handle_IGESData_IGESEntity BaseEntity() const;
  //! returns total number of possible instance locations <br>
  Standard_EXPORT     Standard_Integer NbLocations() const;
  //! returns the center of the imaginary circle <br>
  Standard_EXPORT     gp_Pnt CenterPoint() const;
  //! returns the Transformed center of the imaginary circle <br>
  Standard_EXPORT     gp_Pnt TransformedCenterPoint() const;
  //! returns the radius of the imaginary circle <br>
  Standard_EXPORT     Standard_Real CircleRadius() const;
  //! returns the start angle in radians <br>
  Standard_EXPORT     Standard_Real StartAngle() const;
  //! returns the delta angle in radians <br>
  Standard_EXPORT     Standard_Real DeltaAngle() const;
  //! returns 0 if all elements to be displayed <br>
  Standard_EXPORT     Standard_Integer ListCount() const;
  //! returns True if (ListCount = 0) all elements are to be displayed <br>
  Standard_EXPORT     Standard_Boolean DisplayFlag() const;
  //! returns 0 if half or fewer of the elements of  the array are defined <br>
//!         1 if half or more of the elements are defined <br>
  Standard_EXPORT     Standard_Boolean DoDontFlag() const;
  //! returns whether Index is to be processed (DO) <br>
//! or not to be processed(DON'T) <br>
//! if (ListCount = 0) return theDoDontFlag <br>
//! raises exception if Index <= 0 or Index > ListCount(). <br>
  Standard_EXPORT     Standard_Boolean PositionNum(const Standard_Integer Index) const;
  //! returns the Index'th value position <br>
//! raises exception if Index <= 0 or Index > ListCount(). <br>
  Standard_EXPORT     Standard_Integer ListPosition(const Standard_Integer Index) const;




  DEFINE_STANDARD_RTTI(IGESDraw_CircArraySubfigure)

protected:




private: 


Handle_IGESData_IGESEntity theBaseEntity;
Standard_Integer theNbLocations;
gp_XYZ theCenter;
Standard_Real theRadius;
Standard_Real theStartAngle;
Standard_Real theDeltaAngle;
Standard_Integer theDoDontFlag;
Handle_TColStd_HArray1OfInteger thePositions;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
