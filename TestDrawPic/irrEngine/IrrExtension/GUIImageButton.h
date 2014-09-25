#ifndef GUIImageButton_h__
#define GUIImageButton_h__

#include "Irrlicht/CGUIImage.h"

class	CGUIImageButton : public irr::gui::CGUIImage
{
public:

	enum EImageState
	{
		EIS_NORMAL = irr::gui::EGET_COUNT + 1,
		EIS_HOVERD = EIS_NORMAL + 1,
		EIS_PRESSED = EIS_HOVERD + 1,
		EIS_LEFTUP = EIS_PRESSED + 1
	};

public:

	//! constructor
	CGUIImageButton(irr::gui::IGUIEnvironment* environment, irr::gui::IGUIElement* parent, irr::s32 id, irr::core::rect<irr::s32> rectangle);

	//! called if an event happened.
	virtual bool OnEvent(const irr::SEvent& event);

	EImageState		GetState() const { return State_; }

	void			SetState(EImageState state) { State_ = state; }

	bool			IsHoldPressed() const { return HoldPressed_; }

	void			SetHoldPressed(bool val) { HoldPressed_ = val; }

private:

	EImageState	State_;
	irr::core::position2di	StartPoint_;
	bool					HoldPressed_;
};
#endif // GUIImageButton_h__