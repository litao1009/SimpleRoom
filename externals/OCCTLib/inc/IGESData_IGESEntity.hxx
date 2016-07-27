// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _IGESData_IGESEntity_HeaderFile
#define _IGESData_IGESEntity_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_IGESData_IGESEntity_HeaderFile
#include <Handle_IGESData_IGESEntity.hxx>
#endif

#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Handle_IGESData_IGESEntity_HeaderFile
#include <Handle_IGESData_IGESEntity.hxx>
#endif
#ifndef _IGESData_DefSwitch_HeaderFile
#include <IGESData_DefSwitch.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Standard_Character_HeaderFile
#include <Standard_Character.hxx>
#endif
#ifndef _Handle_TCollection_HAsciiString_HeaderFile
#include <Handle_TCollection_HAsciiString.hxx>
#endif
#ifndef _Interface_EntityList_HeaderFile
#include <Interface_EntityList.hxx>
#endif
#ifndef _MMgt_TShared_HeaderFile
#include <MMgt_TShared.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _IGESData_DefType_HeaderFile
#include <IGESData_DefType.hxx>
#endif
#ifndef _Handle_IGESData_LineFontEntity_HeaderFile
#include <Handle_IGESData_LineFontEntity.hxx>
#endif
#ifndef _IGESData_DefList_HeaderFile
#include <IGESData_DefList.hxx>
#endif
#ifndef _Handle_IGESData_LevelListEntity_HeaderFile
#include <Handle_IGESData_LevelListEntity.hxx>
#endif
#ifndef _Handle_IGESData_ViewKindEntity_HeaderFile
#include <Handle_IGESData_ViewKindEntity.hxx>
#endif
#ifndef _Handle_IGESData_TransfEntity_HeaderFile
#include <Handle_IGESData_TransfEntity.hxx>
#endif
#ifndef _Handle_IGESData_LabelDisplayEntity_HeaderFile
#include <Handle_IGESData_LabelDisplayEntity.hxx>
#endif
#ifndef _Handle_IGESData_ColorEntity_HeaderFile
#include <Handle_IGESData_ColorEntity.hxx>
#endif
#ifndef _Standard_CString_HeaderFile
#include <Standard_CString.hxx>
#endif
#ifndef _Handle_Standard_Type_HeaderFile
#include <Handle_Standard_Type.hxx>
#endif
class TCollection_HAsciiString;
class Interface_InterfaceError;
class IGESData_ReadWriteModule;
class IGESData_GeneralModule;
class IGESData_IGESReaderTool;
class IGESData_DirChecker;
class IGESData_IGESType;
class IGESData_LineFontEntity;
class IGESData_LevelListEntity;
class IGESData_ViewKindEntity;
class IGESData_TransfEntity;
class IGESData_LabelDisplayEntity;
class IGESData_ColorEntity;
class gp_GTrsf;
class Interface_EntityIterator;
class Standard_Type;
class Interface_EntityList;


//! defines root of IGES Entity definition, including Directory <br>
//!           Part, lists of (optionnal) Properties and Associativities <br>
class IGESData_IGESEntity : public MMgt_TShared {

public:

