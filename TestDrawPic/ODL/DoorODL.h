#ifndef DoorODL_h__
#define DoorODL_h__

#pragma once

#include "BaseODL.h"

class CDoorODL : public CBaseODL
{
public:

	CDoorODL();
	~CDoorODL();

public:

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_DOOR; }
};

typedef	std::shared_ptr<CDoorODL>	CDoorODLSPtr;
typedef	std::weak_ptr<CDoorODL>		CDoorODLWPtr;

#endif // DoorODL_h__
