#include "stdafx.h"
#include "SkinUnitODL.h"

#include "irrEngine/irrEngine.h"

#include "BRepAlgoAPI_Common.hxx"
#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "TopoDS_Face.hxx"
#include "BRepTools_WireExplorer.hxx"
#include "TopExp_Explorer.hxx"
#include "TopoDS.hxx"
#include "BRep_Tool.hxx"
#include "gp_Pnt.hxx"

CSkinUnitODL::CSkinUnitODL(void)
{
	m_imgSkin = nullptr;
	m_fRotate = 0.0f;
	m_attImage.SetWrapMode(Gdiplus::WrapMode::WrapModeTile);
	m_clGrout = Gdiplus::Color::Gray;
	m_nGroutX = 0;
	m_nGroutY = 0;
	m_nWrapMode = (INT)Gdiplus::WrapModeTile;
	m_strSkinPath=_T("");
}


CSkinUnitODL::~CSkinUnitODL(void)
{
	if (m_imgSkin)
	{
		delete m_imgSkin;
	}
}

void CSkinUnitODL::DrawImage(Gdiplus::Graphics& gcDrawer, Gdiplus::RectF rtDrawArea, Gdiplus::REAL fScale)
{
	if (m_imgSkin)
	{
		Gdiplus::GraphicsPath gcPath;
		gcPath.AddRectangle(rtDrawArea);
		DrawImage(gcDrawer, gcPath, Gdiplus::PointF(rtDrawArea.X,rtDrawArea.Y), fScale);
	}
}

void CSkinUnitODL::DrawImage(Gdiplus::Graphics& gcDrawer, Gdiplus::GraphicsPath& gcPath, Gdiplus::PointF& ptOffset, Gdiplus::REAL fScale)
{
	if (m_imgSkin)
	{
		if (m_nWrapMode>=4)
		{
			//����ģʽ
			Gdiplus::RectF rtArea;
			gcPath.GetBounds(&rtArea);
			Gdiplus::GraphicsPath gcDrawPath;
			//ͼƬ�м�X��(width-imagewidth )/2 + left;
			//Y: (height - imageheight)/2 +top;

			Gdiplus::REAL fX0 = (rtArea.Width - m_fSkinWidth-m_nGroutX) /2.0f + rtArea.X;
			Gdiplus::REAL fY0 = (rtArea.Height - m_fSkinHeight-m_nGroutY) /2.0f + rtArea.Y;
			Gdiplus::REAL fX1 = m_fSkinWidth + m_nGroutX+ fX0;
			Gdiplus::REAL fY1 = m_fSkinHeight +m_nGroutY+ fY0;
			std::vector<Gdiplus::PointF> arrPt;
			arrPt.emplace_back(fX0, fY0);
			arrPt.emplace_back(fX1, fY0);
			arrPt.emplace_back(fX1, fY1);
			arrPt.emplace_back(fX0, fY1);
			Gdiplus::Matrix mx;
			Gdiplus::PointF ptCenter=Gdiplus::PointF((fX1+fX0)/2.0f, (fY1+fY0)/2.0f);
			mx.RotateAt(m_fRotate, ptCenter);
			mx.TransformPoints(arrPt.data(), arrPt.size());
			gcDrawPath.AddPolygon(arrPt.data(),arrPt.size());
			//���ͼƬ��path���ڵ�ǰ����Path������в�����������
			{
				BRepBuilderAPI_MakePolygon ply1;
				for (auto& ptPos:arrPt)
				{
					ply1.Add(gp_Pnt(ptPos.X, 0.0f, ptPos.Y));
				}
				ply1.Close();
				TopoDS_Face face1 = BRepBuilderAPI_MakeFace(ply1.Wire()).Face();
				std::vector<Gdiplus::PointF> arrPic;
				arrPic.resize(gcDrawPath.GetPointCount());
				gcPath.GetPathPoints(arrPic.data(), arrPic.size());

				BRepBuilderAPI_MakePolygon ply2;
				for (auto& ptPos:arrPic)
				{
					ply2.Add(gp_Pnt(ptPos.X, 0.0f, ptPos.Y));
				}
				ply2.Close();
				TopoDS_Face face2 = BRepBuilderAPI_MakeFace(ply2.Wire()).Face();

				BRepAlgoAPI_Common bc(face1, face2);
				auto face = bc.Shape();
				TopExp_Explorer expWire(face, TopAbs_WIRE);
				std::vector<Gdiplus::PointF> arrDraw;
				for ( BRepTools_WireExplorer expVertex(TopoDS::Wire(expWire.Current())); expVertex.More(); expVertex.Next() )
				{
					auto pnt = BRep_Tool::Pnt(expVertex.CurrentVertex());
					arrDraw.emplace_back(static_cast<Gdiplus::REAL>(pnt.X()), static_cast<Gdiplus::REAL>(pnt.Z()));
				}
				gcDrawPath.Reset();
				gcDrawPath.AddPolygon(arrDraw.data(), arrDraw.size());
			}

			Gdiplus::WrapMode wmMode=(Gdiplus::WrapMode)m_nWrapMode;
			if (std::fabs(m_fRotate)<0.001f)
			{
				Gdiplus::TextureBrush brush(m_imgSkin, wmMode);
				mx.Translate(fX0, fY0);
				brush.SetTransform(&mx);
				gcDrawer.FillPath(&brush, &gcDrawPath);
			}
			else
			{
				Gdiplus::TextureBrush brush(m_imgSkin, wmMode);
				mx.Translate(fX0, fY0);
				brush.SetTransform(&mx);
				gcDrawer.FillPath(&brush, &gcDrawPath);
			}
		}
		else
		{
			//����ģʽ
			Gdiplus::WrapMode wmMode=(Gdiplus::WrapMode)m_nWrapMode;
			if (std::fabs(m_fRotate)<0.001f)
			{
				Gdiplus::TextureBrush brush(m_imgSkin, wmMode);
				brush.TranslateTransform(ptOffset.X, ptOffset.Y);
				gcDrawer.FillPath(&brush, &gcPath);
			}
			else
			{
				Gdiplus::TextureBrush brush(m_imgSkin, wmMode);
				brush.TranslateTransform(ptOffset.X, ptOffset.Y);
				brush.RotateTransform(m_fRotate);
				gcDrawer.FillPath(&brush, &gcPath);
			}
		}
		
	}
}

