#include "stdafx.h"

#include "CornerODL.h"
#include "MeshSceneNode/CornerMeshNode2D.h"

class	CornerODL::Imp
{
public:

	CornerMeshNode2D*	MeshNode2D_;
};


CornerODL::CornerODL(const SRenderContextWPtr& rc):BaseODL(rc),ImpUPtr_(new Imp)
{
	ImpUPtr_->MeshNode2D_ = nullptr;
}

CornerODL::~CornerODL()
{
	if ( ImpUPtr_->MeshNode2D_ )
	{
		ImpUPtr_->MeshNode2D_->drop();
	}
}

void CornerODL::Init()
{
	auto node = new CornerMeshNode2D(GetDataSceneNode()->GetSceneNode2D());
	ImpUPtr_->MeshNode2D_ = node;
}

void CornerODL::SetVisible( bool vis )
{
	GetDataSceneNode()->GetSceneNode2D()->setVisible(vis);
}

bool CornerODL::GetVisible() const
{
	return GetDataSceneNode()->GetSceneNode2D()->isVisible();
}

void CornerODL::SetPosition( const gp_Pnt& pos )
{
	Position_ = pos;
	GetDataSceneNode()->setPosition(irr::core::vector3df(static_cast<float>(pos.X()), static_cast<float>(pos.Y()), static_cast<float>(pos.Z())));
}
