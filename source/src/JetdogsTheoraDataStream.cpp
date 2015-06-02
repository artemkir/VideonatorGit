#include "JetdogsTheoraDataStream.h"
#include <sys/stat.h>
#include <wchar.h>

#include <stdlib.h>
#include <string.h>


namespace Jetdogs
{
	JetdogsTheoraDataStream::JetdogsTheoraDataStream(unsigned char *_buf, int _size)
	{
		buf = 0;
		buf_size = _size;

		buf = (unsigned char *)malloc(_size);

		if(buf == 0)
			return;

		memcpy(buf,_buf,_size);

		cur_pos = buf;
		end = buf + buf_size;
	}

	bool JetdogsTheoraDataStream::hasBuf()
	{
		return buf != 0;
	}

	JetdogsTheoraDataStream::~JetdogsTheoraDataStream()
	{
		if(hasBuf())
		{
			free(buf);
		}

	}

	int JetdogsTheoraDataStream::read(void* output,int nBytes)
	{
		int cnt = nBytes;

		if(cur_pos + cnt > end)
			cnt = end - cur_pos;

		if(cnt == 0)
			return 0;

		memcpy(output, cur_pos, cnt);

		cur_pos += cnt;

		return cnt;
	}

	void JetdogsTheoraDataStream::seek(unsigned long byte_index)
	{
		//mStream->seek(byte_index);

		cur_pos = buf + byte_index;
	}

	std::string JetdogsTheoraDataStream::repr()
	{
		return "test";//mName;
	}

	unsigned long JetdogsTheoraDataStream::size()
	{
		return buf_size;
	}

	unsigned long JetdogsTheoraDataStream::tell()
	{
		return cur_pos - buf;
	}


	//---------------
	JetdogsTheoraDataStreamFromFile::JetdogsTheoraDataStreamFromFile(std::wstring filename)
	{
		fname = filename;

		file = 0;

        #if defined(UNITY_WIN)
            _wfopen_s(&file, filename.c_str(),L"rb");

            struct _stat64i32 st;
            _wstat(filename.c_str(), &st);
            fsize = st.st_size;
        #else




        #endif

	}

	JetdogsTheoraDataStreamFromFile::~JetdogsTheoraDataStreamFromFile()
	{
		if (file)
		{
			fclose(file);
			file = 0;
		}
	}

	int JetdogsTheoraDataStreamFromFile::read(void* output,int nBytes)
	{
		return fread(output,1,nBytes,file);
	}

	void JetdogsTheoraDataStreamFromFile::seek(unsigned long byte_index)
	{
		fseek(file,byte_index,SEEK_SET);
	}

	std::string JetdogsTheoraDataStreamFromFile::repr()
	{
		return "";
	}

	unsigned long JetdogsTheoraDataStreamFromFile::size()
	{
		return fsize;
	}

	unsigned long JetdogsTheoraDataStreamFromFile::tell()
	{
		return ftell(file);
	}

} // end namespace Jetdogs
