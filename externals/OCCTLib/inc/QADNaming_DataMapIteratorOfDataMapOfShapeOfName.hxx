// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _QADNaming_DataMapIteratorOfDataMapOfShapeOfName_HeaderFile
#define _QADNaming_DataMapIteratorOfDataMapOfShapeOfName_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _TCollection_BasicMapIterator_HeaderFile
#include <TCollection_BasicMapIterator.hxx>
#endif
#ifndef _Handle_QADNaming_DataMapNodeOfDataMapOfShapeOfName_HeaderFile
#include <Handle_QADNaming_DataMapNodeOfDataMapOfShapeOfName.hxx>
#endif
class Standard_NoSuchObject;
class TopoDS_Shape;
class TCollection_AsciiString;
class TopTools_ShapeMapHasher;
class QADNaming_DataMapOfShapeOfName;
class QADNaming_DataMapNodeOfDataMapOfShapeOfName;



class QADNaming_DataMapIteratorOfDataMapOfShapeOfName  : public TCollection_BasicMapIterator {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   QADNaming_DataMapIteratorOfDataMapOfShapeOfName();
  
  Standard_EXPORT   QADNaming_DataMapIteratorOfDataMapOfShapeOfName(const QADNaming_DataMapOfShapeOfName& aMap);
  
  Standard_EXPORT     void Initialize(const QADNaming_DataMapOfShapeOfName& aMap) ;
  
  Standard_EXPORT    const TopoDS_Shape& Key() const;
  
  Standard_EXPORT    const TCollection_AsciiString& Value() const;





protected:





private:





};





// other Inline functions and methods (like "C++: function call" methods)


#endif
