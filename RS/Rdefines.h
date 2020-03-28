#pragma once
#define DIRECTINPUT_VERSION 0x800
#include<dinput.h>
#include<Windows.h>
#include<string>
#include<time.h>
#include<thread>
#include<io.h>
#include<tchar.h>
//#include<opencv/cv.h>  
//#include<opencv/highgui.h> 

#include <d3d11_1.h>
#include <d3dcommon.h>
#include <dxgi.h>
#include<d3dcompiler.h>
#include<DirectXMath.h>

#include<process.h>
//#include <d3dx11.h>
//#include <d3dx10math.h>
//#include<xnamath.h>



#include<D2D1.h>
#include<DWrite.h>
#include<wincodec.h>
#include<vector>
#include<mutex>
#include<fstream>
#include<TlHelp32.h>
using namespace std;
using namespace DirectX;
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")//使用dinput要链接dxguid.lib

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#pragma comment(lib,"d3dcompiler.lib")


#define DeviceType_Joy 1
#define DeviceType_KeyBoard 2
#define DeviceType_Mouse 3
#define DeviceType_All 0
static bool PQuit = false;
static POINT menuPoint = { -1,-1 };
static vector<WPARAM> inputStr; 
//static bool bsetList = false;
static bool bBanExe=false;
typedef int DeviceType;
typedef wchar_t RSKEYSTATE;

#define RSKeyDown 128
#define RSKey0 0
#define RSKeyUp 129


#define ReleaseCom(X){if(X)X->Release();X=0;}
#define HR(X) {if(FAILED(X)) {MessageBox(NULL,0,0,0|MB_ICONERROR); return false;}}



static ID2D1Bitmap* D2DCreateBitmap(ID2D1RenderTarget* d2dRenderTarget, IWICImagingFactory* g_image_factory, const wchar_t* FilePath)
{
	ID2D1Bitmap* g_bitmap = 0;
	if (!g_image_factory)
	{
		if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL
			, CLSCTX_INPROC_SERVER, __uuidof(g_image_factory)
			, (LPVOID*)&g_image_factory)))
			return nullptr;
	}

	IWICBitmapDecoder* bitmapdecoder = NULL;

	if (FAILED(g_image_factory->CreateDecoderFromFilename(FilePath, NULL
		, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &bitmapdecoder)))
	{
		MessageBox(0, L"CreateBitmap Failed! Check the bitmap file Or Code", L"Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	IWICBitmapFrameDecode* pframe = NULL;
	bitmapdecoder->GetFrame(0, &pframe);
	IWICFormatConverter* fmtcovter = NULL;	g_image_factory->CreateFormatConverter(&fmtcovter);

	fmtcovter->Initialize(pframe, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone
		, NULL, 0.0f, WICBitmapPaletteTypeCustom);
	d2dRenderTarget->CreateBitmapFromWicBitmap(fmtcovter, NULL, &g_bitmap);

	g_image_factory->Release();
	fmtcovter->Release();
	pframe->Release();
	bitmapdecoder->Release();
	return g_bitmap;
}

