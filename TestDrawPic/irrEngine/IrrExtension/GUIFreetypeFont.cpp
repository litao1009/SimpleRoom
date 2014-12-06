#include "stdafx.h"

#include "GUIFreetypeFont.h"

#include "SMesh.h"
#include "SMeshBuffer.h"

#include "irrEngine/irrEngine.h"

#if COMPILE_WITH_FREETYPE

#include <cassert>
#include <vector>
#include <string>

using namespace irr;
using namespace gui;
using namespace scene;
using namespace core;
using namespace video;

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif


// --------------------------------------------------------
CGUITTGlyph::CGUITTGlyph()
	: IReferenceCounted()
	,cached(false)
	,size(0)
	,top(0)
	,left(0)
	,texw(0)
	,texh(0)
	,imgw(0)
	,imgh(0)
	,tex(NULL)
	,top16(0)
	,left16(0)
	,texw16(0)
	,texh16(0)
	,imgw16(0)
	,imgh16(0)
	,tex16(NULL)
	,image(NULL)
{
}

CGUITTGlyph::~CGUITTGlyph()
{
	delete[] image;
}

//void CGUITTGlyph::cache(u32 idx_, CGUITTFace& ttFace_, video::IVideoDriver* driver_, irr::core::dimension2d<irr::u32> &largestSize)
void CGUITTGlyph::cache(u32 idx_, const CGUIFreetypeFont * freetypeFont)
{
	assert(freetypeFont);

	FT_Face face = freetypeFont->TrueTypeFace->face;

	FT_Set_Pixel_Sizes(face, 0, size);
	if ( size > freetypeFont->LargestGlyph.Height )
		freetypeFont->LargestGlyph.Height = size;

	if ( !FT_Load_Glyph(face, idx_, FT_LOAD_NO_HINTING|FT_LOAD_NO_BITMAP) )
	{
		FT_GlyphSlot glyph = face->glyph;
		FT_Bitmap  bits;

		if (glyph->format == ft_glyph_format_outline )
		{
			if (!FT_Render_Glyph( glyph, FT_RENDER_MODE_NORMAL))
			{
				bits = glyph->bitmap;
				u8 *pt = bits.buffer;
				delete[] image;
				image = new u8[bits.width * bits.rows];
				memcpy(image,pt,bits.width * bits.rows);
				top = glyph->bitmap_top;
				left = glyph->bitmap_left;
				imgw = 1;
				imgh = 1;
				texw = bits.width;
				texh = bits.rows;
				for(;;)
				{
					if (imgw > texw)
					{
						break;
					}
					else
					{
						imgw <<= 1;
					}
				}
				for(;;)
				{
					if (imgh > texh)
					{
						break;
					}
					else
					{
						imgh <<= 1;
					}
				}
				if (imgw > imgh)
				{
					imgh = imgw;
				}
				else
				{
					imgw = imgh;
				}

				s32 offx = left;
				s32 offy = size - top;
				if ( offx+texw > freetypeFont->LargestGlyph.Width )
					freetypeFont->LargestGlyph.Width = offx+texw;
				if ( offy+texh > freetypeFont->LargestGlyph.Height )
					freetypeFont->LargestGlyph.Height = offy+texh;

				u32 *texd = new u32[imgw*imgh];
				memset(texd,0,imgw*imgh*sizeof(u32));
				u32 *texp = texd;
				bool cflag = (freetypeFont->Driver->getDriverType() == video::EDT_DIRECT3D8);
				for (int i = 0;i < bits.rows;i++)
				{
					u32 *rowp = texp;
					for (int j = 0;j < bits.width;j++)
					{
						if (*pt)
						{
							if (cflag)
							{
								*rowp = *pt;
								*rowp *= 0x01010101;
							}
							else
							{
								*rowp = *pt << 24;
								*rowp |= 0xffffff;
							}
						}
						else
						{
							*rowp = 0;
						}
						pt++;
						rowp++;
					}
					texp += imgw;
				}

				c8 name[128];
				sprintf(name,"ttf%d_%d_%p",idx_, size, freetypeFont );
				video::IImage *img = freetypeFont->Driver->createImageFromData(video::ECF_A8R8G8B8,core::dimension2d<u32>(imgw,imgh),texd);
				setGlyphTextureFlags(freetypeFont->Driver);
				tex = freetypeFont->Driver->addTexture(name,img);
				img->drop();
				restoreTextureFlags(freetypeFont->Driver);
				delete[] texd;
				cached = true;
			}
		}
	}

	if (!FT_Load_Glyph(face,idx_,FT_LOAD_NO_HINTING|FT_LOAD_RENDER|FT_LOAD_MONOCHROME))
	{
		FT_GlyphSlot glyph = face->glyph;
		FT_Bitmap bits = glyph->bitmap;
		u8 *pt = bits.buffer;
		top16 = glyph->bitmap_top;
		left16 = glyph->bitmap_left;
		imgw16 = 1;
		imgh16 = 1;
		texw16 = bits.width;
		texh16 = bits.rows;
		for(;;)
		{
			if (imgw16 >= texw16)
			{
				break;
			}
			else
			{
				imgw16 <<= 1;
			}
		}
		for(;;)
		{
			if (imgh16 >= texh16)
			{
				break;
			}
			else
			{
				imgh16 <<= 1;
			}
		}
		if (imgw16 > imgh16)
		{
			imgh16 = imgw16;
		}
		else
		{
			imgw16 = imgh16;
		}

		s32 offx = left;
		s32 offy = size - top;
		if ( offx+texw > freetypeFont->LargestGlyph.Width )
			freetypeFont->LargestGlyph.Width = offx+texw;
		if ( offy+texh > freetypeFont->LargestGlyph.Height )
			freetypeFont->LargestGlyph.Height = offy+texh;


		u16 *texd16 = new u16[imgw16*imgh16];
		memset(texd16,0,imgw16*imgh16*sizeof(u16));
		u16 *texp16 = texd16;
		for (int y = 0;y < bits.rows;y++)
		{
			u16 *rowp = texp16;
			for (int x = 0;x < bits.width;x++)
			{
				if (pt[y * bits.pitch + (x / 8)] & (0x80 >> (x % 8)))
				{
					*rowp = 0xffff;
				}
				rowp++;
			}
			texp16 += imgw16;
		}
		c8 name[128];
		sprintf(name,"ttf%d_%d_%p_16",idx_, size, freetypeFont );
		video::IImage *img = freetypeFont->Driver->createImageFromData(video::ECF_A1R5G5B5,core::dimension2d<u32>(imgw16,imgh16),texd16);
		setGlyphTextureFlags(freetypeFont->Driver);
		tex16 = freetypeFont->Driver->addTexture(name,img);
		img->drop();
		restoreTextureFlags(freetypeFont->Driver);
		//		freetypeFont->Driver->makeColorKeyTexture(tex16,video::SColor(0,0,0,0));
		delete[] texd16;
	}
}

