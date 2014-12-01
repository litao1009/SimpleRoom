#include "stdafx.h"

#include "PillarODL.h"

#include "ODL/MeshSceneNode/PillarMeshNode2D.h"
#include "SMesh.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"

#include "BRepPrimAPI_MakeBox.hxx"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

class	PillarODL::Imp
{
public:

	Imp()
	{
		OffsetHeight_ = 0;
		MeshNode2D_ =nullptr;
	}

public:

	PillarMeshNode2D*	MeshNode2D_;
	float				OffsetHeight_;
	gp_XYZ				Size_;
	BaseODLWList		AlignList_;
};

PillarODL::PillarODL( const SRenderContextWPtr& rc ):BaseODL(rc), ImpUPtr_(new Imp)
{

}

PillarODL::~PillarODL()
{

}

float PillarODL::GetOffsetHeight() const
{
	return ImpUPtr_->OffsetHeight_;
}

void PillarODL::SetOffsetHeight( float val )
{
	ImpUPtr_->OffsetHeight_ = val;
}

void PillarODL::SetSize( float xLength, float yLength, float zLength )
{
	ImpUPtr_->Size_ = gp_XYZ(xLength, yLength, zLength);
}

void PillarODL::SetSize( const gp_XYZ& size )
{
	ImpUPtr_->Size_ = size;
}

const gp_XYZ& PillarODL::GetSize() const
{
	return ImpUPtr_->Size_;
}

void PillarODL::UpdateMesh()
{
	auto solid = BRepPrimAPI_MakeBox(gp_Pnt((ImpUPtr_->Size_/2).Reversed()), gp_Pnt(ImpUPtr_->Size_/2)).Shape();
	SetBaseShape(solid);

	auto meshbuf = ODLTools::NEW_CreateMeshBuffer(solid);
	auto mesh = new SMesh;
	mesh->addMeshBuffer(meshbuf);
	mesh->recalculateBoundingBox();

	GetDataSceneNode()->setMesh(mesh);

	meshbuf->drop();
	mesh->drop();

	ImpUPtr_->MeshNode2D_->UpdateMesh(static_cast<float>(ImpUPtr_->Size_.X()), static_cast<float>(ImpUPtr_->Size_.Z()));

	GetDataSceneNode()->AddToDepthPass();
	SetDefaultTexture();
}

void PillarODL::Init()
{
	auto node = new PillarMeshNode2D(GetDataSceneNode()->GetSceneNode2D());
	node->setPosition(irr::core::vector3df(0,400,0));
	ImpUPtr_->MeshNode2D_ = node;
	node->drop();
}

void PillarODL::SetDefaultTexture()
{
	auto tex = GetDataSceneNode()->getSceneManager()->getVideoDriver()->getTexture("../Data/Resource/3D/wall.jpg");
	assert(tex);

	auto uLen = 4000.f;//tex->getSize().Width;
	auto vLen = 3000.f;//tex->getSize().Height;

	irr::core::matrix4 texMat;
	texMat.setScale(irr::core::vector3df(1/uLen, 1/vLen, 1));
	auto meshBufferPtr = GetDataSceneNode()->getMesh()->getMeshBuffer(0);

	meshBufferPtr->getMaterial().setTextureMatrix(0, texMat);
	GetDataSceneNode()->setMaterialTexture(0, tex);
}

BaseODLList PillarODL::GetAlignList() const
{
	BaseODLList ret;

	for ( auto& curODL : ImpUPtr_->AlignList_ )
	{
		ret.push_back(curODL.lock());
	}

	return ret;
}

void PillarODL::SetAlignList( const BaseODLList& list )
{
	BaseODLWList toSave;
	for ( auto& curODL : list )
	{
		toSave.push_back(curODL);
	}

	ImpUPtr_->AlignList_ = toSave;
}

void PillarODL::UpdatePicking()
{
	ImpUPtr_->MeshNode2D_->SetPicking(IsPicking());
}

void PillarODL::UpdateSweeping()
{
	ImpUPtr_->MeshNode2D_->SetSweeping(IsSweeping());
}

void PillarODL::AddAlign( const BaseODLSPtr& align )
{
	ImpUPtr_->AlignList_.push_back(align);
}

void PillarODL::SetVaildPosition( bool val )
{
	ImpUPtr_->MeshNode2D_->SetLineColor(val ? 0xFF8F8F8F : 0xFFFF0000);
}

void PillarODL::Update2DMesh()
{
	ImpUPtr_->MeshNode2D_->UpdateMesh(static_cast<float>(ImpUPtr_->Size_.X()), static_cast<float>(ImpUPtr_->Size_.Z()));
}

