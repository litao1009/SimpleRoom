// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _IFSelect_SessionDumper_HeaderFile
#define _IFSelect_SessionDumper_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_IFSelect_SessionDumper_HeaderFile
#include <Handle_IFSelect_SessionDumper.hxx>
#endif

#ifndef _Handle_IFSelect_SessionDumper_HeaderFile
#include <Handle_IFSelect_SessionDumper.hxx>
#endif
#ifndef _MMgt_TShared_HeaderFile
#include <MMgt_TShared.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Handle_Standard_Transient_HeaderFile
#include <Handle_Standard_Transient.hxx>
#endif
class IFSelect_SessionFile;
class Standard_Transient;
class TCollection_AsciiString;


//! A SessionDumper is called by SessionFile. It takes into <br>
//!           account a set of classes (such as Selections, Dispatches ...). <br>
//!           SessionFile writes the Type (as defined by cdl) of each Item <br>
//!           and its general Parameters. It manages the names of the Items. <br>
//! <br>
//!           A SessionDumper must be able to Write the Parameters which are <br>
//!           own of each Item it takes into account, given its Class, then <br>
//!           to Recognize the Type and Read its Own Parameters to create <br>
//!           an Item of this Type with these own Parameters. <br>
//! <br>
//!           Then, there must be defined one sub-type of SessionDumper per <br>
//!           consistent set of classes (e.g. a package). <br>
//! <br>
//!           By Own Parameters, understand Parameters given at Creation Time <br>
//!           if there are, or specific of a given class, apart from those <br>
//!           defined at superclass levels (e.g. Final Selection for a <br>
//!           Dispatch, Input Selection for a SelectExtract or SelectDeduct, <br>
//!           Direct Status for a SelectExtract, etc...). <br>
//! <br>
//!           The Parameters are those stored in a WorkSession, they can be <br>
//!           of Types : IntParam, HAsciiString (for TextParam), Selection, <br>
//!           Dispatch. <br>
//! <br>
//!           SessionDumpers are organized in a Library which is used by <br>
//!           SessionFile. They are put at Creation Time in this Library. <br>
class IFSelect_SessionDumper : public MMgt_TShared {

public:

  //! Returns the First item of the Library of Dumper. The Next ones <br>
//!           are then obtained by Next on the returned items <br>
  Standard_EXPORT   static  Handle_IFSelect_SessionDumper First() ;
  //! Returns the Next SesionDumper in the Library. Returns a Null <br>
//!           Handle at the End. <br>
  Standard_EXPORT     Handle_IFSelect_SessionDumper Next() const;
  //! Writes the Own Parameters of a given Item, if it forecast to <br>
//!           manage its Type. <br>
//!           Returns True if it has recognized the Type of the Item (in <br>
//!           this case, it is assumed to have written the Own Parameters if <br>
//!           there are some), False else : in that case, SessionFile will <br>
//!           try another SessionDumper in the Library. <br>
//!           WriteOwn can use these methods from SessionFile : SendVoid, <br>
//!           SendItem, SendText, and if necessary, WorkSession. <br>
  Standard_EXPORT   virtual  Standard_Boolean WriteOwn(IFSelect_SessionFile& file,const Handle(Standard_Transient)& item) const = 0;
  //! Recognizes a Type (given as <type>) then Creates an Item of <br>
//!           this Type with the Own Parameter, as required. <br>
//!           Returns True if it has recognized the Type (in this case, it <br>
//!           is assumed to have created the Item, returned as <item>), <br>
//!           False else : in that case, SessionFile will try another <br>
//!           SessionDumper in the Library. <br>
//!           ReadOwn can use these methods from SessionFile to access Own <br>
//!           Parameters : NbOwnParams, IsVoid, IsText, TextValue, ItemValue <br>
  Standard_EXPORT   virtual  Standard_Boolean ReadOwn(IFSelect_SessionFile& file,const TCollection_AsciiString& type,Handle(Standard_Transient)& item) const = 0;




  DEFINE_STANDARD_RTTI(IFSelect_SessionDumper)

protected:

  //! The Initialization puts a just created SessionDumper in the <br>
//!           Library of SessionDumper. Then, it suffices to create once <br>
//!           a SessionDumper to fill the Library with it <br>
  Standard_EXPORT   IFSelect_SessionDumper();



private: 


Handle_IFSelect_SessionDumper thenext;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
