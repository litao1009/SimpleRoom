#include "stdafx.h"

#include "ODLTools.h"

//OCCT
#include "BRep_Tool.hxx"
#include "BRepTools.hxx"
#include "Geom_Surface.hxx"
#include "BRepMesh_IncrementalMesh.hxx"
#include "TColgp_Array1OfPnt.hxx"
#include "Poly_Array1OfTriangle.hxx"
#include "Poly_Triangulation.hxx"
#include "GeomAPI_ProjectPointOnSurf.hxx"
#include "TopExp_Explorer.hxx"
#include "TopoDS.hxx"
#include "GeomLProp_SLProps.hxx"
#include "Bnd_Box.hxx"
#include "BRepBndLib.hxx"
#include "gp_Quaternion.hxx"

//Irrlicht
#include "CDynamicMeshBuffer.h"
#include "SMesh.h"
#include "SMeshBuffer.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

ODLTools::SSingelMeshOpt ODLTools::CreateFaceMesh( TopoDS_Face& faceShape, const gp_Dir& faceDir )
{
	SSingleMesh ret;

	Handle(Geom_Surface) surf = BRep_Tool::Surface(faceShape);
	Standard_Real umin,umax,vmin,vmax;
	BRepTools::UVBounds(faceShape, umin, umax, vmin, vmax);

	BRepMesh_IncrementalMesh(faceShape, 0.1);

	TopLoc_Location L;
	Handle (Poly_Triangulation) polyTriangles = BRep_Tool::Triangulation(faceShape,L);

	if ( polyTriangles.IsNull() )
	{
		return boost::none;
	}

	const TColgp_Array1OfPnt& aNodes = polyTriangles->Nodes();
	const Poly_Array1OfTriangle& triangles = polyTriangles->Triangles();

	auto meshBufferPtr = new CDynamicMeshBuffer(EVT_STANDARD, EIT_32BIT);
	meshBufferPtr->setHardwareMappingHint(EHM_STATIC);
	meshBufferPtr->getVertexBuffer().reallocate(aNodes.Length());
	meshBufferPtr->getIndexBuffer().reallocate(triangles.Length()*3);

	vector3df normal(static_cast<f32>(faceDir.X()), static_cast<f32>(faceDir.Y()), static_cast<f32>(faceDir.Z()));
	//SColor clr(0xff, rand()&0xff, rand()&0xff, rand()&0xff);
	SColor clr(0xff, 0xff, 0xff, 0xff);
	TColgp_Array1OfPnt transNodes(1, aNodes.Length());
	for( auto index=1; index<=aNodes.Length(); ++index )
	{
		auto pnt = aNodes(index).Transformed(L);
		transNodes(index) = pnt;

		GeomAPI_ProjectPointOnSurf projpnta(pnt, surf);
		Quantity_Parameter u,v;
		projpnta.LowerDistanceParameters(u, v);

		auto adjU = Mix(umin, umax, u);
		auto adjV = Mix(vmin, vmax, v);
		vector3df pos(static_cast<f32>(pnt.X()), static_cast<f32>(pnt.Y()), static_cast<f32>(pnt.Z()));
		vector2df coord(static_cast<f32>(adjU),static_cast<f32>(adjV));
		S3DVertex meshVertex(pos, normal, clr, coord);

		meshBufferPtr->getVertexBuffer().push_back(meshVertex);
	}
	meshBufferPtr->recalculateBoundingBox();

	{//重置到原点

		auto& vbuf = meshBufferPtr->getVertexBuffer();
		matrix4 m;

		{//移动中心到原点
			auto bbox = meshBufferPtr->getBoundingBox();
			auto center1 = bbox.getCenter();

			matrix4 mtrans;
			mtrans.setTranslation(-center1);

			m = mtrans * m;

			for ( u32 index=0; index<vbuf.size(); ++index )
			{
				mtrans.translateVect(vbuf[index].Pos);
			}
			meshBufferPtr->recalculateBoundingBox();
		}

		{//旋转到(0,0,1)
			auto angleDir = normal.getHorizontalAngle();

			matrix4 mrotate;

			mrotate.setRotationDegrees(-angleDir);
			m = mrotate * m;

			for ( u32 index=0; index<vbuf.size(); ++index )
			{
				mrotate.transformVect(vbuf[index].Pos);
			}
			meshBufferPtr->recalculateBoundingBox();			
		}

		{//移动原点到左上
			auto bbox = meshBufferPtr->getBoundingBox();
			auto center = bbox.getCenter();
			auto extent = bbox.getExtent();
			auto origin = center;
			origin.X -= extent.X/2;
			origin.Y += extent.Y/2;

			auto vec = center - origin;

			matrix4 mtrans;
			mtrans.setTranslation(vec);
			m = mtrans * m;

			for ( u32 index=0; index<vbuf.size(); ++index )
			{
				mtrans.transformVect(vbuf[index].Pos);
			}
			meshBufferPtr->recalculateBoundingBox();
		}

		m.makeInverse();

		auto trans = m.getTranslation();
		auto rotate = m.getRotationDegrees();

		ret.Translation_ = trans;
		ret.Rotation_ = rotate;
		ret.Box_ = meshBufferPtr->getBoundingBox();
	}

	for( auto index=1,n1=0,n2=0,n3=0; index<=triangles.Length(); ++index )
	{
		triangles(index).Get(n1,n2,n3);

		gp_Vec v1(transNodes(n1), transNodes(n2));
		gp_Vec v2(transNodes(n1), transNodes(n3));

		auto v = v1.Crossed(v2);

		if ( gp_Dir(v).Angle(faceDir) > M_PI_2 )
		{
			meshBufferPtr->getIndexBuffer().push_back(n1-1);
			meshBufferPtr->getIndexBuffer().push_back(n2-1);
			meshBufferPtr->getIndexBuffer().push_back(n3-1);
		}
		else
		{
			meshBufferPtr->getIndexBuffer().push_back(n1-1);
			meshBufferPtr->getIndexBuffer().push_back(n3-1);
			meshBufferPtr->getIndexBuffer().push_back(n2-1);
		}
	}

	meshBufferPtr->getMaterial().PolygonOffsetDirection = EPO_BACK;
	meshBufferPtr->getMaterial().PolygonOffsetFactor = 7;

	auto mesh = new SMesh;
	mesh->addMeshBuffer(meshBufferPtr);
	meshBufferPtr->drop();
	mesh->recalculateBoundingBox();

	ret.Mesh_ = mesh;

	return ret;
}

