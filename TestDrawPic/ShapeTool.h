#pragma once
//////////////////////////////////////////////////////////////////////////
//����������������״��λ����Ϣ
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
	//ǽ
	static void CreateWall(const std::vector<gp_Pnt>& arrPoint, gp_Dir wallDir, float wallHeight, float wallThick, CBaseODLSPtr pObjGroup=nullptr);
	//��������1�����þ��Σ�arrPoint=4���㣬4����λ�����������±���
	//pObjGroup
	static CBaseODLSPtr CreateBeamPillar(const std::vector<gp_Pnt>& arrPoint, float wallHeight, CBaseODLSPtr pObjGroup=nullptr);

};

