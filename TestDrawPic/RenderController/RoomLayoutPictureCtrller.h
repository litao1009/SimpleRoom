#pragma once

#include "irrEngine/IRenderController.h"
#include "ODL/BaseODLFwd.h"

class	RoomLayoutPictureCtrller : public IRenderController
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	RoomLayoutPictureCtrller();
	~RoomLayoutPictureCtrller();

public://IRenderController

	virtual	void	Init();

	virtual	bool	PreRender3D();

	virtual void	PostRender3D();

	virtual bool	OnPostEvent(const irr::SEvent& event);

public:

	void	SetRootODL(const BaseODLWPtr& odl) { BaseODL_ = odl; }

private:

	BaseODLWPtr	BaseODL_;

};

typedef	std::shared_ptr<RoomLayoutPictureCtrller>	RoomPictureCtrllerSPtr;