#include "stdafx.h"

#include "GUIController.h"
#include "irrlicht.h"
#include "IrrEngine/SRenderContext.h"
#include "EventUserType.h"

#include <functional>

static const auto ImageBorderX = 3;
static const auto ImageBorderY = 3;

using namespace irr;
using namespace gui;

CGUIImageButton*	AddImage( IGUIEnvironment* env, video::ITexture* tex, core::position2di pos, bool useAlphaChannel=true, IGUIElement* parent=0, irr::s32 id=-1 )
{
	auto sz = core::dimension2di(tex->getOriginalSize());

	auto img = new CGUIImageButton(env, env->getRootGUIElement(), -1, core::recti(irr::core::vector2di(0,1), sz));
	img->setUseAlphaChannel(true);
	img->setImage(tex);
	img->drop();

	return img;
}


GUIController::GUIController()
{
	Icon_ = irr::gui::ECI_NORMAL;
	Minimum_ = false;
	CameraVisible_ = true;
	CameraRestore_ = true;
	TopCameraActive_ = true;
	MayaCameraActive_ = false;
	FPSCameraActive_ = false;

	SetName("GUIController");
}


void GUIController::PostInit( SRenderContextSPtr sprc )
{
	IRenderController::PostInit(sprc);

	auto driver = sprc->Smgr_->getVideoDriver();

	sprc->SetOnResize();

	{//resize
		auto tex = driver->getTexture("../Data/Resource/3D/resize.png");
		auto guiImage = AddImage(sprc->GUIEnv_.get(), tex, core::vector2di(0,1));
		ResizeImage_.reset(guiImage, std::bind([](IGUIElement*){}, std::placeholders::_1));
		ResizeImage_->SetHoldPressed(true);
	}

	{//minimum
		auto tex = driver->getTexture("../Data/Resource/3D/minimum.png");
		auto guiImage = AddImage(sprc->GUIEnv_.get(), tex, core::vector2di(0,1));
		MinimumImage_.reset(guiImage, std::bind([](IGUIElement*){}, std::placeholders::_1));
	}

	{//restore
		auto tex = driver->getTexture("../Data/Resource/3D/restore.png");
		auto guiImage = AddImage(sprc->GUIEnv_.get(), tex, core::vector2di(0,1));
		RestoreImage_.reset(guiImage, std::bind([](IGUIElement*){}, std::placeholders::_1));
		RestoreImage_->setVisible(false);
	}

	{//camera
		auto showTex = driver->getTexture("../Data/Resource/3D/camera.png");
		auto hideTex = driver->getTexture("../Data/Resource/3D/camera_d.png");

		auto guiImage = AddImage(sprc->GUIEnv_.get(), showTex, core::vector2di(0,1));

		ShowCameraImage_.reset(guiImage, std::bind([](IGUIElement*){}, std::placeholders::_1));

		ShowCameraTex_.reset(showTex, [](video::ITexture*){});
		HideCameraTex_.reset(hideTex, [](video::ITexture*){});
	}

	{//camera type
		auto fpsActTex = driver->getTexture("../Data/Resource/3D/fps_cam_a.png");
		auto fpsDActTex = driver->getTexture("../Data/Resource/3D/fps_cam_d.png");
		auto mayaActTex = driver->getTexture("../Data/Resource/3D/maya_cam_a.png");
		auto mayaDActTex = driver->getTexture("../Data/Resource/3D/maya_cam_d.png");
		auto topActTex = driver->getTexture("../Data/Resource/3D/top_cam_a.png");
		auto topDActTex = driver->getTexture("../Data/Resource/3D/top_cam_d.png");

		auto fpsImage = AddImage(sprc->GUIEnv_.get(), fpsDActTex, core::vector2di(0,1));
		auto mayaImage = AddImage(sprc->GUIEnv_.get(), mayaDActTex, core::vector2di(0,1));
		auto topImage = AddImage(sprc->GUIEnv_.get(), topActTex, core::vector2di(0,1));

		FPSCameraImage_.reset(fpsImage, [](IGUIImage*){});
		MayaCameraImage_.reset(mayaImage, [](IGUIImage*){});
		TopCameraImage_.reset(topImage, [](IGUIImage*){});

		MayaActiveTex_.reset(mayaActTex, [](video::ITexture*){});
		MayaDActiveTex_.reset(mayaDActTex, [](video::ITexture*){});
		FPSActiveTex_.reset(fpsActTex, [](video::ITexture*){});
		FPSDActiveTex_.reset(fpsDActTex, [](video::ITexture*){});
		TopActiveTex_.reset(topActTex, [](video::ITexture*){});
		TopDActiveTex_.reset(topDActTex, [](video::ITexture*){});
	}
}

