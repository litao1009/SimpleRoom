#ifndef DrawingLineCtrller_h__
#define DrawingLineCtrller_h__

#pragma once

#include "irrEngine/IRenderController.h"

#include "IMeshBuffer.h"

class	DrawingLineCtrller : public IRenderController
{
public:

	enum EDrawingLineState
	{
		EDLS_BEGIN,
		EDLS_DRAWING,
		EDLS_FINISH,
		EDLS_COUNT
	};

public:

	DrawingLineCtrller();
	~DrawingLineCtrller();

public://IRenderController

	virtual	void	PreInit(SRenderContextSPtr sprc);

	virtual	void	PostInit(SRenderContextSPtr sprc);

	virtual bool	OnPostEvent(const irr::SEvent& event);

	virtual	void	OnResize(const SRenderContext& rc);

	virtual bool	PreRender3D(const SRenderContext& rc);
	virtual void	PostRender3D(const SRenderContext& rc);

	virtual bool	PreRender2D(const SRenderContext& rc);
	virtual void	PostRender2D(const SRenderContext& rc);

public:

	EDrawingLineState	GetState() const { return State_; }

	void				Reset();

	irr::scene::IMeshBuffer*	GetMeshBuffer() const;

private:

	irr::scene::IMeshBuffer*	Pnts_;
	irr::core::vector2di		CurrentPos_;
	irr::video::SColor			PntColor_;
	EDrawingLineState			State_;

};
#endif // DrawingLineCtrller_h__
