#include "stdafx.h"
#include "RoomStyleLineBLL.h"


CRoomStyleLineBLL::CRoomStyleLineBLL(void)
{
}


CRoomStyleLineBLL::~CRoomStyleLineBLL(void)
{
}

void CRoomStyleLineBLL::GetPointList( INT nIndex, std::vector<Gdiplus::PointF>& arrPoint )
{
	switch (nIndex)
	{
	case  0:
		{
			/*1¡¢¾ØÐÎ
			©°©¤©¤©¤©¤©¤©¤©¤©¤©´
			©¦                ©¦
			©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼
			x0,y0:0.0, 0.0
			x1,y0:3600.0, 0.0
			x1,y1:3600,1800.0
			x0,y1:0,1800*/
			arrPoint.push_back(Gdiplus::PointF(0.0, 0.0));
			arrPoint.push_back(Gdiplus::PointF(3600.0, 0.0));
			arrPoint.push_back(Gdiplus::PointF(3600.0, 1800.0));
			arrPoint.push_back(Gdiplus::PointF(0.0, 1800.0));
			arrPoint.push_back(Gdiplus::PointF(0.0, 0.0));
		}
		break;
	case  1:
		{
			/*2¡¢Í¹ÐÍ
                  ©°©¤©¤©´
            ©°©¤©¤©¼    ©¸©¤©¤©¤©¤©´
            ©¦                    ©¦
            ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¼
			x0,y0: 0.0,500.0
			x1,y0: 600.0,500.0
			x1,y1: 600.0,0.0
			x2,y1: 1000.0,0.0
			x2,y0: 1000.0,500.0
			x3,y0: 1600.0,500.0
			x3,y2: 1600.0,1200.0
			x0,y2: 0.0,1200.0*/
			arrPoint.push_back(Gdiplus::PointF(0.0, 1000.0));
			arrPoint.push_back(Gdiplus::PointF(1200.0, 1000.0));
			arrPoint.push_back(Gdiplus::PointF(1200.0, 0.0));
			arrPoint.push_back(Gdiplus::PointF(2000.0, 0.0));
			arrPoint.push_back(Gdiplus::PointF(2000.0, 1000.0));
			arrPoint.push_back(Gdiplus::PointF(3200.0, 1000.0));
			arrPoint.push_back(Gdiplus::PointF(3200.0, 2400.0));
			arrPoint.push_back(Gdiplus::PointF(0.0, 2400.0));
			arrPoint.push_back(Gdiplus::PointF(0.0, 1000.0));
		}
		break;
	case  2:
		{
			/*
			3¡¢ÓÒÍ¹
                          ©°©¤©¤©¤©´
                          ©¦      ©¦
            ©°©¤©¤©¤©¤©¤©¤©¼      ©¦
            ©¦                    ©¦
            ©¦                    ©¦
            ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¼

			x0,y0: 0.0,600.0
			x1,y0: 1200.0,600
			x1,y1: 1200.0,0.0
			x2,y1: 1600.0,0.0
			x2,y2: 1600.0, 1400.0
			x0,y2: 0.0, 1400.0*/
			arrPoint.push_back(Gdiplus::PointF(0.0, 1200.0));
			arrPoint.push_back(Gdiplus::PointF(2400.0, 1200.0));
			arrPoint.push_back(Gdiplus::PointF(2400.0, 0.0));
			arrPoint.push_back(Gdiplus::PointF(3200.0, 0.0));
			arrPoint.push_back(Gdiplus::PointF(3200.0, 2800.0));
			arrPoint.push_back(Gdiplus::PointF(0.0, 2800.0));
			arrPoint.push_back(Gdiplus::PointF(0.0, 1200.0));
		}
		break;
	case  3:
		{
			/*4¡¢×óÍ¹
            ©°©¤©¤©¤©´
            ©¦      ©¦
            ©¦      ©¸©¤©¤©¤©¤©¤©¤©´
            ©¦                    ©¦
            ©¦                    ©¦
            ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¼

			x0,y0: 0.0,0.0
			x1,y0: 400.0, 0.0
			x1,y1: 400.0, 400.0
			x2,y1: 1200.0, 400.0
			x2, y2: 1200.0,1200.0
			x0,y2: 0.0, 1200.0*/
			arrPoint.push_back(Gdiplus::PointF(0.0, 0));
			arrPoint.push_back(Gdiplus::PointF(800.0, 0.0));
			arrPoint.push_back(Gdiplus::PointF(800.0, 800.0));
			arrPoint.push_back(Gdiplus::PointF(2400.0, 800.0));
			arrPoint.push_back(Gdiplus::PointF(2400.0, 2400.0));
			arrPoint.push_back(Gdiplus::PointF(0.0, 2400.0));
			arrPoint.push_back(Gdiplus::PointF(0.0, 0.0));
		}
		break;
	case  4:
		{
			/*5¡¢×óÌÝÐÎ
			©°©¤©¤©¤©¤
			©¦        £Ü
			©¦          £Ü
			©¦            £Ü
			©¦              £Ü
			©¦                £Ü
			©¸©¤©¤©¤©¤©¤©¤©¤©¤©¤£Ü

			x0,y0: 0.0, 0.0
			x1,y0: 400.0,0.0
			x2, y1: 800.0, 1200.0
			x0,y1: 0.0, 1200.0*/

			arrPoint.push_back(Gdiplus::PointF(0.0, 0.0));
			arrPoint.push_back(Gdiplus::PointF(800.0, 0.0));
			arrPoint.push_back(Gdiplus::PointF(1600.0, 2400.0));
			arrPoint.push_back(Gdiplus::PointF(0.0, 2400.0));
			arrPoint.push_back(Gdiplus::PointF(0.0, 0.0));
		}
		break;
	case  5:
		{
			/*6¡¢ÓÒÌÝÐÎ
                 /©¤©¤©¤©¤©´
                /         ©¦
               /          ©¦
              /           ©¦
             /            ©¦
           /©¤©¤©¤©¤©¤©¤©¤©¼

			x0,y0: 800.0,0.0
			x1,y0: 1600.0,0.0
			x1,y1: 1600.0,1200.0
			x0,y1: 0.0, 1200.0*/
			arrPoint.push_back(Gdiplus::PointF(1600.0, 0.0));
			arrPoint.push_back(Gdiplus::PointF(3200.0, 0.0));
			arrPoint.push_back(Gdiplus::PointF(3200.0, 2400.0));
			arrPoint.push_back(Gdiplus::PointF(0.0, 2400.0));
			arrPoint.push_back(Gdiplus::PointF(1600.0, 0.0));
		}
		break;
	case  6:
		{

		}
		break;
	}
}
