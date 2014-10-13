#include "stdafx.h"

#include "WindowODL.h"
#include "ODL/MeshSceneNode/WindowMeshNode2D.h"

class	CWindowODL::Imp
{
public:

	Imp()
	{
		MeshNode2D_ = nullptr;
	}

	WindowMeshNode2D*	MeshNode2D_;
};

CWindowODL::CWindowODL():ImpUPtr_(new Imp)
{
	
}

CWindowODL::~CWindowODL()
{
	if ( ImpUPtr_->MeshNode2D_ )
	{
		ImpUPtr_->MeshNode2D_->drop();
	}
}

void CWindowODL::Update2DZone( float xLength, float zLength )
{
	ImpUPtr_->MeshNode2D_->UpdateMesh(xLength, zLength);
}

void CWindowODL::Set2DLineColor( const irr::video::SColor& clr )
{
	ImpUPtr_->MeshNode2D_->SetLineColor(clr);
}

void CWindowODL::Init()
{
	auto node = new WindowMeshNode2D(GetDataSceneNode()->GetSceneNode2D());
	node->UpdateMesh(700, 300);
	ImpUPtr_->MeshNode2D_ = node;
}

void CWindowODL::Draw2DMesh()
{
	ImpUPtr_->MeshNode2D_->render();
}
