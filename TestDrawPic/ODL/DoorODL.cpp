#include "stdafx.h"

#include "DoorODL.h"
#include "WallODL.h"

#include "ODL/MeshSceneNode/DoorMeshNode2D.h"

#include "BRepPrimAPI_MakeBox.hxx"

class	DoorODL::Imp
{
public:

	Imp()
	{
		MeshNode2D_ = nullptr;
	}

	DoorMeshNode2D*	MeshNode2D_;
};

DoorODL::DoorODL(const SRenderContextWPtr& rc):HoleODL(rc),ImpUPtr_(new Imp)
{
	
}

DoorODL::~DoorODL()
{
	if ( ImpUPtr_->MeshNode2D_ )
	{
		ImpUPtr_->MeshNode2D_->drop();
	}
}

void DoorODL::UpdateHole()
{
	HoleODL::UpdateHole();
	
	auto size = GetHoleSize();

	ImpUPtr_->MeshNode2D_->UpdateMesh(static_cast<float>(size.X()), static_cast<float>(size.Z()));
}

void DoorODL::Set2DLineColor( const irr::video::SColor& clr )
{
	ImpUPtr_->MeshNode2D_->SetLineColor(clr);
}

void DoorODL::Init()
{
	auto node = new DoorMeshNode2D(GetDataSceneNode()->GetSceneNode2D());
	node->setPosition(irr::core::vector3df(0,400,0));
	ImpUPtr_->MeshNode2D_ = node;
	SetHoleSize(900, 2000, 200);
	SetOffsetSize(0, 0, 50);
	UpdateHole();
}