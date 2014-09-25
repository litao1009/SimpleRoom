#include "stdafx.h"

#include "SRenderContext.h"
#include "irrEngine.h"
#include "irrReceiver.h"
#include "SimpleFPSAnimator.h"
#include "SimpleMayaAnimator.h"
#include "SimpleOrthoAnimator.h"
#include "OrthoCameraRH.h"
#include "CameraFollowSplineAnimator.h"
#include "XEffect/SSAORenderCallback.h"

#include "Irrlicht/CIrrDeviceWin32.h"
#include "Irrlicht/os.h"
#include "Irrlicht/ISceneNodeAnimatorFinishing.h"

static const auto ImageBorderX = 3;
static const auto ImageBorderY = 3;

class	SRenderContext::Imp
{
public:

	Imp()
	{
		TmpCameraAnimator_ = nullptr;
		ToSwitchCamera_ = nullptr;
		TmpCamera_ = nullptr;
	}

	~Imp() {}

public:

	irr::scene::ISceneNodeAnimatorFinishing*	TmpCameraAnimator_;
	irr::scene::ICameraSceneNode*				ToSwitchCamera_;
	irr::scene::ICameraSceneNode*				TmpCamera_;
};

SRenderContext::SRenderContext( void* Hwnd, irr::video::SColor clr ):ImpUPtr_(new Imp)
{
	auto baseDevice = IrrEngine::GetInstance()->GetDevice();
	auto baseSmgr = baseDevice->getSceneManager();
	auto baseDriver = baseDevice->getVideoDriver();
	auto baseFs = baseDevice->getFileSystem();

	{//RC
		auto src = IrrEngine::GetInstance()->GetDevice()->getVideoDriver()->getExposedVideoData();

		VideoData_.OpenGLWin32.HWnd = Hwnd;
		VideoData_.OpenGLWin32.HDc = GetDC((HWND)Hwnd);
		 
		PIXELFORMATDESCRIPTOR pfd={0};
		pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);
		int pf = GetPixelFormat((HDC)src.OpenGLWin32.HDc);
		DescribePixelFormat((HDC)VideoData_.OpenGLWin32.HDc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
		SetPixelFormat((HDC)VideoData_.OpenGLWin32.HDc, pf, &pfd);
		VideoData_.OpenGLWin32.HRc = src.OpenGLWin32.HRc;
	}

	OnResize_ = false;

	Minimum_ = false;

	CameraVisible_ = true;
	CameraRestore_ = true;

	Icon_ = irr::gui::ECI_NORMAL;

	BackGroundClr_ = clr;


	RECT viewRect;
	GetClientRect((HWND)Hwnd, &viewRect);

	{//Cursor
		irr::core::dimension2du drect(viewRect.right, viewRect.bottom);
		auto cur = new irr::CIrrDeviceWin32::CCursorControl(static_cast<irr::CIrrDeviceWin32*>(baseDevice), drect, (HWND)Hwnd, false);
		CursorControl_.reset(cur, std::bind(&irr::gui::ICursorControl::drop, std::placeholders::_1));
	}

	{//GUI
		auto guimgr = irr::gui::createGUIEnvironment(baseFs, baseDriver, baseDevice->getOSOperator());
		GUIEnv_.reset(guimgr, std::bind(&irr::gui::IGUIEnvironment::drop, std::placeholders::_1));

		{//resize
			auto tex = baseDriver->getTexture("../Data/Resource/3D/resize.png");
			auto texSize = tex->getSize();
			auto guiImage = GUIEnv_->addImage(tex, irr::core::vector2di(viewRect.right-texSize.Width-ImageBorderX,ImageBorderY));
			ResizeImage_.reset(guiImage, std::bind([](irr::gui::IGUIElement*){}, std::placeholders::_1));
		}

		{//minimum
			auto tex = baseDriver->getTexture("../Data/Resource/3D/minimum.png");
			auto guiImage = GUIEnv_->addImage(tex, irr::core::vector2di(ImageBorderX,ImageBorderY));
			MinimumImage_.reset(guiImage, std::bind([](irr::gui::IGUIElement*){}, std::placeholders::_1));
		}

		{//restore
			auto tex = baseDriver->getTexture("../Data/Resource/3D/restore.png");
			auto guiImage = GUIEnv_->addImage(tex, irr::core::vector2di(ImageBorderX,ImageBorderY));
			RestoreImage_.reset(guiImage, std::bind([](irr::gui::IGUIElement*){}, std::placeholders::_1));
			RestoreImage_->setVisible(false);
		}

		{//camera
			auto showTex = baseDriver->getTexture("../Data/Resource/3D/camera.png");
			auto hideTex = baseDriver->getTexture("../Data/Resource/3D/camera_d.png");
			auto texSize = showTex->getSize();
			auto guiImage = GUIEnv_->addImage(showTex, irr::core::vector2di((viewRect.right-texSize.Width)/2,ImageBorderY));

			ShowCameraImage_.reset(guiImage, std::bind([](irr::gui::IGUIElement*){}, std::placeholders::_1));

			ShowCameraTex_.reset(showTex, [](irr::video::ITexture*){});
			HideCameraTex_.reset(hideTex, [](irr::video::ITexture*){});
		}

		{//camera type
			auto fpsActTex = baseDriver->getTexture("../Data/Resource/3D/fps_cam_a.png");
			auto fpsDActTex = baseDriver->getTexture("../Data/Resource/3D/fps_cam_d.png");
			auto mayaActTex = baseDriver->getTexture("../Data/Resource/3D/maya_cam_a.png");
			auto mayaDActTex = baseDriver->getTexture("../Data/Resource/3D/maya_cam_d.png");
			auto topActTex = baseDriver->getTexture("../Data/Resource/3D/top_cam_a.png");
			auto topDActTex = baseDriver->getTexture("../Data/Resource/3D/top_cam_d.png");

			auto texSize = fpsActTex->getSize();

			auto fpsImage = GUIEnv_->addImage(fpsDActTex, irr::core::vector2di(viewRect.right-texSize.Width-ImageBorderX,viewRect.bottom-texSize.Height-ImageBorderY));
			auto mayaImage = GUIEnv_->addImage(mayaDActTex, irr::core::vector2di(viewRect.right-texSize.Width-ImageBorderX,viewRect.bottom-texSize.Height*2-ImageBorderY));
			auto topImage = GUIEnv_->addImage(topActTex, irr::core::vector2di(viewRect.right-texSize.Width-ImageBorderX,viewRect.bottom-texSize.Height*3-ImageBorderY));

			FPSCameraImage_.reset(fpsImage, [](irr::gui::IGUIImage*){});
			MayaCameraImage_.reset(mayaImage, [](irr::gui::IGUIImage*){});
			TopCameraImage_.reset(topImage, [](irr::gui::IGUIImage*){});

			MayaActiveTex_.reset(mayaActTex, [](irr::video::ITexture*){});
			MayaDActiveTex_.reset(mayaDActTex, [](irr::video::ITexture*){});
			FPSActiveTex_.reset(fpsActTex, [](irr::video::ITexture*){});
			FPSDActiveTex_.reset(fpsDActTex, [](irr::video::ITexture*){});
			TopActiveTex_.reset(topActTex, [](irr::video::ITexture*){});
			TopDActiveTex_.reset(topDActTex, [](irr::video::ITexture*){});
		}
	}

	{//smgr
		auto smgr = irr::scene::createSceneManager(baseDriver, baseFs, CursorControl_.get(), GUIEnv_.get());
		Smgr_.reset(smgr, std::bind(&irr::scene::ISceneManager::drop, std::placeholders::_1));
	}

	{
		{//Maya camera
			auto camera = Smgr_->addCameraSceneNodeRH(0, irr::core::vector3df(0, 4000, -2000), irr::core::vector3df(0,1600,1000));
			auto animator = new CSimpleMayaAnimator(CursorControl_.get());
			camera->addAnimator(animator);
			animator->drop();

			camera->setFOV(1.f/3.f*3.14f);
			camera->setFarValue( 10000.0f );
			camera->setNearValue( 200.f );

			MayaCamera_.reset(camera, [](irr::scene::ICameraSceneNode*){});
		}

		{//FPS camera
			auto camera = Smgr_->addCameraSceneNodeRH(0, irr::core::vector3df(0, 1600, 0), irr::core::vector3df(0,1600,1000));
			auto animator = new CSimpleFPSAnimator(CursorControl_.get());
			camera->addAnimator(animator);
			animator->drop();

			camera->setFOV(1.f/3.f*3.14f);
			camera->setFarValue( 8000.0f );
			camera->setNearValue( 20.f );

			FPSCamera_.reset(camera, [](irr::scene::ICameraSceneNode*){});
		}

		{//Top camera
			auto camera = new COrthoCameraRH(Smgr_->getRootSceneNode(), Smgr_.get(), -1, irr::core::vector3df(0, 4000, 0), irr::core::vector3df(0,0,0));
			camera->drop();
			auto animator = new CSimpleOrthoAnimator(CursorControl_.get());
			camera->addAnimator(animator);
			animator->drop();

			camera->setFOV(1.f/3.f*3.14f);
			camera->setFarValue( 10000.0f );
			camera->setNearValue( 200.f );
			camera->setUpVector(irr::core::vector3df(0,0,0-1));
			camera->SetBaseWidth(6000);

			TopCamera_.reset(camera, [](irr::scene::ICameraSceneNode*){});
			Smgr_->setActiveCamera(TopCamera_.get());			
		}
	}

	{//Skybox
		auto skyNode = Smgr_->addSkyBoxSceneNode(
			baseDriver->getTexture("../Data/Resource/3D/sky_up_.jpg"),
			baseDriver->getTexture("../Data/Resource/3D/sky_dn_.jpg"),
			baseDriver->getTexture("../Data/Resource/3D/sky_lf_.jpg"),
			baseDriver->getTexture("../Data/Resource/3D/sky_rt_.jpg"),
			baseDriver->getTexture("../Data/Resource/3D/sky_ft_.jpg"),
			baseDriver->getTexture("../Data/Resource/3D/sky_bk_.jpg"));
		skyNode->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
	}

	{//Effect
		Effect_ = std::make_shared<EffectHandler>(baseDevice, baseDriver->getScreenSize(), true, true, true);
		Effect_->setActiveSceneManager(Smgr_.get());
		Effect_->setAmbientColor(irr::video::SColor(255, 100, 100, 100));
		Effect_->setClearColour(irr::video::SColor(255,100, 100, 100));
		Effect_->enableDepthPass(true);

		//Effect_->addShadowLight(SShadowLight(1024, irr::core::vector3df(0, 4000, 0), irr::core::vector3df(0,0,0), irr::video::SColorf(1,1,1,1), 20, 10000, 120*irr::core::DEGTORAD64, true));
		// Generate and set the random vector user texture for the SSAO to use.
		auto randVecTexture = Effect_->generateRandomVectorTexture(irr::core::dimension2du(512, 512));
		Effect_->setPostProcessingUserTexture(randVecTexture);
		
		irr::s32 SSAO = Effect_->addPostProcessingEffectFromFile("../Data/Resource/3D/shaders/SSAO.glsl");
		irr::s32 BlurH = Effect_->addPostProcessingEffectFromFile("../Data/Resource/3D/shaders/BlurHP.glsl");
		irr::s32 BlurV = Effect_->addPostProcessingEffectFromFile("../Data/Resource/3D/shaders/BlurVP.glsl");
		irr::s32 SSAOCombine = Effect_->addPostProcessingEffectFromFile("../Data/Resource/3D/shaders/SSAOCombine.glsl");

		//Effect_->addPostProcessingEffectFromFile("../Data/Resource/3D/shaders/BrightPass.glsl");
		//Effect_->addPostProcessingEffectFromFile("../Data/Resource/3D/shaders/BlurHP.glsl");
		//Effect_->addPostProcessingEffectFromFile("../Data/Resource/3D/shaders/BlurVP.glsl");
		//Effect_->addPostProcessingEffectFromFile("../Data/Resource/3D/shaders/BloomP.glsl");

		auto ssaoCallback = new SSAORenderCallback(SSAO);
		Effect_->setPostProcessingRenderCallback(SSAO, ssaoCallback);
		
	}
}

