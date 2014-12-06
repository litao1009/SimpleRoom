#include "stdafx.h"

#include "RoomODL.h"
#include "CornerODL.h"
#include "FloorODL.h"

#include "SMesh.h"
#include "SMeshBuffer.h"
#include "irrEngine/IrrExtension/FreetypeFontManager.h"
#include "irrEngine/SRenderContext.h"
#include "irrEngine/irrEngine.h"

#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "GProp_GProps.hxx"
#include "BRepGProp.hxx"
//#include "StlAPI.hxx"

#include <boost/format.hpp>

class	RoomODL::Imp
{
public:

	Imp()
	{
		NameMesh_ =nullptr;
		AreaMesh_ = nullptr;
		FirstBuild_ = true;
	}

	static	std::wstring	NewRoomName()
	{
		static unsigned nameCount = 0;

		//std::wstring newName = L"房间 " + std::to_wstring(nameCount++);
		std::wstring newName = L"未命名房间";
		return newName;
	}

public:

	gp_Pnt			Center_;
	bool			FirstBuild_;
	FloorODLWPtr	Floor_;
	std::wstring	RoomName_;
	irr::scene::IMeshSceneNode*	NameMesh_;
	irr::scene::IMeshSceneNode*	AreaMesh_;
};

RoomODL::RoomODL( const SRenderContextWPtr& rc ):BaseODL(rc),ImpUPtr_(new Imp)
{

}

RoomODL::~RoomODL()
{

}

void RoomODL::SetWallList( const WallODLList& list )
{
	WallODLWList vals;
	vals.reserve(list.size());
	for ( auto & curWall : list )
	{
		vals.push_back(curWall);
	}

	WallList_ = vals;
}

void RoomODL::SetCornerList( const CornerODLList& list )
{
	CornerODLWList vals;
	vals.reserve(list.size());
	for ( auto & curCorner : list )
	{
		vals.push_back(curCorner);
	}

	CornerList_ = vals;
}

WallODLList RoomODL::GetWallList() const
{
	WallODLList vals;
	vals.reserve(WallList_.size());
	for ( auto & curWall : WallList_ )
	{
		vals.push_back(curWall.lock());
	}

	return vals;
}

CornerODLList RoomODL::GetCornerList() const
{
	CornerODLList vals;
	vals.reserve(CornerList_.size());
	for ( auto & curCorner : CornerList_ )
	{
		vals.push_back(curCorner.lock());
	}

	return vals;
}

