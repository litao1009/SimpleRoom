#include "stdafx.h"

#include "RoomODL.h"


RoomODL::RoomODL( const SRenderContextWPtr& rc ):CBaseODL(rc)
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
