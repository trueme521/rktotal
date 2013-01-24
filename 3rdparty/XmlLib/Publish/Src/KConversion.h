//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KConversion.h
//  Version     :   1.0
//  Creater     :   YangXD(yxdh)
//  Date        :   2006-2-22 11:45:56
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _K_CONVERSION_H_
#define _K_CONVERSION_H_


namespace KXML
{
#ifndef __ATLCONV_H__	// not include the atlconv.h


#if defined(_UNICODE) || defined(UNICODE)

#define CA2T CA2W
#define CT2A CW2A

#else  // !_UNICODE && !UNICODE

#define CW2T CW2A
#define CT2W CA2W

#endif  // #if defined(_UNICODE) || defined(UNICODE)


inline bool Utf8_To_Utf16(wchar_t *dest, size_t *destLen, const char *src, size_t srcLen)
{
	const unsigned char kUtf8Limits[5] = { 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

	size_t destPos = 0, srcPos = 0;
	for (;;)
	{
		unsigned char c;
		int numAdds;
		if (srcPos == srcLen)
		{
			*destLen = destPos;
			return true;
		}
		c = (unsigned char)src[srcPos++];

		if (c < 0x80)
		{
			if (dest)
				dest[destPos] = (wchar_t)c;
			destPos++;
			continue;
		}
		if (c < 0xC0)
			break;
		for (numAdds = 1; numAdds < 5; numAdds++)
			if (c < kUtf8Limits[numAdds])
				break;
		unsigned int value = (c - kUtf8Limits[numAdds - 1]);

		do
		{
			unsigned char c2;
			if (srcPos == srcLen)
				break;
			c2 = (unsigned char)src[srcPos++];
			if (c2 < 0x80 || c2 >= 0xC0)
				break;
			value <<= 6;
			value |= (c2 - 0x80);
		}
		while (--numAdds != 0);

		if (value < 0x10000)
		{
			if (dest)
				dest[destPos] = (wchar_t)value;
			destPos++;
		}
		else
		{
			value -= 0x10000;
			if (value >= 0x100000)
				break;
			if (dest)
			{
				dest[destPos + 0] = (wchar_t)(0xD800 + (value >> 10));
				dest[destPos + 1] = (wchar_t)(0xDC00 + (value & 0x3FF));
			}
			destPos += 2;
		}
	}
	*destLen = destPos;
	return false;
}

inline bool Utf16_To_Utf8(char *dest, size_t *destLen, const wchar_t *src, size_t srcLen)
{
	const unsigned char kUtf8Limits[5] = { 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
	size_t destPos = 0, srcPos = 0;
	for (;;)
	{
		unsigned numAdds;
		unsigned int value;
		if (srcPos == srcLen)
		{
			*destLen = destPos;
			return true;
		}
		value = src[srcPos++];
		if (value < 0x80)
		{
			if (dest)
				dest[destPos] = (char)value;
			destPos++;
			continue;
		}
		if (value >= 0xD800 && value < 0xE000)
		{
			unsigned int c2;
			if (value >= 0xDC00 || srcPos == srcLen)
				break;
			c2 = src[srcPos++];
			if (c2 < 0xDC00 || c2 >= 0xE000)
				break;
			value = ((value - 0xD800) << 10) | (c2 - 0xDC00);
		}
		for (numAdds = 1; numAdds < 5; numAdds++)
			if (value < (((unsigned int)1) << (numAdds * 5 + 6)))
				break;
		if (dest)
			dest[destPos] = (char)(kUtf8Limits[numAdds - 1] + (value >> (6 * numAdds)));
		destPos++;
		do
		{
			numAdds--;
			if (dest)
				dest[destPos] = (char)(0x80 + ((value >> (6 * numAdds)) & 0x3F));
			destPos++;
		}
		while (numAdds != 0);
	}
	*destLen = destPos;
	return false;
}

template< int t_nBufferLength = 128 >
class CA2WEX
{
public:
	CA2WEX(const char* psz) throw() :
		m_psz(m_szBuffer)
	{
		if (psz == NULL)
		{
			m_psz = NULL;
			return;
		}


		size_t destLen = 0;
		Utf8_To_Utf16(NULL, &destLen, psz, strlen(psz));


		if(destLen >= t_nBufferLength)
		{
			m_psz = static_cast<wchar_t*>(malloc((destLen + 1) * sizeof(wchar_t)));
			if (m_psz == NULL)
			{
				return;
			}
		}


		Utf8_To_Utf16(m_psz, &destLen, psz, strlen(psz));
		m_psz[destLen] = 0;
	}

	~CA2WEX() throw()
	{
		if(m_psz && m_psz != m_szBuffer)
		{
			free(m_psz);
		}
	}

	operator wchar_t*() const throw()
	{
		return(m_psz);
	}

private:
	wchar_t*	m_psz;
	wchar_t		m_szBuffer[t_nBufferLength];

private:
	CA2WEX(const CA2WEX&) throw();
	CA2WEX& operator=(const CA2WEX&) throw();
};
typedef CA2WEX<> CA2W;


template< int t_nBufferLength = 128 >
class CW2AEX
{
public:
	CW2AEX(const wchar_t* psz) throw() :
		m_psz(m_szBuffer)
	{
		if (psz == NULL)
		{
			m_psz = NULL;
			return;
		}


		size_t destLen = 0;
		Utf16_To_Utf8(NULL, &destLen, psz, wcslen(psz));


		if(destLen >= t_nBufferLength)
		{
			m_psz = static_cast<char*>(malloc((destLen + 1) * sizeof(char)));
			if (m_psz == NULL)
			{
				return;
			}
		}


		Utf16_To_Utf8(m_psz, &destLen, psz, wcslen(psz));
		m_psz[destLen] = 0;
	}

	~CW2AEX() throw()
	{
		if(m_psz && m_psz != m_szBuffer)
		{
			free(m_psz);
		}
	}

	operator char*() const throw()
	{
		return(m_psz);
	}

private:
	char*	m_psz;
	char	m_szBuffer[t_nBufferLength];

private:
	CW2AEX(const CW2AEX&) throw();
	CW2AEX& operator=(const CW2AEX&) throw();
};
typedef CW2AEX<> CW2A;


#endif // #ifndef __ATLCONV_H__
}


#endif	// #ifndef _K_CONVERSION_H_
