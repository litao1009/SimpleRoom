// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Plate_SequenceOfPinpointConstraint_HeaderFile
#define _Plate_SequenceOfPinpointConstraint_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _TCollection_BaseSequence_HeaderFile
#include <TCollection_BaseSequence.hxx>
#endif
#ifndef _Handle_Plate_SequenceNodeOfSequenceOfPinpointConstraint_HeaderFile
#include <Handle_Plate_SequenceNodeOfSequenceOfPinpointConstraint.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
class Standard_NoSuchObject;
class Standard_OutOfRange;
class Plate_PinpointConstraint;
class Plate_SequenceNodeOfSequenceOfPinpointConstraint;



class Plate_SequenceOfPinpointConstraint  : public TCollection_BaseSequence {
public:

  DEFINE_STANDARD_ALLOC

  
      Plate_SequenceOfPinpointConstraint();
  
  Standard_EXPORT     void Clear() ;
~Plate_SequenceOfPinpointConstraint()
{
  Clear();
}
  
  Standard_EXPORT    const Plate_SequenceOfPinpointConstraint& Assign(const Plate_SequenceOfPinpointConstraint& Other) ;
   const Plate_SequenceOfPinpointConstraint& operator =(const Plate_SequenceOfPinpointConstraint& Other) 
{
  return Assign(Other);
}
  
  Standard_EXPORT     void Append(const Plate_PinpointConstraint& T) ;
  
        void Append(Plate_SequenceOfPinpointConstraint& S) ;
  
  Standard_EXPORT     void Prepend(const Plate_PinpointConstraint& T) ;
  
        void Prepend(Plate_SequenceOfPinpointConstraint& S) ;
  
        void InsertBefore(const Standard_Integer Index,const Plate_PinpointConstraint& T) ;
  
        void InsertBefore(const Standard_Integer Index,Plate_SequenceOfPinpointConstraint& S) ;
  
  Standard_EXPORT     void InsertAfter(const Standard_Integer Index,const Plate_PinpointConstraint& T) ;
  
        void InsertAfter(const Standard_Integer Index,Plate_SequenceOfPinpointConstraint& S) ;
  
  Standard_EXPORT    const Plate_PinpointConstraint& First() const;
  
  Standard_EXPORT    const Plate_PinpointConstraint& Last() const;
  
        void Split(const Standard_Integer Index,Plate_SequenceOfPinpointConstraint& Sub) ;
  
  Standard_EXPORT    const Plate_PinpointConstraint& Value(const Standard_Integer Index) const;
   const Plate_PinpointConstraint& operator()(const Standard_Integer Index) const
{
  return Value(Index);
}
  
  Standard_EXPORT     void SetValue(const Standard_Integer Index,const Plate_PinpointConstraint& I) ;
  
  Standard_EXPORT     Plate_PinpointConstraint& ChangeValue(const Standard_Integer Index) ;
    Plate_PinpointConstraint& operator()(const Standard_Integer Index) 
{
  return ChangeValue(Index);
}
  
  Standard_EXPORT     void Remove(const Standard_Integer Index) ;
  
  Standard_EXPORT     void Remove(const Standard_Integer FromIndex,const Standard_Integer ToIndex) ;





protected:





private:

  
  Standard_EXPORT   Plate_SequenceOfPinpointConstraint(const Plate_SequenceOfPinpointConstraint& Other);




};

#define SeqItem Plate_PinpointConstraint
#define SeqItem_hxx <Plate_PinpointConstraint.hxx>
#define TCollection_SequenceNode Plate_SequenceNodeOfSequenceOfPinpointConstraint
#define TCollection_SequenceNode_hxx <Plate_SequenceNodeOfSequenceOfPinpointConstraint.hxx>
#define Handle_TCollection_SequenceNode Handle_Plate_SequenceNodeOfSequenceOfPinpointConstraint
#define TCollection_SequenceNode_Type_() Plate_SequenceNodeOfSequenceOfPinpointConstraint_Type_()
#define TCollection_Sequence Plate_SequenceOfPinpointConstraint
#define TCollection_Sequence_hxx <Plate_SequenceOfPinpointConstraint.hxx>

#include <TCollection_Sequence.lxx>

#undef SeqItem
#undef SeqItem_hxx
#undef TCollection_SequenceNode
#undef TCollection_SequenceNode_hxx
#undef Handle_TCollection_SequenceNode
#undef TCollection_SequenceNode_Type_
#undef TCollection_Sequence
#undef TCollection_Sequence_hxx


// other Inline functions and methods (like "C++: function call" methods)


#endif
