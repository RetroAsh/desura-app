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

#include "Common.h"
#include "MainApp.h"

#include "Managers.h"
#include "wx_controls/gcMessageBox.h"

const wchar_t *modules[] =
{
	L"desura.exe",
	L"desura_service.exe",
	L"bin\\uicore.dll",
	L"bin\\usercore.dll",
	L"bin\\webcore.dll",
	L"bin\\mcfcore.dll",
	L"bin\\servicecore.dll",
};

const char *modulesNames[] =
{
	"desura.exe",
	"desura_service.exe",
	"uicore.dll",
	"usercore.dll",
	"webcore.dll",
	"mcfcore.dll",
	"servicecore.dll",
};

CONCOMMAND(cc_test_news, "test_news")
{
	std::vector<gcRefPtr<UserCore::Misc::NewsItem>> itemList;

	if (vArgList.size() == 1)
	{
		itemList.push_back(gcRefPtr<UserCore::Misc::NewsItem>::create(0, 0, "Link Alpha", "http://www.desura.com"));
		itemList.push_back(gcRefPtr<UserCore::Misc::NewsItem>::create(0, 0, "Link Bravo", "http://www.desura.com/groups"));
		itemList.push_back(gcRefPtr<UserCore::Misc::NewsItem>::create(0, 0, "Link Charlie", "http://www.desura.com/mods"));
		itemList.push_back(gcRefPtr<UserCore::Misc::NewsItem>::create(0, 0, "Link Delta", "http://www.desura.com/games"));
	}
	else
	{
		itemList.push_back(gcRefPtr<UserCore::Misc::NewsItem>::create(0, 0, "Test News Link", vArgList[1].c_str()));
	}

	dynamic_cast<MainApp*>(g_pMainApp)->onNewsUpdate(itemList);
	g_pMainApp->showNews();
}


extern const char* GetAppVersion();
extern "C" const char* GetMCFCoreVersion();
extern const char* GetUICoreVersion();

CONCOMMAND(cc_clearwccache, "clear_namecache")
{
	if (GetWebCore())
		GetWebCore()->clearNameCache();
}

CONCOMMAND(cc_PrintVersion, "version")
{
	Msg("--------------------------------------------------\n");
	Msg(gcString("Desura Version\t\t: {0}\n",	GetAppVersion()));
	Msg(gcString("MCFCore Version\t\t: {0}\n",	GetMCFCoreVersion()));
	Msg(gcString("UICore Version\t\t: {0}\n",	GetUICoreVersion()));
	Msg(gcString("UserCore Version\t\t: {0}\n",	GetUserCoreVersion()));
	Msg(gcString("WebCore Version\t\t: {0}\n",	GetWebCoreVersion()));
	Msg("--------------------------------------------------\n");
}

CONCOMMAND(cc_CheckCert, "checkcert")
{
#ifdef WIN32
	Msg("Checking Digital Signature:\n");
	Msg("--------------------------------------------------\n");

	wchar_t exePath[255];
	GetModuleFileNameW(nullptr, exePath, 255);

	size_t exePathLen = Safe::wcslen(exePath, 255);
	for (size_t x=exePathLen; x>0; x--)
	{
		if (exePath[x] == '\\')
			break;
		else
			exePath[x] = '\0';
	}

	for (size_t x=0; x<6; x++)
	{
		gcWString path(L"{0}{2}{1}", exePath, modules[x], DIRS_STR);
		char msg[255] = {0};

		uint32 res = UTIL::WIN::validateCert(path.c_str(), msg, 255);

#ifndef DEBUG
		if (res == TRUST_E_NOSIGNATURE)
			WarningS("Warning: {0}: {1}\n", modulesNames[x], msg);
		else
#endif
			Msg(gcString("{0}: {1}\n", modulesNames[x], msg));
	}
	Msg("--------------------------------------------------\n");
#endif
}

CONCOMMAND(cc_PrintThreads, "threadlist")
{
	if (!GetThreadManager())
	{
		WarningS("Cant print thread list as ThreadManager is nullptr.\n");
	}
	else
	{
		GetThreadManager()->printThreadList();
	}
}

#ifdef DEBUG

