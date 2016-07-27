// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _math_Matrix_HeaderFile
#define _math_Matrix_HeaderFile

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
#ifndef _math_DoubleTabOfReal_HeaderFile
#include <math_DoubleTabOfReal.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
#ifndef _math_Vector_HeaderFile
#include <math_Vector.hxx>
#endif
#ifndef _Standard_OStream_HeaderFile
#include <Standard_OStream.hxx>
#endif
class Standard_DimensionError;
class Standard_RangeError;
class Standard_DivideByZero;
class math_NotSquare;
class math_SingularMatrix;
class math_Vector;


//! This class implements the real matrix abstract data type. <br>
//!          Matrixes can have an arbitrary range which must be defined <br>
//!          at the declaration and cannot be changed after this declaration <br>
//!          math_Matrix(-3,5,2,4); //a vector with range [-3..5, 2..4] <br>
//!          Matrix values may be initialized and <br>
//!          retrieved using indexes which must lie within the range <br>
//!          of definition of the matrix. <br>
//!          Matrix objects follow "value semantics", that is, they <br>
//!          cannot be shared and are copied through assignment <br>
//!          Matrices are copied through assignement: <br>
//!          math_Matrix M2(1, 9, 1, 3); <br>
//!          ... <br>
//!          M2 = M1; <br>
//!          M1(1) = 2.0;//the matrix M2 will not be modified. <br>
//! <br>
//!          The exception RangeError is raised when trying to access <br>
//!          outside the range of a matrix : <br>
//!          M1(11, 1)=0.0// --> will raise RangeError. <br>
//! <br>
//!          The exception DimensionError is raised when the dimensions of <br>
//!          two matrices or vectors are not compatible. <br>
//!          math_Matrix M3(1, 2, 1, 2); <br>
//!          M3 = M1;   // will raise DimensionError <br>
//!          M1.Add(M3) // --> will raise DimensionError. <br>
//!          A Matrix can be constructed with a a pointer to "c array". <br>
//!          It allows to carry the bounds inside the matrix. <br>
//! Exemple : <br>
//! 	    Standard_Real tab1[10][20]; <br>
//! 	    Standard_Real tab2[200]; <br>
//! <br>
//!      math_Matrix A (tab1[0][0], 1, 10, 1, 20); <br>
//!      math_Matrix B (tab2[0],    1, 10, 1, 20); <br>
class math_Matrix  {
public:

  DEFINE_STANDARD_ALLOC

