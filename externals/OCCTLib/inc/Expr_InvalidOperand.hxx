// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Expr_InvalidOperand_HeaderFile
#define _Expr_InvalidOperand_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Standard_DefineException_HeaderFile
#include <Standard_DefineException.hxx>
#endif
#ifndef _Standard_SStream_HeaderFile
#include <Standard_SStream.hxx>
#endif

#ifndef _Expr_ExprFailure_HeaderFile
#include <Expr_ExprFailure.hxx>
#endif
#ifndef _Handle_Expr_InvalidOperand_HeaderFile
#include <Handle_Expr_InvalidOperand.hxx>
#endif

#if !defined No_Exception && !defined No_Expr_InvalidOperand
#define Expr_InvalidOperand_Raise_if(CONDITION,MESSAGE) \
  if (CONDITION) Expr_InvalidOperand::Raise(MESSAGE);
#else
#define Expr_InvalidOperand_Raise_if(CONDITION,MESSAGE)
#endif

DEFINE_STANDARD_EXCEPTION(Expr_InvalidOperand,Expr_ExprFailure)

#endif
