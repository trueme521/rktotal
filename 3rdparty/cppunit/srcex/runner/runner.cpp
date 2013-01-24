// runner.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "framework/kis_base.h"
#include "cppunit/cppunit.h"

int _tmain(int argc, _TCHAR* argv[])
{
	USES_CONVERSION;

	BOOL fPause = FALSE;
	LPCWSTR szModule = NULL;
	LPCSTR szClass = NULL;
	LPCSTR szMethod = NULL;
	UINT idxArg = 0;
	LPCWSTR targetModule = NULL;

	KCppunitModule::CreateInstance(GetModuleHandle(NULL));

	for (int i = 1; i < __argc; ++i)
	{
		if (__wargv[i][0] == '/' || __wargv[i][0] == '-')
		{
			switch (tolower(__wargv[i][1]))
			{
			case 'p':
				fPause = TRUE;
				break;
			case 'm':
				targetModule = __wargv[++i];
				break;
			}
		}
		else
		{
			switch (idxArg)
			{
			case 0:	szModule = __wargv[i]; break;
			case 1: szClass = W2A(__wargv[i]); break;
			case 2: szMethod = W2A(__wargv[i]); break;
			}
			++idxArg;
		}
	}
	if (idxArg > 0)
	{
		TestApp app(rununitConsole);
		HRESULT hr = app.AddUnit(szModule);
		if (FAILED(hr))
		{
			printf("load %s failed!!!", W2A(szModule));
			return -1;
		}
		LPCWSTR szModuleName = wcsrchr(szModule, '\\');
		if (szModuleName)
			++szModuleName;
		else
		{
			szModuleName = wcsrchr(szModule, '/');
			if (szModuleName)
				++szModuleName;
			else
				szModuleName = szModule;
		}
		if (targetModule)
			_CppUnit_FilterCase(targetModule, szClass, szMethod);
		else
			_CppUnit_FilterCase(szModuleName, szClass, szMethod);
	}
	if (fPause)
	{
		getchar();
	}

	return 0;
}