bool RoomODL::Build()
{
	auto cornerList = GetCornerList();
	if ( cornerList.size() < 3 )
	{
		return false;
	}

	BRepBuilderAPI_MakePolygon mp;

	for ( auto& curCorner : cornerList )
	{
		mp.Add(curCorner->GetPosition());
	}
	mp.Close();

	if ( Standard_False == mp.IsDone() )
	{
		return false;
	}

	auto face = BRepBuilderAPI_MakeFace(mp.Wire()).Face();

	if ( Standard_True == face.IsNull() )
	{
		return false;
	}

	SetBaseShape(face);

	{//floor
		if ( !ImpUPtr_->Floor_.expired() )
		{
			ImpUPtr_->Floor_.lock()->RemoveFromParent();
		}

		auto floor = FloorODL::CreateByFace(GetRenderContextWPtr(), face);
		AddChild(floor);

		ImpUPtr_->Floor_ = floor;
	}

	if ( ImpUPtr_->FirstBuild_ )
	{
		ImpUPtr_->FirstBuild_ = false;

		{
			auto lableMeshBuf = ODLTools::NEW_CreateRectMeshBuffer(0.5f);
			lableMeshBuf->getMaterial().DiffuseColor = 0xFF000000;
			lableMeshBuf->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_FONT);

			auto lableMesh = new irr::scene::SMesh;
			lableMesh->addMeshBuffer(lableMeshBuf);
			lableMeshBuf->recalculateBoundingBox();

			auto lableNode = GetDataSceneNode()->getSceneManager()->addMeshSceneNode(lableMesh, GetDataSceneNode()->GetSceneNode2D());
			ImpUPtr_->NameMesh_ = lableNode;

			lableMeshBuf->drop();
			lableMesh->drop();
		}

		{
			auto lableMeshBuf = ODLTools::NEW_CreateRectMeshBuffer(0.5f);
			lableMeshBuf->getMaterial().DiffuseColor = 0xFF000000;
			lableMeshBuf->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_FONT);

			auto lableMesh = new irr::scene::SMesh;
			lableMesh->addMeshBuffer(lableMeshBuf);
			lableMeshBuf->recalculateBoundingBox();

			auto lableNode = GetDataSceneNode()->getSceneManager()->addMeshSceneNode(lableMesh, GetDataSceneNode()->GetSceneNode2D());
			ImpUPtr_->AreaMesh_ = lableNode;

			lableMeshBuf->drop();
			lableMesh->drop();
		}
	}

	GProp_GProps System;
	BRepGProp::SurfaceProperties(face,System);
	ImpUPtr_->Center_ = System.CentreOfMass();
	auto& massCenter = ImpUPtr_->Center_;

	{//name
		if ( ImpUPtr_->RoomName_.empty() )
		{
			ImpUPtr_->RoomName_ = Imp::NewRoomName();
			auto font = FreetypeFontMgr::GetInstance().GetTtFont(GetRenderContextWPtr().lock()->Smgr_->getVideoDriver(), "simsun.ttc", 64);
			assert(font);

			auto txtMesh = font->GenerateTextMesh(ImpUPtr_->RoomName_.c_str());
			ImpUPtr_->NameMesh_->setMesh(txtMesh);
			txtMesh->drop();

			ImpUPtr_->NameMesh_->setScale(irr::core::vector3df(200.f, 1, 200.f));
			ImpUPtr_->NameMesh_->setPosition(irr::core::vector3df(static_cast<float>(massCenter.X()), 0, static_cast<float>(massCenter.Z())-100));
		}
		else
		{
			ImpUPtr_->NameMesh_->setPosition(irr::core::vector3df(static_cast<float>(massCenter.X()), 0, static_cast<float>(massCenter.Z())-100));
		}
	}

	{//Area
		auto area = System.Mass();
		area /= 1000 * 1000;

		boost::wformat fmt(L"%.2f");
		fmt % area;

		auto font = FreetypeFontMgr::GetInstance().GetTtFont(GetRenderContextWPtr().lock()->Smgr_->getVideoDriver(), "arial.ttf", 32);
		assert(font);

		auto txtMesh = font->GenerateTextMesh((fmt.str()+L"m2").c_str());
		ImpUPtr_->AreaMesh_->setMesh(txtMesh);
		txtMesh->drop();

		ImpUPtr_->AreaMesh_->setScale(irr::core::vector3df(150.f, 1, 150.f));
		ImpUPtr_->AreaMesh_->setPosition(irr::core::vector3df(static_cast<float>(massCenter.X()), 0, static_cast<float>(massCenter.Z())+100));
	}
	
	return true;
}

double RoomODL::GetArea() const
{
	auto shape = GetBaseShape();
	assert(Standard_False == shape.IsNull());

	GProp_GProps System;
	BRepGProp::SurfaceProperties(shape,System);
	
	return System.Mass();
}

void RoomODL::SetName( const std::wstring& name )
{
	ImpUPtr_->RoomName_ = name;

	auto font = FreetypeFontMgr::GetInstance().GetTtFont(GetRenderContextWPtr().lock()->Smgr_->getVideoDriver(), "simsun.ttc", 64);
	assert(font);

	auto txtMesh = font->GenerateTextMesh(ImpUPtr_->RoomName_.c_str());
	ImpUPtr_->NameMesh_->setMesh(txtMesh);
}

const std::wstring& RoomODL::GetName() const
{
	return ImpUPtr_->RoomName_;
}

const gp_Pnt& RoomODL::GetCenter() const
{
	return ImpUPtr_->Center_;
}

void RoomODL::UpdateSweeping()
{
	if ( IsSweeping() )
	{
		ImpUPtr_->NameMesh_->getMaterial(0).DiffuseColor = 0xFFFFFF00;
	}
	else
	{
		ImpUPtr_->NameMesh_->getMaterial(0).DiffuseColor = 0xFF000000;
	}
}

void RoomODL::UpdatePicking()
{
	if ( IsPicking() )
	{
		ImpUPtr_->NameMesh_->getMaterial(0).DiffuseColor = 0xFF0000FF;
	}
	else
	{
		ImpUPtr_->NameMesh_->getMaterial(0).DiffuseColor = 0xFF000000;
	}
}
