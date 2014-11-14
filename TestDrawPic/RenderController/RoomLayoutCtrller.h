#pragma once

#include "StatesController.h"
#include "ODL/GraphODLFwd.h"

enum class ERoomLayoutSatate
{
	ERS_BROWSE,
	ERS_DRAWING,
	ERS_PICTURE,
	ERS_COUNT
};

class	RoomLayoutCtrller : public StatesController<ERoomLayoutSatate>
{
public:

	RoomLayoutCtrller(const GraphODLWPtr& odl, const SRenderContextWPtr& rc);

	virtual	bool	OnPostEvent(const irr::SEvent& evt);

	virtual	void	Init();

private:

	GraphODLWPtr	GraphODL_;
};

typedef	std::shared_ptr<RoomLayoutCtrller>	RoomLayoutCtrllerSPtr;