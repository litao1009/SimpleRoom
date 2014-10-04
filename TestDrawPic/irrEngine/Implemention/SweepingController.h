#ifndef SweepingController_h__
#define SweepingController_h__

#pragma once

#include "irrEngine/IRenderController.h"
#include "ODL/BaseODLFwd.h"

#include <vector>

class	SweepingController : public IRenderController
{
public:

	SweepingController();
	~SweepingController();

public:

	virtual	bool	OnPostEvent(const irr::SEvent& evt);

	virtual bool	PreRender3D(const SRenderContext& rc);

	virtual void	PostRender3D(const SRenderContext& rc);

public:

	void			SetRootODL(CBaseODLWPtr baseODL) { RootODL_ = baseODL; }

private:

	CBaseODLWPtr				RootODL_;
	irr::core::vector2di		CursorIPos_;
	CBaseODLWPtr				SweepingNode_;
};

typedef	std::shared_ptr<SweepingController>	SweepingControllerSPtr;

#endif // SweepingController_h__