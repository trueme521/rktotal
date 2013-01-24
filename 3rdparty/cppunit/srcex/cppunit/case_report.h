// case_report.h 用于输出案例运行结果的函数
// wanghui 2007-8-13

#ifndef __CASE_REPORT_H__
#define __CASE_REPORT_H__
namespace case_report
{
	/*
	案例运行情况报告文件，位于案例.exe同路径下，名称是："案例.exe.csv"
	*/
	// pcszModuleName == NULL, 根据当前Module获取名称
	// pcszModuleName != NULL, 根据指定Module获取名称
	const char* GetReportFileA(const char* pcszModuleName = NULL);

	/*
	每当案例.exe开始执行时，就调用Clear，以清空文件
	*/
	// pcszModuleName == NULL, 根据当前Module清除报告文件
	// pcszModuleName != NULL, 根据指定Module清除报告文件
	void ClearA(const char* pcszModuleName = NULL);
	
	/*
	每执行完案例中的一个case，就调用一次Report
	pcszDateTime 运行日期和时间
	pcszOwnner 案例负责人
	pcszExe 案例.exe的名称
	pcszCase 案例中的case名称
	pcszStat 此case是否通过 (passed, failed)
	*/
	// pcszModuleName == NULL, 根据当前Module产生一条记录
	// pcszModuleName != NULL, 根据指定Module产生一条记录
	void ReportA(const char* pcszDateTime, const char* pcszOwnner, const char* pcszExe, const char* pcszCase, const char* pcszStat, const char* pcszModuleName = NULL);

	// 将记录输出到指定文件
	void ReportToFileA(const char* pcszFileName, const char* pcszDateTime, const char* pcszOwnner, const char* pcszExe, const char* pcszCase, const char* pcszStat);

}

#endif // __CASE_REPORT_H__