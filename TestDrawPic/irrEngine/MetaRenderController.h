#ifndef MetaRenderController_h__
#define MetaRenderController_h__

#pragma 

#include "IRenderController.h"

#include <vector>

class	MetaRenderController : public IRenderController
{
public:

	MetaRenderController();

public://IRenderController

	virtual bool	OnEvent(const irr::SEvent& event);

	virtual bool	OnPreEvent(const irr::SEvent& event);

	virtual bool	OnPostEvent(const irr::SEvent& event);

	virtual	bool	OnGUIEvent(const irr::SEvent& event);

	virtual	void	OnResize();

	virtual bool	PreRender3D();

	virtual void	PostRender3D();

	virtual bool	PreRender2D();

	virtual void	PostRender2D();

public:

	virtual void	PushController(IRenderControllerSPtr controller);

private:

	std::vector<IRenderControllerSPtr>	ControllerList_;
};

typedef	std::shared_ptr<MetaRenderController>	MetaRenderControllerSPtr;

#endif // MetaRenderController_h__
