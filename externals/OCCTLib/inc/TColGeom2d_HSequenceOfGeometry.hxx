// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TColGeom2d_HSequenceOfGeometry_HeaderFile
#define _TColGeom2d_HSequenceOfGeometry_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_TColGeom2d_HSequenceOfGeometry_HeaderFile
#include <Handle_TColGeom2d_HSequenceOfGeometry.hxx>
#endif

#ifndef _TColGeom2d_SequenceOfGeometry_HeaderFile
#include <TColGeom2d_SequenceOfGeometry.hxx>
#endif
#ifndef _MMgt_TShared_HeaderFile
#include <MMgt_TShared.hxx>
#endif
#ifndef _Handle_Geom2d_Geometry_HeaderFile
#include <Handle_Geom2d_Geometry.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
class Standard_NoSuchObject;
class Standard_OutOfRange;
class Geom2d_Geometry;
class TColGeom2d_SequenceOfGeometry;



class TColGeom2d_HSequenceOfGeometry : public MMgt_TShared {

public:

  
      TColGeom2d_HSequenceOfGeometry();
  
        Standard_Boolean IsEmpty() const;
  
        Standard_Integer Length() const;
  
  Standard_EXPORT     void Clear() ;
  
  Standard_EXPORT     void Append(const Handle(Geom2d_Geometry)& anItem) ;
  
  Standard_EXPORT     void Append(const Handle(TColGeom2d_HSequenceOfGeometry)& aSequence) ;
  
  Standard_EXPORT     void Prepend(const Handle(Geom2d_Geometry)& anItem) ;
  
  Standard_EXPORT     void Prepend(const Handle(TColGeom2d_HSequenceOfGeometry)& aSequence) ;
  
  Standard_EXPORT     void Reverse() ;
  
  Standard_EXPORT     void InsertBefore(const Standard_Integer anIndex,const Handle(Geom2d_Geometry)& anItem) ;
  
  Standard_EXPORT     void InsertBefore(const Standard_Integer anIndex,const Handle(TColGeom2d_HSequenceOfGeometry)& aSequence) ;
  
  Standard_EXPORT     void InsertAfter(const Standard_Integer anIndex,const Handle(Geom2d_Geometry)& anItem) ;
  
  Standard_EXPORT     void InsertAfter(const Standard_Integer anIndex,const Handle(TColGeom2d_HSequenceOfGeometry)& aSequence) ;
  
  Standard_EXPORT     void Exchange(const Standard_Integer anIndex,const Standard_Integer anOtherIndex) ;
  
  Standard_EXPORT     Handle_TColGeom2d_HSequenceOfGeometry Split(const Standard_Integer anIndex) ;
  
  Standard_EXPORT     void SetValue(const Standard_Integer anIndex,const Handle(Geom2d_Geometry)& anItem) ;
  
  Standard_EXPORT    const Handle_Geom2d_Geometry& Value(const Standard_Integer anIndex) const;
  
  Standard_EXPORT     Handle_Geom2d_Geometry& ChangeValue(const Standard_Integer anIndex) ;
  
  Standard_EXPORT     void Remove(const Standard_Integer anIndex) ;
  
  Standard_EXPORT     void Remove(const Standard_Integer fromIndex,const Standard_Integer toIndex) ;
  
       const TColGeom2d_SequenceOfGeometry& Sequence() const;
  
        TColGeom2d_SequenceOfGeometry& ChangeSequence() ;
  
  Standard_EXPORT     Handle_TColGeom2d_HSequenceOfGeometry ShallowCopy() const;




  DEFINE_STANDARD_RTTI(TColGeom2d_HSequenceOfGeometry)

protected:




private: 


TColGeom2d_SequenceOfGeometry mySequence;


};

#define Item Handle_Geom2d_Geometry
#define Item_hxx <Geom2d_Geometry.hxx>
#define TheSequence TColGeom2d_SequenceOfGeometry
#define TheSequence_hxx <TColGeom2d_SequenceOfGeometry.hxx>
#define TCollection_HSequence TColGeom2d_HSequenceOfGeometry
#define TCollection_HSequence_hxx <TColGeom2d_HSequenceOfGeometry.hxx>
#define Handle_TCollection_HSequence Handle_TColGeom2d_HSequenceOfGeometry
#define TCollection_HSequence_Type_() TColGeom2d_HSequenceOfGeometry_Type_()

#include <TCollection_HSequence.lxx>

#undef Item
#undef Item_hxx
#undef TheSequence
#undef TheSequence_hxx
#undef TCollection_HSequence
#undef TCollection_HSequence_hxx
#undef Handle_TCollection_HSequence
#undef TCollection_HSequence_Type_


// other Inline functions and methods (like "C++: function call" methods)
inline Handle_TColGeom2d_HSequenceOfGeometry ShallowCopy(const Handle_TColGeom2d_HSequenceOfGeometry& me) {
 return me->ShallowCopy();
}



#endif
