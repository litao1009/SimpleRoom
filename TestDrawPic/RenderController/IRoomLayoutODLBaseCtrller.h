#pragma once

#include "irrEngine/IRenderController.h"
#include "ODL/BaseODLFwd.h"

class	IRoomLayoutODLBaseCtrller : public IRenderController
{
public:

	IRoomLayoutODLBaseCtrller(const SRenderContextWPtr& rc):IRenderController(rc) {}

public:

	void	SetActiveODL(const BaseODLSPtr& odl)
	{
		ActiveODL_ = odl;
	}

	BaseODLWPtr&	GetActiveODL() { return ActiveODL_; }

	void	ResetActiveODL()
	{
		ActiveODL_.reset();
	}

private:

	BaseODLWPtr	ActiveODL_;
};

typedef	std::shared_ptr<IRoomLayoutODLBaseCtrller>	IRoomLayoutODLBaseCtrllerSPtr;