#pragma once

const unsigned char GIV[]={0x12,0x34,0x56,0x78,0x90,0xAB,0xCD,0xEF};
class CDes
{
public:
	CDes();
	~CDes();
public :
	enum PADDING_MODE		//填充模式枚举
	{
		PKCS5=0				
	};
	enum CIPHER_MODE		//分组运算模式
	{
		ECB=0,
		CBC=1
	};
	//加密接口函数
	char* Encrypt ( const char key[8], char* data, int& dataLen, PADDING_MODE pad, CIPHER_MODE cip, const unsigned char IV[8]=GIV);
	//解密接口函
	char* Decrypt ( const char key[8], char* data, int &resultLength, PADDING_MODE pad, CIPHER_MODE cip, const unsigned char IV[8]=GIV);
	char* Decrypt2 ( const char key[8], char* data, int nDataLen, int &resultLength, PADDING_MODE pad, CIPHER_MODE cip, const unsigned char IV[8]=GIV);
private:
	char* m_encryptBuf;
	char* m_decryptBuf;
	unsigned long m_eBufLen;
	unsigned long m_dBufLen; 

	int paddingWithPKCS5( char* pData, int dataLen);
	int unPaddingWithPKCS5( char* pData, int dataLen);
	unsigned long allocEncode(unsigned long newSize);
	unsigned long allocDecode(unsigned long newSize);
};
