// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Geom2dHatch_Hatcher_HeaderFile
#define _Geom2dHatch_Hatcher_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _Geom2dHatch_Intersector_HeaderFile
#include <Geom2dHatch_Intersector.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Geom2dHatch_ElementsOfHatcher_HeaderFile
#include <Geom2dHatch_ElementsOfHatcher.hxx>
#endif
#ifndef _Geom2dHatch_HatchingsOfHatcher_HeaderFile
#include <Geom2dHatch_HatchingsOfHatcher.hxx>
#endif
#ifndef _Handle_Geom2dHatch_DataMapNodeOfHatchingsOfHatcher_HeaderFile
#include <Handle_Geom2dHatch_DataMapNodeOfHatchingsOfHatcher.hxx>
#endif
#ifndef _TopAbs_Orientation_HeaderFile
#include <TopAbs_Orientation.hxx>
#endif
#ifndef _HatchGen_ErrorStatus_HeaderFile
#include <HatchGen_ErrorStatus.hxx>
#endif
class Standard_NoSuchObject;
class Standard_OutOfRange;
class StdFail_NotDone;
class Geom2dAdaptor_Curve;
class Geom2dHatch_Intersector;
class Geom2dHatch_HatchingOfHatcher;
class Geom2dHatch_HatchingsOfHatcher;
class Geom2dHatch_DataMapNodeOfHatchingsOfHatcher;
class Geom2dHatch_DataMapIteratorOfHatchingsOfHatcher;
class Geom2dHatch_ElementOfHatcher;
class Geom2dHatch_ElementsOfHatcher;
class Geom2dHatch_MapOfElementsOfElementsOfHatcher;
class Geom2dHatch_ClassifierOfHatcher;
class Geom2dHatch_FClass2dOfClassifierOfHatcher;
class HatchGen_PointOnHatching;
class HatchGen_Domain;



class Geom2dHatch_Hatcher  {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   Geom2dHatch_Hatcher(const Geom2dHatch_Intersector& Intersector,const Standard_Real Confusion2d,const Standard_Real Confusion3d,const Standard_Boolean KeepPnt = Standard_False,const Standard_Boolean KeepSeg = Standard_False);
  
  Standard_EXPORT     void Intersector(const Geom2dHatch_Intersector& Intersector) ;
  
       const Geom2dHatch_Intersector& Intersector() ;
  
        Geom2dHatch_Intersector& ChangeIntersector() ;
  
  Standard_EXPORT     void Confusion2d(const Standard_Real Confusion) ;
  
        Standard_Real Confusion2d() const;
  
  Standard_EXPORT     void Confusion3d(const Standard_Real Confusion) ;
  
        Standard_Real Confusion3d() const;
  
  Standard_EXPORT     void KeepPoints(const Standard_Boolean Keep) ;
  
        Standard_Boolean KeepPoints() const;
  
  Standard_EXPORT     void KeepSegments(const Standard_Boolean Keep) ;
  
        Standard_Boolean KeepSegments() const;
  
        void Clear() ;
  
       const Geom2dAdaptor_Curve& ElementCurve(const Standard_Integer IndE) const;
  
  Standard_EXPORT     Standard_Integer AddElement(const Geom2dAdaptor_Curve& Curve,const TopAbs_Orientation Orientation = TopAbs_FORWARD) ;
  
  Standard_EXPORT     void RemElement(const Standard_Integer IndE) ;
  
  Standard_EXPORT     void ClrElements() ;
  
       const Geom2dAdaptor_Curve& HatchingCurve(const Standard_Integer IndH) const;
  
  Standard_EXPORT     Standard_Integer AddHatching(const Geom2dAdaptor_Curve& Curve) ;
  
  Standard_EXPORT     void RemHatching(const Standard_Integer IndH) ;
  
  Standard_EXPORT     void ClrHatchings() ;
  
        Standard_Integer NbPoints(const Standard_Integer IndH) const;
  
       const HatchGen_PointOnHatching& Point(const Standard_Integer IndH,const Standard_Integer IndP) const;
  
  Standard_EXPORT     void Trim() ;
  
