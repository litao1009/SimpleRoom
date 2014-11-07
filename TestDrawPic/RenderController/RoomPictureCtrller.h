#pragma once

#include "irrEngine/IRenderController.h"
#include "ODL/BaseODLFwd.h"

class	RoomPictureCtrller : public IRenderController
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	RoomPictureCtrller();
	~RoomPictureCtrller();

public://IRenderController

	virtual	void	Init();

	virtual	bool	PreRender3D();

	virtual void	PostRender3D();

	virtual bool	OnPostEvent(const irr::SEvent& event);

public:

	void	SetRootODL(const CBaseODLWPtr& odl) { BaseODL_ = odl; }

private:

	CBaseODLWPtr	BaseODL_;

};

typedef	std::shared_ptr<RoomPictureCtrller>	RoomPictureCtrllerSPtr;