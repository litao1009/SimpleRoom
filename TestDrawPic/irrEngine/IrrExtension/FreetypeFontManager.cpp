#include "stdafx.h"

#include "FreetypeFontManager.h"
#include "GUIFreetypeFont.h"

#include <sstream>
#include <string>

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

static	const std::string&	GetFontDir()
{
	static std::string str;
	if ( str.empty() )
	{
		auto size = GetWindowsDirectory(NULL, 0);
		str.resize(size);
		auto data = &str[0];
		GetWindowsDirectoryA(data, size);
		str.pop_back();

		str += "\\fonts\\";
	}

	return str;
}

CGUIFreetypeFont* FreetypeFontMgr::GetTtFont(irr::video::IVideoDriver* driver, const char* filename_, unsigned int size_, bool antiAlias_, bool transparency_)
{
	if ( !filename_ || !strlen(filename_) )
		return NULL;

	auto faceName = GetFontDir() + filename_;

	// Make a unique font name for the given settings.
	// We need a new font for each setting, but only a new face when loading a different fontfile
	std::string fontString(MakeFontIdentifier(faceName.c_str(), size_, antiAlias_, transparency_));
	FontMap::iterator itFont = FontMap_.find(fontString);
	if  (itFont != FontMap_.end() )
		return itFont->second;

	// check if the face is already loaded
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
		if ( !face->load(faceName.c_str()) )
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