irr::scene::IMesh* ODLTools::CreateSimpleRectMesh( const irr::core::rectf& rect )
{
	vector3df normal(0,0,1);
	SColor clr(255,255,255,255);

	auto meshBufferPtr = new CDynamicMeshBuffer(EVT_STANDARD, EIT_32BIT);
	meshBufferPtr->setHardwareMappingHint(EHM_STATIC);
	meshBufferPtr->getVertexBuffer().reallocate(4);
	meshBufferPtr->getIndexBuffer().reallocate(6);

	meshBufferPtr->getVertexBuffer().push_back(S3DVertex(vector3df(0), normal, clr, vector2df(0,1)));
	meshBufferPtr->getVertexBuffer().push_back(S3DVertex(vector3df(rect.getWidth(), 0, 0), normal, clr, vector2df(1,1)));
	meshBufferPtr->getVertexBuffer().push_back(S3DVertex(vector3df(rect.getWidth(), -rect.getHeight(), 0), normal, clr, vector2df(1,0)));
	meshBufferPtr->getVertexBuffer().push_back(S3DVertex(vector3df(0, -rect.getHeight(), 0), normal, clr, vector2df(0,0)));

	meshBufferPtr->getIndexBuffer().push_back(0);
	meshBufferPtr->getIndexBuffer().push_back(2);
	meshBufferPtr->getIndexBuffer().push_back(1);
	meshBufferPtr->getIndexBuffer().push_back(0);
	meshBufferPtr->getIndexBuffer().push_back(3);
	meshBufferPtr->getIndexBuffer().push_back(2);

	meshBufferPtr->recalculateBoundingBox();
	meshBufferPtr->getMaterial().PolygonOffsetDirection = EPO_FRONT;
	meshBufferPtr->getMaterial().PolygonOffsetFactor = 1;

	auto mesh = new SMesh;
	mesh->addMeshBuffer(meshBufferPtr);
	meshBufferPtr->drop();
	mesh->recalculateBoundingBox();

	return mesh;
}

