#pragma once

#define WM_IRR_DLG_MSG						WM_USER+1
#define DEFINE_USER_MESSAGE(msg)			(WM_USER + msg)

enum EUserType
{
	EUT_BEGIN = 100,

//-----------------------------------------------------------------------------����ͼ
	//��ǽ
	EUT_ROOMLAYOUT_DRAWLINE,
	//��ǽ���
	EUT_ROOMLAYOUT_DRAWLINE_FINISH,

	//������
	EUT_ROOMLAYOUT_DRAWROOM,

	//������ġͼ
	EUT_ROOMLAYOUT_PICTURE_SETTING,
	//������ġͼ�ɼ�
	EUT_ROOMLAYOUT_PICTURE_VISIBLE,
	//����ġͼ�ο���
	EUT_ROOMLAYOUT_PICTUREREFLINE_DRAW,
	//������ġͼ�ο��ߴ�
	EUT_ROOMLAYOUT_PICTUREREFLINE_SIZE,
	//������ġͼ͸����
	EUT_ROOMLAYOUT_PICTURE_ALPHA,
	//������ġͼλ��
	EUT_ROOMLAYOUT_PICTURE_POSITION,
	//��ġͼ�������
	EUT_ROOMLAYOUT_PICTURE_FINISH,
//-----------------------------------------------------------------------------����ͼ

	EUT_END
};



#define WM_USER_ROOMLAYOUT_PICTUREREFLINE_SIZE	DEFINE_USER_MESSAGE(EUT_ROOMLAYOUT_PICTUREREFLINE_SIZE)