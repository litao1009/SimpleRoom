#ifndef PublicDefine_h__
#define PublicDefine_h__


#define DOUBLE_ZERO 0.00000000001f

enum ENUM_ViewType
{
	//俯视图：正对顶面的全景视图
	EVT_TOP=1,
	//编辑面视图
	EVT_FACE = 3,
	
	//
	//立面图：针对某个方向的全景视图,左视、右视、后视图、前视图，都称作立面图，通过视觉位置确定
	
	//三维图
	
};

enum ENUM_RECT_SELECTED_POS
{
	ERSP_None = 0,
	ERSP_LeftTop = 1,
	ERSP_Top = 2,
	ERSP_RightTop = 3,
	ERSP_Right = 4,
	ERSP_RightBottom = 5,
	ERSP_Bottom = 6,
	ERSP_LeftBottom = 7,
	ERSP_Left = 8,
	ERSP_All = 10,
};

struct StObjInfo
{
	INT GTypeID;		//1、
	INT ValueID;
	BOOL IsSkin;		//是皮肤修改
	float XLen;
	float YLen;
	float ZLen;
	TCHAR ObjName[256];
	TCHAR ShowPicPath[256];
	TCHAR SkinPath[256];
	TCHAR ModelPath[256];
	StObjInfo()
	{
		ZeroMemory(this, sizeof(StObjInfo));
	}
};
struct StCoverInfo
{
	int SkinWidth;
	int SkinHeight;
	int XGrout;
	int YGrout;
	DWORD GroutColor;			//缝隙颜色:ARGB
	TCHAR SkinPath[256];
	StCoverInfo()
	{
		ZeroMemory(this, sizeof(StCoverInfo));
	}
};
enum ENUM_CreateStep
{
	ECS_STANDBY=0,
	//开始
	ECS_BEGIN=1,
	//进行中
	ECS_MOVING=2,
	//已结束
	ECS_END=3,
};

enum ENUM_OBOJECT_TYPE
{
	//基类
	EOBT_Base = 0,
	//对象组
	EOBT_OBJ_Group= 1,
	//绘制线
	EOBT_GuideLine=2,
	//绘制矩形
	EOBT_GuideRect = 3,
	//户型图
	EOBT_Guide_Room_Style = 4,
	//户型 墙线
	EOBT_Guide_Room_Style_Line = 5,
	//设计方案
	EOBT_Design = 8,
	//墙
	EOBT_Wall= 11,
	//墙面
	EOBT_Wall_Face = 12,
	//墙面分区
	EOBT_Wall_Face_Zone = 13,
	//梁
	EOBT_Beam = 14,
	/*EOBT_Beam_Face = 15,
	EOBT_Beam_Face_Zone = 16,*/
	//柱
	EOBT_Pillar = 17,
	/*EOBT_Pillar_Face = 18,
	EOBT_Pillar_Face_Zone = 19,*/
	//地
	EOBT_Floor=21,
	//地皮
	EOBT_Floor_Zone=23,
	//天花板
	EOBT_Sky=31,
	//天花板皮
	EOBT_Sky_Zone=32,

	//房间装饰
	EOBT_Decor = 41,

	//相机
	EOBT_Camera=50
};

enum ENUM_OPERAT_TYPE
{
	//啥都不创建
	EOT_Create_None= 0,
	//正在创建墙壁by line
	EOT_Create_Wall_Line = 11,
	//创建墙byrect
	EOT_Create_Wall_Rect = 15,
	//创建梁用rect
	EOT_Create_Beam_Rect = 16,
	//用Rect创建柱
	EOT_Create_Pillar_Rect =17,
	//正在创建墙纸
	EOT_Create_Wall_Face = 21,
	//创建地板砖
	EOT_Create_Floor_Zone=32,
};

//墙面的位置
enum ENUM_SIDE_POS
{
	ESP_Up=1,
	ESP_Down =2,
	ESP_Other=3,
};

enum ENUM_Group_Type
{
	//5101地板砖
	EGT_GroundTiles = 5101,
	//5102木地板
	EGT_FloorBoard = 5102,
	//5104扣条(木地板与门之间的接缝设备)
	EGT_Buckle = 5104,
	//5201墙纸
	EGT_WallPaper=5201,
	//5202墙砖
	EGT_WallBrick=5202,
	//5203腰线
	EGT_BeltCourse=5203,
	//5401房间门
	EGT_RoomDoor = 5401,
	//5402房间窗
	EGT_RoomWindow = 5402,
	//5405飘窗台
	EGT_BayWindow = 5405,

};

#endif // PublicDefine_h__