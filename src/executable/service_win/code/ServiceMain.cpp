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
#include "serviceMain.h"
#include "ServiceCoreI.h"
#include "Tracer.h"

#include <Winbase.h>


typedef void* (*FactoryBuilderFN)(const char*);


bool SetDllPath(const char* wdir);
uint32 ValidateCert(const wchar_t* pwszSourceFile, char* message, size_t size);

void SetCrashSettings(const wchar_t* user, bool upload);
void OnPipeDisconnect();


CGCServiceApp::CGCServiceApp()
{
	m_pServiceCore = nullptr;
	m_Fh = nullptr;

	m_MiniDump.setTracerSharedMemoryName(g_Tracer.getSharedMemName());
}

CGCServiceApp::~CGCServiceApp()
{
}

void CGCServiceApp::setCrashSettings(const wchar_t* user, bool upload)
{
	m_MiniDump.setUser(user);
	m_MiniDump.setUpload(upload);
}

bool CGCServiceApp::start(int argc, char** argv)
{
	DeleteFile("desura_service_old.exe");

	std::string wdir;

	for (size_t x=0; x<(size_t)argc; x++)
	{
		if (argv[x] && std::string(argv[x]) == "-wdir")
		{
			if (x+1 <= (size_t)argc && argv[x+1])
				wdir = argv[x+1];
		}
	}

	if (wdir.empty())
		wdir = ".\\bin";

	DeleteFile("desura_service_old.exe");
	DeleteFile("desura_old.exe");

	if (!SetDllPath(wdir.c_str()))
	{
		log("Failed to set dll path. :(\n");
		return false;
	}

	DeleteFile("desura_service_old.exe");
	DeleteFile("desura_old.exe");

#if !defined(DEBUG) && defined(DESURA_OFFICIAL_BUILD) && defined(WITH_CODESIGN)
	char message[255] = {0};

	gcWString strPath(".\\bin\\servicecore.dll");

	if (ValidateCert(strPath.c_str(), message, 255) != ERROR_SUCCESS)
	{
		log("Failed cert check on servicecore.dll: ");
		log(message);
		log("\n");
		return false;
	}
#endif

	if (!m_SCDLL.load("servicecore.dll"))
	{
		log("Failed to load servicecore.dll.\n");
		return false;
	}

	FactoryBuilderFN factory = m_SCDLL.getFunction<FactoryBuilderFN>("FactoryBuilderSC");

	if (!factory)
	{
		log("Failed to load FactoryBuilderSC function.\n");
		return false;
	}

	m_pServiceCore = (ServiceCoreI*)factory(SERVICE_CORE);

	if (!m_pServiceCore)
	{
		log("Failed to create service core.\n");
		return false;
	}

	m_pServiceCore->setDisconnectCallback(&OnPipeDisconnect);
	m_pServiceCore->setCrashSettingCallback(&SetCrashSettings);
	m_pServiceCore->startPipe();
	m_pServiceCore->setTracer(&g_Tracer);

    return true;
}

void CGCServiceApp::onDisconnect()
{
	//worst has come and the pipe has disconnected somehow, so we are going to stop
	//if it fails not much more we can do here
	log("Service Disconnected unexpectedly.\n");

	if (!m_pServiceCore->stopService(SERVICE_NAME))
		exit(1);
}

void CGCServiceApp::stop()
{
	if (m_pServiceCore)
	{
		m_pServiceCore->setTracer(nullptr);
		m_pServiceCore->stopPipe();
		m_pServiceCore->destroy();
	}

	if (m_Fh)
		fclose(m_Fh);

	m_Fh = nullptr;
	m_pServiceCore = nullptr;
}

void CGCServiceApp::log(const char* msg)
{
	if (!m_Fh)
	{
		m_Fh = Safe::fopen("desura_service.log", "a");

		char dateStr[9];
		char timeStr[9];
		_strdate_s(dateStr);
		_strtime_s(timeStr);

		if (m_Fh)
			fprintf(m_Fh, "\n\nStarted Logging: %s at %s\n", dateStr, timeStr);
	}

	if (m_Fh)
		fprintf(m_Fh, "%s", msg);

	printf("%s", msg);
}
