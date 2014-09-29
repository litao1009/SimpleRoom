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

	virtual	ENUM_OBOJECT_TYPE	GetType() const { return EOBT_Wall_Face; }
};


typedef	std::shared_ptr<CWallFaceODL>	CWallFaceODLSPtr;
typedef	std::weak_ptr<CWallFaceODL>		CWallFaceODLWPtr;
