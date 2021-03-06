#include "stdafx.h"

#include "DesignODL.h"
#include "ODL/GraphODL.h"

#include "irrEngine/irrEngine.h"
#include "irrEngine/SRenderContext.h"

#include "ISceneManager.h"
#include "irrEngine/IrrExtension/CameraFollowSplineAnimator.h"
#include "irrEngine/IrrExtension/CombineSceneNode.h"

#include "RenderController/GridController.h"
#include "RenderController/TopPickingController.h"
#include "RenderController/GUIController.h"
#include "RenderController/StatesController.h"
#include "RenderController/CameraController.h"
#include "RenderController/FlyCameraController.h"
#include "RenderController/UpdateTransformingCtrller.h"
#include "RenderController/RoomLayoutCtrller.h"


#include "BRepBuilderAPI_MakeEdge.hxx"
#include "TopoDS_Edge.hxx"
#include "gce_MakeLin.hxx"
#include "BRepBndLib.hxx"

#include "StatusMgr.h"

enum class ERenderState
{
	ERS_TOP_VIEW,
	ERS_MAYA_VIEW,
	ERS_FPS_VIEW,
	ERS_ANIMATION,
	ERS_EDIT,
	ERS_COUNT
};

class	DesignODL::Imp : public IRenderController
{
public:

	BaseODLWPtr							DesignODL_;
	TopPickingControllerSPtr				TopPickingController_;
	StatesController<ERenderState>::SPtr	StatesController_;
	GUIControllerSPtr						GUIController_;
	CameraControllerSPtr					CameraController_;
	FlyCameraControllerSPtr					FlyCameraController_;
	UpdateTransformingCtrllerSPtr			UpdateTransformingCtrller_;

public:

	Imp():IRenderController(SRenderContextWPtr())
	{
		
	}

public://IRenderController

	virtual	bool	OnPostEvent(const irr::SEvent& evt)
	{
		return false;
	}

	virtual	bool	PreRender3D()
	{
		if ( StatesController_->GetCurrentState() == ERenderState::ERS_ANIMATION )
		{
			if ( !FlyCameraController_->IsFlying() )
			{
				if ( GetRenderContextSPtr()->Smgr_->getActiveCamera() == CameraController_->GetTopCamera().get() )
				{
					StatesController_->SetCurrentState(ERenderState::ERS_TOP_VIEW);
				}
				else if ( GetRenderContextSPtr()->Smgr_->getActiveCamera() == CameraController_->GetMayaCamera().get() )
				{
					StatesController_->SetCurrentState(ERenderState::ERS_MAYA_VIEW);
				}
				else if ( GetRenderContextSPtr()->Smgr_->getActiveCamera() == CameraController_->GetFPSCamera().get() )
				{
					StatesController_->SetCurrentState(ERenderState::ERS_FPS_VIEW);
				}
			}

			CBaseSceneNode::SetRenderMode(CBaseSceneNode::ESNT_3D);
			GetRenderContextSPtr()->EnableDepthPass(true);

			return false;
		}

		if ( !StatusMgr::GetInstance().RealWorld_ && GUIController_->IsTopCameraActive() )
		{
			CBaseSceneNode::SetRenderMode(CBaseSceneNode::ESNT_2D);
			GetRenderContextSPtr()->EnableDepthPass(false);
		}
		else
		{
			CBaseSceneNode::SetRenderMode(CBaseSceneNode::ESNT_3D);
			GetRenderContextSPtr()->EnableDepthPass(true);
		}

		SwitchCameraIfNeed();

		if ( StatesController_->GetCurrentState() == ERenderState::ERS_TOP_VIEW )
		{
			TopPickingController_->SetEnable(true);

			if ( StatusMgr::GetInstance().DrawingState_ != StatusMgr::EDS_NONE )
			{
				TopPickingController_->SetEnable(false);
			}
			
			if ( StatusMgr::GetInstance().PuttingState_ != StatusMgr::EPS_NONE )
			{
				TopPickingController_->SetEnable(false);
			}
		}

		return false;
	}

public:

