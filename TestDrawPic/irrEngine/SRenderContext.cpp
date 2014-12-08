#include "stdafx.h"

#include "SRenderContext.h"
#include "irrEngine.h"

#include "XEffect/XEffects.h"
#include "XEffect/SSAORenderCallback.h"

#include "MetaRenderController.h"

#include "Irrlicht/CIrrDeviceWin32.h"
#include "Irrlicht/os.h"

typedef	std::shared_ptr<EffectHandler>	EffectHandlerSPtr;

class	SRenderContext::Imp
{
public:

	Imp()
	{
		OnResize_ = false;
		RenderController_ = std::make_shared<MetaRenderController>();
	}

	~Imp() {}

public:

	bool						OnResize_;

	MetaRenderControllerSPtr	RenderController_;

	//特效管理器
	EffectHandlerSPtr		Effect_;
};

SRenderContext::SRenderContext( void* Hwnd, irr::video::SColor clr ):ImpUPtr_(new Imp)
{
	auto baseDevice = IrrEngine::GetInstance()->GetDevice();
	auto baseSmgr = baseDevice->getSceneManager();
	auto baseDriver = baseDevice->getVideoDriver();
	auto baseFs = baseDevice->getFileSystem();
	auto& imp_ = *ImpUPtr_;

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

	BackGroundClr_ = clr;


	RECT viewRect;
	GetClientRect((HWND)Hwnd, &viewRect);

	{//Cursor
		irr::core::dimension2du drect(viewRect.right, viewRect.bottom);
		auto cur = new irr::CIrrDeviceWin32::CCursorControl(static_cast<irr::CIrrDeviceWin32*>(baseDevice), drect, (HWND)Hwnd, true);
		CursorControl_.reset(cur, std::bind(&irr::gui::ICursorControl::drop, std::placeholders::_1));
	}

	{//GUI
		auto guimgr = irr::gui::createGUIEnvironment(baseFs, baseDriver, baseDevice->getOSOperator());
		GUIEnv_.reset(guimgr, std::bind(&irr::gui::IGUIEnvironment::drop, std::placeholders::_1));
		GUIEnv_->setUserEventReceiver(imp_.RenderController_.get());
	}

	{//smgr
		auto smgr = irr::scene::createSceneManager(baseDriver, baseFs, CursorControl_.get(), GUIEnv_.get());
		Smgr_.reset(smgr, std::bind(&irr::scene::ISceneManager::drop, std::placeholders::_1));
	}

	{//Effect
		imp_.Effect_ = std::make_shared<EffectHandler>(baseDevice, baseDriver->getScreenSize(), true, true, true);
		imp_.Effect_->setActiveSceneManager(Smgr_.get());

		auto effectClr = BackGroundClr_;
		effectClr.setRed(effectClr.getRed() > 0xa0 ? 0xa0 : effectClr.getRed());
		effectClr.setGreen(effectClr.getGreen() > 0xa0 ? 0xa0 : effectClr.getGreen());
		effectClr.setBlue(effectClr.getBlue() > 0xa0 ? 0xa0 : effectClr.getBlue());

		imp_.Effect_->setAmbientColor(effectClr);
		imp_.Effect_->setClearColour(BackGroundClr_);
		//imp_.Effect_->enableDepthPass(true);

		imp_.Effect_->addShadowLight(SShadowLight(1024, irr::core::vector3df(1000, 4000, 1000), irr::core::vector3df(0,0,0), irr::video::SColorf(1,1,1,1), 20, 10000, 120*irr::core::DEGTORAD64, false));
		// Generate and set the random vector user texture for the SSAO to use.
		auto randVecTexture = imp_.Effect_->generateRandomVectorTexture(irr::core::dimension2du(512, 512));
		imp_.Effect_->setPostProcessingUserTexture(randVecTexture);
		
		//irr::s32 SSAO = imp_.Effect_->addPostProcessingEffectFromFile("../Data/Resource/3D/shaders/SSAO.glsl");
		//auto ssaoCallback = new SSAORenderCallback(SSAO);
		//imp_.Effect_->setPostProcessingRenderCallback(SSAO, ssaoCallback);
		//irr::s32 BlurH = imp_.Effect_->addPostProcessingEffectFromFile("../Data/Resource/3D/shaders/BlurHP.glsl");
		//irr::s32 BlurV = imp_.Effect_->addPostProcessingEffectFromFile("../Data/Resource/3D/shaders/BlurVP.glsl");
		//irr::s32 SSAOCombine = imp_.Effect_->addPostProcessingEffectFromFile("../Data/Resource/3D/shaders/SSAOCombine.glsl");

		//imp_.Effect_->addPostProcessingEffectFromFile("../Data/Resource/3D/shaders/BrightPass.glsl");
		//imp_.Effect_->addPostProcessingEffectFromFile("../Data/Resource/3D/shaders/BlurHP.glsl");
		//imp_.Effect_->addPostProcessingEffectFromFile("../Data/Resource/3D/shaders/BlurVP.glsl");
		//imp_.Effect_->addPostProcessingEffectFromFile("../Data/Resource/3D/shaders/BloomP.glsl");
	}
}