bool CGUITTGlyph::mTexFlag16 = false;
bool CGUITTGlyph::mTexFlag32 = true;
bool CGUITTGlyph::mTexFlagMip = false;

void CGUITTGlyph::setGlyphTextureFlags(video::IVideoDriver* driver_)
{
	mTexFlag16 = driver_->getTextureCreationFlag(video::ETCF_ALWAYS_16_BIT);
	mTexFlag32 = driver_->getTextureCreationFlag(video::ETCF_ALWAYS_32_BIT);
	mTexFlagMip = driver_->getTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS);
	driver_->setTextureCreationFlag(video::ETCF_ALWAYS_16_BIT,false);
	driver_->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT,true);
	driver_->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
}

void CGUITTGlyph::restoreTextureFlags(video::IVideoDriver* driver_)
{
	driver_->setTextureCreationFlag(video::ETCF_ALWAYS_16_BIT, mTexFlag16);
	driver_->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, mTexFlag32);
	driver_->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, mTexFlagMip);
}

// --------------------------------------------------------
FT_Library	CGUITTFace::library  = 0;
int CGUITTFace::countClassObjects = 0;

CGUITTFace::CGUITTFace()
	: face(0)
{
	++countClassObjects;
}

CGUITTFace::~CGUITTFace()
{
	if ( face )
		FT_Done_Face( face );

	--countClassObjects;
	assert(countClassObjects >= 0 );
	if ( !countClassObjects && library )
	{
		FT_Done_FreeType( library );
		library = 0;
	}
}

//! loads a font file
bool CGUITTFace::load(const irr::io::path& filename)
{
	if ( !library )
	{
		if (FT_Init_FreeType( &library ))
		{
			return	false;
		}
	}
	core::stringc ansiFilename(filename);   // path can be anything but freetype can only work with ansi-filenames
	if (FT_New_Face( library,ansiFilename.c_str(),0,&face ))
	{
		return	false;
	}
	return	true;
}

