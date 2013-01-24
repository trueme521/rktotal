#include "7zip.h"

#include "LzmaUtil.h"
extern "C" 
{
	#include "7zFile.h"
};


int Compress7z(const std::wstring & strInFile, const std::wstring & strOutFile)
{
	// open the in & out files
	CFileSeqInStream inStream;
	CFileOutStream outStream;
	int nRetCode = false;

	FileSeqInStream_CreateVTable(&inStream);
	File_Construct(&inStream.file);

	FileOutStream_CreateVTable(&outStream);
	File_Construct(&outStream.file);

	if (InFile_Open(&inStream.file, strInFile.c_str()) != 0)
		goto Exit0;

	if (OutFile_Open(&outStream.file, strOutFile.c_str()) != 0)
		goto Exit0;


	UInt64 fileSize;
	File_GetLength(&inStream.file, &fileSize);
	nRetCode = LzmaUtil_Encode(&outStream.s, &inStream.s, fileSize);

	File_Close(&outStream.file);
	File_Close(&inStream.file);
	
Exit0:
	return 0;
}

int Uncompress7z(const std::wstring & strInFile, const std::wstring & strOutFile)
{
	// open the in & out files

	CFileSeqInStream inStream;
	CFileOutStream outStream;
	int nRetCode = false;

	FileSeqInStream_CreateVTable(&inStream);
	File_Construct(&inStream.file);

	FileOutStream_CreateVTable(&outStream);
	File_Construct(&outStream.file);

	if (InFile_Open(&inStream.file, strInFile.c_str()) != 0)
		goto Exit0;

	if (OutFile_Open(&outStream.file, strOutFile.c_str()) != 0)
		goto Exit0;


	nRetCode = LzmaUtil_Decode(&outStream.s, &inStream.s);

	File_Close(&outStream.file);
	File_Close(&inStream.file);

Exit0:
	return 0;
}
