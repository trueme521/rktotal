/* -------------------------------------------------------------------------
// 文件名		:	file_stream.h
// 创建人		:	Jiang Wangsheng
// 创建时间		:	2009-7-16 17:29:49
// 功能描述     :	
//
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __XMLCFG_FILE_STREAM_H__
#define __XMLCFG_FILE_STREAM_H__

#include "xmlcfg.h"
#include <fstream>


// -------------------------------------------------------------------------
namespace xmlcfg {
// -------------------------------------------------------------------------


class FileOutputStream : public OutputStream
{
	std::ofstream m_file;

public:
	FileOutputStream() {}

	~FileOutputStream() {
		m_file.flush();
		m_file.close();
	}	

	bool open(LPCWSTR fileName) {
		m_file.open(fileName, std::ios::binary);
		return !(!m_file);
	}

	void close()
	{
		m_file.close();
	}

	bool write(const void* buffer, unsigned int cb)	{
		m_file.write( (char*)buffer, cb );
		return m_file.good();
	}

	bool flush() {
		m_file.flush();
		return m_file.good();
	}
};


// -------------------------------------------------------------------------


class FileInputStream : public InputStream
{
	std::ifstream m_file;

public:
	FileInputStream() {}

	~FileInputStream() {
		m_file.close();
	}

	bool open(LPCWSTR fileName) {
		m_file.open(fileName, std::ios::binary);
		return !(!m_file);
	}

	void close()
	{
		m_file.close();
	}

	bool read(void* buffer, unsigned int cb, unsigned int* readed) {
		m_file.read( (char*)buffer, cb );
		unsigned int __readed = m_file.gcount();
		if ( readed )
			*readed = __readed;
		return __readed > 0 ? true : false;
	}
};


// -------------------------------------------------------------------------
}	// End of namespace xmlcfg
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
// $Log: $

#endif /* __XMLCFG_FILE_STREAM_H__ */
