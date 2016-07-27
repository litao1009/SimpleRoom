// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TopOpeBRepDS_SurfaceIterator_HeaderFile
#define _TopOpeBRepDS_SurfaceIterator_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _TopOpeBRepDS_InterferenceIterator_HeaderFile
#include <TopOpeBRepDS_InterferenceIterator.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _TopAbs_Orientation_HeaderFile
#include <TopAbs_Orientation.hxx>
#endif
#ifndef _TopAbs_State_HeaderFile
#include <TopAbs_State.hxx>
#endif
class TopOpeBRepDS_ListOfInterference;



class TopOpeBRepDS_SurfaceIterator  : public TopOpeBRepDS_InterferenceIterator {
public:

  DEFINE_STANDARD_ALLOC

  //! Creates an  iterator on the  Surfaces on solid <br>
//!          described by the interferences in <L>. <br>
  Standard_EXPORT   TopOpeBRepDS_SurfaceIterator(const TopOpeBRepDS_ListOfInterference& L);
  //! Index of the surface in the data structure. <br>
  Standard_EXPORT     Standard_Integer Current() const;
  
  Standard_EXPORT     TopAbs_Orientation Orientation(const TopAbs_State S) const;





protected:





private:





};





// other Inline functions and methods (like "C++: function call" methods)


#endif
