#pragma once

#include "irrEngine/IRenderController.h"
#include "ODL/BaseODLFwd.h"

class	IRoomLayoutODLBaseCtrller : public IRenderController
{
public:

	IRoomLayoutODLBaseCtrller(const SRenderContextWPtr& rc):IRenderController(rc) {}

public:

	void			SetPickingODL(const BaseODLSPtr& odl) { PickingODL_ = odl; }

	BaseODLWPtr&	GetPickingODL() { return PickingODL_; }

	virtual	bool	Valid() = 0;

private:

	BaseODLWPtr	PickingODL_;
};

typedef	std::shared_ptr<IRoomLayoutODLBaseCtrller>	IRoomLayoutODLBaseCtrllerSPtr;