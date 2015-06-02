#include "JetdogsVideoManager.h"
#include "JetdogsTheoraDataStream.h"

#include "TheoraVideoFrame.h"
#include "TheoraTimer.h"
#include <vector>

#include <string.h>

#if defined(UNITY_WIN)
    #include "windows.h"
#endif

namespace Jetdogs
{
	extern JetdogsVideoManager* theoraVideoPlugin;

	int nextPow2(int x)
	{
		int y;
		for (y=1;y<x;y*=2);
		return y;
	}

	class ManualTimer : public TheoraTimer
	{
	public:
		void update(float t)
		{

		}

		void setTime(float time)
		{
			mTime=time;
		}
	};

	JetdogsVideoManager::JetdogsVideoManager(int num_worker_threads) : TheoraVideoManager(num_worker_threads)
	{
		//mPluginName = "TheoraVideoPlugin";
		//mDictionaryName = mPluginName;
		mbInit=false;
		//mTechniqueLevel=mPassLevel=mStateLevel=0;

		//texData = 0;
		videoStream = 0;

		clip = 0;

		initialise();
	}

	bool JetdogsVideoManager::initialise()
	{
		if (mbInit) return false;
		mbInit=true;
		//addBaseParams(); // ExternalTextureSource's function

#if !defined(UNITY_WIN)
        mFrameBufferSize = 0;
        mFrameBuffer = 0;
#endif

		return true;
	}


	bool JetdogsVideoManager::hasClip()
	{
		return clip != 0 && videoStream != 0;
	}

	JetdogsVideoManager::~JetdogsVideoManager()
	{
		shutDown();
	}

	void JetdogsVideoManager::shutDown()
	{
		if (!mbInit) return;

		destroyClip();

		mbInit=false;
	}

    void JetdogsVideoManager::destroyClip()
	{
		if(clip != 0)
		{
			debugOutput("destroyVideoClip: %s Ox%x", clip->getName().c_str(), clip);

			destroyVideoClip(clip);
			clip = 0;

#if !defined(UNITY_WIN)
            if(mFrameBuffer)
            {
                delete [] mFrameBuffer;
                mFrameBuffer = 0;
                mFrameBufferSize = 0;
            }
#endif

			debugOutput("clip destroyed: %x", clip);
		}


		if(videoStream != 0)
		{
			debugOutput("delete videoStream: %x", videoStream);

			delete videoStream;
			videoStream = 0;
		}
	}

	int JetdogsVideoManager::createVideo(const wchar_t *buf,int num)
	{
#if defined(__APPLE__)
        debugOutput("createVideo doesn't implemented yes for OSX!", buf);
        return -1;
#endif


		debugOutput("createVideo: %s", buf);

		if(buf == 0)
			return -2;

		destroyClip();

		videoStream = new JetdogsTheoraDataStreamFromFile(buf);

		if(videoStream == 0)
			return -2;

		//clip = createVideoClip(buf,TH_RGB,0,0 /* pow of 2 */);

		clip = createVideoClip(videoStream,TH_BGRA,num,true /* pow of 2 */);

		int w = getParam(0),
			h = getParam(1);

		debugOutput("createVideo: %d %d", w,h);



		//texData = new unsigned char[w*h*4];

		//if(texData == 0)
		//	return -3;

		if(clip == 0)
			return -2;

		mVideoTime = 0.0f;

		return w*h*4;
	}

	int JetdogsVideoManager::createVideoMem(unsigned char *buf, int size, int num, int outputmode)
	{
		if(size == 0 || buf == 0)
			return -2;

		destroyClip();

		videoStream = new JetdogsTheoraDataStream(buf,size);

		if(videoStream == 0)
			return -2;

		if(num > 100)
		{
			debugOutput("Something wrong with number of precached framed! It's too high! Aborting!");
			return -2;
		}

		clip = createVideoClip(videoStream,(TheoraOutputMode)outputmode,num,true /* pow of 2 */);

		int w = getParam(0),
			h = getParam(1);

#if !defined(UNITY_WIN)
        mFrameBufferSize = w*h*4;
        mFrameBuffer = new unsigned char[mFrameBufferSize];

        memset(mFrameBuffer,0,mFrameBufferSize);
#endif

		//texData = new unsigned char[w*h*4];

		//if(texData == 0)
		//	return -3;

		if(clip == 0)
			return -2;

		mVideoTime = 0.0f;

		return w*h*4;
	}

