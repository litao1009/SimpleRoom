#include "stdafx.h"
#include "DesignODL.h"
#include "irrEngine/irrEngine.h"
#include "irrEngine/SRenderContext.h"
#include "irrEngine/Implemention/CompositionController.h"
#include "irrEngine/Implemention/ResizeController.h"
#include "irrEngine/IrrExtension/CombineSceneNode.h"
#include "irrEngine/IRenderController.h"

#include "irrEngine/Implemention/2D/DrawingLineWallCtrller.h"

class	CDesignODL::Imp : public IRenderController
{
public:

	Imp()
	{
		DrawLineWallCtrller_ = std::make_shared<DrawingLineWallCtrller>();
	}

public://IRenderController

	virtual	bool	OnPostEvent(const irr::SEvent& evt)
	{
		if ( evt.EventType == irr::EET_USER_EVENT )
		{
			if ( evt.UserEvent.UserData1 == EUT_FINISH_DRAW_LINE_WALL )
			{
				assert(DrawLineWallCtrller_->GetState() == DrawingLineWallCtrller::EDWLS_FINISH);

				WallFace_ = DrawLineWallCtrller_->GetShape();
				DrawLineWallCtrller_->Reset();

				return true;
			}
		}

		return false;
	}

	virtual	bool	PreRender3D(const SRenderContext& rc)
	{
		if ( WallFace_.IsNull() )
		{
			return true;
		}

		return true;
	}


public:

	CBaseODLWPtr				DesignODL_;
	DrawingLineWallCtrllerSPtr	DrawLineWallCtrller_;
	TopoDS_Shape				WallFace_;
};

CDesignODL::CDesignODL(HWND Hwnd):ImpSPtr_(new Imp)
{
	Hwnd_ = Hwnd;
}


CDesignODL::~CDesignODL(void)
{

}

void CDesignODL::Init()
{
	m_spRenderContext =  IrrEngine::CreateRenderContext(Hwnd_);
	m_spRenderContext->Init();

	m_spRenderContext->PushController(std::make_shared<ResizeController>(Hwnd_));				//Resize
	m_spRenderContext->PushController(ImpSPtr_->DrawLineWallCtrller_);							//画线生成墙
	m_spRenderContext->PushController(std::static_pointer_cast<IRenderController>(ImpSPtr_));	//与三维交互数据
	m_spRenderContext->PushController(std::make_shared<CompositionController>());				//三维自己的管理器

	auto spNewNode = CCombineSceneNode::Create(m_spRenderContext, shared_from_this(), "DesignODL");
	m_spRenderContext->Smgr_->getRootSceneNode()->addChild(spNewNode.get());
	SetDataSceneNode(spNewNode);
}
