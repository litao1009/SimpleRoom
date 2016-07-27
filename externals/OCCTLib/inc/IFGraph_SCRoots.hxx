// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _IFGraph_SCRoots_HeaderFile
#define _IFGraph_SCRoots_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _IFGraph_StrongComponants_HeaderFile
#include <IFGraph_StrongComponants.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class Interface_Graph;
class IFGraph_StrongComponants;


//! determines strong componants in a graph which are Roots <br>
class IFGraph_SCRoots  : public IFGraph_StrongComponants {
public:

  DEFINE_STANDARD_ALLOC

  //! creates with a Graph, and will analyse : <br>
//!           whole True  : all the contents of the Model <br>
//!           whole False : sub-parts which will be given later <br>
  Standard_EXPORT   IFGraph_SCRoots(const Interface_Graph& agraph,const Standard_Boolean whole);
  //! creates from a StrongComponants which was already computed <br>
  Standard_EXPORT   IFGraph_SCRoots(IFGraph_StrongComponants& subparts);
  //! does the computation <br>
  Standard_EXPORT   virtual  void Evaluate() ;





protected:





private:





};





// other Inline functions and methods (like "C++: function call" methods)


#endif