  //! Constructs a non-initialized  matrix of range [LowerRow..UpperRow, <br>
//!                                        LowerCol..UpperCol] <br>
//! For the constructed matrix: <br>
//! -   LowerRow and UpperRow are the indexes of the <br>
//!   lower and upper bounds of a row, and <br>
//! -   LowerCol and UpperCol are the indexes of the <br>
//!   lower and upper bounds of a column. <br>
  Standard_EXPORT   math_Matrix(const Standard_Integer LowerRow,const Standard_Integer UpperRow,const Standard_Integer LowerCol,const Standard_Integer UpperCol);
  //! constructs a non-initialized matrix of range [LowerRow..UpperRow, <br>
//!                                        LowerCol..UpperCol] <br>
//!       whose values are all initialized with the value InitialValue. <br>
  Standard_EXPORT   math_Matrix(const Standard_Integer LowerRow,const Standard_Integer UpperRow,const Standard_Integer LowerCol,const Standard_Integer UpperCol,const Standard_Real InitialValue);
  //! constructs a matrix of range [LowerRow..UpperRow, <br>
//!                                        LowerCol..UpperCol] <br>
//! Sharing data with a "C array" pointed by Tab. <br>
  Standard_EXPORT   math_Matrix(const Standard_Address Tab,const Standard_Integer LowerRow,const Standard_Integer UpperRow,const Standard_Integer LowerCol,const Standard_Integer UpperCol);
  //! constructs a matrix for copy in initialization. <br>
//! An exception is raised if the matrixes have not the same dimensions. <br>
  Standard_EXPORT   math_Matrix(const math_Matrix& Other);
  //!Initialize all the elements of a matrix to InitialValue. <br>
  Standard_EXPORT     void Init(const Standard_Real InitialValue) ;
  //! Returns the number of rows  of this matrix. <br>
//! Note that for a matrix A you always have the following relations: <br>
//! - A.RowNumber() = A.UpperRow() -   A.LowerRow() + 1 <br>
//! - A.ColNumber() = A.UpperCol() -   A.LowerCol() + 1 <br>
//! - the length of a row of A is equal to the number of columns of A, <br>
//! - the length of a column of A is equal to the number of <br>
//!   rows of A.returns the row range of a matrix. <br>
        Standard_Integer RowNumber() const;
  //! Returns the number of rows  of this matrix. <br>
//! Note that for a matrix A you always have the following relations: <br>
//! - A.RowNumber() = A.UpperRow() -   A.LowerRow() + 1 <br>
//! - A.ColNumber() = A.UpperCol() -   A.LowerCol() + 1 <br>
//! - the length of a row of A is equal to the number of columns of A, <br>
//! - the length of a column of A is equal to the number of <br>
//!   rows of A.returns the row range of a matrix. <br>
        Standard_Integer ColNumber() const;
  //! Returns the value of the Lower index of the row <br>
//!          range of a matrix. <br>
        Standard_Integer LowerRow() const;
  //! Returns the Upper index of the row range <br>
//!          of a matrix. <br>
        Standard_Integer UpperRow() const;
  //! Returns the value of the Lower index of the <br>
//!          column range of a matrix. <br>
        Standard_Integer LowerCol() const;
  //! Returns the value of the upper index of the <br>
//!          column range of a matrix. <br>
        Standard_Integer UpperCol() const;
  //! Computes the determinant of a matrix. <br>
//! An exception is raised if the matrix is not a square matrix. <br>
  Standard_EXPORT     Standard_Real Determinant() const;
  //! Transposes a given matrix. <br>
//! An exception is raised if the matrix is not a square matrix. <br>
  Standard_EXPORT     void Transpose() ;
  //! Inverts a matrix using Gauss algorithm. <br>
//! Exception NotSquare is raised if the matrix is not square. <br>
//! Exception SingularMatrix is raised if the matrix is singular. <br>
  Standard_EXPORT     void Invert() ;
  //! Sets this matrix to the product of the matrix Left, and the matrix Right. <br>
//! Example <br>
//! math_Matrix A (1, 3, 1, 3); <br>
//! math_Matrix B (1, 3, 1, 3); <br>
//! // A = ... , B = ... <br>
//! math_Matrix C (1, 3, 1, 3); <br>
//! C.Multiply(A, B); <br>
//! Exceptions <br>
//! Standard_DimensionError if matrices are of incompatible dimensions, i.e. if: <br>
//! -   the number of columns of matrix Left, or the number of <br>
//!   rows of matrix TLeft is not equal to the number of rows <br>
//!   of matrix Right, or <br>
//! -   the number of rows of matrix Left, or the number of <br>
//!   columns of matrix TLeft is not equal to the number of <br>
//!   rows of this matrix, or <br>
//! -   the number of columns of matrix Right is not equal to <br>
//!   the number of columns of this matrix. <br>
  Standard_EXPORT     void Multiply(const Standard_Real Right) ;
    void operator*=(const Standard_Real Right) 
{
  Multiply(Right);
}
  //! multiplies all the elements of a matrix by the <br>
//!          value <Right>. <br>
  Standard_EXPORT     math_Matrix Multiplied(const Standard_Real Right) const;
    math_Matrix operator*(const Standard_Real Right) const
{
  return Multiplied(Right);
}
  //! Sets this matrix to the product of the <br>
//! transposed matrix TLeft, and the matrix Right. <br>
//! Example <br>
//! math_Matrix A (1, 3, 1, 3); <br>
//! math_Matrix B (1, 3, 1, 3); <br>
//! // A = ... , B = ... <br>
//! math_Matrix C (1, 3, 1, 3); <br>
//! C.Multiply(A, B); <br>
//! Exceptions <br>
//! Standard_DimensionError if matrices are of incompatible dimensions, i.e. if: <br>
//! -   the number of columns of matrix Left, or the number of <br>
//!   rows of matrix TLeft is not equal to the number of rows <br>
//!   of matrix Right, or <br>
//! -   the number of rows of matrix Left, or the number of <br>
//!   columns of matrix TLeft is not equal to the number of <br>
//!   rows of this matrix, or <br>
//! -   the number of columns of matrix Right is not equal to <br>
//!   the number of columns of this matrix. <br>
  Standard_EXPORT     math_Matrix TMultiplied(const Standard_Real Right) const;
friend math_Matrix  operator *(const Standard_Real Left,const math_Matrix& Right);
  //! divides all the elements of a matrix by the value <Right>. <br>
//!          An exception is raised if <Right> = 0. <br>
  Standard_EXPORT     void Divide(const Standard_Real Right) ;
    void operator/=(const Standard_Real Right) 
{
  Divide(Right);
}
  //! divides all the elements of a matrix by the value <Right>. <br>
//!          An exception is raised if <Right> = 0. <br>
  Standard_EXPORT     math_Matrix Divided(const Standard_Real Right) const;
    math_Matrix operator/(const Standard_Real Right) const
{
  return Divided(Right);
}
  //! adds the matrix <Right> to a matrix. <br>
//! An exception is raised if the dimensions are different. <br>
//! 	Warning <br>
//! In order to save time when copying matrices, it is <br>
//! preferable to use operator += or the function Add <br>
//! whenever possible. <br>
  Standard_EXPORT     void Add(const math_Matrix& Right) ;
    void operator+=(const math_Matrix& Right) 
{
  Add(Right);
}
  //! adds the matrix <Right> to a matrix. <br>
//! An exception is raised if the dimensions are different. <br>
  Standard_EXPORT     math_Matrix Added(const math_Matrix& Right) const;
    math_Matrix operator+(const math_Matrix& Right) const
{
  return Added(Right);
}
  //! sets a  matrix to the addition of <Left> and <Right>. <br>
//! An exception is raised if the dimensions are different. <br>
  Standard_EXPORT     void Add(const math_Matrix& Left,const math_Matrix& Right) ;
  //! Subtracts the matrix <Right> from <me>. <br>
//!          An exception is raised if the dimensions are different. <br>
//!    	Warning <br>
//! In order to avoid time-consuming copying of matrices, it <br>
//! is preferable to use operator -= or the function <br>
//! Subtract whenever possible. <br>
  Standard_EXPORT     void Subtract(const math_Matrix& Right) ;
    void operator-=(const math_Matrix& Right) 
{
  Subtract(Right);
}
  //! Returns the result of the subtraction of <Right> from <me>. <br>
//!          An exception is raised if the dimensions are different. <br>
  Standard_EXPORT     math_Matrix Subtracted(const math_Matrix& Right) const;
    math_Matrix operator-(const math_Matrix& Right) const
{
  return Subtracted(Right);
}
  //! Sets the values of this matrix, <br>
//! -   from index I1 to index I2 on the row dimension, and <br>
//! -   from index J1 to index J2 on the column dimension, <br>
//!   to those of matrix M. <br>
//! Exceptions <br>
//! Standard_DimensionError if: <br>
//! -   I1 is less than the index of the lower row bound of this matrix, or <br>
//! -   I2 is greater than the index of the upper row bound of this matrix, or <br>
//! -   J1 is less than the index of the lower column bound of this matrix, or <br>
//! -   J2 is greater than the index of the upper column bound of this matrix, or <br>
//! -   I2 - I1 + 1 is not equal to the number of rows of matrix M, or <br>
//! -   J2 - J1 + 1 is not equal to the number of columns of matrix M. <br>
  Standard_EXPORT     void Set(const Standard_Integer I1,const Standard_Integer I2,const Standard_Integer J1,const Standard_Integer J2,const math_Matrix& M) ;
  //! Sets the row of index Row of a matrix to the vector <V>. <br>
//!          An exception is raised if the dimensions are different. <br>
//!          An exception is raises if <Row> is inferior to the lower <br>
//!          row of the matrix or <Row> is superior to the upper row. <br>
  Standard_EXPORT     void SetRow(const Standard_Integer Row,const math_Vector& V) ;
  //! Sets the column of index Col of a matrix to the vector <V>. <br>
//!          An exception is raised if the dimensions are different. <br>
//!          An exception is raises if <Col> is inferior to the lower <br>
//!          column of the matrix or <Col> is superior to the upper <br>
//!          column. <br>
  Standard_EXPORT     void SetCol(const Standard_Integer Col,const math_Vector& V) ;
  //! Sets the diagonal of a matrix to the value <Value>. <br>
//! An exception is raised if the matrix is not square. <br>
  Standard_EXPORT     void SetDiag(const Standard_Real Value) ;
  //! Returns the row of index Row of a matrix. <br>
  Standard_EXPORT     math_Vector Row(const Standard_Integer Row) const;
  //! Returns the column of index <Col> of a matrix. <br>
  Standard_EXPORT     math_Vector Col(const Standard_Integer Col) const;
  //! Swaps the rows of index Row1 and Row2. <br>
//! An exception is raised if <Row1> or <Row2> is out of range. <br>
  Standard_EXPORT     void SwapRow(const Standard_Integer Row1,const Standard_Integer Row2) ;
  //! Swaps the columns of index <Col1> and <Col2>. <br>
//! An exception is raised if <Col1> or <Col2> is out of range. <br>
  Standard_EXPORT     void SwapCol(const Standard_Integer Col1,const Standard_Integer Col2) ;
  //! Teturns the transposed of a matrix. <br>
//! An exception is raised if the matrix is not a square matrix. <br>
  Standard_EXPORT     math_Matrix Transposed() const;
  //! Returns the inverse of a matrix. <br>
//! Exception NotSquare is raised if the matrix is not square. <br>
//! Exception SingularMatrix is raised if the matrix is singular. <br>
  Standard_EXPORT     math_Matrix Inverse() const;
  //! Returns the product of the transpose of a matrix with <br>
//! the matrix <Right>. <br>
//! An exception is raised if the dimensions are different. <br>
  Standard_EXPORT     math_Matrix TMultiply(const math_Matrix& Right) const;
  //! Computes a matrix as the product of 2 vectors. <br>
//! An exception is raised if the dimensions are different. <br>
//! <me> = <Left> * <Right>. <br>
  Standard_EXPORT     void Multiply(const math_Vector& Left,const math_Vector& Right) ;
  //! Computes a matrix as the product of 2 matrixes. <br>
//! An exception is raised if the dimensions are different. <br>
  Standard_EXPORT     void Multiply(const math_Matrix& Left,const math_Matrix& Right) ;
  //! Computes a matrix to the product of the transpose of <br>
//!          the matrix <TLeft> with the matrix <Right>. <br>
//! An exception is raised if the dimensions are different. <br>
  Standard_EXPORT     void TMultiply(const math_Matrix& TLeft,const math_Matrix& Right) ;
  //! Sets a matrix to the Subtraction of the matrix <Right> <br>
//!           from the matrix <Left>. <br>
//! An exception is raised if the dimensions are different. <br>
  Standard_EXPORT     void Subtract(const math_Matrix& Left,const math_Matrix& Right) ;
  //! Accesses (in read or write mode) the value of index <Row> <br>
//!          and <Col> of a matrix. <br>
//!          An exception is raised if <Row> and <Col> are not <br>
//!          in the correct range. <br>
        Standard_Real& Value(const Standard_Integer Row,const Standard_Integer Col) const;
      Standard_Real& operator()(const Standard_Integer Row,const Standard_Integer Col) const
{
  return Value(Row,Col);
}
  //! Matrixes are copied through assignement. <br>
//!          An exception is raised if the dimensions are differents. <br>
  Standard_EXPORT     math_Matrix& Initialized(const math_Matrix& Other) ;
    math_Matrix& operator=(const math_Matrix& Other) 
{
  return Initialized(Other);
}
  //! Returns the product of 2 matrices. <br>
//!          An exception is raised if the dimensions are different. <br>
  Standard_EXPORT     void Multiply(const math_Matrix& Right) ;
    void operator*=(const math_Matrix& Right) 
{
  Multiply(Right);
}
  //! Returns the product of 2 matrices. <br>
//!          An exception is raised if the dimensions are different. <br>
  Standard_EXPORT     math_Matrix Multiplied(const math_Matrix& Right) const;
    math_Matrix operator*(const math_Matrix& Right) const
{
  return Multiplied(Right);
}
  //! Returns the product of a matrix by a vector. <br>
//!          An exception is raised if the dimensions are different. <br>
  Standard_EXPORT     math_Vector Multiplied(const math_Vector& Right) const;
    math_Vector operator*(const math_Vector& Right) const
{
  return Multiplied(Right);
}
  //! Returns the opposite of a matrix. <br>
//!          An exception is raised if the dimensions are different. <br>
  Standard_EXPORT     math_Matrix Opposite() ;
    math_Matrix operator-() 
{
  return Opposite();
}
  //! Prints information on the current state of the object. <br>
//!          Is used to redefine the operator <<. <br>
  Standard_EXPORT     void Dump(Standard_OStream& o) const;


friend class math_Vector;



protected:

  //! The new lower row of the matrix is set to <LowerRow> <br>
  Standard_EXPORT     void SetLowerRow(const Standard_Integer LowerRow) ;
  //! The new lower column of the matrix is set to the column <br>
//!          of range <LowerCol>. <br>
  Standard_EXPORT     void SetLowerCol(const Standard_Integer LowerCol) ;
  //! The new lower row of the matrix is set to <LowerRow> <br>
//!         and the new lower column of the matrix is set to the column <br>
//!          of range <LowerCol>. <br>
        void SetLower(const Standard_Integer LowerRow,const Standard_Integer LowerCol) ;




private:



Standard_Integer LowerRowIndex;
Standard_Integer UpperRowIndex;
Standard_Integer LowerColIndex;
Standard_Integer UpperColIndex;
math_DoubleTabOfReal Array;


};


#include <math_Matrix.lxx>



// other Inline functions and methods (like "C++: function call" methods)


#endif