  //! gives IGES typing info (includes "Type" and "Form" data) <br>
  Standard_EXPORT     IGESData_IGESType IGESType() const;
  //! gives IGES Type Number (often coupled with Form Number) <br>
  Standard_EXPORT     Standard_Integer TypeNumber() const;
  //! Returns the form number for that <br>
//!           type of an IGES entity. The default form number is 0. <br>
  Standard_EXPORT     Standard_Integer FormNumber() const;
  //! Returns the Entity which has been recorded for a given <br>
//!           Field Number, i.e. without any cast. Maps with : <br>
//!           3 : Structure   4 : LineFont     5 : LevelList     6 : View <br>
//!           7 : Transf(ormation Matrix)      8 : LabelDisplay <br>
//!           13 : Color.  Other values give a null handle <br>
//!           It can then be of any kind, while specific items have a Type <br>
  Standard_EXPORT     Handle_IGESData_IGESEntity DirFieldEntity(const Standard_Integer fieldnum) const;
  //! returns True if an IGESEntity is defined with a Structure <br>
//!           (it is normally reserved for certain classes, such as Macros) <br>
  Standard_EXPORT     Standard_Boolean HasStructure() const;
  //! Returns Structure (used by some types of IGES Entities only) <br>
//!           Returns a Null Handle if Structure is not defined <br>
  Standard_EXPORT     Handle_IGESData_IGESEntity Structure() const;
  //! Returns the definition status of LineFont <br>
  Standard_EXPORT   virtual  IGESData_DefType DefLineFont() const;
  //! Returns LineFont definition as an Integer (if defined as Rank) <br>
//!           If LineFont is defined as an Entity, returns a negative value <br>
  Standard_EXPORT     Standard_Integer RankLineFont() const;
  //! Returns LineFont as an Entity (if defined as Reference) <br>
//!           Returns a Null Handle if DefLineFont is not "DefReference" <br>
  Standard_EXPORT     Handle_IGESData_LineFontEntity LineFont() const;
  //! Returns the definition status of Level <br>
  Standard_EXPORT   virtual  IGESData_DefList DefLevel() const;
  //! Returns the level the entity <br>
//!           belongs to. Returns -1 if the entity belongs to more than one  level. <br>
  Standard_EXPORT     Standard_Integer Level() const;
  //! Returns LevelList if Level is <br>
//!           defined as a list. Returns a null handle if DefLevel is not DefSeveral. <br>
  Standard_EXPORT     Handle_IGESData_LevelListEntity LevelList() const;
  //! Returns the definition status of <br>
//!           the view. This can be: none, one or several. <br>
  Standard_EXPORT   virtual  IGESData_DefList DefView() const;
  //! Returns the view of this IGES entity. <br>
//!           This view can be a single view or a list of views. <br>
//!           Warning A null handle is returned if the view is not defined. <br>
  Standard_EXPORT     Handle_IGESData_ViewKindEntity View() const;
  //! Returns the view as a single view <br>
//!           if it was defined as such and not as a list of views. <br>
//!           Warning A null handle is returned if DefView does not have the value DefOne. <br>
  Standard_EXPORT     Handle_IGESData_ViewKindEntity SingleView() const;
  //! Returns the view of this IGES entity as a list. <br>
//!           Warning A null handle is returned if the <br>
//!           definition status does not have the value DefSeveral. <br>
  Standard_EXPORT     Handle_IGESData_ViewKindEntity ViewList() const;
  //! Returns True if a Transformation Matrix is defined <br>
  Standard_EXPORT     Standard_Boolean HasTransf() const;
  //! Returns the Transformation Matrix (under IGES definition) <br>
//!           Returns a Null Handle if there is none <br>
//!           for a more complete use, see Location & CompoundLocation <br>
  Standard_EXPORT     Handle_IGESData_TransfEntity Transf() const;
  //! Returns True if a LabelDisplay mode is defined for this entity <br>
  Standard_EXPORT     Standard_Boolean HasLabelDisplay() const;
  //! Returns the Label Display <br>
//!           Associativity Entity if there is one. Returns a null handle if there is none. <br>
  Standard_EXPORT     Handle_IGESData_LabelDisplayEntity LabelDisplay() const;
  //! gives Blank Status (0 visible, 1 blanked) <br>
  Standard_EXPORT     Standard_Integer BlankStatus() const;
  //! gives Subordinate Switch (0-1-2-3) <br>
  Standard_EXPORT     Standard_Integer SubordinateStatus() const;
  //! gives Entity's Use Flag (0 to 5) <br>
  Standard_EXPORT     Standard_Integer UseFlag() const;
  //! gives Hierarchy status (0-1-2) <br>
  Standard_EXPORT     Standard_Integer HierarchyStatus() const;
  //! Returns the LineWeight Number (0  not defined), see also LineWeight <br>
  Standard_EXPORT     Standard_Integer LineWeightNumber() const;
  //! Returns the true Line Weight, computed from LineWeightNumber and <br>
//!           Global Parameter in the Model by call to SetLineWeight <br>
  Standard_EXPORT     Standard_Real LineWeight() const;
  //! Returns the definition status of Color. <br>
  Standard_EXPORT   virtual  IGESData_DefType DefColor() const;
  //! Returns the color definition as <br>
//!           an integer value if the color was defined as a rank. <br>
//!           Warning A negative value is returned if the color was defined as an entity. <br>
  Standard_EXPORT     Standard_Integer RankColor() const;
  //! Returns the IGES entity which <br>
//! describes the color of the entity. <br>
//! Returns a null handle if this entity was defined as an integer. <br>
  Standard_EXPORT     Handle_IGESData_ColorEntity Color() const;
  //! returns "reserved" alphanumeric values res1 and res2 <br>
//!           res1 and res2 have to be reserved as Character[9 at least] <br>
//!           (remark : their content is changed) <br>
//!           returned values are ended by null character in 9th <br>
//!           returned Boolean is False if res1 and res2 are blank, true else <br>
  Standard_EXPORT     Standard_Boolean CResValues(const Standard_CString res1,const Standard_CString res2) const;
  //! Returns true if a short label is defined. <br>
//!           A short label is a non-blank 8-character string. <br>
  Standard_EXPORT     Standard_Boolean HasShortLabel() const;
  //! Returns the label value for this IGES entity as a string. <br>
//!           Warning If the label is blank, this string is null. <br>
  Standard_EXPORT     Handle_TCollection_HAsciiString ShortLabel() const;
  //! Returns true if a subscript number is defined. <br>
//!           A subscript number is an integer used to identify a label. <br>
  Standard_EXPORT   virtual  Standard_Boolean HasSubScriptNumber() const;
  //! Returns the integer subscript number used to identify this IGES entity. <br>
//! Warning 0 is returned if no subscript number is defined for this IGES entity. <br>
  Standard_EXPORT     Standard_Integer SubScriptNumber() const;
  //! Initializes a directory field as an Entiy of any kind <br>
//!           See DirFieldEntity for more details <br>
  Standard_EXPORT     void InitDirFieldEntity(const Standard_Integer fieldnum,const Handle(IGESData_IGESEntity)& ent) ;
  //! Initializes Transf, or erases it if <ent> is given Null <br>
  Standard_EXPORT     void InitTransf(const Handle(IGESData_TransfEntity)& ent) ;
  //! Initializes View, or erases it if <ent> is given Null <br>
  Standard_EXPORT     void InitView(const Handle(IGESData_ViewKindEntity)& ent) ;
  //! Initializes LineFont : if <ent> is not Null, it gives LineFont, <br>
//!           else <rank> gives or erases (if zero) RankLineFont <br>
  Standard_EXPORT     void InitLineFont(const Handle(IGESData_LineFontEntity)& ent,const Standard_Integer rank = 0) ;
  //! Initializes Level : if <ent> is not Null, it gives LevelList, <br>
//!           else <val> gives or erases (if zero) unique Level <br>
  Standard_EXPORT     void InitLevel(const Handle(IGESData_LevelListEntity)& ent,const Standard_Integer val = 0) ;
  //! Initializes Color data : if <ent> is not Null, it gives Color, <br>
//!           else <rank> gives or erases (if zero) RankColor <br>
  Standard_EXPORT     void InitColor(const Handle(IGESData_ColorEntity)& ent,const Standard_Integer rank = 0) ;
  //! Initializes the Status of Directory Part <br>
  Standard_EXPORT     void InitStatus(const Standard_Integer blank,const Standard_Integer subordinate,const Standard_Integer useflag,const Standard_Integer hierarchy) ;
  //! Sets a new Label to an IGES Entity <br>
//!           If <sub> is given, it sets value of SubScriptNumber <br>
//!           else, SubScriptNumber is erased <br>
  Standard_EXPORT     void SetLabel(const Handle(TCollection_HAsciiString)& label,const Standard_Integer sub = -1) ;
  //! Initializes various data (those not yet seen above), or erases <br>
//!           them if they are given as Null (Zero for <weightnum>) : <br>
//!           <str> for Structure, <lab> for LabelDisplay, and <br>
//!           <weightnum> for WeightNumber <br>
  Standard_EXPORT     void InitMisc(const Handle(IGESData_IGESEntity)& str,const Handle(IGESData_LabelDisplayEntity)& lab,const Standard_Integer weightnum) ;
  //! Returns True if an entity has one and only one parent, defined <br>
//!           by a SingleParentEntity Type Associativity (explicit sharing). <br>
//!           Thus, implicit sharing remains defined at model level <br>
//!           (see class ToolLocation) <br>
  Standard_EXPORT     Standard_Boolean HasOneParent() const;
  //! Returns the Unique Parent (in the sense given by HasOneParent) <br>
//!           Error if there is none or several <br>
  Standard_EXPORT     Handle_IGESData_IGESEntity UniqueParent() const;
  //! Returns Location given by Transf in Directory Part (see above) <br>
//!           It must be considered for local definition : if the Entity is <br>
//!           set in a "Parent", that one can add its one Location, but this <br>
//!           is not taken in account here : see CompoundLocation for that. <br>
//!           If no Transf is defined, returns Identity <br>
//!           If Transf is itself compound, gives the final result <br>
  Standard_EXPORT     gp_GTrsf Location() const;
  //! Returns Location considered for Vectors, i.e. without its <br>
//!           Translation Part. As Location, it gives local definition. <br>
  Standard_EXPORT     gp_GTrsf VectorLocation() const;
  //! Returns Location by taking in account a Parent which has its <br>
//!           own Location : that one will be combined to that of <me> <br>
//!           The Parent is considered only if HasOneParent is True, <br>
//!           else it is ignored and CompoundLocation = Location <br>
  Standard_EXPORT     gp_GTrsf CompoundLocation() const;
  //! says if a Name is defined, as Short Label or as Name Property <br>
//!           (Property is looked first, else ShortLabel is considered) <br>
  Standard_EXPORT     Standard_Boolean HasName() const;
  //! returns Name value as a String (Property Name or ShortLabel) <br>
//!           if SubNumber is defined, it is concatenated after ShortLabel <br>
//!           as follows label(number). Ignored with a Property Name <br>
  Standard_EXPORT     Handle_TCollection_HAsciiString NameValue() const;
  //! Returns True if the Entity is defined with an Associativity <br>
//!           list, even empty (that is, file contains its length 0) <br>
//!           Else, the file contained NO idencation at all about this list. <br>
  Standard_EXPORT     Standard_Boolean ArePresentAssociativities() const;
  //! gives number of recorded associativities (0  no list defined) <br>
  Standard_EXPORT     Standard_Integer NbAssociativities() const;
  //! Returns the Associativity List under the form of an EntityIterator. <br>
  Standard_EXPORT     Interface_EntityIterator Associativities() const;
  //! gives how many Associativities have a given type <br>
  Standard_EXPORT     Standard_Integer NbTypedAssociativities(const Handle(Standard_Type)& atype) const;
  //! returns the Associativity of a given Type (if only one exists) <br>
//!           Error if none or more than one <br>
  Standard_EXPORT     Handle_IGESData_IGESEntity TypedAssociativity(const Handle(Standard_Type)& atype) const;
  //! Sets "me" in the Associativity list of another Entity <br>
  Standard_EXPORT     void Associate(const Handle(IGESData_IGESEntity)& ent) const;
  //! Resets "me" from the Associativity list of another Entity <br>
  Standard_EXPORT     void Dissociate(const Handle(IGESData_IGESEntity)& ent) const;
  //! Returns True if the Entity is defined with a Property list, <br>
//!           even empty (that is, file contains its length 0) <br>
//!           Else, the file contained NO idencation at all about this list <br>
  Standard_EXPORT     Standard_Boolean ArePresentProperties() const;
  //! Gives number of recorded properties (0  no list defined) <br>
  Standard_EXPORT     Standard_Integer NbProperties() const;
  //! Returns Property List under the form of an EntityIterator <br>
  Standard_EXPORT     Interface_EntityIterator Properties() const;
  //! gives how many Properties have a given type <br>
  Standard_EXPORT     Standard_Integer NbTypedProperties(const Handle(Standard_Type)& atype) const;
  //! returns the Property of a given Type <br>
//!           Error if none or more than one <br>
  Standard_EXPORT     Handle_IGESData_IGESEntity TypedProperty(const Handle(Standard_Type)& atype,const Standard_Integer anum = 0) const;
  //! Adds a Property in the list <br>
  Standard_EXPORT     void AddProperty(const Handle(IGESData_IGESEntity)& ent) ;
  //! Removes a Property from the list <br>
  Standard_EXPORT     void RemoveProperty(const Handle(IGESData_IGESEntity)& ent) ;
  //! computes and sets "true" line weight according IGES rules from <br>
//!           global data MaxLineWeight (maxv) and LineWeightGrad (gradw), <br>
//!           or sets it to defw (Default) if LineWeightNumber is null <br>
  Standard_EXPORT     void SetLineWeight(const Standard_Real defw,const Standard_Real maxw,const Standard_Integer gradw) ;


friend class IGESData_ReadWriteModule;
friend class IGESData_GeneralModule;
friend class IGESData_IGESReaderTool;
friend class IGESData_DirChecker;


