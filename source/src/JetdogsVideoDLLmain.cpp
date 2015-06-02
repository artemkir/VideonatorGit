#include "JetdogsVideoManager.h"
#include <stdio.h>
#include <string.h>

#if defined(UNITY_WIN)
    #include <windows.h>
    #include <d3d9.h>
    #include <d3d9caps.h>

    #define VIDEO_EXPORT extern "C" __declspec(dllexport)
#else

#if defined(UNITY_OSX)
    #include <OpenGl/gl.h>
    #include <OpenGL/OpenGL.h>
#elif defined(UNITY_LINUX)
    #include <GL/gl.h>

#else
    #include <OpenGLES/gltypes.h>
    #include <OpenGLES/ES2/gl.h>
#endif

    #define VIDEO_EXPORT extern "C"
#endif

#include <math.h>

namespace Jetdogs
{
	JetdogsVideoManager* theoraVideoPlugin = 0;

	void video_log(std::string message)
	{
		//MessageBox(0,message.c_str(),0,0);
	}

	typedef void (*FuncPtr)( const char * );

	FuncPtr Debug = 0;

	VIDEO_EXPORT void setDebugFunction( FuncPtr fp )
    {
        Debug = fp;
    }

	void _debugOutput(const std::string &str)
	{
		String str2 = std::string("VIDEONATOR: ") + str;

#if defined(UNITY_WIN) || defined(UNITY_LINUX)
		if(Debug != 0)
		{
			Debug(str2.c_str());
			return;
		}
#endif
		::printf(str2.c_str());
	}

	void __debugOutput(std::string str)
	{
		const std::string s = str;

		_debugOutput(s);
	}



	void debugOutput ( const char * format, ... )
	{
	  char buffer[256];
	  memset(buffer, 0, sizeof(buffer));

	  va_list args;
	  va_start (args, format);

#if defined(UNITY_WIN)
	  vsnprintf_s(buffer, _countof(buffer), _TRUNCATE, format, args);
#else
	  vsnprintf(buffer, sizeof(buffer), format, args);
#endif

        _debugOutput(buffer);

	  va_end (args);

	}


	VIDEO_EXPORT int dllStartPlugin()
	{
        ::printf("dllStartPlugin!");

		if(theoraVideoPlugin != 0)
			return -3;

		TheoraVideoManager::setLogFunction(__debugOutput);
		// Create our new External Texture Source PlugIn
		theoraVideoPlugin = new JetdogsVideoManager();

		return (int)theoraVideoPlugin;
		// Register with Manager
		//ExternalTextureSourceManager::getSingleton().setExternalTextureSource("ogg_video",theoraVideoPlugin);
		//Root::getSingleton().addFrameListener(theoraVideoPlugin);
	}

	VIDEO_EXPORT void dllStopPlugin()
	{
		//Root::getSingleton().removeFrameListener(theoraVideoPlugin);

		if(theoraVideoPlugin != 0)
		{
			delete theoraVideoPlugin;

			theoraVideoPlugin = 0;
		}
	}

	VIDEO_EXPORT int getDXCaps(int param);

	VIDEO_EXPORT int createVideo(const wchar_t *buf, int num_precached)
	{
		if(theoraVideoPlugin == 0)
                {
                 //dllStartPlugin();
			return -3;
                }

		debugOutput("Render caps: %d %d %d %d", getDXCaps(0), getDXCaps(1), getDXCaps(2), getDXCaps(3));

		return theoraVideoPlugin->createVideo(buf,num_precached);
	}


    VIDEO_EXPORT int _createVideoMem(unsigned char *buf, int size, int num, int outputmode)
    {
        debugOutput("createVideo! ");

        if(theoraVideoPlugin == 0)
        {
            //FIX for IOS
            //dllStartPlugin();
            return -3;
        }

        return theoraVideoPlugin->createVideoMem(buf,size,num,outputmode);
    }

    VIDEO_EXPORT int createVideoMem(unsigned char *buf, int size, int num)
    {
        int outputmode = 2;

#if defined(UNITY_WIN)
        outputmode = 5;
#endif
        return _createVideoMem(buf,size,num,outputmode);
    }


	VIDEO_EXPORT int getSetParam(int param)
	{
		if(theoraVideoPlugin == 0)
			return -3;

		return theoraVideoPlugin->getParam(param);
	}

	static void* g_TexturePointer = 0;
	static int g_TextureW = 0;
	static int g_TextureH = 0;

	static float g_Time = 0.0f;


    VIDEO_EXPORT void assignTimeFromUnity (float t)
    {
        //::printf("\ntime: %f",t);
        g_Time = t;
    }

	VIDEO_EXPORT void assignTextureFromUnity (void* texturePtr, int texW, int texH)
	{
        //::printf("\nassignTextureFromUnity: %u",(unsigned)texturePtr);

		g_TexturePointer = texturePtr;
		g_TextureW = texW;
		g_TextureH = texH;
	}

#if defined(UNITY_WIN)
	IDirect3DDevice9* g_DeviceType = 0;

