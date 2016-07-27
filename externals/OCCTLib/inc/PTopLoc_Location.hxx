// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _PTopLoc_Location_HeaderFile
#define _PTopLoc_Location_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _Handle_PTopLoc_ItemLocation_HeaderFile
#include <Handle_PTopLoc_ItemLocation.hxx>
#endif
#ifndef _Standard_Storable_HeaderFile
#include <Standard_Storable.hxx>
#endif
#ifndef _Handle_PTopLoc_Datum3D_HeaderFile
#include <Handle_PTopLoc_Datum3D.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Standard_PrimitiveTypes_HeaderFile
#include <Standard_PrimitiveTypes.hxx>
#endif
class PTopLoc_ItemLocation;
class Standard_NoSuchObject;
class PTopLoc_Datum3D;


Standard_EXPORT const Handle(Standard_Type)& STANDARD_TYPE(PTopLoc_Location);

//! A Storable  composed local coordinate system. Made <br>
//!          with local   coordinate systems raised   to  power <br>
//!          elevation. <br>
//! <br>
//!          A Location is either : <br>
//! <br>
//!          * The Identity. <br>
//! <br>
//!          * The product  of a Datum3D raised  to a power and <br>
//!          an other Location called the next Location. <br>
class PTopLoc_Location  {

public:

  DEFINE_STANDARD_ALLOC

  //! Creates an Identity Location. <br>
  Standard_EXPORT   PTopLoc_Location();
  //! Creates a location being the product. <br>
//!          N * D ^ P <br>
  Standard_EXPORT   PTopLoc_Location(const Handle(PTopLoc_Datum3D)& D,const Standard_Integer P,const PTopLoc_Location& N);
  //! True when the location is an identity. <br>
  Standard_EXPORT     Standard_Boolean IsIdentity() const;
  //! Returns the first Datum. An error is raised if the <br>
//!          location is an identity. <br>
  Standard_EXPORT     Handle_PTopLoc_Datum3D Datum3D() const;
  //! Returns the power elevation of the first datum. An <br>
//!          error is raised if the location is an identity. <br>
  Standard_EXPORT     Standard_Integer Power() const;
  //! Returns next Location. An error is raised if the <br>
//!          location is an identity. <br>
  Standard_EXPORT     PTopLoc_Location Next() const;
    Handle(PTopLoc_ItemLocation) _CSFDB_GetPTopLoc_LocationmyData() const { return myData; }
    void _CSFDB_SetPTopLoc_LocationmyData(const Handle(PTopLoc_ItemLocation)& p) { myData = p; }



protected:




private: 


Handle_PTopLoc_ItemLocation myData;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