bool GUIController::OnPreEvent( const irr::SEvent& event )
{
	if ( ResizeImage_->GetState() == CGUIImageButton::EIS_PRESSED )
	{
		if ( event.EventType == EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_MOUSE_MOVED)
		{
			SEvent evt;
			evt.EventType = EET_USER_EVENT;
			evt.UserEvent.UserData1 = EUT_RESIZE_CHANGE_DURING;
			auto spParent = GetParentSPtr();
			assert(spParent);
			spParent->OnPreEvent(evt);

			return true;
		}
	}

	auto sprc = GetRenderContextSPtr();
	if ( sprc )
	{
		sprc->CursorControl_->setActiveIcon(Icon_);
	}

	return false;
}

bool GUIController::OnGUIEvent( const irr::SEvent& event )
{
	if ( !IsEnable() )
	{
		return false;
	}

	auto ret = false;

	if ( event.EventType == EET_GUI_EVENT )
	{
		//Reisze
		if ( event.GUIEvent.Caller == ResizeImage_.get() )
		{
			if ( event.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_NORMAL )
			{
				Icon_ = ECI_NORMAL;
			}

			if ( event.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_HOVERD )
			{
				Icon_ = ECI_SIZENESW;
			}

			if ( event.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_PRESSED )
			{
				SEvent evt;
				evt.EventType = EET_USER_EVENT;
				evt.UserEvent.UserData1 = EUT_RESIZE_CHANGE_BEGIN;
				auto spParent = GetParentSPtr();
				assert(spParent);
				spParent->OnPreEvent(evt);

				Icon_ = ECI_SIZENESW;

				ret = true;
			}

			if ( event.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_LEFTUP )
			{
				SEvent evt;
				evt.EventType = EET_USER_EVENT;
				evt.UserEvent.UserData1 = EUT_RESIZE_CHANGE_END;
				auto spParent = GetParentSPtr();
				assert(spParent);
				spParent->OnPreEvent(evt);

				Icon_ = ECI_SIZENESW;

				ret = true;
			}
		}

		//Mininum
		if ( event.GUIEvent.Caller == MinimumImage_.get() )
		{
			if ( event.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_PRESSED )
			{
				Minimum_ = true;
				CameraRestore_ = CameraVisible_;
				CameraVisible_ = false;

				SEvent evt;
				evt.EventType = EET_USER_EVENT;
				evt.UserEvent.UserData1 = EUT_RESIZE_MINIMUM;
				auto spParent = GetParentSPtr();
				assert(spParent);
				spParent->OnPreEvent(evt);

				ret = true;
			}
		}

		//Restore
		if ( event.GUIEvent.Caller == RestoreImage_.get() )
		{
			if ( event.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_PRESSED )
			{
				Minimum_ = false;
				CameraVisible_ = CameraRestore_;

				SEvent evt;
				evt.EventType = EET_USER_EVENT;
				evt.UserEvent.UserData1 = EUT_RESIZE_RESTORE;
				auto spParent = GetParentWPtr().lock();
				assert(spParent);
				spParent->OnPreEvent(evt);

				ret = true;
			}
		}

		//CameraVisible
		if ( event.GUIEvent.Caller == this->ShowCameraImage_.get() )
		{
			if ( event.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_PRESSED )
			{
				CameraVisible_ = !CameraVisible_;
				CameraRestore_ = CameraVisible_;

				ret = true;
			}
		}

		//Camera
		if ( event.GUIEvent.Caller == this->TopCameraImage_.get() )
		{
			if ( event.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_PRESSED )
			{
				TopCameraImage_->setImage(TopActiveTex_.get());
				FPSCameraImage_->setImage(FPSDActiveTex_.get());
				MayaCameraImage_->setImage(MayaDActiveTex_.get());
				TopCameraActive_ = true;
				MayaCameraActive_ = false;
				FPSCameraActive_ = false;

				ret = true;
			}
		}

		if ( event.GUIEvent.Caller == this->FPSCameraImage_.get() )
		{
			if ( event.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_PRESSED )
			{
				TopCameraImage_->setImage(TopDActiveTex_.get());
				FPSCameraImage_->setImage(FPSActiveTex_.get());
				MayaCameraImage_->setImage(MayaDActiveTex_.get());
				TopCameraActive_ = false;
				MayaCameraActive_ = false;
				FPSCameraActive_ = true;

				ret = true;
			}
		}

		if ( event.GUIEvent.Caller == this->MayaCameraImage_.get() )
		{
			if ( event.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_PRESSED )
			{
				TopCameraImage_->setImage(TopDActiveTex_.get());
				FPSCameraImage_->setImage(FPSDActiveTex_.get());
				MayaCameraImage_->setImage(MayaActiveTex_.get());
				TopCameraActive_ = false;
				MayaCameraActive_ = true;
				FPSCameraActive_ = false;

				ret = true;
			}
		}
	}

	auto sprc = GetRenderContextSPtr();
	if ( sprc )
	{
		sprc->CursorControl_->setActiveIcon(Icon_);
	}

	return ret;
}