	VIDEO_EXPORT int getDXCaps(int param)
	{
		if(!g_DeviceType)
			return -1;

		D3DCAPS9 caps;
		g_DeviceType->GetDeviceCaps(&caps);

		int res = -1;

		switch(param)
		{
		//<= 256 - bad
		case 0: res = (int)caps.MaxPointSize; break;

		//true - bad
		case 1: res = (caps.TextureCaps & D3DPTEXTURECAPS_POW2) != 0; break;

		// false - bad
		case 2: res = (caps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP) != 0; break;

		//<= 1 - bad
		case 3: res = caps.NumSimultaneousRTs; break;
		}


		return res;
	}
#else
    VIDEO_EXPORT int getDXCaps(int param)
    {
        return -1;
    }

#endif

	VIDEO_EXPORT void UnitySetGraphicsDevice (void* device, int deviceType, int eventType)
	{

        String n[] = {"kGfxRendererOpenGL",             // OpenGL
                        "kGfxRendererD3D9",				// Direct3D 9
                        "kGfxRendererD3D11",			// Direct3D 11
                        "kGfxRendererGCM",				// Sony PlayStation 3 GCM
                        "kGfxRendererNull",				// "null" device (used in batch mode)
                        "kGfxRendererHollywood",		// Nintendo Wii
                        "kGfxRendererXenon",			// Xbox 360
                        "kGfxRendererOpenGLES_Obsolete",
                        "kGfxRendererOpenGLES20Mobile",	// OpenGL ES 2.0
                        "kGfxRendererMolehill_Obsolete",
                        "kGfxRendererOpenGLES20Desktop_Obsolete",
                        "kGfxRendererOpenGLES30"};

         debugOutput("\nUnitySetGraphicsDevice: %u %s %d",(unsigned)device,n[deviceType].c_str(),eventType);


        if(deviceType >= kGfxRendererCount)
        {
            ::printf("\nWrong device type!");
            return;
        }



		if(eventType == 0)
		{
#if defined(UNITY_WIN)
			if (deviceType == 1)
			{
				g_DeviceType = (IDirect3DDevice9*)device;

			}

			if (deviceType != kGfxRendererD3D9)
            {
                ::printf("\nDevice type != kGfxRendererD3D9. Device type is %s",n[deviceType].c_str());
                return;
            }
#else

#if defined(UNITY_OSX) || defined(UNITY_LINUX)

            if (deviceType != kGfxRendererOpenGL)
            {
                ::printf("\nDevice type != kGfxRendererOpenGL. Device type is %s",n[deviceType].c_str());
                return;
            }

#elif UNITY_IPHONE
            if (deviceType != kGfxRendererOpenGLES20Mobile && deviceType != kGfxRendererOpenGLES30)
            {
                ::printf("\nDevice type != kGfxRendererOpenGLES20Mobile. Device type is %s",n[deviceType].c_str());
                return;
            }
#endif

#endif

			dllStartPlugin();
		}
		else if(eventType == 1)
		{
#if defined(UNITY_WIN)
			g_DeviceType = 0;
#endif

			//dllStopPlugin();
		}
	}

	VIDEO_EXPORT void UnityRenderEvent (int eventID)
	{
        //debugOutput("UnityRenderEvent %d %d %d",eventID,theoraVideoPlugin,theoraVideoPlugin->hasClip());

		if(theoraVideoPlugin == 0)
			return;

		if(!theoraVideoPlugin->hasClip())
			return;

#if defined(UNITY_WIN)
		// Update native texture from code
		if (g_TexturePointer)
		{

			IDirect3DTexture9* d3dtex = (IDirect3DTexture9*)g_TexturePointer;
			D3DSURFACE_DESC desc;
			d3dtex->GetLevelDesc (0, &desc);
			D3DLOCKED_RECT lr;
			d3dtex->LockRect (0, &lr, NULL, 0);
			//FillTextureFromCode (desc.Width, desc.Height, lr.Pitch, (unsigned char*)lr.pBits);

			theoraVideoPlugin->updateVideoData(g_Time,(unsigned char*)lr.pBits,desc.Width,desc.Height,desc.Format);

			d3dtex->UnlockRect (0);
		}

#else
        if (g_TexturePointer)
        {
            GLuint gltex = (GLuint)(size_t)(g_TexturePointer);
            glBindTexture (GL_TEXTURE_2D, gltex);
            //int texWidth = theoraVideoPlugin->getParam(0), texHeight = theoraVideoPlugin->getParam(1);
            //glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
            //glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);

            //unsigned char* data = new unsigned char[texWidth*texHeight*4];
            //FillTextureFromCode (texWidth, texHeight, texHeight*4, data);

            //g_Time += 0.02f;

            //debugOutput("\nupdate video data %d %d %f 0x%u",texWidth,texHeight, g_Time, (unsigned)g_TexturePointer);

            //memset(data, 0, texWidth*texHeight*4);

            theoraVideoPlugin->updateVideoData(g_Time,theoraVideoPlugin->getVideoData(),g_TextureW,g_TextureH,0);

            glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, g_TextureW,g_TextureH, GL_RGBA, GL_UNSIGNED_BYTE, theoraVideoPlugin->getVideoData());

             //glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, texWidth,texHeight, GL_RGBA, GL_UNSIGNED_BYTE, theoraVideoPlugin->getVideoData());


            //delete[] data;
        }

#endif
	}
}
