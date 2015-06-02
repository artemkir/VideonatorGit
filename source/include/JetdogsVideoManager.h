#ifndef _JetdogsVideoManager_h
#define _JetdogsVideoManager_h

#include "UnityPluginInterface.h"

#include "JetdogsVideoExport.h"
#include "TheoraVideoManager.h"
#include <string>
#include <map>

typedef std::string String;

namespace Jetdogs
{
	//class TexturePtr;

	class JetdogsTheoraDataStream;
	
	class JetdogsVideoManager : public TheoraVideoManager
	{
		float mVideoTime;
		TheoraDataSource *videoStream;

		TheoraVideoClip* clip;
        
#if !defined(UNITY_WIN)
        size_t mFrameBufferSize;
        unsigned char *mFrameBuffer;
#endif
		//std::map<std::string,TexturePtr> mTextures;
		bool mbInit;
	public:
		JetdogsVideoManager(int num_worker_threads=1);
		~JetdogsVideoManager();

		/**
			@remarks
				This function is called to init this plugin - do not call directly
		*/
		bool initialise();
		void shutDown();
		
	  //bool setParameter(const String &name,const String &value);
        //String getParameter(const String &name) const;
        
		TheoraVideoManager* getTheoraVideoManager();

		int createVideo(const wchar_t *buf, int num_precached);
		int getParam(int param);
		void updateVideoData(float time_since_last_frame, unsigned char *dest,int tex_w, int tex_h, int tex_fmt);
		unsigned char* getVideoData();

		int createVideoMem(unsigned char *buf, int size,int num, int output);
	
		void destroyClip();

		bool hasClip();
        
        //unsigned char* beginFrame(float time_since_last_frame);
        //int endFrame();

	};

}
#endif

