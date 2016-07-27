// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _PBRep_PolygonOnClosedTriangulation_HeaderFile
#define _PBRep_PolygonOnClosedTriangulation_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_PBRep_PolygonOnClosedTriangulation_HeaderFile
#include <Handle_PBRep_PolygonOnClosedTriangulation.hxx>
#endif

#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Handle_PPoly_PolygonOnTriangulation_HeaderFile
#include <Handle_PPoly_PolygonOnTriangulation.hxx>
#endif
#ifndef _PBRep_PolygonOnTriangulation_HeaderFile
#include <PBRep_PolygonOnTriangulation.hxx>
#endif
#ifndef _Handle_PPoly_Triangulation_HeaderFile
#include <Handle_PPoly_Triangulation.hxx>
#endif
class PPoly_PolygonOnTriangulation;
class PPoly_Triangulation;
class PTopLoc_Location;


class PBRep_PolygonOnClosedTriangulation : public PBRep_PolygonOnTriangulation {

public:

  
  Standard_EXPORT   PBRep_PolygonOnClosedTriangulation(const Handle(PPoly_PolygonOnTriangulation)& P1,const Handle(PPoly_PolygonOnTriangulation)& P2,const Handle(PPoly_Triangulation)& S,const PTopLoc_Location& L);
  //! Returns True. <br>
  Standard_EXPORT   virtual  Standard_Boolean IsPolygonOnClosedTriangulation() const;
  
  Standard_EXPORT     Handle_PPoly_PolygonOnTriangulation PolygonOnTriangulation2() const;

  PBRep_PolygonOnClosedTriangulation( )
{
  
}
PBRep_PolygonOnClosedTriangulation(const Storage_stCONSTclCOM& a) : PBRep_PolygonOnTriangulation(a)
{
  
}
    Handle(PPoly_PolygonOnTriangulation) _CSFDB_GetPBRep_PolygonOnClosedTriangulationmyPolygon2() const { return myPolygon2; }
    void _CSFDB_SetPBRep_PolygonOnClosedTriangulationmyPolygon2(const Handle(PPoly_PolygonOnTriangulation)& p) { myPolygon2 = p; }



  DEFINE_STANDARD_RTTI(PBRep_PolygonOnClosedTriangulation)

protected:




private: 


Handle_PPoly_PolygonOnTriangulation myPolygon2;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