bool ODLTools::UpdateSimpleRectMesh( irr::scene::IMesh* mesh, const irr::core::rectf& rect )
{
	if ( nullptr == mesh )
	{
		return false;
	}
	auto meshBufferPtr = static_cast<IDynamicMeshBuffer*>(mesh->getMeshBuffer(0));
	auto& vbuf = meshBufferPtr->getVertexBuffer();

	vbuf[1].Pos = vector3df(rect.getWidth(), 0, 0);
	vbuf[2].Pos = vector3df(rect.getWidth(), -rect.getHeight(), 0);
	vbuf[3].Pos = vector3df(0, -rect.getHeight(), 0);

	meshBufferPtr->recalculateBoundingBox();
	meshBufferPtr->setDirty(EBT_VERTEX);

	static_cast<SMesh*>(mesh)->recalculateBoundingBox();

	return true;
}

irr::scene::IMesh* ODLTools::CreateMesh( TopoDS_Shape& shape )
{
	auto meshBufferPtr = new CDynamicMeshBuffer(EVT_STANDARD, EIT_32BIT);
	meshBufferPtr->setHardwareMappingHint(EHM_STATIC);

	for ( TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next() )
	{
		auto& curFace = TopoDS::Face(exp.Current());

		Handle(Geom_Surface) surf = BRep_Tool::Surface(curFace);
		
		Standard_Real umin,umax,vmin,vmax;
		BRepTools::UVBounds(curFace, umin, umax, vmin, vmax);
		auto faceDir = GeomLProp_SLProps(surf, umin, vmin, 1, 0.001).Normal();
		if ( TopAbs_REVERSED == curFace.Orientation() )
		{
			faceDir.Reverse();
		}

		BRepMesh_IncrementalMesh(curFace, 0.1);

		TopLoc_Location L;
		Handle (Poly_Triangulation) polyTriangles = BRep_Tool::Triangulation(curFace,L);

		if ( polyTriangles.IsNull() )
		{
			return nullptr;
		}

		const TColgp_Array1OfPnt& aNodes = polyTriangles->Nodes();
		const Poly_Array1OfTriangle& triangles = polyTriangles->Triangles();
		
		auto vsize = meshBufferPtr->getVertexBuffer().allocated_size();
		auto isize = meshBufferPtr->getIndexBuffer().allocated_size();
		auto curVertexSize = meshBufferPtr->getVertexBuffer().size();
		meshBufferPtr->getVertexBuffer().reallocate(vsize + aNodes.Length());
		meshBufferPtr->getIndexBuffer().reallocate(isize + triangles.Length()*3);

		vector3df normal(static_cast<f32>(faceDir.X()), static_cast<f32>(faceDir.Y()), static_cast<f32>(faceDir.Z()));

		SColor clr(0xff, 0xff, 0xff, 0xff);
		TColgp_Array1OfPnt transNodes(1, aNodes.Length());
		for( auto index=1; index<=aNodes.Length(); ++index )
		{
			auto pnt = aNodes(index).Transformed(L);
			transNodes(index) = pnt;

			GeomAPI_ProjectPointOnSurf projpnta(pnt, surf);
			Quantity_Parameter u,v;
			projpnta.LowerDistanceParameters(u, v);

			auto adjU = Mix(umin, umax, u);
			auto adjV = Mix(vmin, vmax, v);
			vector3df pos(static_cast<f32>(pnt.X()), static_cast<f32>(pnt.Y()), static_cast<f32>(pnt.Z()));
			vector2df coord(static_cast<f32>(u-umin),static_cast<f32>(v-vmin));
			S3DVertex meshVertex(pos, normal, clr, coord);

			meshBufferPtr->getVertexBuffer().push_back(meshVertex);
		}
		meshBufferPtr->recalculateBoundingBox();

		for( auto index=1,n1=0,n2=0,n3=0; index<=triangles.Length(); ++index )
		{
			triangles(index).Get(n1,n2,n3);

			gp_Vec v1(transNodes(n1), transNodes(n2));
			gp_Vec v2(transNodes(n1), transNodes(n3));

			auto v = v1.Crossed(v2);

			if ( gp_Dir(v).Angle(faceDir) > M_PI_2 )
			{
				meshBufferPtr->getIndexBuffer().push_back(n1-1+curVertexSize);
				meshBufferPtr->getIndexBuffer().push_back(n2-1+curVertexSize);
				meshBufferPtr->getIndexBuffer().push_back(n3-1+curVertexSize);
			}
			else
			{
				meshBufferPtr->getIndexBuffer().push_back(n1-1+curVertexSize);
				meshBufferPtr->getIndexBuffer().push_back(n3-1+curVertexSize);
				meshBufferPtr->getIndexBuffer().push_back(n2-1+curVertexSize);
			}
		}

		meshBufferPtr->getMaterial().PolygonOffsetDirection = EPO_BACK;
		meshBufferPtr->getMaterial().PolygonOffsetFactor = 7;
	}

	auto mesh = new SMesh;
	mesh->addMeshBuffer(meshBufferPtr);
	meshBufferPtr->drop();
	mesh->recalculateBoundingBox();

	return mesh;
}

ODLTools::SMeshSet ODLTools::CreateMesh( const TopoDS_Shape& shape, const boost::optional<gp_Dir>& refDir /*= boost::none*/, bool withIndependentFace /*= false */ )
{
	SMeshSet ret;

	auto totalMeshBuffer = new CDynamicMeshBuffer(EVT_STANDARD, EIT_32BIT);
	totalMeshBuffer->setHardwareMappingHint(EHM_STATIC);
	decltype(totalMeshBuffer) curFaceMeshBuffer = nullptr;

	auto toBuildShape = shape;

	gp_Trsf preTrans;
	
	{//调整Shape
		Bnd_Box rawBox;
		BRepBndLib::Add(toBuildShape, rawBox);
		
		Standard_Real xMin,xMax,yMin,yMax,zMin,zMax;
		rawBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);

		gp_Trsf tranToOriginal, roateToDZ;
		tranToOriginal.SetTranslation(gp_Pnt((xMax+xMin)/2, (yMax+yMin)/2, (zMax+zMin)/2), gp::Origin());
		if ( refDir )
		{
			roateToDZ.SetRotation(gp_Quaternion(*refDir, gp::DZ()));
		}

		preTrans = roateToDZ * tranToOriginal;
		toBuildShape.Move(preTrans);
	}

	{//生成Mesh
		for ( TopExp_Explorer exp(toBuildShape, TopAbs_FACE); exp.More(); exp.Next() )
		{
			auto& curFace = TopoDS::Face(exp.Current());

			Handle(Geom_Surface) surf = BRep_Tool::Surface(curFace);

			Standard_Real umin,umax,vmin,vmax;
			BRepTools::UVBounds(curFace, umin, umax, vmin, vmax);
			auto faceDir = GeomLProp_SLProps(surf, umin, vmin, 1, 0.001).Normal();
			if ( TopAbs_REVERSED == curFace.Orientation() )
			{
				faceDir.Reverse();
			}

			gp_Trsf faceTrans;
			{
				Bnd_Box faceBox;
				BRepBndLib::Add(curFace, faceBox);
				Standard_Real xMin,xMax,yMin,yMax,zMin,zMax;
				faceBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);

				gp_Trsf tranToOriginal, roateToDZ;
				tranToOriginal.SetTranslation(gp_Pnt((xMax+xMin)/2, (yMax+yMin)/2, (zMax+zMin)/2), gp::Origin());

				if ( refDir )
				{
					roateToDZ.SetRotation(gp_Quaternion(*refDir, gp::DZ()));
				}

				faceTrans = roateToDZ * tranToOriginal;				
			}

			BRepMesh_IncrementalMesh(curFace, 0.1);

			TopLoc_Location L;
			Handle (Poly_Triangulation) polyTriangles = BRep_Tool::Triangulation(curFace,L);

			if ( polyTriangles.IsNull() )
			{
				return ret;
			}

			if ( withIndependentFace )
			{
				curFaceMeshBuffer = new CDynamicMeshBuffer(EVT_STANDARD, EIT_32BIT);
				curFaceMeshBuffer->setHardwareMappingHint(EHM_STATIC);
			}

			const TColgp_Array1OfPnt& aNodes = polyTriangles->Nodes();
			const Poly_Array1OfTriangle& triangles = polyTriangles->Triangles();

			auto vsize = totalMeshBuffer->getVertexBuffer().allocated_size();
			auto isize = totalMeshBuffer->getIndexBuffer().allocated_size();
			auto curVertexSize = totalMeshBuffer->getVertexBuffer().size();
			totalMeshBuffer->getVertexBuffer().reallocate(vsize + aNodes.Length());
			totalMeshBuffer->getIndexBuffer().reallocate(isize + triangles.Length()*3);

			vector3df normal(static_cast<f32>(faceDir.X()), static_cast<f32>(faceDir.Y()), static_cast<f32>(faceDir.Z()));

			SColor clr(0xff, 0xff, 0xff, 0xff);
			TColgp_Array1OfPnt transNodes(1, aNodes.Length());
			for( auto index=1; index<=aNodes.Length(); ++index )
			{
				auto pnt = aNodes(index).Transformed(L);
				auto facePnt = pnt.Transformed(faceTrans);

				transNodes(index) = pnt;

				GeomAPI_ProjectPointOnSurf projpnta(pnt, surf);
				Quantity_Parameter u,v;
				projpnta.LowerDistanceParameters(u, v);

				auto adjU = Mix(umin, umax, u);
				auto adjV = Mix(vmin, vmax, v);
				vector3df pos(static_cast<f32>(pnt.X()), static_cast<f32>(pnt.Y()), static_cast<f32>(pnt.Z()));
				vector2df coord(static_cast<f32>(u-umin),static_cast<f32>(v-vmin));
				S3DVertex meshVertex(pos, normal, clr, coord);

				totalMeshBuffer->getVertexBuffer().push_back(meshVertex);

				if ( curFaceMeshBuffer )
				{
					vector3df pos(static_cast<f32>(facePnt.X()), static_cast<f32>(facePnt.Y()), static_cast<f32>(facePnt.Z()));
					S3DVertex faceVertex(pos, normal, clr, coord);

					curFaceMeshBuffer->getVertexBuffer().push_back(faceVertex);
				}
			}

			totalMeshBuffer->recalculateBoundingBox();
			if ( curFaceMeshBuffer )
			{
				curFaceMeshBuffer->recalculateBoundingBox();
			}

			for( auto index=1,n1=0,n2=0,n3=0; index<=triangles.Length(); ++index )
			{
				triangles(index).Get(n1,n2,n3);

				gp_Vec v1(transNodes(n1), transNodes(n2));
				gp_Vec v2(transNodes(n1), transNodes(n3));

				auto v = v1.Crossed(v2);

				if ( gp_Dir(v).Angle(faceDir) > M_PI_2 )
				{
					totalMeshBuffer->getIndexBuffer().push_back(n1-1+curVertexSize);
					totalMeshBuffer->getIndexBuffer().push_back(n2-1+curVertexSize);
					totalMeshBuffer->getIndexBuffer().push_back(n3-1+curVertexSize);

					if ( curFaceMeshBuffer )
					{
						curFaceMeshBuffer->getIndexBuffer().push_back(n1-1);
						curFaceMeshBuffer->getIndexBuffer().push_back(n2-1);
						curFaceMeshBuffer->getIndexBuffer().push_back(n3-1);
					}
				}
				else
				{
					totalMeshBuffer->getIndexBuffer().push_back(n1-1+curVertexSize);
					totalMeshBuffer->getIndexBuffer().push_back(n3-1+curVertexSize);
					totalMeshBuffer->getIndexBuffer().push_back(n2-1+curVertexSize);

					if ( curFaceMeshBuffer )
					{
						curFaceMeshBuffer->getIndexBuffer().push_back(n1-1);
						curFaceMeshBuffer->getIndexBuffer().push_back(n3-1);
						curFaceMeshBuffer->getIndexBuffer().push_back(n2-1);
					}
				}
			}

			if ( curFaceMeshBuffer )
			{
				SSingleMesh newFace;

				faceTrans.Invert();
				auto faceTranslation = faceTrans.TranslationPart();
				auto faceRotation = faceTrans.GetRotation();
				Standard_Real rX,rY,rZ;
				faceRotation.GetEulerAngles(gp_Extrinsic_XYZ, rX, rY, rZ);

				auto newMesh = new SMesh;
				newMesh->addMeshBuffer(curFaceMeshBuffer);
				curFaceMeshBuffer->drop();

				newFace.Shape_ = curFace;
				newFace.Mesh_ = newMesh;
				newFace.Dir_.set(static_cast<float>(faceDir.X()), static_cast<float>(faceDir.Y()), static_cast<float>(faceDir.Z()));
				newFace.Translation_.set(static_cast<float>(faceTranslation.X()), static_cast<float>(faceTranslation.Y()), static_cast<float>(faceTranslation.Z()));
				newFace.Rotation_.set(static_cast<float>(radToDeg(rX)), static_cast<float>(radToDeg(rY)), static_cast<float>(radToDeg(rZ)));
				curFaceMeshBuffer = nullptr;

				ret.Faces_.push_back(newFace);
			}
		}
	}

	auto totoalMesh = new SMesh;
	totoalMesh->addMeshBuffer(totalMeshBuffer);
	totoalMesh->recalculateBoundingBox();
	totalMeshBuffer->drop();

	SSingleMesh retMesh;

	preTrans.Invert();
	auto solidTranslation = preTrans.TranslationPart();
	auto solidRotation = preTrans.GetRotation();
	Standard_Real rX,rY,rZ;
	solidRotation.GetEulerAngles(gp_Extrinsic_XYZ, rX, rY, rZ);
	retMesh.Translation_.set(static_cast<float>(solidTranslation.X()), static_cast<float>(solidTranslation.Y()), static_cast<float>(solidTranslation.Z()));
	retMesh.Rotation_.set(static_cast<float>(radToDeg(rX)), static_cast<float>(radToDeg(rY)), static_cast<float>(radToDeg(rZ)));

	retMesh.Mesh_ = totoalMesh;
	ret.RawMesh_ = retMesh;

	return ret;
}

