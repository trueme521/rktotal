
#pragma once
#ifndef __LIB_7_ZIP_H__
#define __LIB_7_ZIP_H__
#include <string>

int Compress7z(const std::wstring & strInFile, const std::wstring & strOutFile);
int Uncompress7z(const std::wstring & strInFile, const std::wstring & strOutFile);

#endif //__LIB_7_ZIP_H__
