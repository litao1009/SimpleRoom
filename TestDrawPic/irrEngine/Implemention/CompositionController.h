#ifndef CompositionController_h__
#define CompositionController_h__

#pragma once

#include "irrEngine/IRenderController.h"

class	CompositionController : public IRenderController
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	CompositionController();

	~CompositionController();

public:

	virtual	void	PreInit(SRenderContextSPtr sprc);

	virtual	void	OnResize(const SRenderContext& rc);

public:

	virtual bool	PreRender3D(const SRenderContext& rc);
	virtual void	PostRender3D(const SRenderContext& rc);

	virtual bool	PreRender2D(const SRenderContext& rc);
	virtual void	PostRender2D(const SRenderContext& rc);

private:

	
};
#endif // CompositionController_h__
