#include "stdafx.h"
#include "ObjGroupODL.h"
#include "SceneNode/CDataSceneNode.h"

CObjGroupODL::CObjGroupODL(void)
{
	m_ObjType = EOBT_OBJ_Group;
}


CObjGroupODL::~CObjGroupODL(void)
{
}

void CObjGroupODL::SetIsTopSelected(bool isSelected)
{
	CBaseODL::SetIsTopSelected(isSelected);
	for ( auto& curChild : GetChildrenList() )
	{
		curChild->SetIsTopSelected(isSelected);
	}
}

bool CObjGroupODL::IsFaceInShape( BRepExtrema_DistShapeShape& dss )
{
	int nCount = 0;
	for (auto& curItem : GetChildrenList() )
	{
		if (curItem->IsFaceInShape(dss))
		{
			nCount++;
		}
	}
	return nCount>0;
}

bool CObjGroupODL::HasTopCollisionWith( const TopoDS_Shape& theShape )
{
	int nCount = 0;
	for (auto& curItem : GetChildrenList() )
	{
		if (curItem->HasTopCollisionWith(theShape))
		{
			nCount++;
		}
	}
	return nCount>0;
}

