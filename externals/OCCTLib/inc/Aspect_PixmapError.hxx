// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Aspect_PixmapError_HeaderFile
#define _Aspect_PixmapError_HeaderFile

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

#ifndef _Standard_OutOfRange_HeaderFile
#include <Standard_OutOfRange.hxx>
#endif
#ifndef _Handle_Aspect_PixmapError_HeaderFile
#include <Handle_Aspect_PixmapError.hxx>
#endif

#if !defined No_Exception && !defined No_Aspect_PixmapError
#define Aspect_PixmapError_Raise_if(CONDITION,MESSAGE) \
  if (CONDITION) Aspect_PixmapError::Raise(MESSAGE);
#else
#define Aspect_PixmapError_Raise_if(CONDITION,MESSAGE)
#endif

DEFINE_STANDARD_EXCEPTION(Aspect_PixmapError,Standard_OutOfRange)

#endif