ODLTools::SSingelMeshOpt ODLTools::CalculateRelation( const TopoDS_Shape& faceShape, const gp_Dir& faceDir )
{
	SSingleMesh ret;

	Bnd_Box bb;
	BRepBndLib::Add(faceShape, bb);

	if( bb.IsVoid() )
	{
		return boost::none;
	}

	Standard_Real xMin,xMax,yMin,yMax,zMin,zMax;
	bb.Get(xMin,yMin,zMin,xMax,yMax,zMax);

	aabbox3df bbox;
	bbox.reset(static_cast<float>(xMin),static_cast<float>(yMin),static_cast<float>(zMin));
	bbox.addInternalPoint(static_cast<float>(xMax),static_cast<float>(yMax),static_cast<float>(zMax));

	vector3df normal(static_cast<float>(faceDir.X()), static_cast<float>(faceDir.Y()), static_cast<float>(faceDir.Z()));
	{//重置到原点

		matrix4 m;

		{//移动中心到原点
			auto center1 = bbox.getCenter();

			matrix4 mtrans;
			mtrans.setTranslation(-center1);

			m = mtrans * m;

			mtrans.transformBoxEx(bbox);
		}

		{//旋转到(0,0,1)
			auto angleDir = normal.getHorizontalAngle();

			matrix4 mrotate;

			mrotate.setRotationDegrees(-angleDir);
			m = mrotate * m;

			mrotate.transformBoxEx(bbox);		
		}

		{//移动原点到左上
			auto center = bbox.getCenter();
			auto extent = bbox.getExtent();
			auto origin = center;
			origin.X -= extent.X/2;
			origin.Y += extent.Y/2;

			auto vec = center - origin;

			matrix4 mtrans;
			mtrans.setTranslation(vec);
			m = mtrans * m;
			mtrans.transformBoxEx(bbox);
		}

		m.makeInverse();

		auto trans = m.getTranslation();
		auto rotate = m.getRotationDegrees();

		ret.Translation_ = trans;
		ret.Rotation_ = rotate;
		ret.Box_ = bbox;
	}

	return ret;
}

