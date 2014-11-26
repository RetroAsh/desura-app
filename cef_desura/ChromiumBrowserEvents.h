///////////// Copyright 2010 DesuraNet. All rights reserved. /////////////
//
//   Project     : desura_libcef_dll_wrapper
//   File        : ChromiumBrowserEvents.h
//   Description :
//      [TODO: Write the purpose of ChromiumBrowserEvents.h.]
//
//   Created On: 7/1/2010 11:35:47 PM
//   Created By: Mark Chandler <mailto:mark@moddb.com>
////////////////////////////////////////////////////////////////////////////

#ifndef DESURA_CHROMIUMBROWSEREVENTS_H
#define DESURA_CHROMIUMBROWSEREVENTS_H
#ifdef _WIN32
#pragma once
#endif

#include "include/cef_app.h"
#include "cef_desura_includes/ChromiumBrowserI.h"
#include "include/cef_client.h"

class ChromiumBrowser;


class ChromiumEventInfoI
{
public:
	virtual ChromiumDLL::ChromiumBrowserEventI* GetCallback()=0;
	virtual void SetBrowser(CefRefPtr<CefBrowser> browser)=0;
	virtual CefRefPtr<CefBrowser> GetBrowser()=0;
	virtual void setContext(CefRefPtr<CefV8Context> context)=0;
};

/////////////////////////////////////////////////////////////////////////////////////////////
/// LifeSpanHandler
/////////////////////////////////////////////////////////////////////////////////////////////

class LifeSpanHandler : public CefLifeSpanHandler, public virtual ChromiumEventInfoI
{
public:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser);
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser);
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> parentBrowser, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, const CefString& url, CefRefPtr<CefClient>& client, CefBrowserSettings& settings);
};


/////////////////////////////////////////////////////////////////////////////////////////////
/// LifeSpanHandler
/////////////////////////////////////////////////////////////////////////////////////////////

class LoadHandler : public CefLoadHandler, public virtual ChromiumEventInfoI
{
public:
	virtual void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame);
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode);
	virtual bool OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& failedUrl, CefString& errorText);
};


/////////////////////////////////////////////////////////////////////////////////////////////
/// RequestHandler
/////////////////////////////////////////////////////////////////////////////////////////////

class RequestHandler : public CefRequestHandler, public virtual ChromiumEventInfoI
{
public:
	virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool isRedirect);
};


/////////////////////////////////////////////////////////////////////////////////////////////
/// DisplayHandler
/////////////////////////////////////////////////////////////////////////////////////////////

class DisplayHandler : public CefDisplayHandler, public virtual ChromiumEventInfoI
{
public:
	virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString& message, const CefString& source, int line);
};


/////////////////////////////////////////////////////////////////////////////////////////////
/// KeyboardHandler
/////////////////////////////////////////////////////////////////////////////////////////////

class KeyboardHandler : public CefKeyboardHandler, public virtual ChromiumEventInfoI
{
public:
	virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event);
};

/////////////////////////////////////////////////////////////////////////////////////////////
/// MenuHandler
/////////////////////////////////////////////////////////////////////////////////////////////

class MenuHandler : public CefContextMenuHandler, public virtual ChromiumEventInfoI
{
public:
	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model);
};


/////////////////////////////////////////////////////////////////////////////////////////////
/// JSDialogHandler
/////////////////////////////////////////////////////////////////////////////////////////////

class JSDialogHandler : public CefJSDialogHandler, public virtual ChromiumEventInfoI
{
public:
	virtual bool OnJSAlert(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& message);
	virtual bool OnJSConfirm(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& message, bool& retval);
	virtual bool OnJSPrompt(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& message, const CefString& defaultValue, bool& retval, CefString& result);
};

/////////////////////////////////////////////////////////////////////////////////////////////
/// ChromiumBrowserEvents
/////////////////////////////////////////////////////////////////////////////////////////////

class ChromiumBrowserEvents : 
	public CefClient
	, public virtual ChromiumEventInfoI
	, public LifeSpanHandler
	, public LoadHandler
	, public RequestHandler
	, public DisplayHandler
	, public KeyboardHandler
	, public MenuHandler
	, public JSDialogHandler
{
public:
	ChromiumBrowserEvents(ChromiumBrowser* pParent);

	void setCallBack(ChromiumDLL::ChromiumBrowserEventI* cbe);
	void setParent(ChromiumBrowser* parent);

	virtual ChromiumDLL::ChromiumBrowserEventI* GetCallback();
	virtual void SetBrowser(CefRefPtr<CefBrowser> browser);
	virtual CefRefPtr<CefBrowser> GetBrowser();
	virtual void setContext(CefRefPtr<CefV8Context> context);

	virtual CefRefPtr<CefLifeSpanHandler>	GetLifeSpanHandler()	{ return (CefLifeSpanHandler*)this; }
	virtual CefRefPtr<CefLoadHandler>		GetLoadHandler()		{ return (CefLoadHandler*)this; }
	virtual CefRefPtr<CefRequestHandler>	GetRequestHandler()		{ return (CefRequestHandler*)this; }
	virtual CefRefPtr<CefDisplayHandler>	GetDisplayHandler()		{ return (CefDisplayHandler*)this; }
	virtual CefRefPtr<CefKeyboardHandler>	GetKeyboardHandler()	{ return (CefKeyboardHandler*)this; }
	virtual CefRefPtr<CefMenuHandler>		GetMenuHandler()		{ return (CefMenuHandler*)this; }
	virtual CefRefPtr<CefJSDialogHandler>	GetJSDialogHandler()	{ return (CefJSDialogHandler*)this; }

private:
	CefRefPtr<CefBrowser> m_Browser;
	ChromiumBrowser* m_pParent;
	ChromiumDLL::ChromiumBrowserEventI *m_pEventCallBack;

	IMPLEMENT_REFCOUNTING(ChromiumBrowserEvents);
};


#endif //DESURA_CHROMIUMBROWSEREVENTS_H
