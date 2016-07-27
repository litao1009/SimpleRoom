// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TColGeom2d_Array1OfBoundedCurve_HeaderFile
#define _TColGeom2d_Array1OfBoundedCurve_HeaderFile

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
#ifndef _Handle_Geom2d_BoundedCurve_HeaderFile
#include <Handle_Geom2d_BoundedCurve.hxx>
#endif
class Standard_RangeError;
class Standard_DimensionMismatch;
class Standard_OutOfRange;
class Standard_OutOfMemory;
class Geom2d_BoundedCurve;



class TColGeom2d_Array1OfBoundedCurve  {
public:

  DEFINE_STANDARD_ALLOC

  
      TColGeom2d_Array1OfBoundedCurve(const Standard_Integer Low,const Standard_Integer Up);
  
      TColGeom2d_Array1OfBoundedCurve(const Handle(Geom2d_BoundedCurve)& Item,const Standard_Integer Low,const Standard_Integer Up);
  
  Standard_EXPORT     void Init(const Handle(Geom2d_BoundedCurve)& V) ;
  
        void Destroy() ;
~TColGeom2d_Array1OfBoundedCurve()
{
  Destroy();
}
  
        Standard_Boolean IsAllocated() const;
  
  Standard_EXPORT    const TColGeom2d_Array1OfBoundedCurve& Assign(const TColGeom2d_Array1OfBoundedCurve& Other) ;
   const TColGeom2d_Array1OfBoundedCurve& operator =(const TColGeom2d_Array1OfBoundedCurve& Other) 
{
  return Assign(Other);
}
  
        Standard_Integer Length() const;
  
        Standard_Integer Lower() const;
  
        Standard_Integer Upper() const;
  
        void SetValue(const Standard_Integer Index,const Handle(Geom2d_BoundedCurve)& Value) ;
  
       const Handle_Geom2d_BoundedCurve& Value(const Standard_Integer Index) const;
     const Handle_Geom2d_BoundedCurve& operator ()(const Standard_Integer Index) const
{
  return Value(Index);
}
  
        Handle_Geom2d_BoundedCurve& ChangeValue(const Standard_Integer Index) ;
      Handle_Geom2d_BoundedCurve& operator ()(const Standard_Integer Index) 
{
  return ChangeValue(Index);
}





protected:





private:

  
  Standard_EXPORT   TColGeom2d_Array1OfBoundedCurve(const TColGeom2d_Array1OfBoundedCurve& AnArray);


Standard_Integer myLowerBound;
Standard_Integer myUpperBound;
Standard_Address myStart;
Standard_Boolean isAllocated;


};

#define Array1Item Handle_Geom2d_BoundedCurve
#define Array1Item_hxx <Geom2d_BoundedCurve.hxx>
#define TCollection_Array1 TColGeom2d_Array1OfBoundedCurve
#define TCollection_Array1_hxx <TColGeom2d_Array1OfBoundedCurve.hxx>

#include <TCollection_Array1.lxx>

#undef Array1Item
#undef Array1Item_hxx
#undef TCollection_Array1
#undef TCollection_Array1_hxx


// other Inline functions and methods (like "C++: function call" methods)


#endif
