// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _SelectMgr_CompositionFilter_HeaderFile
#define _SelectMgr_CompositionFilter_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Handle_SelectMgr_CompositionFilter_HeaderFile
#include <Handle_SelectMgr_CompositionFilter.hxx>
#endif

#ifndef _SelectMgr_ListOfFilter_HeaderFile
#include <SelectMgr_ListOfFilter.hxx>
#endif
#ifndef _SelectMgr_Filter_HeaderFile
#include <SelectMgr_Filter.hxx>
#endif
#ifndef _Handle_SelectMgr_Filter_HeaderFile
#include <Handle_SelectMgr_Filter.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _TopAbs_ShapeEnum_HeaderFile
#include <TopAbs_ShapeEnum.hxx>
#endif
class SelectMgr_Filter;
class SelectMgr_ListOfFilter;


//! A framework to define a compound filter composed of <br>
//! two or more simple filters. <br>
class SelectMgr_CompositionFilter : public SelectMgr_Filter {

public:

  //! Adds the filter afilter to a filter object created by a <br>
//! filter class inheriting this framework. <br>
  Standard_EXPORT     void Add(const Handle(SelectMgr_Filter)& afilter) ;
  //! Removes the filter aFilter from this framework. <br>
  Standard_EXPORT     void Remove(const Handle(SelectMgr_Filter)& aFilter) ;
  //! Returns true if this framework is empty. <br>
  Standard_EXPORT     Standard_Boolean IsEmpty() const;
  //! Returns true if the filter aFilter is in this framework. <br>
  Standard_EXPORT     Standard_Boolean IsIn(const Handle(SelectMgr_Filter)& aFilter) const;
  //! Returns the list of stored filters from this framework. <br>
       const SelectMgr_ListOfFilter& StoredFilters() const;
  //! Clears the filters used in this framework. <br>
  Standard_EXPORT     void Clear() ;
  
  Standard_EXPORT   virtual  Standard_Boolean ActsOn(const TopAbs_ShapeEnum aStandardMode) const;




  DEFINE_STANDARD_RTTI(SelectMgr_CompositionFilter)

protected:


SelectMgr_ListOfFilter myFilters;


private: 




};


#include <SelectMgr_CompositionFilter.lxx>



// other Inline functions and methods (like "C++: function call" methods)


#endif
