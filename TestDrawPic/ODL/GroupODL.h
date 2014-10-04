#ifndef ObjGroupODL_h__
#define ObjGroupODL_h__

#pragma once

#include "BaseODL.h"

class CGroupODL : public CBaseODL
{
public:

	CGroupODL(void);
	~CGroupODL(void);

public:

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_GROUP; }
};

typedef	std::shared_ptr<CGroupODL>	CGroupODLSPtr;

#endif // ObjGroupODL_h__
