#pragma once

#include "StatesController.h"

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

	virtual	bool	OnPostEvent(const irr::SEvent& evt);
};

typedef	std::shared_ptr<RoomLayoutCtrller>	RoomLayoutCtrllerSPtr;