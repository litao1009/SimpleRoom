#pragma once
//////////////////////////////////////////////////////////////////////////
//���ౣ��ͼƬ���Ѿ���ͼƬ��ʾ���������
//ֻ����ͼƬ��ͼƬ������Ϣ����ˣ�����BaseODL�̳�
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
	//Ŀ����������꼰��С
	//���ű���
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
	
	//������ɵ�ͼƬ
	Gdiplus::Bitmap* m_imgSkin;
	Gdiplus::ImageAttributes m_attImage;		//ͼ������ƽ������
	CString m_strSkinPath;
	Gdiplus::Color m_clGrout;
	int m_nGroutX;

	int m_nGroutY;
	INT m_nWrapMode;			//5������һ��ͼƬ

	Gdiplus::REAL		m_fSkinWidth;		//ͼƬ������

	Gdiplus::REAL		m_fSkinHeight;		//ͼƬ����߶�

	Gdiplus::REAL m_fRotate;			//��ʾʱ����ת�ĽǶ�
	
};

typedef	std::shared_ptr<CSkinUnitODL>	CSkinUnitODLSPtr;
typedef	std::weak_ptr<CSkinUnitODL>		CSkinUnitODLWPtr;
