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
	��Ⱦ���������ģ�һ�������豸��DC������һ��ʵ��
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

	//����������
	ISceneManagerSPtr		Smgr_;

	//��Ч������
	EffectHandlerSPtr		Effect_;

	//��������
	ICursorControlSPtr		CursorControl_;

	//GUI������
	IGUIEnvironmentSPtr		GUIEnv_;

	//��ʱ��
	ITimerSPtr				Timer_;

	//��ǰ���ļ�ѡ�ڵ�
	ISceneNodeSPtr			SelectedNode_;

	//������ɫ
	irr::video::SColor		BackGroundClr_;

	//�����
	irr::gui::ECURSOR_ICON	Icon_;

	//resizeͼ��
	IGUIImageSPtr			ResizeImage_;
	
	//��С��ͼ��
	IGUIImageSPtr			MinimumImage_;
	
	//�ָ�ͼ��
	IGUIImageSPtr			RestoreImage_;

	//�������ͼ��
	IGUIImageSPtr			ShowCameraImage_;
	ITextureSPtr			ShowCameraTex_;
	ITextureSPtr			HideCameraTex_;

	//FPS���ͼ��
	IGUIImageSPtr			FPSCameraImage_;
	ITextureSPtr			FPSActiveTex_;
	ITextureSPtr			FPSDActiveTex_;

	//Maya���ͼ��
	IGUIImageSPtr			MayaCameraImage_;
	ITextureSPtr			MayaActiveTex_;
	ITextureSPtr			MayaDActiveTex_;

	//TOP���ͼ��
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