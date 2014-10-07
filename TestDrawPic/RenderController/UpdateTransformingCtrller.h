#ifndef UpdateTransformingCtrller_h__
#define UpdateTransformingCtrller_h__

#pragma once

#include "irrEngine/IRenderController.h"
#include "ODL/BaseODLFwd.h"

class	UpdateTransformingCtrller : public IRenderController
{
public:

	UpdateTransformingCtrller();
	~UpdateTransformingCtrller();

public://IRenderController

	virtual bool	PreRender3D();

public:

	void	SetRootODL(const CBaseODLWPtr& rootODL) { RootODL_ = rootODL; }

private:

	CBaseODLWPtr		RootODL_;
};

typedef	std::shared_ptr<UpdateTransformingCtrller>	UpdateTransformingCtrllerSPtr;

#endif // UpdateTransformingCtrller_h__