CONCOMMAND(cc_testexception, "testexception")
{
	try
	{
		throw gcException(ERR_XML_NOPRIMENODE, 5, "This is a very very very very very very very very very very very very very very very very very long message. This is a very very very very very very very very very very very very very very very very very long message. This is a very very very very very very very very very very very very very very very very very long message");
	}
	catch(gcException &e)
	{
		Msg("{0}", e);
	}
}

CONCOMMAND(cc_testformat, "testformat")
{
	gcString str("{0}", gcException(ERR_XML_NOPRIMENODE, 5, "Message"));
	Msg(str.c_str());
}

CONCOMMAND(cc_crash, "testcrash")
{
	UserCore::UserI *temp=nullptr;
	temp->logIn("crash", "crash");
}

CONCOMMAND(cc_MsgboxTest, "testmsgbox")
{
	gcMessageBox(g_pMainApp->getMainWindow(), "This is a short message!", "Desura message box test! (yes no, exclamation)", wxYES_NO|wxICON_EXCLAMATION);
	gcMessageBox(g_pMainApp->getMainWindow(), "This\n has\n a\n lot\n of\n new\n lines\n!", "Desura message box test! (ok, hand)", wxOK|wxICON_HAND);
	gcMessageBox(g_pMainApp->getMainWindow(), "This is a very very very very very very very very very very very very very very very very very long message", "Desura message box test! (close, question)", wxCLOSE|wxICON_QUESTION);
	gcMessageBox(g_pMainApp->getMainWindow(), "This is a short message!", "Desura message box test! (apply cancel, information)", wxAPPLY|wxCANCEL|wxICON_INFORMATION);
	gcMessageBox(nullptr, "This message box doesnt have a parent!", "Desura message box test! (nullptr parent)", wxOK|wxICON_INFORMATION);
}

CONCOMMAND(cc_NewsTest, "testnews")
{
	MainApp *ma = dynamic_cast<MainApp*>(g_pMainApp);

	std::vector<gcRefPtr<UserCore::Misc::NewsItem>> itemList;

	auto a = gcRefPtr<UserCore::Misc::NewsItem>::create(0, 0, "News item 1", "http://www.desura.com");
	auto b = gcRefPtr<UserCore::Misc::NewsItem>::create(0, 0, "News item 2", "http://www.desura.com/mods");
	auto c = gcRefPtr<UserCore::Misc::NewsItem>::create(0, 0, "News item 3", "http://www.desura.com/games");

	itemList.push_back(a);
	itemList.push_back(b);
	itemList.push_back(c);

	ma->onNewsUpdate(itemList);
	ma->showNews();
}

CONCOMMAND(cc_GiftTest, "testgifts")
{
	MainApp *ma = dynamic_cast<MainApp*>(g_pMainApp);

	std::vector<gcRefPtr<UserCore::Misc::NewsItem>> itemList;

	auto a = gcRefPtr<UserCore::Misc::NewsItem>::create(0, 0, "News item 1", "http://www.desura.com");
	auto b = gcRefPtr<UserCore::Misc::NewsItem>::create(0, 0, "News item 2", "http://www.desura.com/mods");
	auto c = gcRefPtr<UserCore::Misc::NewsItem>::create(0, 0, "News item 3", "http://www.desura.com/games");

	itemList.push_back(a);
	itemList.push_back(b);
	itemList.push_back(c);

	ma->onGiftUpdate(itemList);
}

#endif

CONCOMMAND(cc_testinstallscript, "test_install_script")
{
	if (vArgList.size() < 4)
	{
		Msg("test_install_script [path] [install path] [function]\n\ntest javascript install script.");
	}
	else
	{
		Color c(0,0,255);

		LogMsg(MT_MSG, gcString("Running install script {0},\n\t Install Path: {1}\n\tCalling function {2}\n", vArgList[1], vArgList[2], vArgList[3]), &c);
		GetUserCore()->runInstallScript(vArgList[1].c_str(), vArgList[2].c_str(), vArgList[3].c_str());
	}
}


bool updoaddumpsChange(CVar* var, const char* val);
bool betaChange(CVar* var, const char* val);
bool corecountChange(CVar* var, const char* val);
bool autoStartChange(CVar* var, const char* val);
bool forceShortcutChange(CVar* var, const char* val);
bool OnLinuxBinChange(CVar* var, const char* val);
bool OnLinuxArgsChange(CVar* var, const char* val);

