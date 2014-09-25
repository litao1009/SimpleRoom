#ifndef irrTarget_h__
#define irrTarget_h__

#pragma once

#include "irrlicht.h"

#include "SRenderContextFwd.h"
#include "irrEngineFwd.h"
#include "Irrlicht/CIrrDeviceWin32.h"
#include "IRenderController.h"
#include "XEffect/XEffects.h"

typedef	std::shared_ptr<irr::scene::ISceneManager>				ISceneManagerSPtr;
typedef	std::shared_ptr<irr::scene::ISceneNode>					ISceneNodeSPtr;
typedef	std::shared_ptr<irr::scene::ICameraSceneNode>			ICameraSceneNodeSPtr;
typedef	std::shared_ptr<irr::CIrrDeviceWin32::CCursorControl>	ICursorControlSPtr;
typedef std::shared_ptr<irr::gui::IGUIEnvironment>				IGUIEnvironmentSPtr;
typedef	std::shared_ptr<irr::ITimer>							ITimerSPtr;
typedef	std::shared_ptr<irr::video::ITexture>					ITextureSPtr;
typedef	std::shared_ptr<EffectHandler>							EffectHandlerSPtr;


/*
	渲染场景上下文，一个绘制设备（DC）保存一个实例
*/
class	SRenderContext
{
	friend class IrrEngine;
	friend class irrReceiver;

	typedef	std::shared_ptr<irr::IEventReceiver>	IEventReceiverSPtr;
	typedef	std::shared_ptr<irr::gui::IGUIImage>	IGUIImageSPtr;

	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	SRenderContext(void* Hwnd, irr::video::SColor clr);

	~SRenderContext();

public:

	bool	PostEvent(irr::SEvent& evt);

	void	Render();

	void	UpdateScene();

	void	OnResize(bool force = false);

	void	SetOnResize();

	void	SetMinimum(bool val);

	bool	IsCameraVisible() const
	{
		return CameraVisible_;
	}

	void	SetCameraVisible(bool val)
	{
		CameraVisible_ = val;
		if ( CameraVisible_ )
		{
			ShowCameraImage_->setImage(ShowCameraTex_.get());
		}
		else
		{
			ShowCameraImage_->setImage(HideCameraTex_.get());
		}
	}

	bool	IsFPSCameraActive() const
	{
		return Smgr_->getActiveCamera() == FPSCamera_.get();
	}

	bool	IsMayaCameraActive() const
	{
		return Smgr_->getActiveCamera() == MayaCamera_.get();
	}

	void	SetRenderController(IRenderControllerSPtr controler);

public:

	IRenderControllerSPtr	RenderController_;

	//场景管理器
	ISceneManagerSPtr		Smgr_;

	//特效管理器
	EffectHandlerSPtr		Effect_;

	//鼠标控制器
	ICursorControlSPtr		CursorControl_;

	//GUI控制器
	IGUIEnvironmentSPtr		GUIEnv_;

	//计时器
	ITimerSPtr				Timer_;

	//当前鼠标的拣选节点
	ISceneNodeSPtr			SelectedNode_;

	//背景颜色
	irr::video::SColor		BackGroundClr_;

	//鼠标光标
	irr::gui::ECURSOR_ICON	Icon_;

	//resize图标
	IGUIImageSPtr			ResizeImage_;
	
	//最小化图标
	IGUIImageSPtr			MinimumImage_;
	
	//恢复图标
	IGUIImageSPtr			RestoreImage_;

	//相机显隐图标
	IGUIImageSPtr			ShowCameraImage_;
	ITextureSPtr			ShowCameraTex_;
	ITextureSPtr			HideCameraTex_;

	//FPS相机图标
	IGUIImageSPtr			FPSCameraImage_;
	ITextureSPtr			FPSActiveTex_;
	ITextureSPtr			FPSDActiveTex_;

	//Maya相机图标
	IGUIImageSPtr			MayaCameraImage_;
	ITextureSPtr			MayaActiveTex_;
	ITextureSPtr			MayaDActiveTex_;

	//TOP相机图标
	IGUIImageSPtr			TopCameraImage_;
	ITextureSPtr			TopActiveTex_;
	ITextureSPtr			TopDActiveTex_;

private:

	void	SwitchToFPSCamera();

	void	SwitchToMayaCamera();

	void	SwitchToTopCamera();

private:

	ICameraSceneNodeSPtr			FPSCamera_;
	ICameraSceneNodeSPtr			MayaCamera_;
	ICameraSceneNodeSPtr			TopCamera_;

	bool							OnResize_;
	bool							Minimum_;
	bool							CameraVisible_;
	bool							CameraRestore_;

	irr::video::SExposedVideoData	VideoData_;
};


#endif // irrTarget_h__