// --------------------------------------------------------
//! constructor
CGUIFreetypeFont::CGUIFreetypeFont(video::IVideoDriver* driver)
	: Driver(driver)
	, TrueTypeFace(0)
{
#ifdef _DEBUG
	setDebugName("CGUIFreetypeFont");
#endif

	if (Driver)
		Driver->grab();
	AntiAlias = false;
	Transparency = false;
}



//! destructor
CGUIFreetypeFont::~CGUIFreetypeFont()
{
	if ( TrueTypeFace )
		TrueTypeFace->drop();
	if (Driver)
		Driver->drop();
	clearGlyphs();
}

bool CGUIFreetypeFont::attach(CGUITTFace *Face,u32 size)
{
	if (!Driver || !Face)
		return false;

	Face->grab();
	if ( TrueTypeFace )
		TrueTypeFace->drop();
	TrueTypeFace = Face;
	if ( !TrueTypeFace )
		return false;

	clearGlyphs();
	Glyphs.reallocate(TrueTypeFace->face->num_glyphs);
	Glyphs.set_used(TrueTypeFace->face->num_glyphs);
	for (int i = 0;i < TrueTypeFace->face->num_glyphs;i++)
	{
		CGUITTGlyph * glyph = new CGUITTGlyph();

		glyph->size = size;
		//		glyph->cache((wchar_t)i + 1);

		Glyphs[i] = glyph;
	}

	// TODO: this is a workaround to get a probably ok height for getDimensions. So we check a few extreme characters which usually make trouble.
	getGlyphByChar(L'A');
	getGlyphByChar(L'g');
	getGlyphByChar(L'.');
	getGlyphByChar(L'(');

	return	true;
}

void CGUIFreetypeFont::clearGlyphs()
{
	for ( unsigned int i=0; i < Glyphs.size(); ++i )
	{
		if ( Glyphs[i] )
		{
			Glyphs[i]->drop();
		}
		Glyphs[i] = 0;
	}
}

u32 CGUIFreetypeFont::getGlyphByChar(wchar_t c) const
{
	u32 idx = FT_Get_Char_Index( TrueTypeFace->face, c );
	if ( idx && !Glyphs[idx - 1]->cached )
		Glyphs[idx - 1]->cache(idx, this);
	return	idx;
}

//! returns the dimension of a text
core::dimension2d<u32> CGUIFreetypeFont::getDimension(const wchar_t* text) const
{
	core::dimension2d<u32> dim(0, Glyphs[0]->size);

	for(const wchar_t* p = text; *p; ++p)
	{
		dim.Width += getWidthFromCharacter(*p);
	}

	// TODO: The correct solution might be working with TrueTypeFace->height but I can't figure out how to use units_per_EM
	// even if I know which FT_Render_Mode I used. I'm sure there is some way to figure that out, but I have to give up for now.
	if ( TrueTypeFace && LargestGlyph.Height > dim.Height)
		dim.Height = LargestGlyph.Height;

	return dim;
}


inline u32 CGUIFreetypeFont::getWidthFromCharacter(wchar_t c) const
{
	u32 n = getGlyphByChar(c);
	if ( n > 0)
	{
		int w = Glyphs[n - 1]->texw;
		s32 left = Glyphs[n - 1]->left;
		if (w + left > 0)
			return w + left;
	}
	if (c >= 0x2000)
	{
		return	Glyphs[0]->size;
	}
	else
	{
		return	Glyphs[0]->size / 2;
	}
}


