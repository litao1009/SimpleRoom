// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _HLRAlgo_Coincidence_HeaderFile
#define _HLRAlgo_Coincidence_HeaderFile

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
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _TopAbs_State_HeaderFile
#include <TopAbs_State.hxx>
#endif


//! The Coincidence class is used in an Inteference to <br>
//!          store informations on the "hiding" edge. <br>
//! <br>
//!          2D  Data : The  tangent  and the  curvature of the <br>
//!          projection of the edge  at the intersection point. <br>
//!          This is necesserary  when the intersection  is  at <br>
//!          the extremity of the edge. <br>
//! <br>
//!          3D   Data  :  The   state of  the   edge  near the <br>
//!          intersection   with  the face (before  and after). <br>
//!          This is necessary  when the  intersection is  "ON" <br>
//!          the face. <br>
class HLRAlgo_Coincidence  {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   HLRAlgo_Coincidence();
  
        void Set2D(const Standard_Integer FE,const Standard_Real Param) ;
  
        void SetState3D(const TopAbs_State stbef,const TopAbs_State staft) ;
  
        void Value2D(Standard_Integer& FE,Standard_Real& Param) const;
  
        void State3D(TopAbs_State& stbef,TopAbs_State& staft) const;





protected:





private:



Standard_Integer myFE;
Standard_Real myParam;
TopAbs_State myStBef;
TopAbs_State myStAft;


};


#include <HLRAlgo_Coincidence.lxx>



// other Inline functions and methods (like "C++: function call" methods)


#endif
