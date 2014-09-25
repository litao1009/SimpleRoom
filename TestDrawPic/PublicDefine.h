#ifndef PublicDefine_h__
#define PublicDefine_h__


#define DOUBLE_ZERO 0.00000000001f

enum ENUM_ViewType
{
	//����ͼ�����Զ����ȫ����ͼ
	EVT_TOP=1,
	//�༭����ͼ
	EVT_FACE = 3,
	
	//
	//����ͼ�����ĳ�������ȫ����ͼ,���ӡ����ӡ�����ͼ��ǰ��ͼ������������ͼ��ͨ���Ӿ�λ��ȷ��
	
	//��άͼ
	
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
	INT GTypeID;		//1��
	INT ValueID;
	BOOL IsSkin;		//��Ƥ���޸�
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
	DWORD GroutColor;			//��϶��ɫ:ARGB
	TCHAR SkinPath[256];
	StCoverInfo()
	{
		ZeroMemory(this, sizeof(StCoverInfo));
	}
};
enum ENUM_CreateStep
{
	ECS_STANDBY=0,
	//��ʼ
	ECS_BEGIN=1,
	//������
	ECS_MOVING=2,
	//�ѽ���
	ECS_END=3,
};

enum ENUM_OBOJECT_TYPE
{
	//����
	EOBT_Base = 0,
	//������
	EOBT_OBJ_Group= 1,
	//������
	EOBT_GuideLine=2,
	//���ƾ���
	EOBT_GuideRect = 3,
	//����ͼ
	EOBT_Guide_Room_Style = 4,
	//���� ǽ��
	EOBT_Guide_Room_Style_Line = 5,
	//��Ʒ���
	EOBT_Design = 8,
	//ǽ
	EOBT_Wall= 11,
	//ǽ��
	EOBT_Wall_Face = 12,
	//ǽ�����
	EOBT_Wall_Face_Zone = 13,
	//��
	EOBT_Beam = 14,
	/*EOBT_Beam_Face = 15,
	EOBT_Beam_Face_Zone = 16,*/
	//��
	EOBT_Pillar = 17,
	/*EOBT_Pillar_Face = 18,
	EOBT_Pillar_Face_Zone = 19,*/
	//��
	EOBT_Floor=21,
	//��Ƥ
	EOBT_Floor_Zone=23,
	//�컨��
	EOBT_Sky=31,
	//�컨��Ƥ
	EOBT_Sky_Zone=32,

	//����װ��
	EOBT_Decor = 41,

	//���
	EOBT_Camera=50
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

//ǽ���λ��
enum ENUM_SIDE_POS
{
	ESP_Up=1,
	ESP_Down =2,
	ESP_Other=3,
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