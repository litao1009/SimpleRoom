#ifndef BaseODL_h__
#define BaseODL_h__

#pragma once

#include "PublicDefine.h"
#include "ODLTools.h"
#include "BaseODLFwd.h"

#include "BLL/BaseBLL.h"

#include "TSceneNode.h"

class CBaseODL : public TSceneNode<CBaseODL>
{
public:

	CBaseODL(void);
	virtual ~CBaseODL(void);

	virtual	ENUM_OBOJECT_TYPE	GetType() const { return EOBT_Base; }

public:
		
};

#endif // BaseODL_h__
