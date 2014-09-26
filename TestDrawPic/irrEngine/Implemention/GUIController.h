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

	virtual	void	PostInit(SRenderContextSPtr sprc);

	virtual bool	OnPreEvent(const irr::SEvent& evt);

	virtual bool	OnGUIEvent(const irr::SEvent& event);

	virtual	void	OnResize(const SRenderContext& rc);

	virtual bool	PreRender3D(const SRenderContext& rc);
	virtual void	PostRender3D(const SRenderContext& rc);

	virtual bool	PreRender2D(const SRenderContext& rc);
	virtual void	PostRender2D(const SRenderContext& rc);

public:

	bool	IsMinimum() const { return Minimum_; }

	bool	IsCameraVisible() const { return CameraVisible_; }

	bool	IsTopCameraActive() const { return TopCameraActive_; }

	bool	IsMayaCameraActive() const { return MayaCameraActive_; }

	bool	IsFPSCameraActive() const { return FPSCameraActive_; }

private:

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

	bool					Minimum_;
	bool					CameraVisible_;
	bool					CameraRestore_;

	bool					TopCameraActive_;
	bool					MayaCameraActive_;
	bool					FPSCameraActive_;
};

typedef	std::shared_ptr<GUIController>	GUIControllerSPtr;

#endif // GUIController_h__
