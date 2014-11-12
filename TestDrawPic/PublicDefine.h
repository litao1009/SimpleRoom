#ifndef PublicDefine_h__
#define PublicDefine_h__

enum EObjectDisplayLayerType
{
	//基类
	EODLT_BASE,

	//对象组
	EODLT_GROUP,

	//户型图
	EODLT_ROOM_STYLE,

	//户型 墙线
	EODLT_ROOM_STYLE_LINE,

	//设计方案
	EODLT_DESIGN,

	//图
	EODLT_GRAPH,

	//房间
	EODLT_ROOM,

	//墙角
	EODLT_CORNER,

	//墙
	EODLT_WALL,

	//洞
	EODLT_HOLE,

	//墙面
	EODLT_WALLFACE,

	//墙面分区
	EODLT_WALLFACE_ZONE,

	//梁
	EODLT_BEAM,


	//柱
	EODLT_PILLAR,

	//地
	EODLT_FLOOR,

	//地皮
	EODLT_FLOOR_ZONE,

	//天花板
	EODLT_SKY,

	//天花板皮
	EODLT_SKY_ZONE,

	//房间装饰
	EODLT_DECOR,

	EODLT_DOOR,
	EODLT_WINDOW
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