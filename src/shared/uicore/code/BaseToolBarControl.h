/*
Copyright (C) 2011 Mark Chandler (Desura Net Pty Ltd)
Copyright (C) 2014 Bad Juju Games, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA.

Contact us at legal@badjuju.com.
*/

#ifndef DESURA_BASETOOLBARCONTROL_H
#define DESURA_BASETOOLBARCONTROL_H
#ifdef _WIN32
#pragma once
#endif

#include "wx_controls/gcImageHandle.h"
#include "wx_controls/gcControls.h"

class BaseToolBarControl : public gcPanel
{
public:
	BaseToolBarControl(wxWindow* parent);

	Event<int32> onButtonClickedEvent;
	EventV onPageStartLoadingEvent;
	EventV onPageEndLoadingEvent;

	Event<gcString> onSearchEvent;
	Event<gcString> onFullSearchEvent;

	virtual void onActiveToggle(bool state)=0;

protected:
	void onEraseBG(wxEraseEvent& event);
	virtual void onButtonClick(wxCommandEvent &event);

private:
	gcImageHandle m_imgBg;
};

#endif //DESURA_BASETOOLBARCONTROL_H
