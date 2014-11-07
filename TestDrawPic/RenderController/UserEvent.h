#pragma once

#define WM_IRR_DLG_MSG						WM_USER+1
#define DEFINE_USER_MESSAGE(msg)			(WM_USER + msg)

enum EUserType
{
	EUT_BEGIN = 100,

	//��ǽ
	EUT_DRAW_LINE,

	//������
	EUT_DRAW_ROOM,

	//������ġͼ
	EUT_SET_ROOM_PICTURE,

	//������ġͼ�ɼ�
	EUT_ROOM_PICTURE_SHOW,

	//���òο��ߴ�
	EUT_SET_REFERENCE_SIZE,

	//������ġͼ͸����
	EUT_SET_ROOM_PICTURE_ALPHA,

	//������ġͼλ��
	EUT_SET_ROOM_PICTURE_POSITION,

	EUT_END
};



#define WM_USER_SET_REFERENCE_SIZE	DEFINE_USER_MESSAGE(EUT_SET_REFERENCE_SIZE)