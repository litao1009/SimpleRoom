// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TopOpeBRepBuild_Area1dBuilder_HeaderFile
#define _TopOpeBRepBuild_Area1dBuilder_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _TopOpeBRepBuild_AreaBuilder_HeaderFile
#include <TopOpeBRepBuild_AreaBuilder.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Handle_TopOpeBRepBuild_Loop_HeaderFile
#include <Handle_TopOpeBRepBuild_Loop.hxx>
#endif
#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
class TopOpeBRepBuild_PaveSet;
class TopOpeBRepBuild_PaveClassifier;
class TopOpeBRepBuild_LoopSet;
class TopOpeBRepBuild_LoopClassifier;
class TopOpeBRepBuild_Loop;
class TopOpeBRepBuild_ListOfLoop;
class TopOpeBRepBuild_ListIteratorOfListOfLoop;



class TopOpeBRepBuild_Area1dBuilder  : public TopOpeBRepBuild_AreaBuilder {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   TopOpeBRepBuild_Area1dBuilder();
  //! Creates a Area1dBuilder to find the areas of <br>
//! the shapes described by <LS> using the classifier <LC>. <br>
  Standard_EXPORT   TopOpeBRepBuild_Area1dBuilder(TopOpeBRepBuild_PaveSet& LS,TopOpeBRepBuild_PaveClassifier& LC,const Standard_Boolean ForceClass = Standard_False);
  //! Sets a Area1dBuilder to find the areas of <br>
//! the shapes described by <LS> using the classifier <LC>. <br>
  Standard_EXPORT   virtual  void InitAreaBuilder(TopOpeBRepBuild_LoopSet& LS,TopOpeBRepBuild_LoopClassifier& LC,const Standard_Boolean ForceClass = Standard_False) ;
  
  Standard_EXPORT   virtual  void ADD_Loop_TO_LISTOFLoop(const Handle(TopOpeBRepBuild_Loop)& L,TopOpeBRepBuild_ListOfLoop& LOL,const Standard_Address s = NULL) const;
  
  Standard_EXPORT   virtual  void REM_Loop_FROM_LISTOFLoop(TopOpeBRepBuild_ListIteratorOfListOfLoop& ITLOL,TopOpeBRepBuild_ListOfLoop& LOL,const Standard_Address s = NULL) const;
  
  Standard_EXPORT   virtual  void ADD_LISTOFLoop_TO_LISTOFLoop(TopOpeBRepBuild_ListOfLoop& LOL1,TopOpeBRepBuild_ListOfLoop& LOL2,const Standard_Address s = NULL,const Standard_Address s1 = NULL,const Standard_Address s2 = NULL) const;
  
  Standard_EXPORT   static  void DumpList(const TopOpeBRepBuild_ListOfLoop& L) ;





protected:





private:





};





// other Inline functions and methods (like "C++: function call" methods)


#endif
