// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TopOpeBRepDS_Interference_HeaderFile
#define _TopOpeBRepDS_Interference_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_TopOpeBRepDS_Interference_HeaderFile
#include <Handle_TopOpeBRepDS_Interference.hxx>
#endif

#ifndef _TopOpeBRepDS_Transition_HeaderFile
#include <TopOpeBRepDS_Transition.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _TopOpeBRepDS_Kind_HeaderFile
#include <TopOpeBRepDS_Kind.hxx>
#endif
#ifndef _MMgt_TShared_HeaderFile
#include <MMgt_TShared.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Standard_OStream_HeaderFile
#include <Standard_OStream.hxx>
#endif
class TopOpeBRepDS_Transition;
class TCollection_AsciiString;


//! An interference    is   the   description  of  the <br>
//!          attachment of  a new  geometry on a  geometry. For <br>
//!          example an intersection point  on an Edge or on  a <br>
//!          Curve. <br>
//! <br>
//!          The Interference contains the following data : <br>
//! <br>
//!          - Transition :  How the interference  separates the <br>
//!          existing geometry in INSIDE and OUTSIDE. <br>
//! <br>
//!          - SupportType : Type of  the object supporting the <br>
//!          interference. (FACE, EDGE, VERTEX, SURFACE, CURVE). <br>
//! <br>
//!          - Support :  Index  in the data  structure  of the <br>
//!          object supporting the interference. <br>
//! <br>
//!          - GeometryType  :   Type  of the  geometry of  the <br>
//!          interference (SURFACE, CURVE, POINT). <br>
//! <br>
//!          - Geometry : Index  in the data structure  of the <br>
//!          geometry. <br>
//! <br>
class TopOpeBRepDS_Interference : public MMgt_TShared {

public:

  
  Standard_EXPORT   TopOpeBRepDS_Interference();
  
  Standard_EXPORT   TopOpeBRepDS_Interference(const TopOpeBRepDS_Transition& Transition,const TopOpeBRepDS_Kind SupportType,const Standard_Integer Support,const TopOpeBRepDS_Kind GeometryType,const Standard_Integer Geometry);
  
  Standard_EXPORT   TopOpeBRepDS_Interference(const Handle(TopOpeBRepDS_Interference)& I);
  
  Standard_EXPORT    const TopOpeBRepDS_Transition& Transition() const;
  
  Standard_EXPORT     TopOpeBRepDS_Transition& ChangeTransition() ;
  
  Standard_EXPORT     void Transition(const TopOpeBRepDS_Transition& T) ;
  //! return GeometryType + Geometry + SupportType + Support <br>
  Standard_EXPORT     void GKGSKS(TopOpeBRepDS_Kind& GK,Standard_Integer& G,TopOpeBRepDS_Kind& SK,Standard_Integer& S) const;
  
  Standard_EXPORT     TopOpeBRepDS_Kind SupportType() const;
  
  Standard_EXPORT     Standard_Integer Support() const;
  
  Standard_EXPORT     TopOpeBRepDS_Kind GeometryType() const;
  
  Standard_EXPORT     Standard_Integer Geometry() const;
  
  Standard_EXPORT     void SetGeometry(const Standard_Integer GI) ;
  
  Standard_EXPORT     void SupportType(const TopOpeBRepDS_Kind ST) ;
  
  Standard_EXPORT     void Support(const Standard_Integer S) ;
  
  Standard_EXPORT     void GeometryType(const TopOpeBRepDS_Kind GT) ;
  
  Standard_EXPORT     void Geometry(const Standard_Integer G) ;
  
  Standard_EXPORT     Standard_Boolean HasSameSupport(const Handle(TopOpeBRepDS_Interference)& Other) const;
  
  Standard_EXPORT     Standard_Boolean HasSameGeometry(const Handle(TopOpeBRepDS_Interference)& Other) const;
  
  Standard_EXPORT     Standard_OStream& DumpG(Standard_OStream& OS) const;
  
  Standard_EXPORT     Standard_OStream& DumpS(Standard_OStream& OS) const;
  
  Standard_EXPORT   virtual  Standard_OStream& Dump(Standard_OStream& OS) const;
  
  Standard_EXPORT     Standard_OStream& Dump(Standard_OStream& OS,const TCollection_AsciiString& s1,const TCollection_AsciiString& s2) const;




  DEFINE_STANDARD_RTTI(TopOpeBRepDS_Interference)

protected:




private: 


TopOpeBRepDS_Transition myTransition;
Standard_Integer mySupport;
Standard_Integer myGeometry;
TopOpeBRepDS_Kind mySupportType;
TopOpeBRepDS_Kind myGeometryType;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
