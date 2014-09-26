#ifndef MetaRenderController_h__
#define MetaRenderController_h__

#pragma 

#include "IRenderController.h"

#include <vector>

class	MetaRenderController : public IRenderController
{

public://IRenderController

	virtual bool	OnEvent(const irr::SEvent& event);

	virtual bool	OnPreEvent(const irr::SEvent& event);

	virtual bool	OnPostEvent(const irr::SEvent& event);

	virtual	bool	OnGUIEvent(const irr::SEvent& event);

	virtual	void	OnResize(const SRenderContext& rc);

	virtual bool	PreRender3D(const SRenderContext& rc);

	virtual void	PostRender3D(const SRenderContext& rc);

	virtual bool	PreRender2D(const SRenderContext& rc);

	virtual void	PostRender2D(const SRenderContext& rc);

public:

	virtual void	PushController(IRenderControllerSPtr controller);

private:

	std::vector<IRenderControllerSPtr>	ControllerList_;
};

typedef	std::shared_ptr<MetaRenderController>	MetaRenderControllerSPtr;

#endif // MetaRenderController_h__
