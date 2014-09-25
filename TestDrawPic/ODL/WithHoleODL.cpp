#include "stdafx.h"
#include "WithHoleODL.h"

#include "BRepBuilderAPI_Copy.hxx"
#include "BRepAlgoAPI_Cut.hxx"

bool IWithHoleODL::UpdateHoleShape()
{
	if ( Standard_True == m_BaseShape.IsNull() )
	{
		return false;
	}

	TopoDS_Shape tmpShape = BRepBuilderAPI_Copy(m_BaseShape).Shape();

	for ( auto& curHole : m_HoleList )
	{
		auto hole = curHole.lock();
		
		auto holeSolid = hole->GetOffsetHoleShape();
		BRepAlgoAPI_Cut bc(tmpShape, holeSolid);
		tmpShape = bc.Shape();
	}

	m_BuildShape = tmpShape;

	return true;
}

IWithHoleODL::IWithHoleODL()
{
	m_UpdateHoleMesh = true;
}
