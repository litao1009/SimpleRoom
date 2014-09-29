#include "stdafx.h"

#include "DesignODL.h"
#include "ODL/GroupODL.h"
#include "ODL/WallODL.h"

#include "irrEngine/irrEngine.h"
#include "irrEngine/SRenderContext.h"

#include "irrEngine/IrrExtension/CombineSceneNode.h"

#include "irrEngine/Implemention/CompositionController.h"
#include "irrEngine/Implemention/ResizeController.h"
#include "irrEngine/Implemention/GridController.h"
#include "irrEngine/Implemention/2D/DrawingLineWallCtrller.h"
#include "irrEngine/Implemention/2D/DrawingRectWallCtrller.h"

#include "ISceneManager.h"

#include "BRepBuilderAPI_MakeEdge.hxx"
#include "TopoDS_Edge.hxx"
#include "gce_MakeLin.hxx"

#include "ODL/GroupODL.h"
#include "ODL/WallODL.h"

#include "StatusMgr.h"

class	CDesignODL::Imp : public IRenderController
{
public:

	Imp()
	{
		DrawLineWallCtrller_ = std::make_shared<DrawingLineWallCtrller>();
		DrawRectWallCtrller_ = std::make_shared<DrawingRectWallCtrller>();
	}

public://IRenderController

	virtual	bool	OnPostEvent(const irr::SEvent& evt)
	{
		return false;
	}

	virtual	bool	PreRender3D(const SRenderContext& rc)
	{
		auto design = GetDesignSPtr();

		//新建墙
		if ( DrawLineWallCtrller_->GetState() == DrawingLineWallCtrller::EDWLS_FINISH )
		{
			auto wallret = DrawLineWallCtrller_->GetResult();
			if ( wallret.Pnts_.size() <= 1 )
			{
				wallret.Shape_.Nullify();
			}
			DrawLineWallCtrller_->Reset();
			StatusMgr::GetInstance().DrawingState_ = StatusMgr::EDS_NONE;

#pragma message("TODO:墙高配置")
			if ( !wallret.Shape_.IsNull() )
			{
				auto newWalls = CWallODL::CreateWallByBottomFace(design->RenderContext_, wallret.Shape_, wallret.Pnts_, 2600);

				auto newGroup = design->CreateChild<CGroupODL>(design->RenderContext_);
				for ( auto& wall : newWalls )
				{
					newGroup->AddChild(wall);
				}
			}
		}

		//新建墙
		if ( DrawRectWallCtrller_->GetState() == DrawingRectWallCtrller::EDWRS_FINISH )
		{
			auto wallret = DrawRectWallCtrller_->GetResult();
			DrawRectWallCtrller_->Reset();
			StatusMgr::GetInstance().DrawingState_ = StatusMgr::EDS_NONE;
#pragma message("TODO:墙高配置")
			if ( std::get<4>(wallret) )
			{
				auto newWalls = CWallODL::CreateWallByRectRange(design->RenderContext_, std::get<0>(wallret), std::get<1>(wallret), std::get<2>(wallret), 2600);

				auto newGroup = design->CreateChild<CGroupODL>(design->RenderContext_);
				for ( auto& wall : newWalls )
				{
					newGroup->AddChild(wall);
				}
			}
		}

		//拣选
		{
			auto curSmgr = GetDesignSPtr()->GetRenderContext()->Smgr_.get();
			assert(curSmgr && curSmgr->getActiveCamera());
			auto curScreenPos = GetDesignSPtr()->GetRenderContext()->CursorControl_->getPosition();
			auto curLine = GetDesignSPtr()->GetRenderContext()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(curScreenPos, curSmgr->getActiveCamera());
			auto curDir = curLine.getVector().normalize();
			
			gce_MakeLin ml(gp_Pnt(curLine.start.X, curLine.start.Y, curLine.start.Z), gp_Dir(curDir.X, curDir.Y, curDir.Z));
			BRepBuilderAPI_MakeEdge me(ml.Value());
			TopoDS_Edge e = me.Edge();
			BRepExtrema_DistShapeShape selector;
			selector.LoadS1(e);

			for ( auto& curChild : GetDesignSPtr()->GetChildrenList() )
			{
				const auto& curShape = curChild->GetBaseShape();
				if ( curShape.IsNull() )
				{
					continue;
				}

				selector.LoadS2(curShape);
				selector.Perform();
				if ( 0 == selector.NbSolution() )
				{
					continue;
				}

				if ( selector.Value() < 50.f )
				{

				}
			}
		}
		

		return true;
	}

	CDesignODLSPtr	GetDesignSPtr() const { return std::static_pointer_cast<CDesignODL>(DesignODL_.lock()); }

public:

	CBaseODLWPtr						DesignODL_;
	DrawingLineWallCtrllerSPtr			DrawLineWallCtrller_;
	DrawingRectWallCtrllerSPtr			DrawRectWallCtrller_;
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
	{//Imp
		ImpSPtr_->DesignODL_ = shared_from_this();
	}
	
	{//RC
		RenderContext_ =  IrrEngine::CreateRenderContext(Hwnd_);
		RenderContext_->Init();
	}

	{//Controller
		RenderContext_->PushController(std::make_shared<ResizeController>(Hwnd_));				//Resize
		RenderContext_->PushController(std::make_shared<GridController>());						//网格吸附
		RenderContext_->PushController(ImpSPtr_->DrawLineWallCtrller_);							//画线生成墙
		RenderContext_->PushController(ImpSPtr_->DrawRectWallCtrller_);							//画框生成墙
		RenderContext_->PushController(std::static_pointer_cast<IRenderController>(ImpSPtr_));	//与三维交互数据
		RenderContext_->PushController(std::make_shared<CompositionController>());				//三维自己的管理器
	}
	
	{//DataSceneNode
		auto spNewNode = CCombineSceneNode::Create(RenderContext_, shared_from_this());
		RenderContext_->Smgr_->getRootSceneNode()->addChild(spNewNode.get());
		SetDataSceneNode(spNewNode);
	}
}
