#pragma once

#include "irrEngine/IRenderController.h"

class	RoomPictureCtrller : public IRenderController
{
public:

	RoomPictureCtrller();

public://IRenderController

	virtual	bool	PreRender3D();

	virtual void	PostRender3D();

	virtual bool	OnPostEvent(const irr::SEvent& event);

private:

};

typedef	std::shared_ptr<RoomPictureCtrller>	RoomPictureCtrllerSPtr;