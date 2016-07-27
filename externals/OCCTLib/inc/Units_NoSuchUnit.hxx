// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Units_NoSuchUnit_HeaderFile
#define _Units_NoSuchUnit_HeaderFile

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

#ifndef _Standard_NoSuchObject_HeaderFile
#include <Standard_NoSuchObject.hxx>
#endif
#ifndef _Handle_Units_NoSuchUnit_HeaderFile
#include <Handle_Units_NoSuchUnit.hxx>
#endif

#if !defined No_Exception && !defined No_Units_NoSuchUnit
#define Units_NoSuchUnit_Raise_if(CONDITION,MESSAGE) \
  if (CONDITION) Units_NoSuchUnit::Raise(MESSAGE);
#else
#define Units_NoSuchUnit_Raise_if(CONDITION,MESSAGE)
#endif

DEFINE_STANDARD_EXCEPTION(Units_NoSuchUnit,Standard_NoSuchObject)

#endif
