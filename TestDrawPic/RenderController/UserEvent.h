#pragma once

#define WM_IRR_DLG_MSG						WM_USER+1
#define DEFINE_USER_MESSAGE(msg)			(WM_USER + msg)

enum EUserType
{
	EUT_BEGIN = 100,

//-----------------------------------------------------------------------------户型图
	//画墙
	EUT_ROOMLAYOUT_DRAWLINE,
	//画墙完成
	EUT_ROOMLAYOUT_DRAWLINE_FINISH,

	//画房间
	EUT_ROOMLAYOUT_DRAWROOM,

	//设置临摹图
	EUT_ROOMLAYOUT_PICTURE_SETTING,
	//设置临摹图可见
	EUT_ROOMLAYOUT_PICTURE_VISIBLE,
	//画临摹图参考线
	EUT_ROOMLAYOUT_PICTUREREFLINE_DRAW,
	//设置临摹图参考尺寸
	EUT_ROOMLAYOUT_PICTUREREFLINE_SIZE,
	//设置临摹图透明度
	EUT_ROOMLAYOUT_PICTURE_ALPHA,
	//设置临摹图位置
	EUT_ROOMLAYOUT_PICTURE_POSITION,
	//临摹图设置完成
	EUT_ROOMLAYOUT_PICTURE_FINISH,
//-----------------------------------------------------------------------------户型图

	EUT_END
};



#define WM_USER_ROOMLAYOUT_PICTUREREFLINE_SIZE	DEFINE_USER_MESSAGE(EUT_ROOMLAYOUT_PICTUREREFLINE_SIZE)