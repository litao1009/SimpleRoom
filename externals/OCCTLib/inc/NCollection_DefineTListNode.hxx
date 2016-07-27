// Created on: 2002-04-23
// Created by: Alexander KARTOMIN (akm)
// Copyright (c) 2002-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

// Purpose:     Abstract list node class. Used by BaseList
// Remark:      Internal class

#ifndef NCollection_DefineTListNode_HeaderFile
#define NCollection_DefineTListNode_HeaderFile

#include <NCollection_TListNode.hxx>

// ******************************* Class defining list node - for internal use
#define DEFINE_TLISTNODE(_ClassName_, _BaseCollection_, TheItemType)           \
typedef NCollection_TListNode<TheItemType > _ClassName_;

#endif