SRenderContext::~SRenderContext()
{
	ReleaseDC((HWND)VideoData_.OpenGLWin32.HWnd, (HDC)VideoData_.OpenGLWin32.HDc);
}

void SRenderContext::OnResize(bool force)
{
	auto& imp_ = *ImpUPtr_;

	if ( force )
	{
		imp_.OnResize_ = true;
	}

	if ( !imp_.OnResize_ )
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

		imp_.RenderController_->OnResize();

		auto renderSize = Smgr_->getVideoDriver()->getScreenSize();
		imp_.Effect_->setScreenRenderTargetResolution(renderSize);
	}

	imp_.OnResize_ = false;
}

void SRenderContext::SetOnResize()
{
	ImpUPtr_->OnResize_ = true;
}


bool SRenderContext::PostEvent( irr::SEvent& evt )
{
	bool absorbed = false;

	absorbed = ImpUPtr_->RenderController_->OnPreEvent(evt);

	if (!absorbed)
	{
		absorbed = GUIEnv_->postEventFromUser(evt);
	}

	if ( !absorbed )
	{
		absorbed = ImpUPtr_->RenderController_->OnPostEvent(evt);
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

	IrrEngine::GetInstance()->Update(shared_from_this());
	
	driver->beginScene(true, true, BackGroundClr_, VideoData_);	

	auto absorbed3D = imp_.RenderController_->PreRender3D();
	if ( !absorbed3D )
	{
		smgr->drawAll();
		//imp_.Effect_->update();

		imp_.RenderController_->PostRender3D();
	}

	auto absorbed2D = imp_.RenderController_->PreRender2D();
	if ( !absorbed2D )
	{
		guimgr->drawAll();

		imp_.RenderController_->PostRender2D();
	}

	driver->endScene();
}

void SRenderContext::UpdateScene()
{
	Smgr_->getRootSceneNode()->OnAnimate(Timer_->getTime());
}

void SRenderContext::PushController( IRenderControllerSPtr controller )
{
	ImpUPtr_->RenderController_->PushController(controller);
}

void SRenderContext::AddShadowToNode( irr::scene::ISceneNode* node )
{
	ImpUPtr_->Effect_->addShadowToNode(node);
}

void SRenderContext::RemoveShadowFromNode( irr::scene::ISceneNode* node )
{
	ImpUPtr_->Effect_->removeShadowFromNode(node);
}

void SRenderContext::AddNodeToDepthPass( irr::scene::ISceneNode* node )
{
	ImpUPtr_->Effect_->addNodeToDepthPass(node);
}

void SRenderContext::RemoveNodeFromDepthPass( irr::scene::ISceneNode* node )
{
	ImpUPtr_->Effect_->removeNodeFromDepthPass(node);
}

void SRenderContext::Init()
{
	ImpUPtr_->RenderController_->SetRenderContextWPtr(shared_from_this());
}

void SRenderContext::EnableDepthPass( bool val )
{
	ImpUPtr_->Effect_->enableDepthPass(val);
}

void* SRenderContext::GetHandle()
{
	return VideoData_.OpenGLWin32.HWnd;
}
