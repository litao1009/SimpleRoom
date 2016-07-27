// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Expr_GreaterThan_HeaderFile
#define _Expr_GreaterThan_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_Expr_GreaterThan_HeaderFile
#include <Handle_Expr_GreaterThan.hxx>
#endif

#ifndef _Expr_SingleRelation_HeaderFile
#include <Expr_SingleRelation.hxx>
#endif
#ifndef _Handle_Expr_GeneralExpression_HeaderFile
#include <Handle_Expr_GeneralExpression.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Handle_Expr_GeneralRelation_HeaderFile
#include <Handle_Expr_GeneralRelation.hxx>
#endif
class Standard_NumericError;
class Expr_GeneralExpression;
class Expr_GeneralRelation;
class TCollection_AsciiString;



class Expr_GreaterThan : public Expr_SingleRelation {

public:

  //! Creates the relation <exp1> > <exp2>. <br>
  Standard_EXPORT   Expr_GreaterThan(const Handle(Expr_GeneralExpression)& exp1,const Handle(Expr_GeneralExpression)& exp2);
  
  Standard_EXPORT     Standard_Boolean IsSatisfied() const;
  //! Returns a GeneralRelation after replacement of <br>
//!          NamedUnknowns by an associated expression, and after <br>
//!          values computation. <br>
  Standard_EXPORT     Handle_Expr_GeneralRelation Simplified() const;
  //! Replaces NamedUnknowns by associated expressions, <br>
//!          and computes values in <me>. <br>
  Standard_EXPORT     void Simplify() ;
  //! Returns a copy of <me> having the same unknowns and functions. <br>
  Standard_EXPORT     Handle_Expr_GeneralRelation Copy() const;
  //! returns a string representing <me> in a readable way. <br>
  Standard_EXPORT     TCollection_AsciiString String() const;




  DEFINE_STANDARD_RTTI(Expr_GreaterThan)

protected:




private: 




};





// other Inline functions and methods (like "C++: function call" methods)


#endif