  DEFINE_STANDARD_RTTI(IGESData_IGESEntity)

protected:

  //! prepares lists of optionnal data, set values to defaults <br>
  Standard_EXPORT   IGESData_IGESEntity();
  //! Initializes Type and Form Numbers to new values. Reserved for <br>
//!           special uses <br>
  Standard_EXPORT     void InitTypeAndForm(const Standard_Integer typenum,const Standard_Integer formnum) ;
  //! Loads a complete, already loaded, List of Asociativities <br>
//!           (used during Read or Copy Operations) <br>
  Standard_EXPORT     void LoadAssociativities(const Interface_EntityList& list) ;
  //! Loads a complete, already loaded, List of Properties <br>
//!           (used during Read or Copy Operations) <br>
  Standard_EXPORT     void LoadProperties(const Interface_EntityList& list) ;
  //! Removes all properties in once <br>
  Standard_EXPORT     void ClearProperties() ;



private: 

  //! Clears specific IGES data <br>
  Standard_EXPORT     void Clear() ;
  //! Adds an Associativity in the list (called by Associate only) <br>
  Standard_EXPORT     void AddAssociativity(const Handle(IGESData_IGESEntity)& ent) ;
  //! Removes an Associativity from the list (called by Dissociate) <br>
  Standard_EXPORT     void RemoveAssociativity(const Handle(IGESData_IGESEntity)& ent) ;
  //! Removes all associativities in once <br>
  Standard_EXPORT     void ClearAssociativities() ;

Standard_Integer theType;
Standard_Integer theForm;
Handle_IGESData_IGESEntity theStructure;
IGESData_DefSwitch theDefLineFont;
Handle_IGESData_IGESEntity theLineFont;
Standard_Integer theDefLevel;
Handle_IGESData_IGESEntity theLevelList;
Handle_IGESData_IGESEntity theView;
Handle_IGESData_IGESEntity theTransf;
Handle_IGESData_IGESEntity theLabDisplay;
Standard_Integer theStatusNum;
Standard_Integer theLWeightNum;
Standard_Real theLWeightVal;
IGESData_DefSwitch theDefColor;
Handle_IGESData_IGESEntity theColor;
Standard_Character theRes1[9];
Standard_Character theRes2[9];
Handle_TCollection_HAsciiString theShortLabel;
Standard_Integer theSubScriptN;
Interface_EntityList theAssocs;
Interface_EntityList theProps;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif
