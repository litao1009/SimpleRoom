#ifndef CTopoShape_h__
#define CTopoShape_h__

#pragma once

#include "gp_Dir.hxx"

#include "TopoDS_Face.hxx"
#include "TopoDS_Wire.hxx"
#include "TopoDS_Edge.hxx"
#include "TopoDS_Vertex.hxx"

#include "GeomLProp_SLProps.hxx"
#include "TopTools_IndexedMapOfShape.hxx"
#include "TopTools_IndexedDataMapOfShapeListOfShape.hxx"
#include "BRepExtrema_DistShapeShape.hxx"

class	CTopoShape
{

public:

	CTopoShape(const TopoDS_Face& face);
	CTopoShape();

public:

	void	SetFace(const TopoDS_Face& face);

	bool	IsValid() const
	{
		return Standard_False == m_BaseFace.IsNull();
	}
	
	TopoDS_Face&	GetFace()
	{
		return m_BaseFace;
	}

	const TopoDS_Face& GetFace() const
	{
		return m_BaseFace;
	}

	const TopTools_IndexedMapOfShape&	GetWires() const
	{
		return m_AllWire;
	}

	const TopTools_IndexedMapOfShape&	GetEdges() const
	{
		return m_AllEdge;
	}

	const TopTools_IndexedMapOfShape&	GetVertex() const
	{
		return m_AllVertex;
	}

	const gp_Dir&	GetFaceDir() const
	{
		return m_FaceNormal;
	}

	void	SetTranslation(const gp_Trsf& translation);

	BRepExtrema_DistShapeShape& GetDisSS()
	{
		return m_DisSS;
	}

	Standard_Real	GetURange() const
	{
		return m_uRange;
	}

	Standard_Real	GetVRange() const
	{
		return m_vRange;
	}

private:

	Standard_Real m_uMin, m_uMax, m_vMin, m_vMax;
	Standard_Real				m_uRange,m_vRange;
	gp_Dir						m_FaceNormal;
	GeomLProp_SLProps			m_FaceProps;
	Handle(Geom_Surface)		m_Surface;
	TopoDS_Face					m_BaseFace;
	TopTools_IndexedMapOfShape	m_AllWire;
	TopTools_IndexedMapOfShape	m_AllEdge;
	TopTools_IndexedMapOfShape	m_AllVertex;
	BRepExtrema_DistShapeShape	m_DisSS;
	
};
#endif // CTopoShape_h__
