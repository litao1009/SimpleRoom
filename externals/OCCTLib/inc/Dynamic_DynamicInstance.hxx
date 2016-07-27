// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Dynamic_DynamicInstance_HeaderFile
#define _Dynamic_DynamicInstance_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_Dynamic_DynamicInstance_HeaderFile
#include <Handle_Dynamic_DynamicInstance.hxx>
#endif

#ifndef _Handle_Dynamic_DynamicClass_HeaderFile
#include <Handle_Dynamic_DynamicClass.hxx>
#endif
#ifndef _Handle_Dynamic_ParameterNode_HeaderFile
#include <Handle_Dynamic_ParameterNode.hxx>
#endif
#ifndef _MMgt_TShared_HeaderFile
#include <MMgt_TShared.hxx>
#endif
#ifndef _Handle_Dynamic_Parameter_HeaderFile
#include <Handle_Dynamic_Parameter.hxx>
#endif
#ifndef _Standard_CString_HeaderFile
#include <Standard_CString.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
class Dynamic_DynamicClass;
class Dynamic_ParameterNode;
class Dynamic_Parameter;


//! A dynamic  instance is a reference  to the dynamic <br>
//!          class and a  sequence of  parameters which  is the <br>
//!          complete listing of all the parameters  of all the <br>
//!          inherited classes. <br>
class Dynamic_DynamicInstance : public MMgt_TShared {

public:

  //! Creates an empty instance of this class. <br>
  Standard_EXPORT   Dynamic_DynamicInstance();
  //! Adds  <aparameter> to  the   sequence of parameters of <br>
//!          <me>. <br>
  Standard_EXPORT     void Parameter(const Handle(Dynamic_Parameter)& aparameter) ;
  //! Adds all the parameters  of <me>,  to the sequence  of <br>
//!          parameters of <aninstance>. <br>
  Standard_EXPORT     void Parameter(const Handle(Dynamic_DynamicInstance)& aninstance) const;
  //! Puts the integer value <avalue> into the parameter <br>
//!          object identified by the string <aparameter>. <br>
  Standard_EXPORT     void Parameter(const Standard_CString aparameter,const Standard_Integer avalue) const;
  //! Puts the real value <avalue> into the parameter <br>
//!          object identified by the string <aparameter>. <br>
  Standard_EXPORT     void Parameter(const Standard_CString aparameter,const Standard_Real avalue) const;
  //! Puts the string <avalue> into the parameter <br>
//!          object identified by the string <aparameter>. <br>
  Standard_EXPORT     void Parameter(const Standard_CString aparameter,const Standard_CString avalue) const;
  //! Puts the dynamic instance <avalue> into the parameter <br>
//!          object identified by the string <aparameter>. <br>
  Standard_EXPORT     void Parameter(const Standard_CString aparameter,const Handle(Dynamic_DynamicInstance)& avalue) const;
  //! Searches and returns the parameter object identified <br>
//!          by the string <aparameter>. <br>
  Standard_EXPORT     Handle_Dynamic_Parameter Parameter(const Standard_CString aparameter) const;
  //! Sets the reference of the class. <br>
  Standard_EXPORT     void Class(const Handle(Dynamic_DynamicClass)& aclass) ;
  //! Calls the method identified by the string <amethod>. <br>
  Standard_EXPORT     void Execute(const Standard_CString amethod) const;




  DEFINE_STANDARD_RTTI(Dynamic_DynamicInstance)

protected:




private: 


Handle_Dynamic_DynamicClass thedynamicclass;
Handle_Dynamic_ParameterNode thefirstparameternode;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
