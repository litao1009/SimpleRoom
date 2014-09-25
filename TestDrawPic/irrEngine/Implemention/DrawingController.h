#ifndef DrawingController_h__
#define DrawingController_h__

#pragma once

#include "irrEngine/IRenderController.h"

class	DrawingController : public IRenderController
{
public:

	DrawingController();
	~DrawingController();

public:

	virtual	void	PreInit(SRenderContextSPtr sprc);

	virtual	void	PostInit(SRenderContextSPtr sprc);

	virtual bool	OnEvent(const irr::SEvent& event);

	virtual	void	OnResize(const SRenderContext& rc);

	virtual bool	PreRender3D(const SRenderContext& rc);
	virtual void	PostRender3D(const SRenderContext& rc);

	virtual bool	PreRender2D(const SRenderContext& rc);
	virtual void	PostRender2D(const SRenderContext& rc);
};

#endif // DrawingController_h__
