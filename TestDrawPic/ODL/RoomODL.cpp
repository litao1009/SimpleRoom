#include "stdafx.h"

#include "RoomODL.h"
#include "CornerODL.h"
#include "FloorODL.h"

#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "GProp_GProps.hxx"
#include "BRepGProp.hxx"

class	RoomODL::Imp
{
public:

};

RoomODL::RoomODL( const SRenderContextWPtr& rc ):CBaseODL(rc),ImpUPtr_(new Imp)
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

	auto floor = FloorODL::CreateByFace(GetRenderContextWPtr(), face);
	this->AddChild(floor);

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