void CSkinUnitODL::SetSkinPath( StCoverInfo stInfo)
{
	//���ݵ�ǰ����������ͼƬ
	if (m_imgSkin!=nullptr)
	{
		delete m_imgSkin;
		m_imgSkin=nullptr;
	}
	m_strSkinPath = stInfo.SkinPath;
	m_clGrout =Gdiplus::ARGB(stInfo.GroutColor);
	m_nGroutX = stInfo.XGrout;
	m_nGroutY = stInfo.YGrout;
	m_fSkinWidth = (float)stInfo.SkinWidth;
	m_fSkinHeight = (float) stInfo.SkinHeight;
	Gdiplus::Bitmap *pImg = CreateImageData(m_nGroutX, m_nGroutY, stInfo.SkinWidth, stInfo.SkinHeight, m_clGrout, m_strSkinPath);
	m_imgSkin = pImg;
}

Gdiplus::Bitmap* CSkinUnitODL::CreateImageData( INT nGroutX, INT nGroutY, INT nSrcWidth, INT nSrcHeight, Gdiplus::Color clBackground, CString strPicPath )
{
	//����Ŀ��ͼƬ�������ȡ��߶ȣ�ˮ��ۿ��
	Gdiplus::Bitmap *img=new Gdiplus::Bitmap(nSrcWidth + nGroutX, nSrcHeight + nGroutY);
	//�����������Сλ��
	Gdiplus::Bitmap bmp(strPicPath);
	Gdiplus::Graphics buffer(img);//Gaphics���������ڴ滭��   
	buffer.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);//SmoothingModeAntiAlias);
	buffer.ResetTransform();
	buffer.Clear(clBackground);
	{
		Gdiplus::ImageAttributes attImage;
		attImage.SetWrapMode(Gdiplus::WrapMode::WrapModeTile);

		Gdiplus::Rect rtDraw(nGroutX/2, nGroutY/2, nSrcWidth, nSrcHeight);
		//ͼƬԭʼ�ߴ���bmp�ڣ����� x,y�㣬 ��ҪԭʼͼƬ��λ���� srcX, srcY, Ŀ��ͼ�������С�� srcWidth, srcHeight
		buffer.DrawImage(&bmp, rtDraw, 0, 0, bmp.GetWidth(), bmp.GetHeight(), Gdiplus::Unit::UnitPixel, &attImage);
	}

	return img;
}

void CSkinUnitODL::GetBitmapData(const Gdiplus::Rect* rect, UINT flags, Gdiplus::PixelFormat format, Gdiplus::BitmapData& lockedBitmapData )
{
	m_imgSkin->LockBits(rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &lockedBitmapData);
}

void CSkinUnitODL::ReleaseBitmapData( Gdiplus::BitmapData& lockedBitmapData )
{
	m_imgSkin->UnlockBits(&lockedBitmapData);
}

void CSkinUnitODL::Update()
{
	if (m_imgSkin!=nullptr)
	{
		delete m_imgSkin;
		m_imgSkin=nullptr;
	}
	Gdiplus::Bitmap *pImg = CreateImageData(m_nGroutX, m_nGroutY, (INT)m_fSkinWidth, (INT)m_fSkinHeight, m_clGrout, m_strSkinPath);
	m_imgSkin = pImg;
}

irr::video::ITexture*	CSkinUnitODL::GetTextureData()
{
	Gdiplus::BitmapData data;
	auto uLen = GetSkinWidth()+GetGroutX();
	auto vLen = GetSkinHeight()+GetGroutY();
	Gdiplus::Rect rect(0,0, static_cast<int>(uLen), static_cast<int>(vLen));
	GetBitmapData(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, data);

	irr::core::dimension2du texSize(data.Width, data.Height);
	auto img = IrrEngine::GetInstance()->GetDevice()->getVideoDriver()->createImageFromData(irr::video::ECF_A8R8G8B8, texSize, data.Scan0, false, false);

	auto addr = reinterpret_cast<int>(data.Scan0);
	auto tex = IrrEngine::GetInstance()->GetDevice()->getVideoDriver()->addTexture(std::to_string(addr).c_str(), img);
	img->drop();
	ReleaseBitmapData(data);

	return tex;
}
