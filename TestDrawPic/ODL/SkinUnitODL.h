#pragma once
//////////////////////////////////////////////////////////////////////////
//本类保存图片，已经将图片显示到对象表面
//只保存图片和图片本身信息，因此，不从BaseODL继承
//////////////////////////////////////////////////////////////////////////

#include "../PublicDefine.h"

#include "ITexture.h"

class CSkinUnitODL
{
public:
	CSkinUnitODL(void);
	~CSkinUnitODL(void);
public:
	//Graphics
	//目标区域的坐标及大小
	//缩放比例
	void DrawImage(Gdiplus::Graphics& gcDrawer, Gdiplus::RectF rtDrawArea, Gdiplus::REAL fScale);
	void DrawImage(Gdiplus::Graphics& gcDrawer, Gdiplus::GraphicsPath& gcPath, Gdiplus::PointF& ptOffset, Gdiplus::REAL fScale);
	void GetBitmapData(const Gdiplus::Rect* rect, UINT flags, Gdiplus::PixelFormat format, Gdiplus::BitmapData& lockedBitmapData );

	void ReleaseBitmapData( Gdiplus::BitmapData& lockedBitmapData);

	irr::video::ITexture*	GetTextureData();

public:

	void	SetSkinPath(StCoverInfo stInfo);

	void	Update();


	const Gdiplus::REAL& GetSkinWidth() const	{ return m_fSkinWidth; }
	Gdiplus::REAL& GetSkinWidth()				{ return m_fSkinWidth; }
	void SetSkinWidth(const Gdiplus::REAL& val) { m_fSkinWidth = val; }

	const Gdiplus::REAL& GetSkinHeight() const	{ return m_fSkinHeight; }
	Gdiplus::REAL& GetSkinHeight()				{ return m_fSkinHeight; }
	void SetSkinHeight(const Gdiplus::REAL& val) { m_fSkinHeight = val; }

	const int& GetGroutX() const { return m_nGroutX; }
	int& GetGroutX() { return m_nGroutX; }
	void SetGroutX(const int& val) { m_nGroutX = val; }
	const int& GetGroutY() const { return m_nGroutY; }
	int& GetGroutY() { return m_nGroutY; }
	void SetGroutY(const int& val) { m_nGroutY = val; }

	bool HasPicData()
	{
		return m_imgSkin!=nullptr;
	}

	void GetSetting(int& nGroutX, int& nGroutY, INT& nWrapMode, float& fRotate, Gdiplus::Color& clGrout) const 
	{ nGroutX=m_nGroutX; nGroutY=m_nGroutY; fRotate = m_fRotate; nWrapMode = m_nWrapMode; clGrout = m_clGrout; };
	void SetSetting(int nGroutX, int nGroutY, INT nWrapMode, float fRotate, Gdiplus::Color clGrout) 
	{ m_nGroutX = nGroutX; m_nGroutY = nGroutY; m_nWrapMode =nWrapMode, m_fRotate = fRotate; m_clGrout = clGrout; Update();};

private:
	Gdiplus::Bitmap* CreateImageData(INT nGroutX, INT nGroutY, INT nSrcWidth, INT nSrcHeight, Gdiplus::Color clBackground, CString strPicPath );
private:
	
	//最后生成的图片
	Gdiplus::Bitmap* m_imgSkin;
	Gdiplus::ImageAttributes m_attImage;		//图形纹理平铺属性
	CString m_strSkinPath;
	Gdiplus::Color m_clGrout;
	int m_nGroutX;

	int m_nGroutY;
	INT m_nWrapMode;			//5、单独一张图片

	Gdiplus::REAL		m_fSkinWidth;		//图片代表宽度

	Gdiplus::REAL		m_fSkinHeight;		//图片代表高度

	Gdiplus::REAL m_fRotate;			//显示时，旋转的角度
	
};

typedef	std::shared_ptr<CSkinUnitODL>	CSkinUnitODLSPtr;
typedef	std::weak_ptr<CSkinUnitODL>		CSkinUnitODLWPtr;
