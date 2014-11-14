#pragma once

#include "irrEngine/IRenderController.h"
#include "ODL/GraphODLFwd.h"

class	RoomLayoutPictureCtrller : public IRenderController
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	RoomLayoutPictureCtrller(const GraphODLWPtr& odl, const SRenderContextWPtr& rc);
	~RoomLayoutPictureCtrller();

public://IRenderController

	virtual	void	Init();

	virtual	bool	PreRender3D();

	virtual void	PostRender3D();

	virtual bool	OnPostEvent(const irr::SEvent& event);

private:

	GraphODLWPtr	BaseODL_;

};

typedef	std::shared_ptr<RoomLayoutPictureCtrller>	RoomPictureCtrllerSPtr;