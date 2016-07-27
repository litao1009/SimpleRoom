// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _FEmTool_SparseMatrix_HeaderFile
#define _FEmTool_SparseMatrix_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_FEmTool_SparseMatrix_HeaderFile
#include <Handle_FEmTool_SparseMatrix.hxx>
#endif

#ifndef _MMgt_TShared_HeaderFile
#include <MMgt_TShared.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class StdFail_NotDone;
class math_Vector;


//! Sparse Matrix definition <br>
class FEmTool_SparseMatrix : public MMgt_TShared {

public:

  
  Standard_EXPORT   virtual  void Init(const Standard_Real Value)  = 0;
  
  Standard_EXPORT   virtual  Standard_Real& ChangeValue(const Standard_Integer I,const Standard_Integer J)  = 0;
  //! To make a Factorization of <me> <br>
  Standard_EXPORT   virtual  Standard_Boolean Decompose()  = 0;
  //! Direct Solve of AX = B <br>
  Standard_EXPORT   virtual  void Solve(const math_Vector& B,math_Vector& X) const = 0;
  //! Make Preparation to iterative solve <br>
  Standard_EXPORT   virtual  Standard_Boolean Prepare()  = 0;
  //! Iterative solve  of AX = B <br>
  Standard_EXPORT   virtual  void Solve(const math_Vector& B,const math_Vector& Init,math_Vector& X,math_Vector& Residual,const Standard_Real Tolerance = 1.0e-8,const Standard_Integer NbIterations = 50) const = 0;
  //! returns the product of a SparseMatrix by a vector. <br>
//!          An exception is raised if the dimensions are different <br>
  Standard_EXPORT   virtual  void Multiplied(const math_Vector& X,math_Vector& MX) const = 0;
  //! returns the row range of a matrix. <br>
  Standard_EXPORT   virtual  Standard_Integer RowNumber() const = 0;
  //! returns the column range of the matrix. <br>
  Standard_EXPORT   virtual  Standard_Integer ColNumber() const = 0;




  DEFINE_STANDARD_RTTI(FEmTool_SparseMatrix)

protected:




private: 




};





// other Inline functions and methods (like "C++: function call" methods)


#endif