	bool SwitchCameraIfNeed()
	{
		irr::scene::ISceneNodeAnimatorFinishing* animator = nullptr;
		auto activeCamera = GetRenderContextSPtr()->Smgr_->getActiveCamera();
		decltype(activeCamera) toSwitchCamera = nullptr;
		if ( GUIController_->IsFPSCameraActive() && activeCamera != CameraController_->GetFPSCamera().get() )
		{
			toSwitchCamera = CameraController_->GetFPSCamera().get();
		}
		if ( GUIController_->IsMayaCameraActive() && activeCamera != CameraController_->GetMayaCamera().get() )
		{
			toSwitchCamera = CameraController_->GetMayaCamera().get();
		}
		if ( GUIController_->IsTopCameraActive() && activeCamera != CameraController_->GetTopCamera().get() )
		{
			toSwitchCamera = CameraController_->GetTopCamera().get();
		}

		if ( !toSwitchCamera )
		{
			return false;
		}

		auto camera = GetRenderContextSPtr()->Smgr_->addCameraSceneNodeRH();
		camera->setPosition(activeCamera->getPosition());
		camera->setTarget(activeCamera->getTarget());

		auto posToTarget = activeCamera->getTarget()-activeCamera->getPosition();
		auto xDir = activeCamera->getUpVector().crossProduct(posToTarget);
		xDir.normalize();

		auto zDir = posToTarget;
		zDir.normalize();

		if ( toSwitchCamera == CameraController_->GetFPSCamera().get() )
		{
			camera->setNearValue(toSwitchCamera->getNearValue());
			camera->setFarValue(toSwitchCamera->getFarValue());
			camera->setFOV(toSwitchCamera->getFOV());
			camera->setAspectRatio(toSwitchCamera->getAspectRatio());

			irr::core::plane3df zeroPlane(0,1600,0,0,1,0);
			irr::core::line3df line(activeCamera->getPosition(), activeCamera->getTarget());
			irr::core::vector3df section;
			zeroPlane.getIntersectionWithLine(line.start, line.getVector(), section);

			if ( CameraController_->GetTopCamera().get() == activeCamera )
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

				animator = new CCameraFollowSplineAnimator(GetRenderContextSPtr()->Timer_->getTime(), posPoints, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
				camera->addAnimator(animator);
				animator->drop();

				toSwitchCamera->setPosition(section);
				toSwitchCamera->setTarget(activeCamera->getUpVector()*1000+section);
				toSwitchCamera->updateAbsolutePosition();
			}

			if ( CameraController_->GetMayaCamera().get() == activeCamera )
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
				animator = new CCameraFollowSplineAnimator(GetRenderContextSPtr()->Timer_->getTime(), points, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
				camera->addAnimator(animator);
				animator->drop();

				toSwitchCamera->setPosition(section);
				toSwitchCamera->setTarget(newTarget*1000+section);
				toSwitchCamera->updateAbsolutePosition();
			}
		}

		if ( toSwitchCamera == CameraController_->GetMayaCamera().get() )
		{
			if ( CameraController_->GetTopCamera().get() == activeCamera )
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

				animator = new CCameraFollowSplineAnimator(GetRenderContextSPtr()->Timer_->getTime(), points, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
				camera->addAnimator(animator);
				animator->drop();

				toSwitchCamera->setPosition(newPos);
				toSwitchCamera->setTarget(activeCamera->getTarget());
				toSwitchCamera->updateAbsolutePosition();
			}

			if ( CameraController_->GetFPSCamera().get() == activeCamera )
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

				animator = new CCameraFollowSplineAnimator(GetRenderContextSPtr()->Timer_->getTime(), points, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
				camera->addAnimator(animator);
				animator->drop();

				toSwitchCamera->setPosition(newPos);
				toSwitchCamera->setTarget(activeCamera->getPosition());
				toSwitchCamera->updateAbsolutePosition();
			}
		}

		if ( toSwitchCamera == CameraController_->GetTopCamera().get() )
		{
			if ( CameraController_->GetFPSCamera().get() == activeCamera )
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

				animator = new CCameraFollowSplineAnimator(GetRenderContextSPtr()->Timer_->getTime(), points, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
				camera->addAnimator(animator);
				animator->drop();

				toSwitchCamera->setPosition(newPos);
				toSwitchCamera->setTarget(newTarget);
				toSwitchCamera->updateAbsolutePosition();
			}

			if ( CameraController_->GetMayaCamera().get() == activeCamera )
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

				animator = new CCameraFollowSplineAnimator(GetRenderContextSPtr()->Timer_->getTime(), points, tarPoints, upVectors, 1.5f, 1.5f, 1.5f);
				camera->addAnimator(animator);
				animator->drop();

				toSwitchCamera->setPosition(newPos);
				toSwitchCamera->setTarget(newTarget);
				toSwitchCamera->updateAbsolutePosition();
			}
		}

