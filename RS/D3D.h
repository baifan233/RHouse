#pragma once
#include"Rdefines.h"
struct VERTEX
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};
struct VERTEXPos
{
	XMFLOAT3 position;
};
struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};
class D3D
{
public:	
	bool InitDirect3D(HWND hwnd, int Width, int Height);
	bool LoadContent();
	void Render(bool Present);
	void Present();
	void Clean(HINSTANCE hInstance);
	bool UpdateBuffers(int x,int y,int width,int height);
	IDXGISwapChain* GetSwapChain() { return SwapChain; };
private:
	unsigned int SCREEN_WIDTH = 0, SCREEN_HEIGHT =0;
	bool CompileD3DShader(const wchar_t* filePath, const char* entry, const char* shaderModel, ID3DBlob** buffer);
	ID3D11InputLayout* inputLayout_ = NULL;
	ID3D11Device* d3dDevice = NULL;
	ID3D11DeviceContext* deviceContext = NULL;
	IDXGISwapChain* SwapChain = NULL;
	ID3D11RenderTargetView* md3dRenderTarget = NULL;
	ID3D11DepthStencilView* md3dDepthStencil = NULL;
	ID3D11Texture2D* depthStencilBuffer = NULL;

	ID3D11DepthStencilState* depthStencilState = NULL;
	ID3D11DepthStencilView* depthStencilView = NULL;
	ID3D11VertexShader* gVertexShader = NULL;
	ID3D11PixelShader* gPixelShader = NULL;

	ID3D11Buffer* VertexBuffer_ = NULL;
	ID3D11Buffer* IndexBuffer_ = NULL;
	ID3D11Buffer* ConstentBuffer_=NULL;

	XMMATRIX g_world;
	XMMATRIX g_world2;
	XMMATRIX g_view;
	XMMATRIX g_projection;
};

