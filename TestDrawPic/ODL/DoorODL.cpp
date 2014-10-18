#include "stdafx.h"

#include "DoorODL.h"
#include "WallODL.h"

#include "ODL/MeshSceneNode/DoorMeshNode2D.h"

#include "BRepPrimAPI_MakeBox.hxx"

class	CDoorODL::Imp
{
public:

	Imp()
	{
		MeshNode2D_ = nullptr;

		XLength_ = 0;
		YLength_ = 0;
		ZLength_ = 0;
	}

	DoorMeshNode2D*	MeshNode2D_;
	float			XLength_,YLength_,ZLength_;
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

void CDoorODL::UpdateZone( float xLength, float yLength, float zLength )
{
	auto& xLen_ = ImpUPtr_->XLength_;
	auto& yLen_ = ImpUPtr_->YLength_;
	auto& zLen_ = ImpUPtr_->ZLength_;

	auto xDiff = std::abs(xLen_ - xLength);
	auto yDiff = std::abs(yLen_ - yLength);
	auto zDiff = std::abs(zLen_ - zLength);

	if ( xDiff < 0.001 && yDiff < 0.001 && zDiff < 0.001 )
	{
		return;
	}

	xLen_ = xLength;
	yLen_ = yLength;
	zLen_ = zLength;

	ImpUPtr_->MeshNode2D_->UpdateMesh(xLen_, zLen_);

	auto box = BRepPrimAPI_MakeBox(xLen_, yLen_, zLen_).Shape();
	gp_Trsf tfs;
	tfs.SetTranslationPart(gp_Vec(gp_Pnt(xLen_/2, yLen_/2, zLen_/2), gp::Origin()));
	box.Move(tfs);
	SetBaseShape(box);
}

void CDoorODL::Set2DLineColor( const irr::video::SColor& clr )
{
	ImpUPtr_->MeshNode2D_->SetLineColor(clr);
}

void CDoorODL::Init()
{
	auto node = new DoorMeshNode2D(GetDataSceneNode()->GetSceneNode2D());
	ImpUPtr_->MeshNode2D_ = node;
	UpdateZone(900, 2000, 200);
}

void CDoorODL::Draw2DMesh()
{
	ImpUPtr_->MeshNode2D_->render();
}

std::tuple<float,float,float> CDoorODL::GetZone() const
{
	return std::make_tuple(ImpUPtr_->XLength_, ImpUPtr_->YLength_, ImpUPtr_->ZLength_);
}
