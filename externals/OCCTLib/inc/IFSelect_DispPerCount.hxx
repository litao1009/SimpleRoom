// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _IFSelect_DispPerCount_HeaderFile
#define _IFSelect_DispPerCount_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_IFSelect_DispPerCount_HeaderFile
#include <Handle_IFSelect_DispPerCount.hxx>
#endif

#ifndef _Handle_IFSelect_IntParam_HeaderFile
#include <Handle_IFSelect_IntParam.hxx>
#endif
#ifndef _IFSelect_Dispatch_HeaderFile
#include <IFSelect_Dispatch.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class IFSelect_IntParam;
class Interface_InterfaceError;
class TCollection_AsciiString;
class Interface_Graph;
class IFGraph_SubPartsIterator;


//! A DispPerCount gathers all the input Entities into one or <br>
//!           several Packets, each containing a defined count of Entity <br>
//!           This count is a Parameter of the DispPerCount, given as an <br>
//!           IntParam, thus allowing external control of its Value <br>
class IFSelect_DispPerCount : public IFSelect_Dispatch {

public:

  //! Creates a DispPerCount with no Count (default value 1) <br>
  Standard_EXPORT   IFSelect_DispPerCount();
  //! Returns the Count Parameter used for splitting <br>
  Standard_EXPORT     Handle_IFSelect_IntParam Count() const;
  //! Sets a new Parameter for Count <br>
  Standard_EXPORT     void SetCount(const Handle(IFSelect_IntParam)& count) ;
  //! Returns the effective value of the count parameter <br>
//!           (if Count Parameter not Set or value not positive, returns 1) <br>
  Standard_EXPORT     Standard_Integer CountValue() const;
  //! Returns as Label, "One File per <count> Input Entities" <br>
  Standard_EXPORT     TCollection_AsciiString Label() const;
  //! Returns True, maximum count is given as <nbent> <br>
  Standard_EXPORT   virtual  Standard_Boolean LimitedMax(const Standard_Integer nbent,Standard_Integer& max) const;
  //! Returns True (count is easy to know) and count is computed <br>
//!           from length of input list (RootResult from Final Selection) <br>
  Standard_EXPORT   virtual  Standard_Boolean PacketsCount(const Interface_Graph& G,Standard_Integer& count) const;
  //! Computes the list of produced Packets. It defines Packets in <br>
//!           order to have at most <Count> Entities per Packet, Entities <br>
//!           are given by RootResult from the Final Selection. <br>
  Standard_EXPORT     void Packets(const Interface_Graph& G,IFGraph_SubPartsIterator& packs) const;




  DEFINE_STANDARD_RTTI(IFSelect_DispPerCount)

protected:




private: 


Handle_IFSelect_IntParam thecount;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