SRenderContext::~SRenderContext()
{
	ReleaseDC((HWND)VideoData_.OpenGLWin32.HWnd, (HDC)VideoData_.OpenGLWin32.HDc);
}

void SRenderContext::OnResize(bool force)
{
	if ( force )
	{
		OnResize_ = true;
	}

	if ( !OnResize_ )
	{
		return;
	}

	RECT r;
	GetClientRect((HWND)VideoData_.OpenGLWin32.HWnd, &r);

	if (r.right < 2 || r.bottom < 2)
	{
		irr::os::Printer::log("Ignoring resize");
	}
	else
	{
		irr::os::Printer::log("Resizing window");

		Smgr_->getVideoDriver()->OnResize(irr::core::dimension2du((irr::u32)r.right, (irr::u32)r.bottom));
		CursorControl_->OnResize(Smgr_->getVideoDriver()->getScreenSize());

		if ( RenderController_ )
		{
			RenderController_->OnResize();
		}

		auto renderSize = Smgr_->getVideoDriver()->getScreenSize();
		auto aspect = (irr::f32)renderSize.Width/(irr::f32)renderSize.Height;
		FPSCamera_->setAspectRatio(aspect);
		MayaCamera_->setAspectRatio(aspect);
		TopCamera_->setAspectRatio(aspect);
		Smgr_->getActiveCamera()->setAspectRatio(aspect);

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

		{
			Effect_->setScreenRenderTargetResolution(renderSize);
		}
	}

	OnResize_ = false;
}

