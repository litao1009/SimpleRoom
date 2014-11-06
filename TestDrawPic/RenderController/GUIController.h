#ifndef GUIController_h__
#define GUIController_h__

#pragma once

#include "irrEngine/IRenderController.h"
#include "irrEngine/IrrExtension/GUIImageButton.h"
#include "ICursorControl.h"
#include "IGUIImage.h"
#include "ITexture.h"

typedef	std::shared_ptr<CGUIImageButton>		IGUIImageSPtr;
typedef	std::shared_ptr<irr::video::ITexture>	ITextureSPtr;

class	GUIController : public IRenderController
{
public:

	GUIController();

public://IRenderController

	virtual	void	Init();

	virtual bool	OnPreEvent(const irr::SEvent& evt);

	virtual bool	OnGUIEvent(const irr::SEvent& event);

	virtual	void	OnResize();

	virtual bool	PreRender3D();
	virtual void	PostRender3D();

	virtual bool	PreRender2D();
	virtual void	PostRender2D();

public:

	bool	IsCameraVisible() const { return CameraVisible_; }

	bool	IsTopCameraActive() const { return TopCameraActive_; }

	bool	IsMayaCameraActive() const { return MayaCameraActive_; }

	bool	IsFPSCameraActive() const { return FPSCameraActive_; }

private:

	//�����
	irr::gui::ECURSOR_ICON	Icon_;

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

	//Realͼ��
	IGUIImageSPtr			RealImage_;
	ITextureSPtr			RealActiveTex_;
	ITextureSPtr			RealDActiveTex_;

	bool					Minimum_;
	bool					CameraVisible_;
	bool					CameraRestore_;

	bool					TopCameraActive_;
	bool					MayaCameraActive_;
	bool					FPSCameraActive_;
	
	bool					RealWorld_;
};

typedef	std::shared_ptr<GUIController>	GUIControllerSPtr;

#endif // GUIController_h__
