#include "stdafx.h"

#include "WallODL.h"
#include "WallFaceODL.h"

#include "BRepPrimAPI_MakePrism.hxx"
#include "TopExp.hxx"
#include "TopoDS.hxx"
#include "BRepBndLib.hxx"

#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "SMesh.h"

#include "IrrEngine/IrrEngine.h"

#include "ODL/MeshSceneNode/WallMeshNode2D.h"

CWallODL::CWallODL()
{
	
}


CWallODL::~CWallODL(void)
{
}

void CWallODL::SetDefaultTexture()
{
	auto tex = GetDataSceneNode()->getSceneManager()->getVideoDriver()->getTexture("../Data/Resource/3D/wall.jpg");
	assert(tex);

	auto uLen = 4000.f;//tex->getSize().Width;
	auto vLen = 3000.f;//tex->getSize().Height;

	GetDataSceneNode()->setMaterialTexture(0, tex);
	irr::core::matrix4 texMat;
	texMat.setScale(irr::core::vector3df(1/uLen, 1/vLen, 1));
	auto meshBufferPtr = GetDataSceneNode()->getMesh()->getMeshBuffer(0);
	meshBufferPtr->getMaterial().setTextureMatrix(0, texMat);
}

void CWallODL::UpdateSweeping()
{
	if ( IsSwept() )
	{
		GetDataSceneNode()->GetSceneNode3D()->SaveMaterialType();
		GetDataSceneNode()->GetSceneNode3D()->SetMaterialType(IrrEngine::GetInstance()->GetShaderType(EST_LUMINANCE));
	}
	else
	{
		GetDataSceneNode()->GetSceneNode3D()->ResetMaterialType();
	}
}

CBaseODLSPtr CWallODL::CreateByBottomFace( SRenderContextWPtr renderContext, const TopoDS_Shape& bottomFace, const gp_Dir& wallPathDir, float wallHeight )
{
	auto newWall = CWallODL::Create<CWallODL>(renderContext);

	BRepPrimAPI_MakePrism mp(bottomFace, gp_Vec(0,wallHeight, 0));
	assert(mp.IsDone());

	TopTools_IndexedMapOfShape m;
	TopExp::MapShapes(mp.Shape(), TopAbs_SOLID, m);
	assert(1 == m.Extent());

	auto curSolid = m.FindKey(1);

	gp_Trsf absoluteToRelation;
	{//设置位置
		gp_Trsf rotToDZ;
		gp_Trsf transToCenter;

		rotToDZ.SetRotation(gp_Quaternion(wallPathDir, gp::DZ()));
		curSolid.Move(rotToDZ);

		Bnd_Box wallBox;
		gp_Pnt wallCenter;
		{
			BRepBndLib::Add(curSolid, wallBox);
			Standard_Real xMin,yMin,zMin,xMax,yMax,zMax;
			wallBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);
			wallCenter.SetXYZ(gp_XYZ((xMin+xMax)/2, (yMin+yMax)/2, (zMin+zMax)/2));
		}

		transToCenter.SetTranslationPart(gp_Vec(gp::Origin(), wallCenter).Reversed());

		curSolid.Move(transToCenter);

		absoluteToRelation = transToCenter * rotToDZ;

		auto relationToAbsolute = absoluteToRelation.Inverted();
		auto translation = relationToAbsolute.TranslationPart();
		auto rotation = relationToAbsolute.GetRotation();

		newWall->SetBaseShape(curSolid);

		newWall->SetTranslation(translation);
		newWall->GetDataSceneNode()->setPosition(irr::core::vector3df(static_cast<float>(translation.X()), static_cast<float>(translation.Y()), static_cast<float>(translation.Z())));

		newWall->SetRoration(rotation);
		Standard_Real rX,rY,rZ;
		rotation.GetEulerAngles(gp_Extrinsic_XYZ, rX, rY, rZ);
		newWall->GetDataSceneNode()->setRotation(irr::core::vector3df(static_cast<float>(irr::core::radToDeg(rX)), static_cast<float>(irr::core::radToDeg(rY)), static_cast<float>(irr::core::radToDeg(rZ))));
	}

	{//3D模型
		auto meshBuf = ODLTools::NEW_CreateMeshBuffer(curSolid);
		assert(meshBuf);
		auto newMesh = new irr::scene::SMesh;
		newMesh->addMeshBuffer(meshBuf);
		newMesh->recalculateBoundingBox();

		newWall->GetDataSceneNode()->setMesh(newMesh);
		newMesh->drop();
		meshBuf->drop();
	}

	{//2D模型
		auto wall2DNode = new WallMeshNode2D(newWall->GetDataSceneNode()->GetSceneNode2D());
		wall2DNode->drop();
		auto transformedFace = bottomFace.Moved(absoluteToRelation);
		wall2DNode->UpdateMesh(transformedFace);
	}

	{//设置效果
		newWall->GetDataSceneNode()->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		newWall->GetDataSceneNode()->AddToDepthPass();
		newWall->SetDefaultTexture();
	}

	{//创建墙面

	}

	return newWall;
}
