#include "D3D.h"


//是否限制帧数渲染w
bool mVsyncEnable = false;

//是否全屏
bool enableFullScreen = false;
int point1 = 0, point2 = 0;
static float t = 0;

void D3D::Present() 
{
	if (mVsyncEnable)
	{
		SwapChain->Present(1, 0);
	}
	else
	{
		SwapChain->Present(0, 0);
	}
}
bool D3D::CompileD3DShader(const wchar_t* filePath, const char* entry, const char* shaderModel, ID3DBlob** buffer)
{
	/*DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif*/
	DWORD shaderFlags = 0;
	ID3DBlob* errorBuffer = 0;
	HRESULT result;

	result =D3DCompileFromFile(filePath, 0, 0, entry, shaderModel,
		shaderFlags, 0, buffer, &errorBuffer);

	//hlslFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel,
	//dwShaderFlags, 0, ppBlobOut, & errorBlob

	if (FAILED(result))
	{
		if (errorBuffer != 0)
		{
			OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
			errorBuffer->Release();
		}

		return false;
	}

	if (errorBuffer != 0)
		errorBuffer->Release();

	return true;
}
bool D3D::InitDirect3D(HWND hwnd,int Width,int Height)
{
	this->SCREEN_WIDTH = Width; this->SCREEN_HEIGHT = Height;
	IDXGIFactory* DXGIfactory = NULL;
	//创建工厂
	HR(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)& DXGIfactory));
	IDXGIAdapter* adapter;
	HR(DXGIfactory->EnumAdapters(0, &adapter));
	IDXGIOutput* adaptersOutput;
	HR(adapter->EnumOutputs(0, &adaptersOutput));

	unsigned int numModes = 0;
	DXGI_MODE_DESC* displayModeList;
	HR(adaptersOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, 0));

	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
		return false;
	HR(adaptersOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList));

	unsigned int numerator = -1, denominator = -1;
	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == SCREEN_WIDTH)
		{
			if (displayModeList[i].Height == SCREEN_HEIGHT)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}
	if (numerator == -1 || denominator == -1)
		return false;
	delete[] displayModeList;
	displayModeList = 0;
	ReleaseCom(DXGIfactory);
	ReleaseCom(adapter);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(DXGI_SWAP_CHAIN_DESC));
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	if (mVsyncEnable)//限制帧数渲染
	{
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
	}
	else
	{
		sd.BufferDesc.RefreshRate.Denominator = denominator;
		sd.BufferDesc.RefreshRate.Numerator = numerator;
	}
	//关闭多重采样
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	if (enableFullScreen)
	{
		sd.Windowed = false;
	}
	else
	{
		sd.Windowed = true;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//交换链下后台缓冲区的去处
	sd.Flags = 0;//交换链下全屏模式的可选设置
	sd.BufferCount = 1;//后台缓冲区数量
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;//交换链下显卡驱动选择的显示模式
	sd.OutputWindow = hwnd;

	D3D_FEATURE_LEVEL featurelevel;
	featurelevel = D3D_FEATURE_LEVEL_11_0;
	HR(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		&featurelevel, 1, D3D11_SDK_VERSION, &sd, &SwapChain, &d3dDevice,
		NULL, &deviceContext));

	ID3D11Texture2D* backBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuffer));
	d3dDevice->CreateRenderTargetView(backBuffer, 0, &md3dRenderTarget);
	ReleaseCom(backBuffer);

	D3D11_TEXTURE2D_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthDesc.Width = SCREEN_WIDTH;
	depthDesc.Height = SCREEN_HEIGHT;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;


	HR(d3dDevice->CreateTexture2D(&depthDesc, 0, &depthStencilBuffer));
	HR(d3dDevice->CreateDepthStencilView(depthStencilBuffer, 0, &md3dDepthStencil));//标记mark重点



	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	HR(d3dDevice->CreateDepthStencilState(&depthStencilDesc, &depthStencilState));

	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	HR(d3dDevice->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView));

	deviceContext->OMSetRenderTargets(1, &md3dRenderTarget, md3dDepthStencil);

	D3D11_VIEWPORT vp;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	deviceContext->RSSetViewports(1, &vp);


	/*float FOV = (float)D3DX_PI / 4.0f;
	float screenAspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;*/

	// Create the projection matrix for 3D rendering.
	//D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, FOV, screenAspect, 0.0f, 1.0f);

	// Initialize the world matrix to the identity matrix.
	//D3DXMatrixIdentity(&m_worldMatrix);

	// Create an orthographic projection matrix for 2D rendering.
	//D3DXMatrixOrthoLH(&m_orthoMatrix, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, 1.0f);


	return true;
}
bool D3D::LoadContent()
{
	ID3DBlob* vsBuffer = 0;

	bool compileResult = CompileD3DShader(L"RSS.fx", "VS_Main", +"vs_4_0", &vsBuffer);

	if (compileResult == false)
	{
		MessageBox(0, L"Error loading vertex shader!", L"Compile Error", MB_OK);
		return false;
	}

	HRESULT d3dResult;

	d3dResult = d3dDevice->CreateVertexShader(vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(), 0, &gVertexShader);

	if (FAILED(d3dResult))
	{
		if (vsBuffer)
			vsBuffer->Release();

		return false;
	}

	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);

	d3dResult = d3dDevice->CreateInputLayout(solidColorLayout, totalLayoutElements,
		vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &inputLayout_);

	vsBuffer->Release();

	if (FAILED(d3dResult))
	{
		return false;
	}

	ID3DBlob* psBuffer = 0;

	compileResult = CompileD3DShader(L"RSS.fx", "PS_Main", "ps_4_0", &psBuffer);

	if (compileResult == false)
	{
		MessageBox(0, L"Error loading pixel shader!", L"Compile Error", MB_OK);
		return false;
	}

	d3dResult = d3dDevice->CreatePixelShader(psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(), 0, &gPixelShader);

	psBuffer->Release();

	if (FAILED(d3dResult))
	{
		return false;
	}

	VERTEX vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
	};

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VERTEX) * 8;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = vertices;

	d3dResult = d3dDevice->CreateBuffer(&vertexDesc, &resourceData, &VertexBuffer_);

	if (FAILED(d3dResult))
	{
		return false;
	}

	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	resourceData.pSysMem = indices;
	if (FAILED(d3dDevice->CreateBuffer(&bd, &resourceData, &IndexBuffer_)))
		return false;

	deviceContext->IASetIndexBuffer(IndexBuffer_, DXGI_FORMAT_R16_UINT, 0);

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	if (FAILED(d3dDevice->CreateBuffer(&bd, NULL, &ConstentBuffer_)))
		return false;

	g_world = XMMatrixIdentity();
	g_world2 = XMMatrixIdentity();
	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_view = XMMatrixLookAtLH(Eye, At, Up);
	g_projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, 0.01f, 100.0f);

	return true;
}
void D3D::Render(bool Present)
{
	float rgba[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
	deviceContext->ClearRenderTargetView(md3dRenderTarget, rgba);
	deviceContext->IASetInputLayout(inputLayout_);
	deviceContext->ClearDepthStencilView(md3dDepthStencil,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL
		, 1.0f, 0);

	g_world = XMMatrixRotationY(t);
	//t += 0.001f;
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_world);
	cb.mView = XMMatrixTranspose(g_view);
	cb.mProjection = XMMatrixTranspose(g_projection);
	deviceContext->UpdateSubresource(ConstentBuffer_, 0, NULL, &cb, 0, 0);

	unsigned int stride = sizeof(VERTEX);
	unsigned int offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &VertexBuffer_, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetShader(gVertexShader, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &ConstentBuffer_);
	deviceContext->PSSetShader(gPixelShader, 0, 0);
	deviceContext->DrawIndexed(36, 0, 0);

	XMMATRIX mSpin = XMMatrixRotationZ(-t);//自身旋转
	XMMATRIX mOrbit = XMMatrixRotationY(-t * 2.0f);//绕中心轴的旋转
	XMMATRIX mTranslate = XMMatrixTranslation(-4.0f, 0.0f, 4.0f);//离中心轴的位置
	XMMATRIX mScale = XMMatrixScaling(3.0f, 3.0f, 3.0f);//大小

	g_world2 = mScale * mSpin * mTranslate * mOrbit;


	ConstantBuffer cb2;
	cb2.mWorld = XMMatrixTranspose(g_world2);
	cb2.mView = XMMatrixTranspose(g_view);
	cb2.mProjection = XMMatrixTranspose(g_projection);
	deviceContext->UpdateSubresource(ConstentBuffer_, 0, 0, &cb2, 0, 0);

	stride = sizeof(VERTEX);
	offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &VertexBuffer_, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetShader(gVertexShader, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &ConstentBuffer_);
	deviceContext->PSSetShader(gPixelShader, 0, 0);
	deviceContext->DrawIndexed(36, 0, 0);

	XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, -20.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_view = XMMatrixLookAtLH(Eye, At, Up);

	if (Present)
	{
		if (mVsyncEnable)
		{
			SwapChain->Present(1, 0);
		}
		else
		{
			SwapChain->Present(0, 0);
		}
	}

}
bool D3D::UpdateBuffers(int x, int y, int width, int height)
{
	return true;
}
void D3D::Clean(HINSTANCE hInstance)
{
	UnregisterClassW(L"gotD3D", hInstance);
	ReleaseCom(d3dDevice);
	ReleaseCom(deviceContext);
	ReleaseCom(SwapChain);
	ReleaseCom(md3dRenderTarget);
	ReleaseCom(md3dDepthStencil);
	ReleaseCom(depthStencilState);
	ReleaseCom(depthStencilView);
	ReleaseCom(depthStencilBuffer);

	ReleaseCom(VertexBuffer_);
	ReleaseCom(gVertexShader);
	ReleaseCom(gPixelShader);
	ReleaseCom(inputLayout_);



}