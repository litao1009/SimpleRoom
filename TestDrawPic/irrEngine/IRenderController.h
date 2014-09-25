#ifndef SRenderController_h__
#define SRenderController_h__

#pragma once

#include "SRenderContextFwd.h"

#include "IEventReceiver.h"
#include "ICameraSceneNode.h"

#include <memory>

typedef	std::shared_ptr<irr::scene::ICameraSceneNode>		ICameraSceneNodeSPtr;

class	IRenderController;
typedef	std::shared_ptr<IRenderController>	IRenderControllerSPtr;
typedef	std::weak_ptr<IRenderController>	IRenderControllerWPtr;

class	IRenderController : public irr::IEventReceiver, public std::enable_shared_from_this<IRenderController>
{
public:

	IRenderController()
	{
		Enable_ = true;
	}

public:

	virtual	void	PreInit(SRenderContextSPtr sprc) {}

	virtual	void	PostInit(SRenderContextSPtr sprc) { assert(sprc); RC_ = sprc; }

	virtual bool	OnEvent(const irr::SEvent& event) { return false; }

	virtual	void	OnResize(const SRenderContext& rc) {}

public:

	virtual bool	PreRender3D(const SRenderContext& rc) { return true; }
	virtual void	PostRender3D(const SRenderContext& rc) {}

	virtual bool	PreRender2D(const SRenderContext& rc) { return true; }
	virtual void	PostRender2D(const SRenderContext& rc) {}

public:

	bool	IsEnable() const { return Enable_; }

	void	SetEnable(bool val) { Enable_ = val; }

	IRenderControllerWPtr	GetParent() const { return Parent_; }

	void	SetParent(IRenderControllerWPtr parent) { Parent_ = parent; }

protected:

	SRenderContextWPtr	RC_;

private:

	bool					Enable_;
	IRenderControllerWPtr	Parent_;
};

#endif // SRenderController_h__
