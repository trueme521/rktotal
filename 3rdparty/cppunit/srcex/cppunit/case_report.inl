// case_report.inl 用于输出案例运行结果的函数
// wanghui 2007-8-13
#include "case_report.h"
namespace case_report
{
	/*
	案例运行情况报告文件，位于案例.exe同路径下，名称是："案例.exe.csv"
	*/
	const char* GetReportFileA(const char* pcszModuleName)
	{
		static char szModuleFileName[MAX_PATH] = {0};
		if (0 == szModuleFileName[0])
		{
			if (NULL == pcszModuleName || 0 == pcszModuleName[0])
			{
				::GetModuleFileName(NULL, szModuleFileName, MAX_PATH);
			}
			else
			{
				strcpy(szModuleFileName, pcszModuleName);
			}
			strcat(szModuleFileName, ".caserpt.csv");
		}
		return szModuleFileName;
	}

	/*
	每当案例.exe开始执行时，就调用Clear，以清空文件
	*/
	void ClearA(const char* pcszModuleName)
	{
		remove(GetReportFileA(pcszModuleName));
	}
	/*
	每执行完案例中的一个case，就调用一次Report
	pcszDateTime 运行日期和时间
	pcszOwnner 案例负责人
	pcszExe 案例.exe的名称
	pcszCase 案例中的case名称
	pcszStat 此case是否通过 (passed, failed)
	*/
	void ReportA(const char* pcszDateTime, const char* pcszOwnner, const char* pcszExe, const char* pcszCase, const char* pcszStat, const char* pcszModuleName)
	{
		ReportToFileA(GetReportFileA(pcszModuleName), pcszDateTime, pcszOwnner, pcszExe, pcszCase, pcszStat);
	}

	void ReportToFileA(const char* pcszFileName, const char* pcszDateTime, const char* pcszOwnner, const char* pcszExe, const char* pcszCase, const char* pcszStat)
	{
		std::ofstream ofs;
		ofs.open(pcszFileName, std::ios::out | std::ios::app);
		if (ofs.good())
		{
			ofs.seekp(0, std::ios::end);
			if (0 == ofs.tellp())
				ofs << "DateTime,Ownner,Exe,Case,Stat\n";

			ofs << pcszDateTime << ',' << pcszOwnner << ',' << pcszExe << ',' << pcszCase << ',' << pcszStat;
			ofs << '\n';
		}
	}
}
