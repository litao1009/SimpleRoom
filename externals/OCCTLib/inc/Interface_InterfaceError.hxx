// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Interface_InterfaceError_HeaderFile
#define _Interface_InterfaceError_HeaderFile

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

#ifndef _Standard_Failure_HeaderFile
#include <Standard_Failure.hxx>
#endif
#ifndef _Handle_Interface_InterfaceError_HeaderFile
#include <Handle_Interface_InterfaceError.hxx>
#endif

#if !defined No_Exception && !defined No_Interface_InterfaceError
#define Interface_InterfaceError_Raise_if(CONDITION,MESSAGE) \
  if (CONDITION) Interface_InterfaceError::Raise(MESSAGE);
#else
#define Interface_InterfaceError_Raise_if(CONDITION,MESSAGE)
#endif

DEFINE_STANDARD_EXCEPTION(Interface_InterfaceError,Standard_Failure)

#endif
