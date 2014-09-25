#include "stdafx.h"

#include "CTopoShape.h"
#include "TopExp.hxx"
#include "BRepTools.hxx"
#include "BRep_Tool.hxx"
#include "BRepTools_WireExplorer.hxx"
#include "TopoDS.hxx"
//#include "ShapeFix_Shape.hxx"
//#include "ShapeFix_Face.hxx"
//#include "GProp_GProps.hxx"
//#include "BRepGProp.hxx"	//ÇóÃæ»ý


CTopoShape::CTopoShape( const TopoDS_Face& face ):m_FaceProps(1,1)
{
	SetFace(face);
	m_uMax = m_uMin = m_vMax = m_vMin = 0;
}

CTopoShape::CTopoShape():m_FaceProps(1,1)
{

}

void CTopoShape::SetFace( const TopoDS_Face& face )
{
	m_BaseFace = face;

	BRepTools::UVBounds (m_BaseFace, m_uMin, m_uMax, m_vMin, m_vMax);
	m_Surface = BRep_Tool::Surface(m_BaseFace);
	m_FaceProps = GeomLProp_SLProps(m_Surface, m_uMin, m_vMin, 1, 0.001);
	m_uRange = std::abs(m_uMax-m_uMin);
	m_vRange = std::abs(m_vMax-m_vMin);

	m_FaceNormal = m_FaceProps.Normal();
	if ( TopAbs_REVERSED == m_BaseFace.Orientation() )
	{
		m_FaceNormal.Reverse();
	}

	m_AllWire.Clear();
	m_AllEdge.Clear();
	m_AllVertex.Clear();

	TopExp::MapShapes(m_BaseFace, TopAbs_WIRE, m_AllWire);
	TopExp::MapShapes(m_BaseFace, TopAbs_EDGE, m_AllEdge);

	auto& wire = m_AllWire.FindKey(1);
	for ( BRepTools_WireExplorer wireExp(TopoDS::Wire(wire)); wireExp.More(); wireExp.Next() )
	{
		m_AllVertex.Add(wireExp.CurrentVertex());
	}

	m_DisSS.LoadS2(m_BaseFace);
}

void CTopoShape::SetTranslation( const gp_Trsf& translation )
{
	auto newFace = m_BaseFace;

	newFace.Move(translation);
	SetFace(newFace);
}