	int JetdogsVideoManager::getParam(int param)
	{
		int result = 0;

		if(!clip)
			return -2;

		switch(param)
		{
			//nextPow2??
			case 0: result = nextPow2(clip->getWidth()); break;
			case 1: result = nextPow2(clip->getHeight()); break;
			case 2: clip->play(); break;
			case 3: clip->stop(); break;
			case 4: result = clip->getWidth();  break;
			case 5: result = clip->getHeight(); break;
			case 6: clip->stop(); destroyClip(); break;
			case 7: result = (int)clip->isDone(); break;
			case 8: result = (int)(clip->getDuration()*1000.0f); break;
			case 9: clip->pause(); break;
			case 10: clip->restart(); break;
			case 11: result = (int)(clip->getTimePosition()*1000.0f); break;
			case 12: result = clip->getNumReadyFrames(); break;
			default: result = -1; break;
		}

		//debugOutput("getSetParam parameter: %d returned: %d", param, result);

		return result;
	}

	unsigned char* JetdogsVideoManager::getVideoData()
	{
#if !defined(UNITY_WIN)
        return mFrameBuffer;
#else
		return 0;//texData;
#endif
	}

    /*unsigned char* JetdogsVideoManager::beginFrame(float time_since_last_frame)
    {
        if(!clip)
            return NULL;

        float update_time = 0;

        if (time_since_last_frame > 0.3f)
            update_time = 0.3f;
        else
            update_time = time_since_last_frame;

        update(update_time);


        TheoraVideoFrame* f = clip->getNextFrame();
        if(!f)
            return NULL;

        if(mVideoTime > 0.0f && f->getFrameNumber() == 0)
            return 0;

        //int w=f->getStride(),h=f->getHeight();

        mVideoTime += update_time;

        return f->getBuffer();
    }

    int JetdogsVideoManager::endFrame()
    {
        if(!clip)
            return -2;

        clip->popFrame();

        return 0;
    }*/


    float acc_time = 0;

	void JetdogsVideoManager::updateVideoData(float time_since_last_frame, unsigned char *dest, int tex_w, int tex_h, int tex_fmt)
	{
		if(!clip)
			return;

                if(!dest)
                {
                 debugOutput("Dest buffer is null in updateVideoData!");
                 return;
                }

		float update_time = 0;

		if (time_since_last_frame > 0.3f)
			update_time = 0.3f;
		else
		    update_time = time_since_last_frame;

        /*acc_time += time_since_last_frame;

        ::printf("acc %f fps_time %f frame_time %f\n",acc_time,1.0/clip->getFPS(),time_since_last_frame);

        if(acc_time > 1.0/clip->getFPS())
        {
            acc_time = 0.0f;
        }
        else
        {
            return;
        }*/

		update(update_time);


		TheoraVideoFrame* f = clip->getNextFrame();

		if(!f)
			return;

		if(mVideoTime > 0.0f && f->getFrameNumber() == 0)
			return;

		//int w=f->getStride(),h=f->getHeight();

                int w=f->getStride(),h=f->getHeight();



	/*	if(!tex_w || !tex_h)
		{
			debugOutput("Texture size isn't assigned! Check assignTextureFromUnity parameters!");
		}

		if(tex_w < w)
		{
			w = tex_w;
			//debugOutput("You are trying to use texture size less than video size!");
		}

		if(tex_h < h)
		{
			h = tex_h;
		}*/

		unsigned char *videoData=f->getBuffer();


                //debugOutput("Video size %d %d %d %d!",w,h,dest,videoData);

		//copy data

		memcpy(dest,videoData,w*h*4);

        //memset(dest, 255, w*h*4);

		clip->popFrame();


		mVideoTime += update_time;

		return;

	}



} // end namespace



