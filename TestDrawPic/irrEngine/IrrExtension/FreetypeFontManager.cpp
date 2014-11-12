#include "stdafx.h"

#include "FreetypeFontManager.h"
#include "GUIFreetypeFont.h"

#include <sstream>

FreetypeFontMgr::FreetypeFontMgr()
{
}

FreetypeFontMgr::~FreetypeFontMgr()
{
	for ( auto& curFont : FontMap_ )
	{
		curFont.second->drop();
	}

	for ( auto& curFace : FaceMap_ )
	{
		curFace.second->drop();
	}
}

CGUIFreetypeFont* FreetypeFontMgr::GetTtFont(irr::video::IVideoDriver* driver, const char* filename_, unsigned int size_, bool antiAlias_, bool transparency_)
{
	if ( !filename_ || !strlen(filename_) )
		return NULL;

	// Make a unique font name for the given settings.
	// We need a new font for each setting, but only a new face when loading a different fontfile
	std::string fontString(MakeFontIdentifier(filename_, size_, antiAlias_, transparency_));
	FontMap::iterator itFont = FontMap_.find(fontString);
	if  (itFont != FontMap_.end() )
		return itFont->second;

	// check if the face is already loaded
	std::string faceName(filename_);
	CGUITTFace * face = NULL;
	FaceMap::iterator itFace = FaceMap_.find(faceName);
	if  (itFace != FaceMap_.end() )
	{
		face = itFace->second;
	}

	// no face loaded
	if ( !face )
	{
		// make a new face
		face = new CGUITTFace;
		if ( !face->load(filename_) )
		{
			face->drop();
			return NULL;
		}
		FaceMap_[faceName] = face;
	}

	// access to the video driver in my application.
	CGUIFreetypeFont * font = new CGUIFreetypeFont(driver);

	font->attach(face, size_);
	font->AntiAlias = antiAlias_;
	font->Transparency = transparency_;
	FontMap_[fontString] = font;

	return font;
}

// make a unique font name for different settings.
std::string FreetypeFontMgr::MakeFontIdentifier(const char* filename_, unsigned int size_, bool antiAlias_, bool transparency_)
{
	std::ostringstream stream;
	stream << filename_ << size_;
	if ( antiAlias_ )
		stream << 'a';
	if ( transparency_ )
		stream << 't';

	//fprintf(stderr, "font: %s", stream.str().c_str());

	return stream.str();
}

