#ifndef ODLTools_h__
#define ODLTools_h__

#pragma once

#include "IMesh.h"
#include "rect.h"

#include "TopoDS_Face.hxx"
#include "gp_Dir.hxx"

#include <vector>
#include <boost/optional.hpp>

class	ODLTools
{
public:

	class	SSingleMesh
	{
	public:

		irr::scene::IMesh*		Mesh_;
		irr::core::vector3df	Translation_;
		irr::core::vector3df	Rotation_;
		irr::core::vector3df	Dir_;
		irr::core::aabbox3df	Box_;
		TopoDS_Shape			Shape_;
	};
	typedef	boost::optional<SSingleMesh>	SSingelMeshOpt;

	class	SMeshSet
	{
	public:
		SSingelMeshOpt				RawMesh_;
		std::vector<SSingelMeshOpt>	Faces_;
	};

public:

	static	SSingelMeshOpt		CreateFaceMesh(TopoDS_Face& faceShape, const gp_Dir& faceDir);

	static	SSingelMeshOpt		CalculateRelation(const TopoDS_Shape& faceShape, const gp_Dir& faceDir);

	static	irr::scene::IMesh*	CreateMesh(TopoDS_Shape& shape);

	static	SMeshSet			CreateMesh(const TopoDS_Shape& shape, const boost::optional<gp_Dir>& refDir = boost::none, bool withIndependentFace = false );

	static	irr::scene::IMesh*	CreateSimpleRectMesh(const irr::core::rectf& rect);

	static	bool				UpdateSimpleRectMesh(irr::scene::IMesh* mesh, const irr::core::rectf& rect);

	static	double	Mix(double vmin, double vmax, double vmix)
	{
		auto range = vmax - vmin;
		auto adjMix = vmix - vmin;

		adjMix /= range;

		return adjMix;
	}

public:

	static	irr::scene::IMeshBuffer*	NEW_CreateMeshBuffer( const TopoDS_Shape& shape );

};
#endif // ODLTools_h__
