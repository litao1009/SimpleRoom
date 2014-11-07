#ifndef irrTarget_h__
#define irrTarget_h__

#pragma once

#include "irrlicht.h"

#include "SRenderContextFwd.h"
#include "irrEngineFwd.h"
#include "Irrlicht/CIrrDeviceWin32.h"
#include "IRenderController.h"

typedef	std::shared_ptr<irr::scene::ISceneManager>				ISceneManagerSPtr;
typedef	std::shared_ptr<irr::scene::ISceneNode>					ISceneNodeSPtr;
typedef	std::shared_ptr<irr::CIrrDeviceWin32::CCursorControl>	ICursorControlSPtr;
typedef std::shared_ptr<irr::gui::IGUIEnvironment>				IGUIEnvironmentSPtr;
typedef	std::shared_ptr<irr::ITimer>							ITimerSPtr;


/*
	渲染场景上下文，一个绘制设备（DC）保存一个实例
*/
class	SRenderContext : public std::enable_shared_from_this<SRenderContext>
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	SRenderContext(void* Hwnd, irr::video::SColor clr);

	~SRenderContext();

public:

	void	Init();

	bool	PostEvent(irr::SEvent& evt);

	void	Render();

	void	UpdateScene();

	void	OnResize(bool force = false);

	void	SetOnResize();

	void	AddShadowToNode(irr::scene::ISceneNode* node);

	void	RemoveShadowFromNode(irr::scene::ISceneNode* node);

	void	AddNodeToDepthPass(irr::scene::ISceneNode* node);

	void	RemoveNodeFromDepthPass(irr::scene::ISceneNode* node);

	void	PushController(IRenderControllerSPtr controller);

	void	EnableDepthPass(bool val);

	void*	GetHandle();

public:

	//场景管理器
	ISceneManagerSPtr		Smgr_;

	//鼠标控制器
	ICursorControlSPtr		CursorControl_;

	//GUI控制器
	IGUIEnvironmentSPtr		GUIEnv_;

	//计时器
	ITimerSPtr				Timer_;

	//背景颜色
	irr::video::SColor		BackGroundClr_;

private:

	irr::video::SExposedVideoData	VideoData_;
};


#endif // irrTarget_h__