CVar gc_corecount("gc_corecount", "0", 0, (CVarCallBackFn)&corecountChange);
CVar gc_cleanmcf("gc_cleanmcf", "0", 0);
CVar gc_beta("gc_beta", "0", 0, (CVarCallBackFn)&betaChange);
CVar gc_autostart("gc_autostart", "0", 0, (CVarCallBackFn)&autoStartChange);
CVar gc_uploaddumps("gc_uploaddumps", "1", CFLAG_USER, (CVarCallBackFn)&updoaddumpsChange);
CVar gc_destroymain("gc_destroymain", "0", CFLAG_USER);
CVar gc_noloadtab("gc_noloadtab", "0", CFLAG_USER);
CVar gc_silentlaunch("gc_silentlaunch", "0", CFLAG_USER);
CVar gc_disable_wge("gc_disable_wge", "0", CVAR_WINDOWS_ONLY);
CVar gc_admin_largeupload("gc_admin_largeupload", "0", CFLAG_ADMIN);
CVar gc_ignore_fs_perms("gc_ignore_windows_permissions_against_marks_wishes", "1", CVAR_WINDOWS_ONLY);
CVar gc_disable_forceupdate_shortcut("gc_disable_forceupdate_shortcut", "0", CVAR_LINUX_ONLY, (CVarCallBackFn)&forceShortcutChange);

CVar gc_linux_launch_globalbin("gc_linux_launch_globalbin", "", CVAR_LINUX_ONLY, (CVarCallBackFn)&OnLinuxBinChange);
CVar gc_linux_launch_globalargs("gc_linux_launch_globalargs", "", CVAR_LINUX_ONLY, (CVarCallBackFn)&OnLinuxArgsChange);
CVar gc_safe_uploads("gc_safe_uploads", "0", CFLAG_USER);

CVar gc_mcfcreate_nopatch("gc_mcfcreate_nopatch", "0", CFLAG_USER);

#ifdef DESURA_OFFICIAL_BUILD

bool OnQaTestingChange(CVar* var, const char* val)
{
	//force the value to be set
	var->setValue(val);

	auto userCore = GetUserCore();

	if (userCore)
		userCore->setQATesting(var->getBool());

	return true;
}

CVar gc_qa_testing("gc_qa_testing", "0", CFLAG_NOCALLBACKONLOAD, (CVarCallBackFn)&OnQaTestingChange);

CONCOMMAND(cc_forcetestingupdate, "force_testing_update")
{
	auto userCore = GetUserCore();

	if (userCore)
		userCore->forceQATestingUpdate();
}

#endif

bool OnLinuxBinChange(CVar* var, const char* val)
{
	//force the value to be set
	var->setValue(val);

	auto userCore = GetUserCore();

	if (userCore)
		userCore->getItemManager()->regenLaunchScripts();

	return true;
}

bool OnLinuxArgsChange(CVar* var, const char* val)
{
	//force the value to be set
	var->setValue(val);

	auto userCore = GetUserCore();

	if (userCore)
		userCore->getItemManager()->regenLaunchScripts();

	return true;
}

bool forceShortcutChange(CVar* var, const char* val)
{
	gcString v(val);

	if (v == "true" || v == "1")
	{
		try
		{
			UTIL::FS::FileHandle fh(".ignore_force_shortcut", UTIL::FS::FILE_WRITE);
			fh.write(" ", 1);
		}
		catch (...)
		{
		}
	}
	else
	{
		UTIL::FS::delFile(".ignore_force_shortcut");
	}

	return true;
}

bool updoaddumpsChange(CVar* var, const char* val)
{
	bool ret = true;

	std::string szAppid = UTIL::OS::getConfigValue(APPID);

	uint32 appid = -1;

	if (szAppid.size() > 0)
		appid = Safe::atoi(szAppid.c_str());

	if (appid != UINT_MAX && (appid == BUILDID_BETA || appid == BUILDID_INTERNAL)  && (strcmp(val, "0")==0 || strcmp(val, "false")==0 ))
		ret = false;

	return ret;
}