  Standard_EXPORT     Standard_Integer Trim(const Geom2dAdaptor_Curve& Curve) ;
  
  Standard_EXPORT     void Trim(const Standard_Integer IndH) ;
  
  Standard_EXPORT     void ComputeDomains() ;
  
  Standard_EXPORT     void ComputeDomains(const Standard_Integer IndH) ;
  
        Standard_Boolean TrimDone(const Standard_Integer IndH) const;
  
        Standard_Boolean TrimFailed(const Standard_Integer IndH) const;
  
        Standard_Boolean IsDone() const;
  
  Standard_EXPORT     Standard_Boolean IsDone(const Standard_Integer IndH) const;
  
        HatchGen_ErrorStatus Status(const Standard_Integer IndH) const;
  
        Standard_Integer NbDomains(const Standard_Integer IndH) const;
  
  Standard_EXPORT    const HatchGen_Domain& Domain(const Standard_Integer IndH,const Standard_Integer IDom) const;
  
  Standard_EXPORT     void Dump() const;





protected:

  
        Geom2dHatch_ElementOfHatcher& Element(const Standard_Integer IndE) ;
  
        Geom2dHatch_HatchingOfHatcher& Hatching(const Standard_Integer IndH) ;




private:

  
  Standard_EXPORT     Standard_Boolean Trim(const Standard_Integer IndH,const Standard_Integer IndE) ;
  
  Standard_EXPORT     Standard_Boolean GlobalTransition(HatchGen_PointOnHatching& Point) ;


Geom2dHatch_Intersector myIntersector;
Standard_Real myConfusion2d;
Standard_Real myConfusion3d;
Standard_Boolean myKeepPoints;
Standard_Boolean myKeepSegments;
Standard_Integer myNbElements;
Geom2dHatch_ElementsOfHatcher myElements;
Standard_Integer myNbHatchings;
Geom2dHatch_HatchingsOfHatcher myHatchings;


};

#define TheCurveE Geom2dAdaptor_Curve
#define TheCurveE_hxx <Geom2dAdaptor_Curve.hxx>
#define TheCurveH Geom2dAdaptor_Curve
#define TheCurveH_hxx <Geom2dAdaptor_Curve.hxx>
#define TheIntersector Geom2dHatch_Intersector
#define TheIntersector_hxx <Geom2dHatch_Intersector.hxx>
#define HatchGen_Hatching Geom2dHatch_HatchingOfHatcher
#define HatchGen_Hatching_hxx <Geom2dHatch_HatchingOfHatcher.hxx>
#define HatchGen_Hatchings Geom2dHatch_HatchingsOfHatcher
#define HatchGen_Hatchings_hxx <Geom2dHatch_HatchingsOfHatcher.hxx>
#define HatchGen_DataMapNodeOfHatchings Geom2dHatch_DataMapNodeOfHatchingsOfHatcher
#define HatchGen_DataMapNodeOfHatchings_hxx <Geom2dHatch_DataMapNodeOfHatchingsOfHatcher.hxx>
#define HatchGen_DataMapIteratorOfHatchings Geom2dHatch_DataMapIteratorOfHatchingsOfHatcher
#define HatchGen_DataMapIteratorOfHatchings_hxx <Geom2dHatch_DataMapIteratorOfHatchingsOfHatcher.hxx>
#define HatchGen_DataMapNodeOfHatchings Geom2dHatch_DataMapNodeOfHatchingsOfHatcher
#define HatchGen_DataMapNodeOfHatchings_hxx <Geom2dHatch_DataMapNodeOfHatchingsOfHatcher.hxx>
#define HatchGen_DataMapIteratorOfHatchings Geom2dHatch_DataMapIteratorOfHatchingsOfHatcher
#define HatchGen_DataMapIteratorOfHatchings_hxx <Geom2dHatch_DataMapIteratorOfHatchingsOfHatcher.hxx>
#define HatchGen_Element Geom2dHatch_ElementOfHatcher
#define HatchGen_Element_hxx <Geom2dHatch_ElementOfHatcher.hxx>
#define HatchGen_Elements Geom2dHatch_ElementsOfHatcher
#define HatchGen_Elements_hxx <Geom2dHatch_ElementsOfHatcher.hxx>
#define HatchGen_MapOfElementsOfElements Geom2dHatch_MapOfElementsOfElementsOfHatcher
#define HatchGen_MapOfElementsOfElements_hxx <Geom2dHatch_MapOfElementsOfElementsOfHatcher.hxx>
#define HatchGen_MapOfElementsOfElements Geom2dHatch_MapOfElementsOfElementsOfHatcher
#define HatchGen_MapOfElementsOfElements_hxx <Geom2dHatch_MapOfElementsOfElementsOfHatcher.hxx>
#define HatchGen_Classifier Geom2dHatch_ClassifierOfHatcher
#define HatchGen_Classifier_hxx <Geom2dHatch_ClassifierOfHatcher.hxx>
#define HatchGen_FClass2dOfClassifier Geom2dHatch_FClass2dOfClassifierOfHatcher
#define HatchGen_FClass2dOfClassifier_hxx <Geom2dHatch_FClass2dOfClassifierOfHatcher.hxx>
#define HatchGen_FClass2dOfClassifier Geom2dHatch_FClass2dOfClassifierOfHatcher
#define HatchGen_FClass2dOfClassifier_hxx <Geom2dHatch_FClass2dOfClassifierOfHatcher.hxx>
#define Handle_HatchGen_DataMapNodeOfHatchings Handle_Geom2dHatch_DataMapNodeOfHatchingsOfHatcher
#define HatchGen_DataMapNodeOfHatchings_Type_() Geom2dHatch_DataMapNodeOfHatchingsOfHatcher_Type_()
#define Handle_HatchGen_DataMapNodeOfHatchings Handle_Geom2dHatch_DataMapNodeOfHatchingsOfHatcher
#define HatchGen_DataMapNodeOfHatchings_Type_() Geom2dHatch_DataMapNodeOfHatchingsOfHatcher_Type_()
#define HatchGen_Hatcher Geom2dHatch_Hatcher
#define HatchGen_Hatcher_hxx <Geom2dHatch_Hatcher.hxx>

