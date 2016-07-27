// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Message_ProgressScale_HeaderFile
#define _Message_ProgressScale_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _Handle_TCollection_HAsciiString_HeaderFile
#include <Handle_TCollection_HAsciiString.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Standard_CString_HeaderFile
#include <Standard_CString.hxx>
#endif
class TCollection_HAsciiString;


//! Internal data structure for scale in ProgressIndicator <br>
//! <br>
//!          Basically it defines three things: <br>
//!          - name that can be used for generating user messages <br>
//!          - limits and characteristics of the current scale, <br>
//!            along with derived coefficients to map it into basic scale [0-1] <br>
//!          - methods for conversion of values from current scale <br>
//!            to basic one and back <br>
//! <br>
//!          NOTE: There is no special protection against bad input data <br>
//!          like min > max etc. except cases when it can cause exception <br>
class Message_ProgressScale  {
public:

  DEFINE_STANDARD_ALLOC

  //! Creates scale ranged from 0 to 100 with step 1 <br>
  Standard_EXPORT   Message_ProgressScale();
  
        void SetName(const Standard_CString theName) ;
  //! Sets scale name <br>
        void SetName(const Handle(TCollection_HAsciiString)& theName) ;
  //! Gets scale name <br>
//!          Name may be Null handle if not set <br>
        Handle_TCollection_HAsciiString GetName() const;
  //! Sets minimum value of scale <br>
        void SetMin(const Standard_Real theMin) ;
  //! Gets minimum value of scale <br>
        Standard_Real GetMin() const;
  //! Sets minimum value of scale <br>
        void SetMax(const Standard_Real theMax) ;
  //! Gets minimum value of scale <br>
        Standard_Real GetMax() const;
  //! Set both min and max <br>
        void SetRange(const Standard_Real min,const Standard_Real max) ;
  //! Sets default step <br>
        void SetStep(const Standard_Real theStep) ;
  //! Gets default step <br>
        Standard_Real GetStep() const;
  //! Sets flag for infinite scale <br>
        void SetInfinite(const Standard_Boolean theInfinite = Standard_True) ;
  //! Gets flag for infinite scale <br>
        Standard_Boolean GetInfinite() const;
  //! Set all scale parameters <br>
        void SetScale(const Standard_Real min,const Standard_Real max,const Standard_Real step,const Standard_Boolean theInfinite = Standard_True) ;
  //! Defines span occupied by the scale on the basis scale <br>
        void SetSpan(const Standard_Real first,const Standard_Real last) ;
  
        Standard_Real GetFirst() const;
  //! Return information on span occupied by the scale on the base scale <br>
        Standard_Real GetLast() const;
  
  Standard_EXPORT     Standard_Real LocalToBase(const Standard_Real val) const;
  //! Convert value from this scale to base one and back <br>
  Standard_EXPORT     Standard_Real BaseToLocal(const Standard_Real val) const;





protected:





private:



Handle_TCollection_HAsciiString myName;
Standard_Real myMin;
Standard_Real myMax;
Standard_Real myStep;
Standard_Boolean myInfinite;
Standard_Real myFirst;
Standard_Real myLast;


};


#include <Message_ProgressScale.lxx>



// other Inline functions and methods (like "C++: function call" methods)


#endif
