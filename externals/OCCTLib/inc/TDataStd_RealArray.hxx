// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TDataStd_RealArray_HeaderFile
#define _TDataStd_RealArray_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_TDataStd_RealArray_HeaderFile
#include <Handle_TDataStd_RealArray.hxx>
#endif

#ifndef _Handle_TColStd_HArray1OfReal_HeaderFile
#include <Handle_TColStd_HArray1OfReal.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _TDF_Attribute_HeaderFile
#include <TDF_Attribute.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Handle_TDF_Attribute_HeaderFile
#include <Handle_TDF_Attribute.hxx>
#endif
#ifndef _Handle_TDF_RelocationTable_HeaderFile
#include <Handle_TDF_RelocationTable.hxx>
#endif
#ifndef _Standard_OStream_HeaderFile
#include <Standard_OStream.hxx>
#endif
#ifndef _Handle_TDF_DeltaOnModification_HeaderFile
#include <Handle_TDF_DeltaOnModification.hxx>
#endif
class TColStd_HArray1OfReal;
class TDataStd_DeltaOnModificationOfRealArray;
class Standard_GUID;
class TDF_Label;
class TDF_Attribute;
class TDF_RelocationTable;
class TDF_DeltaOnModification;


//! A framework for an attribute composed of a real number array. <br>
class TDataStd_RealArray : public TDF_Attribute {

public:

  //! class methods <br>
//!          ============= <br>//! Returns the GUID for arrays of reals. <br>
  Standard_EXPORT   static const Standard_GUID& GetID() ;
  //! Finds or creates on the <label> a real array attribute <br>
//! with the specified <lower> and <upper> boundaries. <br>
//! If attribute is already set, all input parameters are refused and the found <br>
//! attribute is returned. <br>
  Standard_EXPORT   static  Handle_TDataStd_RealArray Set(const TDF_Label& label,const Standard_Integer lower,const Standard_Integer upper,const Standard_Boolean isDelta = Standard_False) ;
  //! Initialize the inner array with bounds from <lower> to <upper> <br>
  Standard_EXPORT     void Init(const Standard_Integer lower,const Standard_Integer upper) ;
  //! Sets  the   <Index>th  element  of   the  array to <Value> <br>
//! OutOfRange exception is raised if <Index> doesn't respect Lower and Upper bounds of the internal  array. <br>
  Standard_EXPORT     void SetValue(const Standard_Integer Index,const Standard_Real Value) ;
  //! Return the value of  the  <Index>th element of the array <br>
//! <br>
  Standard_EXPORT     Standard_Real Value(const Standard_Integer Index) const;
    Standard_Real operator ()(const Standard_Integer Index) const
{
  return Value(Index);
}
  //!  Returns the lower boundary of the array. <br>
  Standard_EXPORT     Standard_Integer Lower() const;
  //! Returns the upper boundary of the array. <br>
  Standard_EXPORT     Standard_Integer Upper() const;
  //! Returns the number of elements of the array of reals <br>
//!    in terms of the number of elements it contains. <br>
  Standard_EXPORT     Standard_Integer Length() const;
  //! Sets the inner array <myValue> of the RealArray attribute <br>
//! to <newArray>. If value of <newArray> differs from <myValue>, <br>
//! Backup performed and myValue refers to new instance of HArray1OfReal <br>
//! that holds <newArray> values <br>
//! If <isCheckItems> equal True each item of <newArray> will be checked with each <br>
//! item of <myValue> for coincidence (to avoid backup). <br>
  Standard_EXPORT     void ChangeArray(const Handle(TColStd_HArray1OfReal)& newArray,const Standard_Boolean isCheckItems = Standard_True) ;
  //! Returns the handle of this array of reals. <br>
       const Handle_TColStd_HArray1OfReal Array() const;
  
        Standard_Boolean GetDelta() const;
  //! for  internal  use  only! <br>
        void SetDelta(const Standard_Boolean isDelta) ;
  
  Standard_EXPORT   TDataStd_RealArray();
  
  Standard_EXPORT    const Standard_GUID& ID() const;
  
  Standard_EXPORT     void Restore(const Handle(TDF_Attribute)& With) ;
  
  Standard_EXPORT     Handle_TDF_Attribute NewEmpty() const;
  //! Note. Uses inside ChangeArray() method <br>
  Standard_EXPORT     void Paste(const Handle(TDF_Attribute)& Into,const Handle(TDF_RelocationTable)& RT) const;
  
  Standard_EXPORT   virtual  Standard_OStream& Dump(Standard_OStream& anOS) const;
  //! Makes a DeltaOnModification between <me> and <br>
//!         <anOldAttribute>. <br>
  Standard_EXPORT   virtual  Handle_TDF_DeltaOnModification DeltaOnModification(const Handle(TDF_Attribute)& anOldAttribute) const;


friend class TDataStd_DeltaOnModificationOfRealArray;


  DEFINE_STANDARD_RTTI(TDataStd_RealArray)

protected:




private: 

  
        void RemoveArray() ;

Handle_TColStd_HArray1OfReal myValue;
Standard_Boolean myIsDelta;


};


#include <TDataStd_RealArray.lxx>



// other Inline functions and methods (like "C++: function call" methods)


#endif