void SRenderContext::SetOnResize()
{
	OnResize_ = true;
}

void SRenderContext::SetMinimum( bool val )
{
	Minimum_ = val;

	if ( Minimum_ )
	{
		RestoreImage_->setVisible(true);
		MinimumImage_->setVisible(false);
		ResizeImage_->setVisible(false);
		ShowCameraImage_->setVisible(false);
		FPSCameraImage_->setVisible(false);
		MayaCameraImage_->setVisible(false);
		TopCameraImage_->setVisible(false);
		CameraRestore_ = CameraVisible_;
		CameraVisible_ = false;
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
		CameraVisible_ = CameraRestore_;
	}
}

bool SRenderContext::PostEvent( irr::SEvent& evt )
{
	CursorControl_->setActiveIcon(Icon_);

	bool absorbed = false;

	if ( RenderController_ )
	{
		absorbed = RenderController_->OnEvent(evt);
	}

	if (!absorbed)
	{
		absorbed = GUIEnv_->postEventFromUser(evt);
	}

	if (!absorbed)
	{
		Smgr_->postEventFromUser(evt);
	}

	return false;
}

void SRenderContext::Render()
{
	auto driver = Smgr_->getVideoDriver();
	auto smgr = Smgr_.get();
	auto guimgr = GUIEnv_.get();
	auto camera = smgr->getActiveCamera();
	auto curcon = CursorControl_.get();
	auto& imp_ = *ImpUPtr_;

	Timer_->tick();
	curcon->update();
	OnResize();

	driver->beginScene(true, true, BackGroundClr_, VideoData_);

	auto needRender3D = true;
	auto needRender2D = true;

	if ( !Minimum_ )
	{
		if ( RenderController_ )
		{
			needRender3D = RenderController_->PreRender3D(*this);
		}
		
		if ( needRender3D )
		{
			//smgr->drawAll();
			Effect_->update();

			if ( imp_.TmpCameraAnimator_ )
			{
				if ( imp_.TmpCameraAnimator_->hasFinished() )
				{
					smgr->setActiveCamera(imp_.ToSwitchCamera_);
					imp_.TmpCamera_->remove();
					imp_.TmpCamera_ = nullptr;
					imp_.TmpCameraAnimator_ = nullptr;
					imp_.ToSwitchCamera_ = nullptr;
				}
			}

			if ( RenderController_ )
			{
				RenderController_->PostRender3D(*this);
			}
		}
	}

	if ( RenderController_ )
	{
		needRender2D = RenderController_->PreRender2D(*this);
	}

	if ( needRender2D )
	{
		guimgr->drawAll();

		if ( RenderController_ )
		{
			RenderController_->PostRender2D(*this);
		}
	}

	driver->endScene();
}

