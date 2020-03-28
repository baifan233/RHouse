#pragma once
#include"Rdefines.h"
#define buttonFlag_Normal 0
#define buttonFlag_OnClick 1
#define buttonFlag_MouseOn 2
enum RButtonState
{
	RButtonState_Normal = 0,
	RButtonState_OnClick = 1,
	RButtonState_MouseOn = 2,
};
enum RControlType
{
	RControlType_Button = 0,
	RControlType_Slip = 1,
	RControlType_Edit = 2,
};
struct ControlDescStruct
{
	BYTE type;
	BYTE page;
	D2D1_COLOR_F NormalColor;
	D2D1_COLOR_F MouseOnColor;
	D2D1_COLOR_F OnClickColor;
	ID2D1Bitmap* NormalBitmap;
	ID2D1Bitmap* MouseOnBitmap;
	ID2D1Bitmap* OnClickBitmap;
	int transformTime;

};
struct ButtonStruct
{
	float bitmapOpacity = 1.0f;
	ID2D1Bitmap* bitmap;
	D2D1_COLOR_F color = { 0 };
	D2D1_COLOR_F MouseOncolor = { 0 };
	D2D1_COLOR_F OnClickcolor = { 0 };
	D2D1_COLOR_F Textcolor = { 0 };
	D2D1_RECT_F rect = { 0 };
	BYTE id = 0;
	void(*oc)(void) = 0;
	const wchar_t* text = NULL;
	RButtonState RbuttonState;
};
struct SlipStruct
{
	D2D1_RECT_F rect = { 0 };
	D2D1_RECT_F rect2 = { 0 };//拖动小滑块的rect
	D2D1_COLOR_F color = { 0 };
	int value = 0;
	const wchar_t* text = 0;
	//bool vertical;//竖着？不然就横着
	BYTE id = 0;
	int Width = 0;//小滑块宽度
	bool MouseDown = false;
};
struct EditStruct
{
	float bitmapOpacity = 1.0f;
	ID2D1Bitmap* bitmap;
	BYTE id = 0;
	D2D1_COLOR_F color;
	D2D1_RECT_F rect;
	int cursorPos;
	int selectNum;
	wstring text;
	bool focus;
	bool selectMode;
	IDWriteTextLayout* textLayout;
};
struct Page
{
	float bitmapOpacity = 1.0f;
	ID2D1Bitmap* bitmap;
	vector<ButtonStruct*> buttonsAddr;
	vector<SlipStruct*> slipsAddr;
	vector<EditStruct*> editsAddr;
	D2D1_COLOR_F backGroundColor{ 1.0f,1.0f,1.0f,1.0f };
};
struct RID
{
	BYTE id;
	RControlType conTrol_type;
	BYTE pointId;
};
struct UIInputStruct
{
	DIMOUSESTATE* inp_mouseState = 0;
	POINT* pt = 0;
	vector<WPARAM>* wParam = 0;
	RSKEYSTATE* rsKeyState = 0;
};
static ID2D1Factory* d2dFactory = NULL;
static IWICImagingFactory* imageFactory = NULL;
static IDWriteFactory* dWriteFactory = NULL;
static IDWriteTextFormat* editTextFormat = NULL;
class UI
{
public:
	int FrameRate = 60;
	UI();
	bool Init(IDXGISwapChain* SwapChain);
	bool Init(HWND hwnd, int screenWidth, int screenHeight);

	HWND hwnd = 0;
	int screenWidth=0;
	int screenHeight=0;
	//private:
	float dpi = 0;
	ID2D1RenderTarget* ren = NULL;
	ID2D1HwndRenderTarget* hwndren = NULL;
	IDWriteTextFormat* m_pText = NULL;
	//Button
public:
	bool CreateRenderThread();
	bool bRenderThread = false;
	HWND RCreateWindow(int width, int height, DWORD winStyle
		, HINSTANCE hInstance, WNDPROC mwWndProc, const wchar_t* ClassName, const wchar_t* icoPath);

	bool SetButtonText(const wchar_t* text, BYTE id);
	BYTE CreateButton(D2D1_COLOR_F color,
		D2D1_COLOR_F Textcolor, const wchar_t* text, D2D1_RECT_F rect, void (*oc)(void));
	bool LoadBitmapForControl(BYTE id, const wchar_t* FilePath);

	BYTE CreateSlip(D2D1_COLOR_F color, D2D1_RECT_F rect, const wchar_t* text/*, bool vertical*/);
	int* GetSlipValueAddress(BYTE id);

	UIInputStruct UIS = { 0 };
	void SetUIS(UIInputStruct uis) { UIS = uis; }
	//void Draw(DIMOUSESTATE inp_mouseState, POINT pt, RSKEYSTATE keyState);
	//画当前页  当前页可由按钮控件更改

	bool SetPageForControl(BYTE id, BYTE pageId);
	void SetCurrentPage(BYTE pageId) { this->pageCurrent = pageId; };
	void SetPageBackGounrdColor(D2D1_COLOR_F color, BYTE pageId)
	{
		if (pageId < 20)
		{
			pages[pageId].backGroundColor = color;
		}
	};
	void LoadBitmapForPageBackgournd(BYTE Pageid, const wchar_t* FilePath);
	~UI();
	//private:
	int frame = 0;

	BYTE pageCurrent = 0;//当前页
	ID2D1SolidColorBrush* p_bControlsBrush = nullptr;
	IDWriteTextLayout* textLayout = nullptr;
	vector<ButtonStruct> buttons;
	vector<SlipStruct> slips;
	vector<Page> pages;

	//Edit
	vector<WPARAM> editInputChar;
	vector<EditStruct> edits;
	wstring GetEditText(BYTE id)
	{
		for (size_t i = 0; i < RIds.size(); i++)
		{
			if (RIds[i].id == id)
			{
				return  edits[RIds[i].pointId].text;
			}
		}
	}
	void SetEditText(BYTE id, wstring text);
	//ID manager
	vector<RID> RIds;
public:
	bool LoadControlFromFile(const wchar_t* filePath);
	void DeleteControl(BYTE id);
	BYTE CreateEdit(D2D1_RECT_F rect, D2D1_COLOR_F color);
};

