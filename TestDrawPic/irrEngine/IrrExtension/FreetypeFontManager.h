#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <string>
#include <map>
#include <irrlicht.h>
#include "GUIFreetypeFont.h"

namespace irr
{
	namespace gui
	{
		class IGUIFont;
	};
};

class CGUITTFace;

class FreetypeFontMgr
{
	FreetypeFontMgr();
	~FreetypeFontMgr();

public:

	static	FreetypeFontMgr&	GetInstance()
	{
		static FreetypeFontMgr	instance;
		return instance;
	}
	
public:

	CGUIFreetypeFont* GetTtFont(irr::video::IVideoDriver* driver, const char* filename_, unsigned int size_, bool antiAlias_=true, bool transparency_=true);

protected:

	std::string MakeFontIdentifier(const char* filename_, unsigned int size_, bool antiAlias_, bool transparency_);

private:

	typedef std::map<std::string, CGUITTFace*> FaceMap;
	FaceMap FaceMap_;

	typedef std::map<std::string, CGUIFreetypeFont*> FontMap;
	FontMap FontMap_;
};

#endif // FONT_MANAGER_H