void GUIController::OnResize(const SRenderContext& rc)
{
	auto renderSize = rc.Smgr_->getVideoDriver()->getScreenSize();

	{
		auto texSize = ResizeImage_->getImage()->getSize();
		irr::core::position2di pos;
		pos.X = renderSize.Width-texSize.Width-ImageBorderX;
		pos.Y = ImageBorderY;
		ResizeImage_->setRelativePosition(pos);
	}

	{
		auto texSize = ShowCameraImage_->getImage()->getSize();
		irr::core::position2di pos;
		if ( renderSize.Width > texSize.Width )
		{
			pos.X = (renderSize.Width-texSize.Width) / 2;
		}
		else
		{
			pos.X = 0;
		}
		pos.Y = ImageBorderY;
		ShowCameraImage_->setRelativePosition(pos);
	}

	{
		auto texSize = MayaActiveTex_->getSize();
		FPSCameraImage_->setRelativePosition(irr::core::vector2di(renderSize.Width-texSize.Width-ImageBorderX,renderSize.Height-texSize.Height-ImageBorderY));
		MayaCameraImage_->setRelativePosition(irr::core::vector2di(renderSize.Width-texSize.Width-ImageBorderX,renderSize.Height-texSize.Height*2-ImageBorderY));
		TopCameraImage_->setRelativePosition(irr::core::vector2di(renderSize.Width-texSize.Width-ImageBorderX,renderSize.Height-texSize.Height*3-ImageBorderY));
	}
}

bool GUIController::PreRender3D( const SRenderContext& rc )
{
	return !Minimum_;
}

void GUIController::PostRender3D( const SRenderContext& rc )
{

}

bool GUIController::PreRender2D( const SRenderContext& rc )
{
	if ( Minimum_ )
	{
		RestoreImage_->setVisible(true);
		MinimumImage_->setVisible(false);
		ResizeImage_->setVisible(false);
		ShowCameraImage_->setVisible(false);
		FPSCameraImage_->setVisible(false);
		MayaCameraImage_->setVisible(false);
		TopCameraImage_->setVisible(false);
	}
	else
	{
		RestoreImage_->setVisible(false);
		MinimumImage_->setVisible(true);
		ResizeImage_->setVisible(true);
		ShowCameraImage_->setVisible(true);
		FPSCameraImage_->setVisible(true);
		MayaCameraImage_->setVisible(true);
		TopCameraImage_->setVisible(true);
	}

	if ( CameraVisible_ )
	{
		ShowCameraImage_->setImage(ShowCameraTex_.get());
	}
	else
	{
		ShowCameraImage_->setImage(HideCameraTex_.get());
	}

	return true;
}

void GUIController::PostRender2D( const SRenderContext& rc )
{

}