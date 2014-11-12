#ifndef PublicDefine_h__
#define PublicDefine_h__

enum EObjectDisplayLayerType
{
	//����
	EODLT_BASE,

	//������
	EODLT_GROUP,

	//����ͼ
	EODLT_ROOM_STYLE,

	//���� ǽ��
	EODLT_ROOM_STYLE_LINE,

	//��Ʒ���
	EODLT_DESIGN,

	//ͼ
	EODLT_GRAPH,

	//����
	EODLT_ROOM,

	//ǽ��
	EODLT_CORNER,

	//ǽ
	EODLT_WALL,

	//��
	EODLT_HOLE,

	//ǽ��
	EODLT_WALLFACE,

	//ǽ�����
	EODLT_WALLFACE_ZONE,

	//��
	EODLT_BEAM,


	//��
	EODLT_PILLAR,

	//��
	EODLT_FLOOR,

	//��Ƥ
	EODLT_FLOOR_ZONE,

	//�컨��
	EODLT_SKY,

	//�컨��Ƥ
	EODLT_SKY_ZONE,

	//����װ��
	EODLT_DECOR,

	EODLT_DOOR,
	EODLT_WINDOW
};

enum ENUM_OPERAT_TYPE
{
	//ɶ��������
	EOT_Create_None= 0,
	//���ڴ���ǽ��by line
	EOT_Create_Wall_Line = 11,
	//����ǽbyrect
	EOT_Create_Wall_Rect = 15,
	//��������rect
	EOT_Create_Beam_Rect = 16,
	//��Rect������
	EOT_Create_Pillar_Rect =17,
	//���ڴ���ǽֽ
	EOT_Create_Wall_Face = 21,
	//�����ذ�ש
	EOT_Create_Floor_Zone=32,
};

enum ENUM_Group_Type
{
	//5101�ذ�ש
	EGT_GroundTiles = 5101,
	//5102ľ�ذ�
	EGT_FloorBoard = 5102,
	//5104����(ľ�ذ�����֮��Ľӷ��豸)
	EGT_Buckle = 5104,
	//5201ǽֽ
	EGT_WallPaper=5201,
	//5202ǽש
	EGT_WallBrick=5202,
	//5203����
	EGT_BeltCourse=5203,
	//5401������
	EGT_RoomDoor = 5401,
	//5402���䴰
	EGT_RoomWindow = 5402,
	//5405Ʈ��̨
	EGT_BayWindow = 5405,

};

#endif // PublicDefine_h__