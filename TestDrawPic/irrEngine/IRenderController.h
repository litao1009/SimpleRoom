#ifndef SRenderController_h__
#define SRenderController_h__

#pragma once

#include "SRenderContextFwd.h"

#include "IEventReceiver.h"
#include "ICameraSceneNode.h"

#include <memory>
#include <string>

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

public://IEventReceiver 

	//No need to override this function normaly
	virtual bool	OnEvent(const irr::SEvent& event) { return OnGUIEvent(event); }

public:

	virtual	void	Init() {}

	virtual	bool	OnGUIEvent(const irr::SEvent& evt) { return false; }

	virtual bool	OnPreEvent(const irr::SEvent& evt) { return false; }

	virtual	bool	OnPostEvent(const irr::SEvent& evt) { return false; }

	virtual	void	OnResize() {}

	virtual bool	PreRender3D() { return false; }

	virtual void	PostRender3D() {}

	virtual bool	PreRender2D() { return false; }

	virtual void	PostRender2D() {}

public:

	bool					IsEnable() const { return Enable_; }

	void					SetEnable(bool val) { Enable_ = val; }
	
	SRenderContextSPtr		GetRenderContextSPtr() const { return RC_.lock(); }

	void					SetRenderContextWPtr(const SRenderContextWPtr& rc) { RC_ = rc; }

	void					SetName(std::string str) { Name_ = str; }

	const std::string&		GetName() const { return Name_; }

private:

	std::string				Name_;
	bool					Enable_;
	SRenderContextWPtr		RC_;
};

#endif // SRenderController_h__
