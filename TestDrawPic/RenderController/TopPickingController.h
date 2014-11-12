#ifndef TopPickingController_h__
#define TopPickingController_h__

#pragma once

#include "irrEngine/IRenderController.h"
#include "ODL/BaseODLFwd.h"

#include <vector>

class	TopPickingController : public IRenderController
{
public:

	TopPickingController();
	~TopPickingController();

public:

	virtual	bool	OnPostEvent(const irr::SEvent& evt);

	virtual bool	PreRender3D();

	virtual void	PostRender3D();

public:

	void			SetRootODL(BaseODLWPtr baseODL) { RootODL_ = baseODL; }

private:

	BaseODLWPtr				RootODL_;

	irr::core::vector2di		CursorIPos_;
	irr::core::vector3df		SavePosition_;

	//扫略对象
	BaseODLWPtr				SweepingNode_;

	//选中对象
	BaseODLWPtr				PickingNode_;

	bool						Ignore_;

	bool						Picking_;
};

typedef	std::shared_ptr<TopPickingController>	TopPickingControllerSPtr;

#endif // TopPickingController_h__