		FlyCameraController_->SetFlying(toSwitchCamera, camera, animator);
		StatesController_->SetCurrentState(ERenderState::ERS_ANIMATION);

		return true;
	}

	DesignODLSPtr	GetDesignSPtr() const { return std::static_pointer_cast<DesignODL>(DesignODL_.lock()); }
};

DesignODL::DesignODL(HWND Hwnd):ImpSPtr_(new Imp),BaseODL(SRenderContextWPtr())
{
	Hwnd_ = Hwnd;
}


DesignODL::~DesignODL(void)
{

}

void DesignODL::Init()
{
	auto& imp_ = *ImpSPtr_;

	{//RC
		RenderContext_ = IrrEngine::CreateRenderContext(Hwnd_, irr::video::SColor(~0));
		RenderContext_->Init();
		imp_.SetRenderContextWPtr(RenderContext_);
	}

	{//Imp
		imp_.TopPickingController_ = std::make_shared<TopPickingController>(RenderContext_);
		imp_.StatesController_ = std::make_shared<StatesController<ERenderState>>(RenderContext_);
		imp_.GUIController_ = std::make_shared<GUIController>(RenderContext_);
		imp_.CameraController_ = std::make_shared<CameraController>(RenderContext_);
		imp_.FlyCameraController_ = std::make_shared<FlyCameraController>(RenderContext_);
		imp_.UpdateTransformingCtrller_ = std::make_shared<UpdateTransformingCtrller>(RenderContext_);

		ImpSPtr_->DesignODL_ = shared_from_this();
		ImpSPtr_->TopPickingController_->SetRootODL(shared_from_this());
		ImpSPtr_->UpdateTransformingCtrller_->SetRootODL(shared_from_this());
	}

	{//DataSceneNode
		auto spNewNode = CCombineSceneNode::Create(RenderContext_, shared_from_this());
		SetDataSceneNode(spNewNode);
		RenderContext_->Smgr_->getRootSceneNode()->addChild(spNewNode.get());
	}

	auto graph = CreateChild<GraphODL>(RenderContext_);

	{//Controller
		RenderContext_->PushController(ImpSPtr_->StatesController_);

		imp_.StatesController_->SetCurrentState(ERenderState::ERS_TOP_VIEW);
		RenderContext_->PushController(std::static_pointer_cast<IRenderController>(ImpSPtr_));

		auto gridCtrller = std::make_shared<GridController>(RenderContext_);

		{//TopView
			imp_.StatesController_->AddController(ERenderState::ERS_TOP_VIEW, imp_.GUIController_);
			imp_.StatesController_->AddController(ERenderState::ERS_TOP_VIEW, imp_.UpdateTransformingCtrller_);
			imp_.StatesController_->AddController(ERenderState::ERS_TOP_VIEW, imp_.CameraController_);
			imp_.StatesController_->AddController(ERenderState::ERS_TOP_VIEW, gridCtrller);

			//����ͼ
			imp_.StatesController_->AddController(ERenderState::ERS_TOP_VIEW, std::make_shared<RoomLayoutCtrller>(graph, RenderContext_));
		}

		{//MayaView
			imp_.StatesController_->AddController(ERenderState::ERS_MAYA_VIEW, imp_.GUIController_);
			imp_.StatesController_->AddController(ERenderState::ERS_MAYA_VIEW, imp_.UpdateTransformingCtrller_);
			imp_.StatesController_->AddController(ERenderState::ERS_MAYA_VIEW, imp_.CameraController_);
		}

		{//FPSView
			imp_.StatesController_->AddController(ERenderState::ERS_FPS_VIEW, imp_.GUIController_);
			imp_.StatesController_->AddController(ERenderState::ERS_FPS_VIEW, imp_.UpdateTransformingCtrller_);
			imp_.StatesController_->AddController(ERenderState::ERS_FPS_VIEW, imp_.CameraController_);
		}

		{//Animator
			imp_.StatesController_->AddController(ERenderState::ERS_ANIMATION, imp_.FlyCameraController_);
			imp_.StatesController_->AddController(ERenderState::ERS_ANIMATION, imp_.UpdateTransformingCtrller_);
		}
	}
}