void SRenderContext::UpdateScene()
{
	Smgr_->getRootSceneNode()->OnAnimate(Timer_->getTime());
}

void SRenderContext::SwitchToFPSCamera()
{
	if ( FPSCamera_.get() == Smgr_->getActiveCamera() )
	{
		return;
	}

	auto activeCamera = Smgr_->getActiveCamera();
	auto camera = Smgr_->addCameraSceneNodeRH();
	camera->setPosition(activeCamera->getPosition());
	camera->setTarget(activeCamera->getTarget());
	camera->setNearValue(FPSCamera_->getNearValue());
	camera->setFarValue(FPSCamera_->getFarValue());
	camera->setFOV(FPSCamera_->getFOV());
	camera->setAspectRatio(FPSCamera_->getAspectRatio());

	auto posToTarget = activeCamera->getTarget()-activeCamera->getPosition();
	auto xDir = activeCamera->getUpVector().crossProduct(posToTarget);
	xDir.normalize();

	auto zDir = posToTarget;
	zDir.normalize();

	irr::core::plane3df zeroPlane(0,1600,0,0,1,0);
	irr::core::line3df line(activeCamera->getPosition(), activeCamera->getTarget());
	irr::core::vector3df section;
	zeroPlane.getIntersectionWithLine(line.start, line.getVector(), section);

	if ( TopCamera_.get() == activeCamera )
	{
		camera->setUpVector(activeCamera->getUpVector());

		irr::core::array<irr::core::vector3df> posPoints;
		posPoints.push_back(camera->getPosition());
		posPoints.push_back(section);

		irr::core::array<irr::core::vector3df> tarPoints;
		tarPoints.push_back(activeCamera->getTarget());
		tarPoints.push_back(activeCamera->getUpVector()*1000+section);
		irr::core::array<irr::core::vector3df> upVectors;
		upVectors.push_back(activeCamera->getUpVector());
		upVectors.push_back(FPSCamera_->getUpVector());
		auto animator = new CCameraFollowSplineAnimator(Timer_->getTime(), posPoints, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
		camera->addAnimator(animator);
		animator->drop();

		FPSCamera_->setPosition(section);
		FPSCamera_->setTarget(activeCamera->getUpVector()*1000+section);
		FPSCamera_->updateAbsolutePosition();

		ImpUPtr_->TmpCamera_ = camera;
		ImpUPtr_->TmpCameraAnimator_ = static_cast<irr::scene::ISceneNodeAnimatorFinishing*>(animator);
		ImpUPtr_->ToSwitchCamera_ = FPSCamera_.get();
	}

	if ( MayaCamera_.get() == activeCamera )
	{
		auto newTarget = posToTarget;
		newTarget.Y = 0;
		newTarget.normalize();

		irr::core::array<irr::core::vector3df> points;
		points.push_back(activeCamera->getPosition());
		points.push_back(section);
		irr::core::array<irr::core::vector3df> tarPoints;
		tarPoints.push_back(activeCamera->getTarget());
		tarPoints.push_back(newTarget*1000+section);
		irr::core::array<irr::core::vector3df> upVectors;
		auto animator = new CCameraFollowSplineAnimator(Timer_->getTime(), points, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
		camera->addAnimator(animator);
		animator->drop();

		FPSCamera_->setPosition(section);
		FPSCamera_->setTarget(newTarget*1000+section);
		FPSCamera_->updateAbsolutePosition();

		ImpUPtr_->TmpCamera_ = camera;
		ImpUPtr_->TmpCameraAnimator_ = static_cast<irr::scene::ISceneNodeAnimatorFinishing*>(animator);
		ImpUPtr_->ToSwitchCamera_ = FPSCamera_.get();
	}

	FPSCameraImage_->setImage(FPSActiveTex_.get());
	MayaCameraImage_->setImage(MayaDActiveTex_.get());
	TopCameraImage_->setImage(TopDActiveTex_.get());
}

void SRenderContext::SwitchToMayaCamera()
{
	auto activeCamera = Smgr_->getActiveCamera();

	if ( MayaCamera_.get() == activeCamera )
	{
		return;
	}

	auto camera = Smgr_->addCameraSceneNodeRH();
	camera->setPosition(activeCamera->getPosition());
	camera->setTarget(activeCamera->getTarget());
	camera->setNearValue(MayaCamera_->getNearValue());
	camera->setFarValue(MayaCamera_->getFarValue());
	camera->setFOV(MayaCamera_->getFOV());
	camera->setAspectRatio(MayaCamera_->getAspectRatio());

	auto posToTarget = activeCamera->getTarget()-activeCamera->getPosition();
	auto xDir = activeCamera->getUpVector().crossProduct(posToTarget);
	xDir.normalize();

	auto zDir = posToTarget;
	zDir.normalize();

	if ( TopCamera_.get() == activeCamera )
	{
		irr::core::matrix4 mat;
		mat.setRotationAxisRadians(irr::core::degToRad(-30.f), xDir);

		auto tarToPos = -posToTarget;
		mat.transformVect(tarToPos);
		auto newPos = camera->getTarget() + tarToPos.normalize()*4000;

		irr::core::array<irr::core::vector3df> points;
		points.push_back(camera->getPosition()+irr::core::vector3df(0,2000,0));
		points.push_back(newPos);

		auto animator = Smgr_->createFollowSplineAnimator(Timer_->getTime(), points, 1.5f, 0.5f, false);
		camera->addAnimator(animator);
		animator->drop();

		MayaCamera_->setPosition(newPos);
		MayaCamera_->setTarget(activeCamera->getTarget());
		MayaCamera_->updateAbsolutePosition();

		ImpUPtr_->TmpCamera_ = camera;
		ImpUPtr_->TmpCameraAnimator_ = static_cast<irr::scene::ISceneNodeAnimatorFinishing*>(animator);
		ImpUPtr_->ToSwitchCamera_ = MayaCamera_.get();
	}

	if ( FPSCamera_.get() == activeCamera )
	{
		irr::core::matrix4 mat;
		mat.setRotationAxisRadians(irr::core::degToRad(60.f), xDir);
		auto tarToPos = -posToTarget;
		tarToPos.normalize();
		mat.transformVect(tarToPos);
		auto newPos = camera->getTarget() + tarToPos * 4000;

		irr::core::array<irr::core::vector3df> points;
		points.push_back(activeCamera->getPosition());
		points.push_back(newPos);
		irr::core::array<irr::core::vector3df> tarPoints;
		tarPoints.push_back(activeCamera->getTarget());
		tarPoints.push_back(activeCamera->getPosition());
		irr::core::array<irr::core::vector3df> upVectors;
		auto animator = new CCameraFollowSplineAnimator(Timer_->getTime(), points, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
		camera->addAnimator(animator);
		animator->drop();

		MayaCamera_->setPosition(newPos);
		MayaCamera_->setTarget(activeCamera->getPosition());
		MayaCamera_->updateAbsolutePosition();

		ImpUPtr_->TmpCamera_ = camera;
		ImpUPtr_->TmpCameraAnimator_ = static_cast<irr::scene::ISceneNodeAnimatorFinishing*>(animator);
		ImpUPtr_->ToSwitchCamera_ = MayaCamera_.get();
	}

	FPSCameraImage_->setImage(FPSDActiveTex_.get());
	MayaCameraImage_->setImage(MayaActiveTex_.get());
	TopCameraImage_->setImage(TopDActiveTex_.get());
}

void SRenderContext::SwitchToTopCamera()
{
	if ( TopCamera_.get() == Smgr_->getActiveCamera() )
	{
		return;
	}

	auto activeCamera = Smgr_->getActiveCamera();

	auto camera = Smgr_->addCameraSceneNodeRH();
	camera->setPosition(activeCamera->getPosition());
	camera->setTarget(activeCamera->getTarget());
	camera->setNearValue(activeCamera->getNearValue());
	camera->setFarValue(activeCamera->getFarValue());
	camera->setFOV(activeCamera->getFOV());
	camera->setAspectRatio(activeCamera->getAspectRatio());

	auto posToTarget = activeCamera->getTarget()-activeCamera->getPosition();
	auto xDir = activeCamera->getUpVector().crossProduct(posToTarget);
	xDir.normalize();

	auto zDir = posToTarget;
	zDir.normalize();

	if ( FPSCamera_.get() == activeCamera )
	{
		auto newPos = activeCamera->getPosition();
		newPos.Y = 5000;
		auto newTarget = newPos;
		newTarget.Y = 0;
		irr::core::array<irr::core::vector3df> points;
		points.push_back(activeCamera->getPosition());
		points.push_back(newPos);
		irr::core::array<irr::core::vector3df> tarPoints;
		tarPoints.push_back(activeCamera->getTarget());
		tarPoints.push_back(newTarget);
		irr::core::array<irr::core::vector3df> upVectors;
		upVectors.push_back(activeCamera->getUpVector());
		upVectors.push_back(TopCamera_->getUpVector());
		auto animator = new CCameraFollowSplineAnimator(Timer_->getTime(), points, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
		camera->addAnimator(animator);
		animator->drop();

		TopCamera_->setPosition(newPos);
		TopCamera_->setTarget(newTarget);
		TopCamera_->updateAbsolutePosition();

		ImpUPtr_->TmpCamera_ = camera;
		ImpUPtr_->TmpCameraAnimator_ = static_cast<irr::scene::ISceneNodeAnimatorFinishing*>(animator);
		ImpUPtr_->ToSwitchCamera_ = TopCamera_.get();
	}

	if ( MayaCamera_.get() == activeCamera )
	{
		auto newPos = activeCamera->getTarget();
		newPos.Y = 5000;
		auto newTarget = newPos;
		newTarget.Y = 0;
		irr::core::array<irr::core::vector3df> points;
		points.push_back(activeCamera->getPosition());
		points.push_back(newPos);
		irr::core::array<irr::core::vector3df> tarPoints;
		tarPoints.push_back(activeCamera->getTarget());
		tarPoints.push_back(newTarget);
		irr::core::array<irr::core::vector3df> upVectors;
		upVectors.push_back(activeCamera->getUpVector());
		upVectors.push_back(TopCamera_->getUpVector());
		auto animator = new CCameraFollowSplineAnimator(Timer_->getTime(), points, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
		camera->addAnimator(animator);
		animator->drop();

		TopCamera_->setPosition(newPos);
		TopCamera_->setTarget(newTarget);
		TopCamera_->updateAbsolutePosition();

		ImpUPtr_->TmpCamera_ = camera;
		ImpUPtr_->TmpCameraAnimator_ = static_cast<irr::scene::ISceneNodeAnimatorFinishing*>(animator);
		ImpUPtr_->ToSwitchCamera_ = TopCamera_.get();
	}

	TopCameraImage_->setImage(TopActiveTex_.get());
	FPSCameraImage_->setImage(FPSDActiveTex_.get());
	MayaCameraImage_->setImage(MayaDActiveTex_.get());
}

void SRenderContext::SetRenderController( IRenderControllerSPtr controler )
{
	RenderController_ = controler;
	GUIEnv_->setUserEventReceiver(RenderController_.get());
}
