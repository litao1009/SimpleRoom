#include "stdafx.h"
#include "DesignODL.h"
#include "irrEngine/irrEngine.h"
#include "irrEngine/SRenderContext.h"
#include "irrEngine/Implemention/CompositionController.h"
#include "irrEngine/Implemention/ResizeController.h"
#include "SceneNode/CDataSceneNode.h"
#include "CameraODL.h"


CDesignODL::CDesignODL(HWND Hwnd)
{
	m_ObjType = EOBT_Design;
	m_spRenderContext =  IrrEngine::CreateRenderContext(Hwnd);
	m_spRenderContext->Init();

	auto controller = std::make_shared<CompositionController>();
	m_spRenderContext->PushController(std::make_shared<ResizeController>(Hwnd));
	m_spRenderContext->PushController(controller);
}


CDesignODL::~CDesignODL(void)
{
}

void CDesignODL::OnDrawTop( Gdiplus::Graphics& gcDrawer, float fScale)
{

	if (IsHide())
	{
		return;
	}
	InitTopColor();
	for ( auto& curChild : GetChildrenList() )
	{
		if (m_spCamera!=curChild)
		{
			curChild->OnDrawTop(gcDrawer, fScale);
		}
	}

	if (m_spCamera)
	{
		auto camera = m_spRenderContext->Smgr_->getActiveCamera();
		auto pos = camera->getPosition();
		auto target = camera->getTarget();
		//m_spCamera->SetCameraInfo(m_spRenderContext->IsCameraVisible(), gp_Pnt(pos.X, pos.Y, pos.Z), gp_Pnt(target.X, target.Y, target.Z), camera->getFOV());
		m_spCamera->OnDrawTop(gcDrawer, fScale);
	}
}
void CDesignODL::Init()
{
	auto newNode = new CDataSceneNode(m_spRenderContext, shared_from_this(), "DesignODL");
	auto spNewNode = CDataSceneNode::TranslateNodeToSPtr(newNode);
	m_spRenderContext->Smgr_->getRootSceneNode()->addChild(spNewNode.get());
	SetSceneNode(spNewNode);

	m_spCamera = std::make_shared<CCameraODL>(m_spRenderContext->Smgr_.get());
	AddChild(m_spCamera);
}
