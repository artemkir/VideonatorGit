#ifndef _JetdogsTheoraDataStream_h
#define _JetdogsTheoraDataStream_h

#include "TheoraDataSource.h"
#include <string>

namespace Jetdogs
{

	class JetdogsTheoraDataStream : public TheoraDataSource
	{
		unsigned char *buf;
		int buf_size;
		unsigned char * cur_pos;
		unsigned char * end;

	public:
		JetdogsTheoraDataStream(unsigned char *_buf, int _size); 
		JetdogsTheoraDataStream(std::string filename,std::string group_name);
		~JetdogsTheoraDataStream();
		bool hasBuf();

		int read(void* output,int nBytes);
		void seek(unsigned long byte_index);
		std::string repr();
		unsigned long size();
		unsigned long tell();
	};


	class JetdogsTheoraDataStreamFromFile : public TheoraDataSource
	{
		std::wstring fname;
		FILE *file;
		long fsize;

	public:
		JetdogsTheoraDataStreamFromFile(std::wstring filename);
		~JetdogsTheoraDataStreamFromFile();
		bool hasBuf();

		int read(void* output,int nBytes);
		void seek(unsigned long byte_index);
		std::string repr();
		unsigned long size();
		unsigned long tell();
	};
}

#endif