irr::scene::IMeshBuffer* ODLTools::NEW_CreateMeshBuffer( const TopoDS_Shape& shape )
{
	auto meshBufferPtr = new CDynamicMeshBuffer(EVT_STANDARD, EIT_32BIT);
	meshBufferPtr->setHardwareMappingHint(EHM_STATIC);

	for ( TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next() )
	{
		auto& curFace = TopoDS::Face(exp.Current());

		Handle(Geom_Surface) surf = BRep_Tool::Surface(curFace);

		Standard_Real umin,umax,vmin,vmax;
		BRepTools::UVBounds(curFace, umin, umax, vmin, vmax);
		auto faceDir = GeomLProp_SLProps(surf, umin, vmin, 1, 0.001).Normal();
		if ( TopAbs_REVERSED == curFace.Orientation() )
		{
			faceDir.Reverse();
		}

		BRepMesh_IncrementalMesh(curFace, 0.1);

		TopLoc_Location L;
		Handle (Poly_Triangulation) polyTriangles = BRep_Tool::Triangulation(curFace,L);

		if ( polyTriangles.IsNull() )
		{
			return nullptr;
		}

		const TColgp_Array1OfPnt& aNodes = polyTriangles->Nodes();
		const Poly_Array1OfTriangle& triangles = polyTriangles->Triangles();

		auto vsize = meshBufferPtr->getVertexBuffer().allocated_size();
		auto isize = meshBufferPtr->getIndexBuffer().allocated_size();
		auto curVertexSize = meshBufferPtr->getVertexBuffer().size();
		meshBufferPtr->getVertexBuffer().reallocate(vsize + aNodes.Length());
		meshBufferPtr->getIndexBuffer().reallocate(isize + triangles.Length()*3);

		vector3df normal(static_cast<f32>(faceDir.X()), static_cast<f32>(faceDir.Y()), static_cast<f32>(faceDir.Z()));

		SColor clr(0xff, 0xff, 0xff, 0xff);
		TColgp_Array1OfPnt transNodes(1, aNodes.Length());
		for( auto index=1; index<=aNodes.Length(); ++index )
		{
			auto pnt = aNodes(index).Transformed(L);
			transNodes(index) = pnt;

			GeomAPI_ProjectPointOnSurf projpnta(pnt, surf);
			Quantity_Parameter u,v;
			projpnta.LowerDistanceParameters(u, v);
			gp_Pnt UV(u-umin, 0, v-vmin);
			UV.Transform(shape.Location().Transformation().Inverted());

			vector3df pos(static_cast<f32>(pnt.X()), static_cast<f32>(pnt.Y()), static_cast<f32>(pnt.Z()));
			vector2df coord(static_cast<f32>(UV.X()),static_cast<f32>(UV.Z()));
			
			S3DVertex meshVertex(pos, normal, clr, coord);

			meshBufferPtr->getVertexBuffer().push_back(meshVertex);
		}
		meshBufferPtr->recalculateBoundingBox();

		for( auto index=1,n1=0,n2=0,n3=0; index<=triangles.Length(); ++index )
		{
			triangles(index).Get(n1,n2,n3);

			gp_Vec v1(transNodes(n1), transNodes(n2));
			gp_Vec v2(transNodes(n1), transNodes(n3));

			auto v = v1.Crossed(v2);

			if ( gp_Dir(v).Angle(faceDir) > M_PI_2 )
			{
				meshBufferPtr->getIndexBuffer().push_back(n1-1+curVertexSize);
				meshBufferPtr->getIndexBuffer().push_back(n2-1+curVertexSize);
				meshBufferPtr->getIndexBuffer().push_back(n3-1+curVertexSize);
			}
			else
			{
				meshBufferPtr->getIndexBuffer().push_back(n1-1+curVertexSize);
				meshBufferPtr->getIndexBuffer().push_back(n3-1+curVertexSize);
				meshBufferPtr->getIndexBuffer().push_back(n2-1+curVertexSize);
			}
		}
	}

	meshBufferPtr->getMaterial().Lighting = false;
	meshBufferPtr->getMaterial().BackfaceCulling = false;

	return meshBufferPtr;
}

