#include "stdafx.h"

#include "CameraController.h"

#include "irrEngine/SRenderContext.h"

#include "irrEngine/IrrExtension/SimpleFPSAnimator.h"
#include "irrEngine/IrrExtension/SimpleMayaAnimator.h"
#include "irrEngine/IrrExtension/SimpleOrthoAnimator.h"
#include "irrEngine/IrrExtension/OrthoCameraRH.h"
#include "irrEngine/IrrExtension/CameraFollowSplineAnimator.h"

CameraController::CameraController(const SRenderContextWPtr& rc):IRenderController(rc)
{
	SetName("CameraController");
}

CameraController::~CameraController()
{

}

void CameraController::Init()
{
	auto driver = GetRenderContextSPtr()->Smgr_->getVideoDriver();

	{
		{//Maya camera
			auto camera = GetRenderContextSPtr()->Smgr_->addCameraSceneNodeRH(0, irr::core::vector3df(0, 4000, -2000), irr::core::vector3df(0,1600,1000));
			auto animator = new CSimpleMayaAnimator(GetRenderContextSPtr()->CursorControl_.get());
			camera->addAnimator(animator);
			animator->drop();

			camera->setFOV(1.f/3.f*3.14f);
			camera->setFarValue( 15000.0f );
			camera->setNearValue( 200.f );

			MayaCamera_.reset(camera, [](irr::scene::ICameraSceneNode*){});
		}

		{//FPS camera
			auto camera = GetRenderContextSPtr()->Smgr_->addCameraSceneNodeRH(0, irr::core::vector3df(0, 1600, 0), irr::core::vector3df(0,1600,1000));
			auto animator = new CSimpleFPSAnimator(GetRenderContextSPtr()->CursorControl_.get());
			camera->addAnimator(animator);
			animator->drop();

			camera->setFOV(1.f/3.f*3.14f);
			camera->setFarValue( 15000.0f );
			camera->setNearValue( 20.f );

			FPSCamera_.reset(camera, [](irr::scene::ICameraSceneNode*){});
		}

		{//Top camera
			auto camera = new COrthoCameraRH(GetRenderContextSPtr()->Smgr_->getRootSceneNode(), GetRenderContextSPtr()->Smgr_.get(), -1, irr::core::vector3df(0, 4000, 0), irr::core::vector3df(0,0,0));
			camera->drop();
			auto animator = new CSimpleOrthoAnimator(GetRenderContextSPtr()->CursorControl_.get());
			camera->addAnimator(animator);
			animator->drop();

			camera->setFOV(1.f/3.f*3.14f);
			camera->setFarValue( 15000.0f );
			camera->setNearValue( 200.f );
			camera->setUpVector(irr::core::vector3df(0,0,-1));
			camera->SetBaseWidth(6000);

			TopCamera_.reset(camera, [](irr::scene::ICameraSceneNode*){});
			GetRenderContextSPtr()->Smgr_->setActiveCamera(TopCamera_.get());		
		}
	}
}

void CameraController::OnResize()
{
	auto renderSize = GetRenderContextSPtr()->Smgr_->getVideoDriver()->getScreenSize();
	auto aspect = (irr::f32)renderSize.Width/(irr::f32)renderSize.Height;
	FPSCamera_->setAspectRatio(aspect);
	MayaCamera_->setAspectRatio(aspect);
	TopCamera_->setAspectRatio(aspect);

	GetRenderContextSPtr()->Smgr_->getActiveCamera()->setAspectRatio(aspect);
}