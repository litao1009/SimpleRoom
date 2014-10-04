#pragma once

#include "BaseODL.h"

#include <vector>
#include <map>

class CWallFaceODL : public CBaseODL
{
public:

	CWallFaceODL();
	~CWallFaceODL(void);

public:

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_WALLFACE; }
};


typedef	std::shared_ptr<CWallFaceODL>	CWallFaceODLSPtr;
typedef	std::weak_ptr<CWallFaceODL>		CWallFaceODLWPtr;