bool betaChange(CVar* var, const char* val)
{
	std::string szAppid = UTIL::OS::getConfigValue(APPID);

	uint32 appid = -1;

	if (szAppid.size() > 0)
		appid = Safe::atoi(szAppid.c_str());

	auto userCore = GetUserCore();

	if (strcmp(val, "1")==0 || strcmp(val, "true")==0)
	{
		if (appid != BUILDID_BETA)
		{
			gc_uploaddumps.setValue("true");

			if (userCore)
				userCore->appNeedUpdate(BUILDID_BETA);
		}
	}
	else
	{
		if (appid != BUILDID_PUBLIC)
		{
			if (userCore)
				userCore->appNeedUpdate(BUILDID_PUBLIC);
		}
	}

	return true;
}

bool corecountChange(CVar* var, const char* val)
{
	uint16 count = Safe::atoi(val);
	uint16 coreCount = UTIL::MISC::getCoreCount();

	if (count > coreCount)
		return false;

	if (count == 0)
	{
		if (coreCount>1)
			coreCount--;

		gc_corecount.setValue(coreCount);
		return false;
	}

	return true;
}


bool autoStartChange(CVar* var, const char* val)
{
#ifdef WIN32
	if (strcmp(val, "1")==0 || strcmp(val, "true")==0)
	{
		char exePath[255];
		GetModuleFileNameA(nullptr, exePath, 255);

		UTIL::WIN::setRegValue(REGRUN, gcString("{0} -autostart", exePath).c_str());
	}
	else
	{
		UTIL::WIN::delRegValue(REGRUN);
	}
#endif // LINUX TODO
	return true;
}

#ifdef DEBUG

class PopUpThread : public Thread::BaseThread
{
public:
	PopUpThread() : Thread::BaseThread("popup")
	{
	}

	virtual void run()
	{
		gcSleep(20000);

		UserCore::Misc::UpdateInfo info;
		info.branch = 500;
		info.build = 200;

		GetUserCore()->getAppUpdateCompleteEvent()(info);
		//MainApp* app = dynamic_cast<MainApp*>(g_pMainApp);
		//app->onAppUpdate(info);
	}
};

CONCOMMAND(cc_test_popup, "test_popup")
{
	PopUpThread* popUpThread  = new PopUpThread();
	popUpThread->start();
}

#include "wx_controls/gcSpinnerProgBar.h"
class SpinnerTest : public wxFrame
{
public:
	SpinnerTest() : wxFrame(nullptr, wxID_ANY, "Test Spinner", wxDefaultPosition, wxSize(200,200))
	{
		this->SetSizeHints( wxDefaultSize, wxDefaultSize );

		wxBoxSizer* bSizer1 = new wxBoxSizer( wxVERTICAL );
		gcSpinnerProgBar* m_pbProgress = new gcSpinnerProgBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1,22 ));

		bSizer1->Add( m_pbProgress, 0, wxALL|wxEXPAND, 5 );

		this->SetSizer( bSizer1 );
		this->Layout();

		this->Centre( wxBOTH );
	}
};

CONCOMMAND(cc_test_spinner, "test_spinner")
{
	SpinnerTest* sp = new SpinnerTest();
	sp->Show();
}


CONCOMMAND(cc_test_form, "test_form")
{
	wxFrame* sp = new wxFrame(nullptr, wxID_ANY, "Test frame", wxDefaultPosition, wxSize(200,200), wxDEFAULT_FRAME_STYLE);
	sp->Show();
}

CONCOMMAND(cc_test_post, "test_post")
{
	HttpHandle hh("http://10.0.0.137/post.php");

	const char buffer[] = {44, 54, 46, 55};

	hh->addPostText("key", "--key--");
	hh->addPostText("action", "uploadchunk");
	hh->addPostText("siteareaid", 12345);
	hh->addPostText("sitearea", "--mods--");
	hh->addPostFileAsBuff("mcf", "upload.mcf", buffer, 4);
	hh->addPostText("uploadsize", 1234);

	hh->postWeb();

	std::string s(hh->getData(), hh->getDataSize());
	printf("[%s]\n", s.c_str());
}

CONCOMMAND(cc_test_crash, "test_crash")
{
	wxFrame* sp = nullptr;
	sp->Show();
}

#endif // DEBUG
