// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _ShapeUpgrade_SplitSurface_HeaderFile
#define _ShapeUpgrade_SplitSurface_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_ShapeUpgrade_SplitSurface_HeaderFile
#include <Handle_ShapeUpgrade_SplitSurface.hxx>
#endif

#ifndef _Handle_TColStd_HSequenceOfReal_HeaderFile
#include <Handle_TColStd_HSequenceOfReal.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Handle_Geom_Surface_HeaderFile
#include <Handle_Geom_Surface.hxx>
#endif
#ifndef _Handle_ShapeExtend_CompositeSurface_HeaderFile
#include <Handle_ShapeExtend_CompositeSurface.hxx>
#endif
#ifndef _MMgt_TShared_HeaderFile
#include <MMgt_TShared.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _ShapeExtend_Status_HeaderFile
#include <ShapeExtend_Status.hxx>
#endif
class TColStd_HSequenceOfReal;
class Geom_Surface;
class ShapeExtend_CompositeSurface;


//! Splits a Surface with a criterion. <br>
class ShapeUpgrade_SplitSurface : public MMgt_TShared {

public:

  //! Empty constructor. <br>
  Standard_EXPORT   ShapeUpgrade_SplitSurface();
  //! Initializes with single supporting surface. <br>
  Standard_EXPORT     void Init(const Handle(Geom_Surface)& S) ;
  //! Initializes with single supporting surface with bounding parameters. <br>
  Standard_EXPORT     void Init(const Handle(Geom_Surface)& S,const Standard_Real UFirst,const Standard_Real ULast,const Standard_Real VFirst,const Standard_Real VLast) ;
  //! Sets U parameters where splitting has to be done <br>
  Standard_EXPORT     void SetUSplitValues(const Handle(TColStd_HSequenceOfReal)& UValues) ;
  //! Sets V parameters where splitting has to be done <br>
  Standard_EXPORT     void SetVSplitValues(const Handle(TColStd_HSequenceOfReal)& VValues) ;
  //! Performs splitting of the supporting surface. <br>
//!          If resulting surface is B-Spline and Segment is True, <br>
//!          the result is composed with segments of the surface bounded <br>
//!          by the U and V SplitValues (method Geom_BSplineSurface::Segment <br>
//!          is used). <br>
//!          If Segment is False, the result is composed with <br>
//!          Geom_RectangularTrimmedSurface all based on the same complete <br>
//!          surface. <br>
//!          Fields myNbResultingRow and myNbResultingCol must be set to <br>
//!          specify the size of resulting grid of surfaces. <br>
  Standard_EXPORT   virtual  void Build(const Standard_Boolean Segment) ;
  //! Calculates points for correction/splitting of the surface. <br>
  Standard_EXPORT   virtual  void Compute(const Standard_Boolean Segment = Standard_True) ;
  //! Performs correction/splitting of the surface. <br>
//!     First defines splitting values by method Compute(), then calls method Build(). <br>
  Standard_EXPORT     void Perform(const Standard_Boolean Segment = Standard_True) ;
  //! returns all the U splitting values including the <br>
//!  First and Last parameters of the input surface <br>
  Standard_EXPORT    const Handle_TColStd_HSequenceOfReal& USplitValues() const;
  //! returns all the splitting V values including the <br>
//!  First and Last parameters of the input surface <br>
  Standard_EXPORT    const Handle_TColStd_HSequenceOfReal& VSplitValues() const;
  //! Returns the status <br>
//! OK    - no splitting is needed <br>
//! DONE1 - splitting required and gives more than one patch <br>
//! DONE2 - splitting is required, but gives only single patch (initial) <br>
//! DONE3 - geometric form of the surface or parametrisation is modified <br>
  Standard_EXPORT     Standard_Boolean Status(const ShapeExtend_Status status) const;
  //! Returns obtained surfaces after splitting as CompositeSurface <br>
  Standard_EXPORT    const Handle_ShapeExtend_CompositeSurface& ResSurfaces() const;




  DEFINE_STANDARD_RTTI(ShapeUpgrade_SplitSurface)

protected:


Handle_TColStd_HSequenceOfReal myUSplitValues;
Handle_TColStd_HSequenceOfReal myVSplitValues;
Standard_Integer myNbResultingRow;
Standard_Integer myNbResultingCol;
Handle_Geom_Surface mySurface;
Standard_Integer myStatus;
Handle_ShapeExtend_CompositeSurface myResSurfaces;


private: 




};





// other Inline functions and methods (like "C++: function call" methods)


#endif
