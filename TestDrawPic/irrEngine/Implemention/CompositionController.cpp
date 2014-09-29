#include "stdafx.h"

#include "CompositionController.h"
#include "irrEngine/SRenderContext.h"

#include "irrEngine/IrrExtension/SimpleFPSAnimator.h"
#include "irrEngine/IrrExtension/SimpleMayaAnimator.h"
#include "irrEngine/IrrExtension/SimpleOrthoAnimator.h"
#include "irrEngine/IrrExtension/OrthoCameraRH.h"
#include "irrEngine/IrrExtension/CameraFollowSplineAnimator.h"
#include "GUIController.h"
#include "MayaCameraController.h"
#include "FlyCameraController.h"

class	CompositionController::Imp
{
public:

	Imp() {}

	~Imp() {}

public:

	ICameraSceneNodeSPtr		FPSCamera_;
	ICameraSceneNodeSPtr		MayaCamera_;
	ICameraSceneNodeSPtr		TopCamera_;

	GUIControllerSPtr			GUIController_;
	MayaCameraControllerSPtr	MayaCameraController_;
	FlyCameraControllerSPtr		FlyCameraController_;
};

CompositionController::CompositionController():ImpUPtr_(new Imp)
{
	auto& imp_ = *ImpUPtr_;

	imp_.GUIController_ = std::make_shared<GUIController>();
	imp_.MayaCameraController_ = std::make_shared<MayaCameraController>();
	imp_.FlyCameraController_ = std::make_shared<FlyCameraController>();

	SetName("CompositionController");
}

CompositionController::~CompositionController()
{

}

void CompositionController::PreInit( SRenderContextSPtr sprc )
{
	auto& imp_ = *ImpUPtr_;
	auto& rc = *sprc;
	auto driver = rc.Smgr_->getVideoDriver();

	{
		{//Maya camera
			auto camera = rc.Smgr_->addCameraSceneNodeRH(0, irr::core::vector3df(0, 4000, -2000), irr::core::vector3df(0,1600,1000));
			auto animator = new CSimpleMayaAnimator(rc.CursorControl_.get());
			camera->addAnimator(animator);
			animator->drop();

			camera->setFOV(1.f/3.f*3.14f);
			camera->setFarValue( 15000.0f );
			camera->setNearValue( 200.f );

			imp_.MayaCamera_.reset(camera, [](irr::scene::ICameraSceneNode*){});
		}

		{//FPS camera
			auto camera = rc.Smgr_->addCameraSceneNodeRH(0, irr::core::vector3df(0, 1600, 0), irr::core::vector3df(0,1600,1000));
			auto animator = new CSimpleFPSAnimator(rc.CursorControl_.get());
			camera->addAnimator(animator);
			animator->drop();

			camera->setFOV(1.f/3.f*3.14f);
			camera->setFarValue( 15000.0f );
			camera->setNearValue( 20.f );

			imp_.FPSCamera_.reset(camera, [](irr::scene::ICameraSceneNode*){});
		}

		{//Top camera
			auto camera = new COrthoCameraRH(rc.Smgr_->getRootSceneNode(), rc.Smgr_.get(), -1, irr::core::vector3df(0, 4000, 0), irr::core::vector3df(0,0,0));
			camera->drop();
			auto animator = new CSimpleOrthoAnimator(rc.CursorControl_.get());
			camera->addAnimator(animator);
			animator->drop();

			camera->setFOV(1.f/3.f*3.14f);
			camera->setFarValue( 15000.0f );
			camera->setNearValue( 200.f );
			camera->setUpVector(irr::core::vector3df(0,0,-1));
			camera->SetBaseWidth(6000);

			imp_.TopCamera_.reset(camera, [](irr::scene::ICameraSceneNode*){});
			rc.Smgr_->setActiveCamera(imp_.TopCamera_.get());			
		}
	}

	{//Skybox
		auto skyNode = rc.Smgr_->addSkyBoxSceneNode(
			driver->getTexture("../Data/Resource/3D/sky_up_.jpg"),
			driver->getTexture("../Data/Resource/3D/sky_dn_.jpg"),
			driver->getTexture("../Data/Resource/3D/sky_lf_.jpg"),
			driver->getTexture("../Data/Resource/3D/sky_rt_.jpg"),
			driver->getTexture("../Data/Resource/3D/sky_ft_.jpg"),
			driver->getTexture("../Data/Resource/3D/sky_bk_.jpg"));
		skyNode->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
	}

	imp_.MayaCameraController_->SetMayaCamera(imp_.MayaCamera_);

	sprc->PushController(std::static_pointer_cast<IRenderController>(imp_.FlyCameraController_));
	sprc->PushController(std::static_pointer_cast<IRenderController>(imp_.GUIController_));

	//sprc->PushController(std::static_pointer_cast<IRenderController>(imp_.MayaCameraController_));
}

