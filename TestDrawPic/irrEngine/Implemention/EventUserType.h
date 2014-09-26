#ifndef EventUserType_h__
#define EventUserType_h__

#pragma once

enum EUserType
{
	//最小化
	EUT_RESIZE_MINIMUM = 100,
	
	//恢复
	EUT_RESIZE_RESTORE,

	//开始移动
	EUT_RESIZE_CHANGE_BEGIN,

	//移动
	EUT_RESIZE_CHANGE_DURING,

	//结束移动
	EUT_RESIZE_CHANGE_END,

	//画墙
	EUT_FINISH_DRAW_LINE_WALL
};

#define WM_IRR_DLG_MSG		WM_USER+1
#define GET_WM_MESSAGE(msg)	(WM_USER + msg)
#define WM_RESIZE_MINIMUM		GET_WM_MESSAGE(EUT_RESIZE_MINIMUM)
#define WM_RESIZE_RESTORE		GET_WM_MESSAGE(EUT_RESIZE_RESTORE)
#define WM_RESIZE_CHANGE_BEGIN	GET_WM_MESSAGE(EUT_RESIZE_CHANGE_BEGIN)
#define WM_RESIZE_CHANGE_DURING	GET_WM_MESSAGE(EUT_RESIZE_CHANGE_DURING)
#define WM_RESIZE_CHANGE_END	GET_WM_MESSAGE(EUT_RESIZE_CHANGE_END)

#endif // EventUserType_h__
