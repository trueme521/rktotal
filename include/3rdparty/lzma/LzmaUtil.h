
#pragma once
#ifndef __LIB_LZAM_UTIL_H__
#define __LIB_LZAM_UTIL_H__

#include "types.h"
#include "7zVersion.h"
extern "C" 
{
	#include "7zFile.h"
};

SRes LzmaUtil_Encode(ISeqOutStream *outStream, ISeqInStream *inStream, UInt64 fileSize);

SRes LzmaUtil_Decode(ISeqOutStream *outStream, ISeqInStream *inStream);

#endif //__LIB_LZAM_UTIL_H__
