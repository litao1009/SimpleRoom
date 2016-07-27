// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StepData_Array1OfField_HeaderFile
#define _StepData_Array1OfField_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class Standard_RangeError;
class Standard_DimensionMismatch;
class Standard_OutOfRange;
class Standard_OutOfMemory;
class StepData_Field;



class StepData_Array1OfField  {
public:

  DEFINE_STANDARD_ALLOC

  
      StepData_Array1OfField(const Standard_Integer Low,const Standard_Integer Up);
  
      StepData_Array1OfField(const StepData_Field& Item,const Standard_Integer Low,const Standard_Integer Up);
  
  Standard_EXPORT     void Init(const StepData_Field& V) ;
  
        void Destroy() ;
~StepData_Array1OfField()
{
  Destroy();
}
  
        Standard_Boolean IsAllocated() const;
  
  Standard_EXPORT    const StepData_Array1OfField& Assign(const StepData_Array1OfField& Other) ;
   const StepData_Array1OfField& operator =(const StepData_Array1OfField& Other) 
{
  return Assign(Other);
}
  
        Standard_Integer Length() const;
  
        Standard_Integer Lower() const;
  
        Standard_Integer Upper() const;
  
        void SetValue(const Standard_Integer Index,const StepData_Field& Value) ;
  
       const StepData_Field& Value(const Standard_Integer Index) const;
     const StepData_Field& operator ()(const Standard_Integer Index) const
{
  return Value(Index);
}
  
        StepData_Field& ChangeValue(const Standard_Integer Index) ;
      StepData_Field& operator ()(const Standard_Integer Index) 
{
  return ChangeValue(Index);
}





protected:





private:

  
  Standard_EXPORT   StepData_Array1OfField(const StepData_Array1OfField& AnArray);


Standard_Integer myLowerBound;
Standard_Integer myUpperBound;
Standard_Address myStart;
Standard_Boolean isAllocated;


};

#define Array1Item StepData_Field
#define Array1Item_hxx <StepData_Field.hxx>
#define TCollection_Array1 StepData_Array1OfField
#define TCollection_Array1_hxx <StepData_Array1OfField.hxx>

#include <TCollection_Array1.lxx>

#undef Array1Item
#undef Array1Item_hxx
#undef TCollection_Array1
#undef TCollection_Array1_hxx


// other Inline functions and methods (like "C++: function call" methods)


#endif