//! draws an text and clips it to the specified rectangle if wanted
void CGUIFreetypeFont::draw(const irr::core::stringw& textstring, const irr::core::rect<s32>& position, video::SColor color, bool hcenter, bool vcenter, const core::rect<s32>* clip)
{
	if (!Driver)
		return;

	core::dimension2d<s32> textDimension;
	core::position2d<s32> offset = position.UpperLeftCorner;
	video::SColor colors[4];
	for (int i = 0;i < 4;i++)
	{
		colors[i] = color;
	}

	const wchar_t * text = textstring.c_str();
	if (hcenter || vcenter)
	{
		textDimension = getDimension(text);

		if (hcenter)
			offset.X = ((position.getWidth() - textDimension.Width)>>1) + offset.X;

		if (vcenter)
			offset.Y = ((position.getHeight() - textDimension.Height)>>1) + offset.Y;
	}

	u32 n;

	while(*text)
	{
		n = getGlyphByChar(*text);
		if ( n > 0)
		{
			if (AntiAlias)
			{
				//				s32 imgw = Glyphs[n-1]->imgw;
				//				s32 imgh = Glyphs[n-1]->imgh;
				s32 texw = Glyphs[n-1]->texw;
				s32 texh = Glyphs[n-1]->texh;
				s32 offx = Glyphs[n-1]->left;
				s32 offy = Glyphs[n-1]->size - Glyphs[n-1]->top;
				if (Driver->getDriverType() != video::EDT_SOFTWARE)
				{
					if (!Transparency)
						color.color |= 0xff000000;
					Driver->draw2DImage(Glyphs[n-1]->tex,core::position2d<s32>(offset.X+offx,offset.Y+offy),core::rect<s32>(0,0,texw,texh),clip,color,true);
				}
				else
				{
					s32 a = color.getAlpha();
					s32 r = color.getRed();
					s32 g = color.getGreen();
					s32 b = color.getBlue();
					u8 *pt = Glyphs[n-1]->image;
					if (!Transparency)	a = 255;
					for (int y = 0;y < texh;y++)
					{
						for (int x = 0;x < texw;x++)
						{
							if (!clip || clip->isPointInside(core::position2d<s32>(offset.X+x+offx,offset.Y+y+offy)))
							{
								if (*pt)
								{
									Driver->draw2DRectangle(video::SColor((a * *pt)/255,r,g,b),core::rect<s32>(offset.X+x+offx,offset.Y+y+offy,offset.X+x+offx+1,offset.Y+y+offy+1));
								}
								pt++;
							}
						}
					}
				}
			}
			else
			{
				//				s32 imgw = Glyphs[n-1]->imgw16;
				//				s32 imgh = Glyphs[n-1]->imgh16;
				s32 texw = Glyphs[n-1]->texw16;
				s32 texh = Glyphs[n-1]->texh16;
				s32 offx = Glyphs[n-1]->left16;
				s32 offy = Glyphs[n-1]->size - Glyphs[n-1]->top16;
				if (!Transparency)
				{
					color.color |= 0xff000000;
				}
				Driver->draw2DImage(Glyphs[n-1]->tex16,
					core::position2d<s32>(offset.X+offx,offset.Y+offy),
					core::rect<s32>(0,0,texw,texh),
					clip, color, true);
			}
			offset.X += getWidthFromCharacter(*text);
		}
		else
		{
			offset.X += getWidthFromCharacter(*text);
		}

		++text;
	}
}

//! Calculates the index of the character in the text which is on a specific position.
s32 CGUIFreetypeFont::getCharacterFromPos(const wchar_t* text, s32 pixel_x) const
{
	s32 x = 0;
	s32 idx = 0;

	while (text[idx])
	{
		x += getWidthFromCharacter(text[idx]);

		if (x >= pixel_x)
			return idx;

		++idx;
	}

	return -1;
}

irr::video::ITexture* CGUIFreetypeFont::GenerateTextTexture( const irr::core::stringw& txt )
{
	std::vector<CGUITTGlyph*> vec;

	{
		auto *text = txt.c_str();
		irr::u32 n = 0;
		while(*text)
		{
			n = getGlyphByChar(*text);
			if ( n > 0)
			{
				vec.push_back(Glyphs[n-1]);
			}
			++text;
		}
	}

	{
		unsigned width = 0;
		unsigned height = 0;
		irr::video::ECOLOR_FORMAT fmt;
		for ( auto& curGlyph : vec )
		{
			auto texw = curGlyph->texw;
			auto texh = curGlyph->texh;
			auto offx = curGlyph->left;
			auto offy = curGlyph->size - curGlyph->top;

			width += texw + offx;
			if ( height < texh )
			{
				height = texh;
				fmt = curGlyph->tex->getColorFormat();
			}
			assert(fmt == curGlyph->tex->getColorFormat());
		}

		auto clrComp = irr::video::ECF_A8R8G8B8 == fmt ? 4 :3;
		auto newImage = Driver->createImage(fmt, irr::core::dimension2du(width, height));
		auto imageData = static_cast<char*>(newImage->lock());
		memset(imageData, 0, width * height * clrComp);

		auto perImageLineWidth = width * clrComp;

		auto imageWidthOffset = 0;
		for ( auto& curGlyph : vec )
		{
			auto texData = static_cast<char*>(curGlyph->tex->lock(irr::video::ETLM_READ_ONLY));	
			
			auto offsetY = height - curGlyph->texh;
			auto offsetX = curGlyph->left;
			imageWidthOffset += offsetX * clrComp;

			for ( unsigned curHeightLine=0; curHeightLine<height; ++curHeightLine )
			{
				if ( curHeightLine >= curGlyph->texh )
				{
					break;
				}

				memcpy(imageData + perImageLineWidth * (curHeightLine + offsetY) + imageWidthOffset, texData + curGlyph->size * clrComp * curHeightLine, curGlyph->texw * clrComp);
			}
			curGlyph->tex->unlock();

			imageWidthOffset += curGlyph->texw * clrComp;
		}

		static	unsigned	ID = 0;
		std::wstring s = txt.c_str();
		s += std::to_wstring(ID++);

		auto ret = Driver->addTexture(s.c_str(), newImage);
		newImage->drop();

		return ret;
	}
}