void CompositionController::OnResize( const SRenderContext& rc )
{
	auto& imp_ = *ImpUPtr_;

	auto renderSize = rc.Smgr_->getVideoDriver()->getScreenSize();
	auto aspect = (irr::f32)renderSize.Width/(irr::f32)renderSize.Height;
	imp_.FPSCamera_->setAspectRatio(aspect);
	imp_.MayaCamera_->setAspectRatio(aspect);
	imp_.TopCamera_->setAspectRatio(aspect);
	
	rc.Smgr_->getActiveCamera()->setAspectRatio(aspect);
}

bool CompositionController::PreRender3D( const SRenderContext& rc )
{
	auto& imp_ = *ImpUPtr_;

	if ( imp_.FlyCameraController_->IsFlying() )
	{
		return true;
	}

	irr::scene::ISceneNodeAnimatorFinishing* animator = nullptr;
	auto activeCamera = rc.Smgr_->getActiveCamera();
	decltype(activeCamera) toSwitchCamera = nullptr;
	if ( imp_.GUIController_->IsFPSCameraActive() && activeCamera != imp_.FPSCamera_.get() )
	{
		toSwitchCamera = imp_.FPSCamera_.get();
	}
	if ( imp_.GUIController_->IsMayaCameraActive() && activeCamera != imp_.MayaCamera_.get() )
	{
		toSwitchCamera = imp_.MayaCamera_.get();
	}
	if ( imp_.GUIController_->IsTopCameraActive() && activeCamera != imp_.TopCamera_.get() )
	{
		toSwitchCamera = imp_.TopCamera_.get();
	}

	if ( toSwitchCamera )
	{
		auto camera = rc.Smgr_->addCameraSceneNodeRH();
		camera->setPosition(activeCamera->getPosition());
		camera->setTarget(activeCamera->getTarget());

		auto posToTarget = activeCamera->getTarget()-activeCamera->getPosition();
		auto xDir = activeCamera->getUpVector().crossProduct(posToTarget);
		xDir.normalize();

		auto zDir = posToTarget;
		zDir.normalize();

		if ( toSwitchCamera == imp_.FPSCamera_.get() )
		{
			camera->setNearValue(toSwitchCamera->getNearValue());
			camera->setFarValue(toSwitchCamera->getFarValue());
			camera->setFOV(toSwitchCamera->getFOV());
			camera->setAspectRatio(toSwitchCamera->getAspectRatio());

			irr::core::plane3df zeroPlane(0,1600,0,0,1,0);
			irr::core::line3df line(activeCamera->getPosition(), activeCamera->getTarget());
			irr::core::vector3df section;
			zeroPlane.getIntersectionWithLine(line.start, line.getVector(), section);

			if ( imp_.TopCamera_.get() == activeCamera )
			{
				camera->setUpVector(activeCamera->getUpVector());

				irr::core::array<irr::core::vector3df> posPoints;
				irr::core::array<irr::core::vector3df> tarPoints;
				irr::core::array<irr::core::vector3df> upVectors;
				posPoints.push_back(camera->getPosition());
				posPoints.push_back(section);			
				tarPoints.push_back(activeCamera->getTarget());
				tarPoints.push_back(activeCamera->getUpVector()*1000+section);
				upVectors.push_back(activeCamera->getUpVector());
				upVectors.push_back(toSwitchCamera->getUpVector());

				animator = new CCameraFollowSplineAnimator(rc.Timer_->getTime(), posPoints, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
				camera->addAnimator(animator);
				animator->drop();

				toSwitchCamera->setPosition(section);
				toSwitchCamera->setTarget(activeCamera->getUpVector()*1000+section);
				toSwitchCamera->updateAbsolutePosition();
			}

			if ( imp_.MayaCamera_.get() == activeCamera )
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
				animator = new CCameraFollowSplineAnimator(rc.Timer_->getTime(), points, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
				camera->addAnimator(animator);
				animator->drop();

				toSwitchCamera->setPosition(section);
				toSwitchCamera->setTarget(newTarget*1000+section);
				toSwitchCamera->updateAbsolutePosition();
			}
		}

		if ( toSwitchCamera == imp_.MayaCamera_.get() )
		{
			if ( imp_.TopCamera_.get() == activeCamera )
			{
				irr::core::matrix4 mat;
				mat.setRotationAxisRadians(irr::core::degToRad(-30.f), xDir);

				auto tarToPos = -posToTarget;
				mat.transformVect(tarToPos);
				auto newPos = camera->getTarget() + tarToPos.normalize()*4000;

				irr::core::array<irr::core::vector3df> points;
				points.push_back(camera->getPosition()+irr::core::vector3df(0,2000,0));
				points.push_back(newPos);
				irr::core::array<irr::core::vector3df> tarPoints;
				irr::core::array<irr::core::vector3df> upVectors;
				upVectors.push_back(activeCamera->getUpVector());
				upVectors.push_back(toSwitchCamera->getUpVector());

				animator = new CCameraFollowSplineAnimator(rc.Timer_->getTime(), points, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
				camera->addAnimator(animator);
				animator->drop();

				toSwitchCamera->setPosition(newPos);
				toSwitchCamera->setTarget(activeCamera->getTarget());
				toSwitchCamera->updateAbsolutePosition();
			}

			if ( imp_.FPSCamera_.get() == activeCamera )
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

				animator = new CCameraFollowSplineAnimator(rc.Timer_->getTime(), points, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
				camera->addAnimator(animator);
				animator->drop();

				toSwitchCamera->setPosition(newPos);
				toSwitchCamera->setTarget(activeCamera->getPosition());
				toSwitchCamera->updateAbsolutePosition();
			}
		}

		if ( toSwitchCamera == imp_.TopCamera_.get() )
		{
			if ( imp_.FPSCamera_.get() == activeCamera )
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
				upVectors.push_back(toSwitchCamera->getUpVector());

				animator = new CCameraFollowSplineAnimator(rc.Timer_->getTime(), points, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
				camera->addAnimator(animator);
				animator->drop();

				toSwitchCamera->setPosition(newPos);
				toSwitchCamera->setTarget(newTarget);
				toSwitchCamera->updateAbsolutePosition();
			}

			if ( imp_.MayaCamera_.get() == activeCamera )
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
				upVectors.push_back(toSwitchCamera->getUpVector());

				animator = new CCameraFollowSplineAnimator(rc.Timer_->getTime(), points, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
				camera->addAnimator(animator);
				animator->drop();

				toSwitchCamera->setPosition(newPos);
				toSwitchCamera->setTarget(newTarget);
				toSwitchCamera->updateAbsolutePosition();
			}
		}

		imp_.FlyCameraController_->SetFlying(toSwitchCamera, camera, animator);
	}

	return false;
}

void CompositionController::PostRender3D( const SRenderContext& rc )
{
	
}

bool CompositionController::PreRender2D( const SRenderContext& rc )
{
	return false;
}

void CompositionController::PostRender2D( const SRenderContext& rc )
{

}