irr::scene::IMeshBuffer* ODLTools::NEW_CreateRectMeshBuffer( float radius )
{
	vector3df normal(0,1,0);
	SColor clr(~0);

	auto meshBufferPtr = new SMeshBuffer;
	meshBufferPtr->Vertices.reallocate(4);
	meshBufferPtr->Indices.reallocate(6);

	meshBufferPtr->Vertices.push_back(S3DVertex(vector3df(-radius, 0, radius), normal, clr, vector2df(0,0)));
	meshBufferPtr->Vertices.push_back(S3DVertex(vector3df(radius, 0, radius), normal, clr, vector2df(1,0)));
	meshBufferPtr->Vertices.push_back(S3DVertex(vector3df(radius, 0, -radius), normal, clr, vector2df(1,-1)));
	meshBufferPtr->Vertices.push_back(S3DVertex(vector3df(-radius, 0, -radius), normal, clr, vector2df(0,-1)));

	meshBufferPtr->Indices.push_back(0);
	meshBufferPtr->Indices.push_back(1);
	meshBufferPtr->Indices.push_back(2);
	meshBufferPtr->Indices.push_back(0);
	meshBufferPtr->Indices.push_back(2);
	meshBufferPtr->Indices.push_back(3);

	meshBufferPtr->recalculateBoundingBox();

	meshBufferPtr->getMaterial().Lighting = false;
	meshBufferPtr->getMaterial().BackfaceCulling = false;

	return meshBufferPtr;
}