irr::scene::IMesh* CGUIFreetypeFont::GenerateTextMesh(const irr::core::stringw& txt )
{
	std::vector<CGUITTGlyph*> vec;
	{
		auto *text = txt.c_str();
		irr::u32 n = 0;
		while(*text)
		{
			n = getGlyphByChar(*text);
			if ( n > 0)
			{
				vec.push_back(Glyphs[n-1]);
			}

			++text;
		}
	}

	auto mesh = new SMesh;

	unsigned totalOffset = 0;

	for ( auto& curGlyph : vec )
	{
		auto texw = curGlyph->texw;
		auto texh = curGlyph->texh;
		auto offx = curGlyph->left;
		auto offy = static_cast<int>(curGlyph->size - curGlyph->texh)-static_cast<int>(curGlyph->size - curGlyph->top);

		auto umin = -static_cast<float>(offx)/curGlyph->imgw;
		auto umax = static_cast<float>(texw)/curGlyph->imgw;
		auto vmin = static_cast<float>(curGlyph->imgh - curGlyph->texh)/curGlyph->imgh;
		auto vmax = 1.f;

		auto meshBuf = new SMeshBuffer;
		meshBuf->getMaterial().Lighting = false;
		meshBuf->getMaterial().BackfaceCulling = false;
		meshBuf->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_FONT);
		meshBuf->getMaterial().DiffuseColor = 0xFF000000;
		meshBuf->getMaterial().setTexture(0, curGlyph->tex);

		meshBuf->Vertices.reallocate(4);

		meshBuf->Vertices.push_back(S3DVertex(vector3df(static_cast<float>(totalOffset), 0, -static_cast<float>(offy)), vector3df(0,1,0), SColor(~0), vector2df(umin, -vmin)));
		meshBuf->Vertices.push_back(S3DVertex(vector3df(static_cast<float>(totalOffset+offx+texw), 0, -static_cast<float>(offy)), vector3df(0,1,0), SColor(~0), vector2df(umax, -vmin)));
		meshBuf->Vertices.push_back(S3DVertex(vector3df(static_cast<float>(totalOffset+offx+texw), 0, -static_cast<float>(texh)-static_cast<float>(offy)), vector3df(0,1,0), SColor(~0), vector2df(umax, -vmax)));
		meshBuf->Vertices.push_back(S3DVertex(vector3df(static_cast<float>(totalOffset), 0, -static_cast<float>(texh)-static_cast<float>(offy)), vector3df(0,1,0), SColor(~0), vector2df(umin, -vmax)));

		meshBuf->Indices.reallocate(6);
		meshBuf->Indices.push_back(0);
		meshBuf->Indices.push_back(1);
		meshBuf->Indices.push_back(2);
		meshBuf->Indices.push_back(0);
		meshBuf->Indices.push_back(2);
		meshBuf->Indices.push_back(3);

		mesh->addMeshBuffer(meshBuf);
		meshBuf->recalculateBoundingBox();
		meshBuf->drop();

		totalOffset += offx + texw;
	}

	mesh->recalculateBoundingBox();
	auto curBox = mesh->getBoundingBox();
	auto center = curBox.getCenter();
	auto extent = curBox.getExtent();

	for ( unsigned index=0; index<mesh->getMeshBufferCount(); ++index )
	{
		auto curBuf = mesh->getMeshBuffer(index);
		for ( unsigned vindex=0; vindex<curBuf->getVertexCount(); ++vindex )
		{
			auto& curPos = curBuf->getPosition(vindex);
			curPos -= center;
			curPos.X /= extent.Z;
			curPos.Z /= extent.Z;
		}
		curBuf->recalculateBoundingBox();
	}
	mesh->recalculateBoundingBox();

	return mesh;
}

#endif // #if COMPILE_WITH_FREETYPE
