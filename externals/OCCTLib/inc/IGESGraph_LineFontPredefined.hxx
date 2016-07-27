// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _IGESGraph_LineFontPredefined_HeaderFile
#define _IGESGraph_LineFontPredefined_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_IGESGraph_LineFontPredefined_HeaderFile
#include <Handle_IGESGraph_LineFontPredefined.hxx>
#endif

#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _IGESData_IGESEntity_HeaderFile
#include <IGESData_IGESEntity.hxx>
#endif


//! defines IGESLineFontPredefined, Type <406> Form <19> <br>
//!          in package IGESGraph <br>
//! <br>
//!          Provides the ability to specify a line font pattern <br>
//!          from a predefined list rather than from <br>
//!          Directory Entry Field 4 <br>
class IGESGraph_LineFontPredefined : public IGESData_IGESEntity {

public:

  
  Standard_EXPORT   IGESGraph_LineFontPredefined();
  //! This method is used to set the fields of the class <br>
//!           LineFontPredefined <br>
//!       - nbProps              : Number of property values (NP = 1) <br>
//!       - aLineFontPatternCode : Line Font Pattern Code <br>
  Standard_EXPORT     void Init(const Standard_Integer nbProps,const Standard_Integer aLineFontPatternCode) ;
  //! returns the number of property values in <me> <br>
  Standard_EXPORT     Standard_Integer NbPropertyValues() const;
  //! returns the Line Font Pattern Code of <me> <br>
  Standard_EXPORT     Standard_Integer LineFontPatternCode() const;




  DEFINE_STANDARD_RTTI(IGESGraph_LineFontPredefined)

protected:




private: 


Standard_Integer theNbPropertyValues;
Standard_Integer theLineFontPatternCode;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
