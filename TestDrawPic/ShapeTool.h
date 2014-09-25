#pragma once
//////////////////////////////////////////////////////////////////////////
//本工具用来计算形状及位置信息
//////////////////////////////////////////////////////////////////////////


#include "ODL/ObjGroupODL.h"
#include "ODL/WallODL.h"
#include "ODL/FloorODL.h"
#include "gp_Pnt.hxx"

#include <vector>

class CShapeTool
{
public:
	CShapeTool(void);
	~CShapeTool(void);

public:
	static double GetWallLength(gp_Pnt stStart, gp_Pnt stStop);
	static bool IsSamePoint(gp_Pnt stStart, gp_Pnt stStop);
	//墙
	static void CreateWall(const std::vector<gp_Pnt>& arrPoint, gp_Dir wallDir, float wallHeight, float wallThick, CBaseODLSPtr pObjGroup=nullptr);
	//建梁或柱1根。用矩形，arrPoint=4个点，4个点位于梁或柱的下表面
	//pObjGroup
	static CBaseODLSPtr CreateBeamPillar(const std::vector<gp_Pnt>& arrPoint, float wallHeight, CBaseODLSPtr pObjGroup=nullptr);

};

