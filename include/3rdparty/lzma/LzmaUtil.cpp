
#include "LzmaUtil.h"
extern "C" 
{
	#include "Alloc.h"
	#include "7zStream.h"
	#include "LzmaDec.h"
	#include "LzmaEnc.h"
};
// 64KB
#define IN_BUF_SIZE (1 << 16)
#define OUT_BUF_SIZE (1 << 16)

void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
void SzFree(void *p, void *address) { p = p; MyFree(address); }
ISzAlloc myAlloc = { SzAlloc, SzFree };

SRes Decode2(CLzmaDec *state, 
			 ISeqOutStream *outStream, 
			 ISeqInStream *inStream,
			 UInt64 unpackSize)
{
	int thereIsSize = (unpackSize != (UInt64)(Int64)-1);
	size_t inPos = 0, inSize = 0, outPos = 0;
	Byte inBuf[IN_BUF_SIZE] = { 0 };
	Byte outBuf[OUT_BUF_SIZE] = { 0 };

	LzmaDec_Init(state);
	for (;;)
	{
		if (inPos == inSize)
		{
			inSize = IN_BUF_SIZE;
			RINOK(inStream->Read(inStream, inBuf, &inSize));
			inPos = 0;
		}
		{
			SRes res;
			SizeT inProcessed = inSize - inPos;
			SizeT outProcessed = OUT_BUF_SIZE - outPos;
			ELzmaFinishMode finishMode = LZMA_FINISH_ANY;
			ELzmaStatus status;
			if (thereIsSize && outProcessed > unpackSize)
			{
				outProcessed = (SizeT)unpackSize;
				finishMode = LZMA_FINISH_END;
			}

			res = LzmaDec_DecodeToBuf(state, outBuf + outPos, &outProcessed,
				inBuf + inPos, &inProcessed, finishMode, &status);
			inPos += inProcessed;
			outPos += outProcessed;
			unpackSize -= outProcessed;

			if (outStream)
				if (outStream->Write(outStream, outBuf, outPos) != outPos)
					return SZ_ERROR_WRITE;

			outPos = 0;

			if (res != SZ_OK || thereIsSize && unpackSize == 0)
				return res;

			if (inProcessed == 0 && outProcessed == 0)
			{
				if (thereIsSize || status != LZMA_STATUS_FINISHED_WITH_MARK)
					return SZ_ERROR_DATA;
				return res;
			}
		}
	}
}

SRes LzmaUtil_Decode(ISeqOutStream *outStream, ISeqInStream *inStream)
{
	UInt64 unpackSize;
	int i;
	SRes res = 0;

	CLzmaDec state;

	/* header: 5 bytes of LZMA properties and 8 bytes of uncompressed size */
	unsigned char header[LZMA_PROPS_SIZE + 8];

	/* Read and parse header */

	RINOK(SeqInStream_Read(inStream, header, sizeof(header)));

	unpackSize = 0;
	for (i = 0; i < 8; i++)
		unpackSize += (UInt64)header[LZMA_PROPS_SIZE + i] << (i * 8);

	LzmaDec_Construct(&state);
	RINOK(LzmaDec_Allocate(&state, header, LZMA_PROPS_SIZE, &myAlloc));
	res = Decode2(&state, outStream, inStream, unpackSize);
	LzmaDec_Free(&state, &myAlloc);
	return res;
}

SRes LzmaUtil_Encode(ISeqOutStream *outStream, ISeqInStream *inStream, UInt64 fileSize)
{
	CLzmaEncHandle enc;
	SRes res;
	CLzmaEncProps props;

	enc = LzmaEnc_Create(&myAlloc);
	if (enc == 0)
		return SZ_ERROR_MEM;

	LzmaEncProps_Init(&props);
	res = LzmaEnc_SetProps(enc, &props);

	if (res == SZ_OK)
	{
		Byte header[LZMA_PROPS_SIZE + 8];
		size_t headerSize = LZMA_PROPS_SIZE;
		int i;

		res = LzmaEnc_WriteProperties(enc, header, &headerSize);
		for (i = 0; i < 8; i++)
			header[headerSize++] = (Byte)(fileSize >> (8 * i));
		if (outStream->Write(outStream, header, headerSize) != headerSize)
			res = SZ_ERROR_WRITE;
		else
		{
			if (res == SZ_OK)
				res = LzmaEnc_Encode(enc, outStream, inStream, NULL, &myAlloc, &myAlloc);
		}
	}
	LzmaEnc_Destroy(enc, &myAlloc, &myAlloc);
	return res;
}
