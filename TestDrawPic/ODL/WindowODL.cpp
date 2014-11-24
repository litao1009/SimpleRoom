#include "stdafx.h"

#include "WindowODL.h"
#include "ODL/MeshSceneNode/WindowMeshNode2D.h"

class	WindowODL::Imp
{
public:

	Imp()
	{
		MeshNode2D_ = nullptr;
	}

	WindowMeshNode2D*	MeshNode2D_;
	float				OffsetHeight_;
};

WindowODL::WindowODL(const SRenderContextWPtr& rc, float offsetHeight):HoleODL(rc),ImpUPtr_(new Imp)
{
	ImpUPtr_->OffsetHeight_ = offsetHeight;
}

WindowODL::~WindowODL()
{
	if ( ImpUPtr_->MeshNode2D_ )
	{
		ImpUPtr_->MeshNode2D_->drop();
	}
}

void WindowODL::UpdateHole()
{
	HoleODL::UpdateHole();

	auto size = GetHoleSize();

	ImpUPtr_->MeshNode2D_->UpdateMesh(static_cast<float>(size.X()), static_cast<float>(size.Z()));
}

void WindowODL::Set2DLineColor( const irr::video::SColor& clr )
{
	ImpUPtr_->MeshNode2D_->SetLineColor(clr);
}

void WindowODL::Init()
{
	auto node = new WindowMeshNode2D(GetDataSceneNode()->GetSceneNode2D());
	node->setPosition(irr::core::vector3df(0,400,0));
	ImpUPtr_->MeshNode2D_ = node;
	SetHoleSize(900, 2000, 200);
	SetOffsetSize(0, 0, 50);
	UpdateHole();
}

void WindowODL::UpdateSweeping()
{
	if ( IsSweeping() )
	{
		Set2DLineColor(irr::video::SColor(0xFFFFFF00));
	}
	else
	{
		Set2DLineColor(irr::video::SColor(0xFF8F8F8F));
	}
}

void WindowODL::UpdatePicking()
{
	if ( IsPicking() )
	{
		Set2DLineColor(irr::video::SColor(0xFF0000FF));
	}
	else
	{
		Set2DLineColor(irr::video::SColor(0xFF8F8F8F));
	}
}

void WindowODL::SetOffsetHeight( float val )
{
	ImpUPtr_->OffsetHeight_ = val;
}

float WindowODL::GetOffsetHeight() const
{
	return ImpUPtr_->OffsetHeight_;
}
