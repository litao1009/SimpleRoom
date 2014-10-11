#include "stdafx.h"

#include "DoorODL.h"
#include "ODL/MeshSceneNode/DoorMeshNode2D.h"

class	CDoorODL::Imp
{
public:

	Imp()
	{
		MeshNode2D_ = nullptr;
	}

	DoorMeshNode2D*	MeshNode2D_;
};

CDoorODL::CDoorODL():ImpUPtr_(new Imp)
{
	
}

CDoorODL::~CDoorODL()
{
	if ( ImpUPtr_->MeshNode2D_ )
	{
		ImpUPtr_->MeshNode2D_->drop();
	}
}

void CDoorODL::Update2DZone( float xLength, float zLength )
{
	ImpUPtr_->MeshNode2D_->UpdateMesh(xLength, zLength);
}

void CDoorODL::Set2DLineColor( const irr::video::SColor& clr )
{
	ImpUPtr_->MeshNode2D_->SetLineColor(clr);
}

void CDoorODL::Init()
{
	auto node = new DoorMeshNode2D(GetDataSceneNode()->GetSceneNode2D());
	node->UpdateMesh(900, 300);
	ImpUPtr_->MeshNode2D_ = node;
}

void CDoorODL::Draw2DMesh()
{
	ImpUPtr_->MeshNode2D_->render();
}
