#pragma once

#include "SCADMesh.h"

#include <memory>

typedef	std::shared_ptr<SCADMesh>	SCADMeshPtr;

class	SBoxInfo
{
public:

	double XLength, YLength, ZLength;
};

class	STrianglePrimInfo
{
public:

	enum ETriangleDir{	ETD_LT, ETD_RT, ETD_LB, ETD_RB, ETD_COUNT };

public:

	double XLength, YLength, ZLength;

	ETriangleDir	Dir;

};

class	SSphereInfo
{
public:

	double Raidus;
};

class	SCylinderInfo
{
public:
	double Height;
	double Raidus;
};


class	SWireInfo
{
public:
	PntsVector			Pnts;
	SVectorInfo			Dir;
};


class	SGapInfo
{
public:	

	enum	EGapType		{ EGT_BOX, EGT_TRIANGLEPRIM, EGT_CYLINDER, EGT_SPHERE, EGT_SEGMENTS, EGT_COUNT };
	enum	EOperationType	{ EOT_CUT, EOT_COUNT };

public:

	SGapInfo():OptType(EOT_CUT) {}

public:

	double			PosX,PosY,PosZ;

	EGapType		GapType;

	union
	{
		SBoxInfo			Box;
		STrianglePrimInfo	Prim;
		SSphereInfo			Sphere;
		SCylinderInfo		Cylinder;
	};
	SWireInfo		Wires;
	EOperationType	OptType;
};

typedef	std::vector<SGapInfo>	Gaps;
typedef	std::shared_ptr<Gaps>	GapsPtr;

class	SCADMeshTool
{

public:

	enum	ECoordSys
	{
		ECS_LEFTHAND,
		ECS_RIGHTHAND,
		ECS_COUNT
	};

	static	DLL_EXPORT bool				SaveToObj(SCADMeshPtr meshPtr, const char* strFileName);

	static	DLL_EXPORT SCADMeshPtr		OpenObjFile(const char* strFileName);

	//************************************
	// Method:    CreateBooleanMesh
	// FullName:  SCADMeshTool::CreateBooleanMesh
	// Access:    public static 
	// Returns:   DLL_EXPORT SCADMeshPtr
	// Qualifier:
	// Parameter: const SVectorInfo & baseSize:X,Y,Z方向的长度
	// Parameter: GapsPtr gapsPtr
	// Parameter: ECoordSys sys
	// Parameter: const SVectorInfo & texRange:X,Y,Z方向的贴图长度
	//************************************
	static	DLL_EXPORT SCADMeshPtr		CreateBooleanMesh(const SVectorInfo& baseSize, GapsPtr gapsPtr, ECoordSys sys = ECS_LEFTHAND);

	//************************************
	// Method:    CreateWireMesh
	// FullName:  SCADMeshTool::CreateWireMesh
	// Access:    public static 
	// Returns:   DLL_EXPORT SCADMeshPtr
	// Qualifier:
	// Parameter: const PntsVector & pnts:点集合
	// Parameter: const SVectorInfo & vDir:生成方向
	// Parameter: GapsPtr gapsPtr:缺口集合
	// Parameter: ECoordSys sys
	// Parameter: const SVectorInfo & texRange:X,Y,Z方向的贴图长度
	//************************************
	static	DLL_EXPORT SCADMeshPtr		CreateWireMesh(const PntsVector& pnts, const SVectorInfo& vDir=std::make_tuple(0.0, 10.0, 0.0), GapsPtr gapsPtr=GapsPtr(), ECoordSys sys = ECS_LEFTHAND);

	static	DLL_EXPORT GapsPtr			CreateGapsPtr();

private:

	SCADMeshTool(ECoordSys sys, double zoff);

private:

	class						SImp;
	std::shared_ptr<SImp>		SImpPtr;
};