#include <HatchGen_Hatcher.lxx>

#undef TheCurveE
#undef TheCurveE_hxx
#undef TheCurveH
#undef TheCurveH_hxx
#undef TheIntersector
#undef TheIntersector_hxx
#undef HatchGen_Hatching
#undef HatchGen_Hatching_hxx
#undef HatchGen_Hatchings
#undef HatchGen_Hatchings_hxx
#undef HatchGen_DataMapNodeOfHatchings
#undef HatchGen_DataMapNodeOfHatchings_hxx
#undef HatchGen_DataMapIteratorOfHatchings
#undef HatchGen_DataMapIteratorOfHatchings_hxx
#undef HatchGen_DataMapNodeOfHatchings
#undef HatchGen_DataMapNodeOfHatchings_hxx
#undef HatchGen_DataMapIteratorOfHatchings
#undef HatchGen_DataMapIteratorOfHatchings_hxx
#undef HatchGen_Element
#undef HatchGen_Element_hxx
#undef HatchGen_Elements
#undef HatchGen_Elements_hxx
#undef HatchGen_MapOfElementsOfElements
#undef HatchGen_MapOfElementsOfElements_hxx
#undef HatchGen_MapOfElementsOfElements
#undef HatchGen_MapOfElementsOfElements_hxx
#undef HatchGen_Classifier
#undef HatchGen_Classifier_hxx
#undef HatchGen_FClass2dOfClassifier
#undef HatchGen_FClass2dOfClassifier_hxx
#undef HatchGen_FClass2dOfClassifier
#undef HatchGen_FClass2dOfClassifier_hxx
#undef Handle_HatchGen_DataMapNodeOfHatchings
#undef HatchGen_DataMapNodeOfHatchings_Type_
#undef Handle_HatchGen_DataMapNodeOfHatchings
#undef HatchGen_DataMapNodeOfHatchings_Type_
#undef HatchGen_Hatcher
#undef HatchGen_Hatcher_hxx


// other Inline functions and methods (like "C++: function call" methods)


#endif
