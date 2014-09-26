#include "stdafx.h"

#include "MayaCameraController.h"

#include "irrEngine/SRenderContext.h"

#include "ODL/BaseODL.h"

MayaCameraController::MayaCameraController()
{
	SetName("MayaCameraController");
}

bool MayaCameraController::PreRender3D( const SRenderContext& rc )
{
	if ( !IsEnable() )
	{
		return true;
	}

	auto driver = rc.Smgr_->getVideoDriver();
	auto smgr = rc.Smgr_.get();
	auto guimgr = rc.GUIEnv_.get();
	auto camera = smgr->getActiveCamera();
	auto curcon = rc.CursorControl_.get();

	if ( smgr->getActiveCamera() != MayaCamera_.get() )
	{
		return true;
	}

	irr::core::line3d<irr::f32> ray;
	irr::core::vector3df intersection;
	irr::core::triangle3df hitTriangle;

	ray.start = camera->getPosition();
	ray.end = camera->getTarget();

	auto selectedSceneNode = smgr->getSceneCollisionManager()->getSceneNodeAndCollisionPointFromRay(ray, intersection, hitTriangle, 0xFF );
	while ( selectedSceneNode )
	{
		assert(nullptr!=dynamic_cast<CCombineSceneNode*>(selectedSceneNode));

		auto dsn = static_cast<CCombineSceneNode*>(selectedSceneNode);
		auto odlType = dsn->GetBaseODLSPtr()->GetType();

		if ( EOBT_Wall == odlType ||
			EOBT_Wall_Face == odlType ||
			EOBT_Wall_Face_Zone == odlType ||
			EOBT_Beam == odlType ||
			EOBT_Pillar == odlType ||
			EOBT_Floor == odlType ||
			EOBT_Floor_Zone == odlType)
		{
			dsn->SaveID();
			dsn->setID(0);
			dsn->setVisible(false);
			TmpNodes_.push_back(dsn);
		}
		else
		{
			dsn->SaveID();
			dsn->setID(0);
			TmpNodes_.push_back(dsn);
		}

		selectedSceneNode = smgr->getSceneCollisionManager()->getSceneNodeAndCollisionPointFromRay(ray, intersection, hitTriangle, 0xFF );
	}

	return true;
}

void MayaCameraController::PostRender3D( const SRenderContext& rc )
{
	for (auto& node : TmpNodes_ )
	{
		node->ResetID();
		node->setVisible(true);
	}

	TmpNodes_.clear();
}

MayaCameraController::~MayaCameraController()
{
	if ( MayaCamera_ )
	{
		MayaCamera_->drop();
	}
}

void MayaCameraController::SetMayaCamera( ICameraSceneNodeSPtr camera )
{
	MayaCamera_ = camera;
	MayaCamera_->grab();
}
