#include "UI.h"
UI::UI()
{
	//��ʼ��ʱ�������20���ؼ�ҳ	
	Page p = { 0 };
	p.bitmapOpacity = 1.0f;
	for (int i = 0; i < 20; i++)
	{
		this->pages.push_back(p);
	}
}
float GetDPI(HWND hwnd)
{
	int zoom = GetDpiForWindow(hwnd);
	float dpi = 0;
	switch (zoom) {
	case 96:
		dpi = 1;
		break;
	case 120:
		dpi = 1.25;
		break;
	case 144:
		dpi = 1.5;
		break;
	case 192:
		dpi = 2;
		break;
	default:
		dpi = -1;
		break;
	}
	return dpi;
}

bool UI::Init(IDXGISwapChain* SwapChain)
{
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory)))
		return false;

	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(dWriteFactory),
		reinterpret_cast<IUnknown**>(&dWriteFactory))))

		return false;
	if (FAILED(dWriteFactory->CreateTextFormat(L"΢���ź�", 0, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_EXTRA_CONDENSED, 20, L"", &m_pText)))
		return false;
	m_pText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	if (!ren && !hwndren)//�����û��ʼ��
	{
		IDXGISurface* surface = 0;
		SwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(&surface));
		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

		if (FAILED(d2dFactory->CreateDxgiSurfaceRenderTarget(surface, props, &ren)))
			return false;


		if (FAILED(ren->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &p_bControlsBrush)))
			return false;

		return true;
	}

	return true;
}

bool UI::Init(HWND hwnd, int screenWidth, int screenHeight)
{
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory)))
		return false;

	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(dWriteFactory),
		reinterpret_cast<IUnknown**>(&dWriteFactory))))

		return false;
	if (FAILED(dWriteFactory->CreateTextFormat(L"΢���ź�", 0, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_EXTRA_CONDENSED, 20, L"", &m_pText)))
		return false;

	m_pText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	if (FAILED(dWriteFactory->CreateTextFormat(L"΢���ź�", 0, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL
		, DWRITE_FONT_STRETCH_EXTRA_CONDENSED, 20, L"", &editTextFormat)))
		return false;
	editTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	editTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	editTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	dpi = GetDPI(hwnd);;
	this->hwnd = hwnd;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	if (FAILED(dWriteFactory->CreateTextLayout(L"asdsada", lstrlenW(L"asdsada"),
		editTextFormat, 1.0f, 1.0f, &textLayout)))
		return false;

	/*textLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	textLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	textLayout->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);*/

	if (!ren && !hwndren)//�����û��ʼ��   (��ֹ�ظ���ʼ��)
	{
		if (hwnd)//������ھ����Ϊ�վʹ������ھ����
		{
			if (FAILED(d2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(screenWidth, screenHeight)), &hwndren)))
				return false;

			if (FAILED(hwndren->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &p_bControlsBrush)))
				return false;

			return true;
		}
		else
			return false;
	}
	else
		return true;
}

bool UI::SetButtonText(const wchar_t* text, BYTE id)
{
	for (size_t i = 0; i <RIds.size(); i++)
	{
		if (RIds[i].id == id)
		{

			buttons[RIds[i].pointId].text = text;

			return true;
		}
	}
	return false;
}

//true���� false��ǳ CNum��ɫ�̶�
D2D1_COLOR_F TurnDarkOrShallow(D2D1_COLOR_F color, bool TurnDark, float Cnum)
{
	float Num = 0;
	if (TurnDark)
		Num = -Cnum;
	else
		Num = Cnum;
	Num /= 255;
	color.r += Num;
	if (color.r >= 1.0f)color.r = 1.0f;
	else if (color.r <= 0)color.r = 0;

	color.g += Num;
	if (color.g >= 1.0f)color.g = 1.0f;
	else if (color.g <= 0)color.g = 0;

	color.b += Num;
	if (color.b >= 1.0f)color.b = 1.0f;
	else if (color.b <= 0)color.b = 0;

	return color;
}

bool PtInControl(D2D1_RECT_F rect, float dpi, POINT pt)
{
	RECT rect2 = { 0 };
	rect2.left = (LONG)(rect.left * dpi);
	rect2.right = (LONG)(rect.right * dpi);
	rect2.top = (LONG)(rect.top * dpi);
	rect2.bottom = (LONG)(rect.bottom * dpi);
	bool pirReuslt = PtInRect(&rect2, pt);
	if (!pirReuslt)return false;//������ھ�����
	else return true;
}

//void UI::Draw(DIMOUSESTATE inp_mouseState, POINT pt, RSKEYSTATE keyState)
//{
//
//	if (ren)
//	{
//
//		ren->BeginDraw();
//		ren->Clear(D2D1::ColorF(D2D1::ColorF::White));
//		bool result = false;
//		//��ť����
//		for (size_t i = 0; i < this->buttons.size(); i++)
//		{
//			if (pt.x == -1 || pt.y == -1)
//				buttons[i]. = false;
//			result = PtInControl(buttons[i].rect, dpi, pt);
//			if (!result)
//			{
//				p_bControlsBrush->SetColor(buttons[i].color);//�л���ˢ��ɫ����ť��ɫ
//				ren->FillRectangle(buttons[i].rect, p_bControlsBrush);//������		
//				p_bControlsBrush->SetColor(buttons[i].Textcolor);//
//				ren->DrawText(buttons[i].text, lstrlenW(buttons[i].text), m_pText, buttons[i].rect, p_bControlsBrush);
//				buttons[i].buttonDown = false;
//			}
//			else
//			{
//				if (inp_mouseState.rgbButtons[0] != RSKeyDown && !buttons[i].buttonDown)
//				{
//					p_bControlsBrush->SetColor(buttons[i].MouseOncolor);
//					ren->FillRectangle(buttons[i].rect, p_bControlsBrush);
//					p_bControlsBrush->SetColor(buttons[i].Textcolor);
//					ren->DrawText(buttons[i].text, lstrlenW(buttons[i].text), m_pText, buttons[i].rect, p_bControlsBrush);
//
//				}
//				else if (inp_mouseState.rgbButtons[0] == RSKeyDown)
//				{
//
//					p_bControlsBrush->SetColor(buttons[i].OnClickcolor);
//					ren->FillRectangle(buttons[i].rect, p_bControlsBrush);
//					p_bControlsBrush->SetColor(buttons[i].Textcolor);
//					ren->DrawText(buttons[i].text, lstrlenW(buttons[i].text), m_pText, buttons[i].rect, p_bControlsBrush);
//					buttons[i].buttonDown = true;
//
//				}
//				else if (buttons[i].buttonDown && inp_mouseState.rgbButtons[0] == 0) {
//					buttons[i].oc();
//					buttons[i].buttonDown = false;
//				}
//
//
//			}
//		}
//
//		D2D1_POINT_2F point0 = { 0 };
//		D2D1_POINT_2F point1 = { 0 };
//		int slipWidth = 0;
//		int slipHeight = 0;
//		float value = 0;
//		int sliplong = 0;
//		int stemp = 0;
//		for (size_t i = 0; i < this->slips.size(); i++)
//		{
//			if (pt.x == -1 || pt.y == -1)
//				slips[i].MouseDown = false;
//			slipHeight = (int)(slips[i].rect.bottom - slips[i].rect.top);
//			slipWidth = slips[i].Width;
//			point0.x = slips[i].rect.left;
//			point0.y = slips[i].rect.top + slipHeight / 2;
//
//			point1.x = slips[i].rect.right;
//			point1.y = point0.y;
//
//			p_bControlsBrush->SetColor(slips[i].color);
//			ren->DrawLine(point0, point1, p_bControlsBrush);
//			result = PtInControl(slips[i].rect, dpi, pt);
//
//			if (!result && !slips[i].MouseDown)
//			{
//				//�����겻��slip�� ����ɫ
//			}
//			else if (!result && slips[i].MouseDown)
//			{
//				if (inp_mouseState.rgbButtons[0] == RSKey0)
//					slips[i].MouseDown = false;
//				if (pt.x < slips[i].rect.left)
//				{
//					slips[i].value = 0;
//					slips[i].rect2.left = slips[i].rect.left;
//					slips[i].rect2.right = slips[i].Width + slips[i].rect.left;
//				}
//				else if (pt.x > slips[i].rect.right)
//				{
//					slips[i].value = 100;
//					slips[i].rect2.left = slips[i].rect.right - slips[i].Width;
//					slips[i].rect2.right = slips[i].rect.right;
//				}
//			}
//			else
//			{
//				if (inp_mouseState.rgbButtons[0] != RSKeyDown)
//				{
//					p_bControlsBrush->SetColor(TurnDarkOrShallow(slips[i].color, false, 40));
//					if (inp_mouseState.rgbButtons[0] == RSKey0)
//						slips[i].MouseDown = false;
//				}
//				else
//				{
//					slips[i].MouseDown = true;
//
//					p_bControlsBrush->SetColor(TurnDarkOrShallow(slips[i].color, true, 40));
//
//					slips[i].rect2.left = pt.x / dpi;
//					slips[i].rect2.left = slips[i].rect2.left - slipWidth / 2;
//					slips[i].rect2.right = slips[i].rect2.left + slipWidth;
//
//					sliplong = (int)(slips[i].rect.right - slips[i].rect.left);
//					stemp = (int)(slips[i].rect.right - slips[i].rect2.left);
//					stemp = sliplong - stemp;
//					value = (float)stemp / (float)sliplong;
//					value *= 100;
//					value += 5;
//					slips[i].value = (int)value;
//
//				}
//			}
//			ren->FillRectangle(slips[i].rect2, p_bControlsBrush);
//			//swprintf_s((wchar_t*)slips[i].text,20, L"%d",value);
//			//ren->DrawTextW(slips[i].text,sizeof(slips[i].text),m_pText,slips[i].rect,p_bControlsBrush);
//		}
//		ren->EndDraw();
//	}
//	else if (hwndren)
//	{
//
//		hwndren->BeginDraw();
//		hwndren->Clear(D2D1::ColorF(D2D1::ColorF::Black));
//		bool result = false;
//		//��ť����
//		for (size_t i = 0; i < this->buttons.size(); i++)
//		{
//			if (pt.x == -1 || pt.y == -1)
//				buttons[i].buttonDown = false;
//			result = PtInControl(buttons[i].rect, dpi, pt);
//			if (!result)
//			{
//				p_bControlsBrush->SetColor(buttons[i].color);//�л���ˢ��ɫ����ť��ɫ
//				hwndren->FillRectangle(buttons[i].rect, p_bControlsBrush);//������		
//				p_bControlsBrush->SetColor(buttons[i].Textcolor);//
//				hwndren->DrawText(buttons[i].text, lstrlenW(buttons[i].text), m_pText, buttons[i].rect, p_bControlsBrush);
//				buttons[i].buttonDown = false;
//			}
//			else
//			{
//				if (inp_mouseState.rgbButtons[0] != RSKeyDown && !buttons[i].buttonDown)
//				{
//					p_bControlsBrush->SetColor(buttons[i].MouseOncolor);
//					hwndren->FillRectangle(buttons[i].rect, p_bControlsBrush);
//					p_bControlsBrush->SetColor(buttons[i].Textcolor);
//					hwndren->DrawText(buttons[i].text, lstrlenW(buttons[i].text), m_pText, buttons[i].rect, p_bControlsBrush);
//
//				}
//				else if (inp_mouseState.rgbButtons[0] == RSKeyDown)
//				{
//
//					p_bControlsBrush->SetColor(buttons[i].OnClickcolor);
//					hwndren->FillRectangle(buttons[i].rect, p_bControlsBrush);
//					p_bControlsBrush->SetColor(buttons[i].Textcolor);
//					hwndren->DrawText(buttons[i].text, lstrlenW(buttons[i].text), m_pText, buttons[i].rect, p_bControlsBrush);
//					buttons[i].buttonDown = true;
//
//				}
//				else if (buttons[i].buttonDown && inp_mouseState.rgbButtons[0] == 0) {
//					buttons[i].oc();
//					buttons[i].buttonDown = false;
//				}
//
//
//			}
//		}
//
//		D2D1_POINT_2F point0 = { 0 };
//		D2D1_POINT_2F point1 = { 0 };
//		int slipWidth = 0;
//		int slipHeight = 0;
//		float value = 0;
//		int sliplong = 0;
//		int stemp = 0;
//		for (size_t i = 0; i < this->slips.size(); i++)
//		{
//			if (pt.x == -1 || pt.y == -1)
//				slips[i].MouseDown = false;
//			slipHeight = (int)(slips[i].rect.bottom - slips[i].rect.top);
//			slipWidth = slips[i].Width;
//			point0.x = slips[i].rect.left;
//			point0.y = slips[i].rect.top + slipHeight / 2;
//
//			point1.x = slips[i].rect.right;
//			point1.y = point0.y;
//
//			p_bControlsBrush->SetColor(slips[i].color);
//			hwndren->DrawLine(point0, point1, p_bControlsBrush);
//			result = PtInControl(slips[i].rect, dpi, pt);
//
//			if (!result && !slips[i].MouseDown)
//			{
//				//�����겻��slip�� ����Ĭ��ɫ
//				p_bControlsBrush->SetColor({ 0.61f,0.61f,0.61f,1.0f });
//			}
//			else if (!result && slips[i].MouseDown)
//			{
//				if (inp_mouseState.rgbButtons[0] == RSKey0)
//					slips[i].MouseDown = false;
//				if (pt.x < slips[i].rect.left)
//				{
//					slips[i].value = 0;
//					slips[i].rect2.left = slips[i].rect.left;
//					slips[i].rect2.right = slips[i].Width + slips[i].rect.left;
//				}
//				else if (pt.x > slips[i].rect.right)
//				{
//					slips[i].value = 100;
//					slips[i].rect2.left = slips[i].rect.right - slips[i].Width;
//					slips[i].rect2.right = slips[i].rect.right;
//				}
//			}
//			else
//			{
//				if (inp_mouseState.rgbButtons[0] != RSKeyDown)
//				{
//					p_bControlsBrush->SetColor(TurnDarkOrShallow(slips[i].color, false, 40));
//					if (inp_mouseState.rgbButtons[0] == RSKey0)
//						slips[i].MouseDown = false;
//				}
//				else
//				{
//					slips[i].MouseDown = true;
//
//					p_bControlsBrush->SetColor(TurnDarkOrShallow(slips[i].color, true, 40));
//
//					slips[i].rect2.left = pt.x / dpi;
//					slips[i].rect2.left = slips[i].rect2.left - slipWidth / 2;
//					slips[i].rect2.right = slips[i].rect2.left + slipWidth;
//
//					sliplong = (int)(slips[i].rect.right - slips[i].rect.left);
//					stemp = (int)(slips[i].rect.right - slips[i].rect2.left);
//					stemp = sliplong - stemp;
//					value = (float)stemp / (float)sliplong;
//					value *= 100;
//					value += 5;
//					slips[i].value = (int)value;
//
//				}
//			}
//			hwndren->FillRectangle(slips[i].rect2, p_bControlsBrush);
//			//swprintf_s((wchar_t*)slips[i].text,20, L"%d",value);
//			//hwndren->DrawTextW(slips[i].text,sizeof(slips[i].text),m_pText,slips[i].rect,p_bControlsBrush);
//		}
//		this->EditDraw(0, pt, inp_mouseState, keyState);
//		hwndren->EndDraw();
//	}
//
//}



mutex layoutLock;
void _cdecl DrawPage(void* param)
{
	UI* ui = (UI*)param;
	ULONGLONG start = 0;
	ULONGLONG end = 0;
	ULONGLONG temp = 0;
	int PerFrameTime = 1000 / ui->FrameRate;
	float opacityTemp = 0.0f;
	while (ui->bRenderThread)
	{
		start = GetTickCount64();
		if (ui->pageCurrent < 20)
		{
			/*
			if (ren)
			{

				ren->BeginDraw();
				ren->Clear(pages[pageCurrent].backGroundColor);
				bool result = false;
				//��ť����
				for (size_t i = 0; i < pages[pageCurrent].buttonsAddr.size(); ++i)
				{
					if (pt.x == -1 || pt.y == -1)
					{
						pages[pageCurrent].buttonsAddr[i]->RbuttonState = RButtonState_Normal;
						++i;
						continue;
					}

					result = PtInControl(pages[pageCurrent].buttonsAddr[i]->rect, dpi, pt);

					if (!result)
					{
						pages[pageCurrent].buttonsAddr[i]->RbuttonState = RButtonState_Normal;
					}
					else
					{
						if (inp_mouseState.rgbButtons[0] != RSKeyDown)
						{
							pages[pageCurrent].buttonsAddr[i]->RbuttonState = RButtonState_OnClick;
						}
						else if (inp_mouseState.rgbButtons[0] == RSKeyDown)
						{

							p_bControlsBrush->SetColor(pages[pageCurrent].buttonsAddr[i]->OnClickcolor);
							ren->FillRectangle(pages[pageCurrent].buttonsAddr[i]->rect, p_bControlsBrush);
							p_bControlsBrush->SetColor(pages[pageCurrent].buttonsAddr[i]->Textcolor);
							ren->DrawText(pages[pageCurrent].buttonsAddr[i]->text, lstrlenW(pages[pageCurrent].buttonsAddr[i]->text), m_pText, pages[pageCurrent].buttonsAddr[i]->rect, p_bControlsBrush);
							pages[pageCurrent].buttonsAddr[i]->buttonDown = true;

						}
						else if (pages[pageCurrent].buttonsAddr[i]->buttonDown && inp_mouseState.rgbButtons[0] == 0) {
							pages[pageCurrent].buttonsAddr[i]->oc();
							pages[pageCurrent].buttonsAddr[i]->buttonDown = false;
						}


					}

					//normal
					p_bControlsBrush->SetColor(pages[pageCurrent].buttonsAddr[i]->color);//�л���ˢ��ɫ����ť��ɫ
					ren->FillRectangle(pages[pageCurrent].buttonsAddr[i]->rect, p_bControlsBrush);//������
					p_bControlsBrush->SetColor(pages[pageCurrent].buttonsAddr[i]->Textcolor);//
					ren->DrawText(pages[pageCurrent].buttonsAddr[i]->text, lstrlenW(pages[pageCurrent].buttonsAddr[i]->text), m_pText, pages[pageCurrent].buttonsAddr[i]->rect, p_bControlsBrush);
					//
					p_bControlsBrush->SetColor(pages[pageCurrent].buttonsAddr[i]->MouseOncolor);
					ren->FillRectangle(pages[pageCurrent].buttonsAddr[i]->rect, p_bControlsBrush);
					p_bControlsBrush->SetColor(pages[pageCurrent].buttonsAddr[i]->Textcolor);
					ren->DrawText(pages[pageCurrent].buttonsAddr[i]->text, lstrlenW(pages[pageCurrent].buttonsAddr[i]->text), m_pText, pages[pageCurrent].buttonsAddr[i]->rect, p_bControlsBrush);

				}

				D2D1_POINT_2F point0 = { 0 };
				D2D1_POINT_2F point1 = { 0 };
				int slipWidth = 0;
				int slipHeight = 0;
				float value = 0;
				int sliplong = 0;
				int stemp = 0;
				for (size_t i = 0; i < pages[pageCurrent].slipsAddr.size(); i++)
				{
					if (pt.x == -1 || pt.y == -1)
						pages[pageCurrent].slipsAddr[i]->MouseDown = false;
					slipHeight = (int)(pages[pageCurrent].slipsAddr[i]->rect.bottom - pages[pageCurrent].slipsAddr[i]->rect.top);
					slipWidth = pages[pageCurrent].slipsAddr[i]->Width;
					point0.x = pages[pageCurrent].slipsAddr[i]->rect.left;
					point0.y = pages[pageCurrent].slipsAddr[i]->rect.top + slipHeight / 2;

					point1.x = pages[pageCurrent].slipsAddr[i]->rect.right;
					point1.y = point0.y;

					p_bControlsBrush->SetColor(pages[pageCurrent].slipsAddr[i]->color);
					ren->DrawLine(point0, point1, p_bControlsBrush);
					result = PtInControl(pages[pageCurrent].slipsAddr[i]->rect, dpi, pt);

					if (!result && !pages[pageCurrent].slipsAddr[i]->MouseDown)
					{
						//�����겻��slip�� ����ɫ
					}
					else if (!result && pages[pageCurrent].slipsAddr[i]->MouseDown)
					{
						if (inp_mouseState.rgbButtons[0] == RSKey0)
							pages[pageCurrent].slipsAddr[i]->MouseDown = false;
						if (pt.x < pages[pageCurrent].slipsAddr[i]->rect.left)
						{
							pages[pageCurrent].slipsAddr[i]->value = 0;
							pages[pageCurrent].slipsAddr[i]->rect2.left = pages[pageCurrent].slipsAddr[i]->rect.left;
							pages[pageCurrent].slipsAddr[i]->rect2.right = pages[pageCurrent].slipsAddr[i]->Width + pages[pageCurrent].slipsAddr[i]->rect.left;
						}
						else if (pt.x > pages[pageCurrent].slipsAddr[i]->rect.right)
						{
							pages[pageCurrent].slipsAddr[i]->value = 100;
							pages[pageCurrent].slipsAddr[i]->rect2.left = pages[pageCurrent].slipsAddr[i]->rect.right - pages[pageCurrent].slipsAddr[i]->Width;
							pages[pageCurrent].slipsAddr[i]->rect2.right = pages[pageCurrent].slipsAddr[i]->rect.right;
						}
					}
					else
					{
						if (inp_mouseState.rgbButtons[0] != RSKeyDown)
						{
							p_bControlsBrush->SetColor(TurnDarkOrShallow(pages[pageCurrent].slipsAddr[i]->color, false, 40));
							if (inp_mouseState.rgbButtons[0] == RSKey0)
								pages[pageCurrent].slipsAddr[i]->MouseDown = false;
						}
						else
						{
							pages[pageCurrent].slipsAddr[i]->MouseDown = true;

							p_bControlsBrush->SetColor(TurnDarkOrShallow(pages[pageCurrent].slipsAddr[i]->color, true, 40));

							pages[pageCurrent].slipsAddr[i]->rect2.left = pt.x / dpi;
							pages[pageCurrent].slipsAddr[i]->rect2.left = pages[pageCurrent].slipsAddr[i]->rect2.left - slipWidth / 2;
							pages[pageCurrent].slipsAddr[i]->rect2.right = pages[pageCurrent].slipsAddr[i]->rect2.left + slipWidth;

							sliplong = (int)(pages[pageCurrent].slipsAddr[i]->rect.right - pages[pageCurrent].slipsAddr[i]->rect.left);
							stemp = (int)(pages[pageCurrent].slipsAddr[i]->rect.right - pages[pageCurrent].slipsAddr[i]->rect2.left);
							stemp = sliplong - stemp;
							value = (float)stemp / (float)sliplong;
							value *= 100;
							value += 5;
							pages[pageCurrent].slipsAddr[i]->value = (int)value;

						}
					}
					ren->FillRectangle(pages[pageCurrent].slipsAddr[i]->rect2, p_bControlsBrush);
					//swprintf_s((wchar_t*)pages[pageCurrent].slipsAddr[i]->text,20, L"%d",value);
					//ren->DrawTextW(pages[pageCurrent].slipsAddr[i]->text,sizeof(pages[pageCurrent].slipsAddr[i]->text),m_pText,pages[pageCurrent].slipsAddr[i]->rect,p_bControlsBrush);
				}
				ren->EndDraw();
			} */
			if (ui->hwndren)
			{
				ui->hwndren->BeginDraw();
				ui->hwndren->Clear(ui->pages[ui->pageCurrent].backGroundColor);

				if (ui->pages[ui->pageCurrent].bitmap)
				{
					ui->hwndren->DrawBitmap(ui->pages[ui->pageCurrent].bitmap, { 0,0,(float)ui->screenWidth,(float)ui->screenHeight }, ui->pages[ui->pageCurrent].bitmapOpacity);
				}
				bool result = false;
				int widthtemp = 0;

				//D2D1_COLOR_F colorTemp = { 0 };

				//��ť����

				for (size_t i = 0; i < ui->pages[ui->pageCurrent].buttonsAddr.size(); i++)
				{
					/*if (inp_mouseState.rgbButtons[1] == RSKeyDown)
					{
						if (true)
						{

							widthtemp = pages[pageCurrent].buttonsAddr[i]->rect.right - pages[pageCurrent].buttonsAddr[i]->rect.left;
							pages[pageCurrent].buttonsAddr[i]->rect.left = pt.x;
							pages[pageCurrent].buttonsAddr[i]->rect.right = pt.x + widthtemp;
							widthtemp = pages[pageCurrent].buttonsAddr[i]->rect.bottom - pages[pageCurrent].buttonsAddr[i]->rect.top;
							pages[pageCurrent].buttonsAddr[i]->rect.top = pt.y;
							pages[pageCurrent].buttonsAddr[i]->rect.bottom = pt.y + widthtemp;

						}
					}*/
					if (ui->pages[ui->pageCurrent].buttonsAddr[i] != nullptr)
					{

						if (ui->UIS.pt->x == -1 || ui->UIS.pt->y == -1)//�����겻�ڴ����� ���ύ ��겻�ھ����ڵ�result�Ҳ��ټ����Ƿ��ھ�����
						{
							result = false;
						}
						else  //�������ڴ����� �ж�ָ���Ƿ��ڿؼ�������
						{
							result = PtInControl(ui->pages[ui->pageCurrent].buttonsAddr[i]->rect, ui->dpi, *ui->UIS.pt);
						}
						if (!result)
						{
							ui->pages[ui->pageCurrent].buttonsAddr[i]->RbuttonState = RButtonState_Normal;
						}
						else
						{
							if (ui->UIS.inp_mouseState->rgbButtons[0] != RSKeyDown && ui->UIS.inp_mouseState->rgbButtons[0] != RSKeyUp)//�������ھ����ڵ���û����
							{
								ui->pages[ui->pageCurrent].buttonsAddr[i]->RbuttonState = RButtonState_MouseOn;
							}
							else
							{
								ui->pages[ui->pageCurrent].buttonsAddr[i]->RbuttonState = RButtonState_OnClick;
							}

						}

						if (ui->pages[ui->pageCurrent].buttonsAddr[i]->RbuttonState == RButtonState_Normal)
						{
							ui->p_bControlsBrush->SetColor(ui->pages[ui->pageCurrent].buttonsAddr[i]->color);//�л���ˢ��ɫ����ť��ɫ
							ui->hwndren->FillRectangle(ui->pages[ui->pageCurrent].buttonsAddr[i]->rect, ui->p_bControlsBrush);//������		
							ui->p_bControlsBrush->SetColor(ui->pages[ui->pageCurrent].buttonsAddr[i]->Textcolor);//
							ui->hwndren->DrawText(ui->pages[ui->pageCurrent].buttonsAddr[i]->text, lstrlenW(ui->pages[ui->pageCurrent].buttonsAddr[i]->text), ui->m_pText, ui->pages[ui->pageCurrent].buttonsAddr[i]->rect, ui->p_bControlsBrush);
							if (ui->pages[ui->pageCurrent].buttonsAddr[i]->bitmap)//�����ͼƬ���ͼ
							{
								opacityTemp < 0.0f ? opacityTemp = 0.0f : opacityTemp -= 0.08;
								ui->pages[ui->pageCurrent].buttonsAddr[i]->bitmapOpacity = opacityTemp;
								ui->hwndren->DrawBitmap(ui->pages[ui->pageCurrent].buttonsAddr[i]->bitmap, ui->pages[ui->pageCurrent].buttonsAddr[i]->rect, ui->pages[ui->pageCurrent].buttonsAddr[i]->bitmapOpacity);
							}
						}
						else if (ui->pages[ui->pageCurrent].buttonsAddr[i]->RbuttonState == RButtonState_MouseOn)
						{
							ui->p_bControlsBrush->SetColor(ui->pages[ui->pageCurrent].buttonsAddr[i]->MouseOncolor);
							ui->hwndren->FillRectangle(ui->pages[ui->pageCurrent].buttonsAddr[i]->rect, ui->p_bControlsBrush);//��ť��ɫ
							if (ui->pages[ui->pageCurrent].buttonsAddr[i]->bitmap)//�����ͼƬ���ͼ
							{
								opacityTemp >= 1.0f ? opacityTemp = 1.0f : opacityTemp += 0.08;
								ui->hwndren->DrawBitmap(ui->pages[ui->pageCurrent].buttonsAddr[i]->bitmap, ui->pages[ui->pageCurrent].buttonsAddr[i]->rect, opacityTemp);
							}
							ui->p_bControlsBrush->SetColor(ui->pages[ui->pageCurrent].buttonsAddr[i]->Textcolor);//��������
							ui->hwndren->DrawText(ui->pages[ui->pageCurrent].buttonsAddr[i]->text, lstrlenW(ui->pages[ui->pageCurrent].buttonsAddr[i]->text), ui->m_pText, ui->pages[ui->pageCurrent].buttonsAddr[i]->rect, ui->p_bControlsBrush);

						}
						else if (ui->pages[ui->pageCurrent].buttonsAddr[i]->RbuttonState == RButtonState_OnClick)
						{
							if (ui->UIS.inp_mouseState->rgbButtons[0] != RSKeyUp)
							{
								ui->p_bControlsBrush->SetColor(ui->pages[ui->pageCurrent].buttonsAddr[i]->OnClickcolor);
								ui->hwndren->FillRectangle(ui->pages[ui->pageCurrent].buttonsAddr[i]->rect, ui->p_bControlsBrush);
								if (ui->pages[ui->pageCurrent].buttonsAddr[i]->bitmap)//�����ͼƬ���ͼ
								{
									ui->hwndren->DrawBitmap(ui->pages[ui->pageCurrent].buttonsAddr[i]->bitmap, ui->pages[ui->pageCurrent].buttonsAddr[i]->rect, ui->pages[ui->pageCurrent].buttonsAddr[i]->bitmapOpacity);
								}
								ui->p_bControlsBrush->SetColor(ui->pages[ui->pageCurrent].buttonsAddr[i]->Textcolor);
								ui->hwndren->DrawText(ui->pages[ui->pageCurrent].buttonsAddr[i]->text, lstrlenW(ui->pages[ui->pageCurrent].buttonsAddr[i]->text), ui->m_pText, ui->pages[ui->pageCurrent].buttonsAddr[i]->rect, ui->p_bControlsBrush);
							}
							else
							{
								ui->pages[ui->pageCurrent].buttonsAddr[i]->RbuttonState = RButtonState_Normal;								
								if (nullptr != ui->pages[ui->pageCurrent].buttonsAddr[i]->oc)
									ui->pages[ui->pageCurrent].buttonsAddr[i]->oc();
							}
						}

					}
				}



				D2D1_POINT_2F point0 = { 0 };
				D2D1_POINT_2F point1 = { 0 };
				int slipWidth = 0;
				int slipHeight = 0;
				float value = 0;
				int sliplong = 0;
				int stemp = 0;
				for (size_t i = 0; i < ui->pages[ui->pageCurrent].slipsAddr.size(); i++)
				{
					if (ui->UIS.pt->x == -1 || ui->UIS.pt->y == -1)
						ui->pages[ui->pageCurrent].slipsAddr[i]->MouseDown = false;
					slipHeight = (int)(ui->pages[ui->pageCurrent].slipsAddr[i]->rect.bottom - ui->pages[ui->pageCurrent].slipsAddr[i]->rect.top);
					slipWidth = ui->pages[ui->pageCurrent].slipsAddr[i]->Width;
					point0.x = ui->pages[ui->pageCurrent].slipsAddr[i]->rect.left;
					point0.y = ui->pages[ui->pageCurrent].slipsAddr[i]->rect.top + slipHeight / 2;

					point1.x = ui->pages[ui->pageCurrent].slipsAddr[i]->rect.right;
					point1.y = point0.y;

					ui->p_bControlsBrush->SetColor(ui->pages[ui->pageCurrent].slipsAddr[i]->color);
					ui->hwndren->DrawLine(point0, point1, ui->p_bControlsBrush);
					result = PtInControl(ui->pages[ui->pageCurrent].slipsAddr[i]->rect, ui->dpi, *ui->UIS.pt);

					if (!result && !ui->pages[ui->pageCurrent].slipsAddr[i]->MouseDown)
					{
						//�����겻��slip�� ����ɫ
					}
					else if (!result && ui->pages[ui->pageCurrent].slipsAddr[i]->MouseDown)
					{
						if (ui->UIS.inp_mouseState->rgbButtons[0] == RSKey0)
							ui->pages[ui->pageCurrent].slipsAddr[i]->MouseDown = false;
						if (ui->UIS.pt->x < ui->pages[ui->pageCurrent].slipsAddr[i]->rect.left)
						{
							ui->pages[ui->pageCurrent].slipsAddr[i]->value = 0;
							ui->pages[ui->pageCurrent].slipsAddr[i]->rect2.left = ui->pages[ui->pageCurrent].slipsAddr[i]->rect.left;
							ui->pages[ui->pageCurrent].slipsAddr[i]->rect2.right = ui->pages[ui->pageCurrent].slipsAddr[i]->Width + ui->pages[ui->pageCurrent].slipsAddr[i]->rect.left;
						}
						else if (ui->UIS.pt->x > ui->pages[ui->pageCurrent].slipsAddr[i]->rect.right)
						{
							ui->pages[ui->pageCurrent].slipsAddr[i]->value = 100;
							ui->pages[ui->pageCurrent].slipsAddr[i]->rect2.left = ui->pages[ui->pageCurrent].slipsAddr[i]->rect.right - ui->pages[ui->pageCurrent].slipsAddr[i]->Width;
							ui->pages[ui->pageCurrent].slipsAddr[i]->rect2.right = ui->pages[ui->pageCurrent].slipsAddr[i]->rect.right;
						}
					}
					else
					{
						if (ui->UIS.inp_mouseState->rgbButtons[0] != RSKeyDown)
						{
							ui->p_bControlsBrush->SetColor(TurnDarkOrShallow(ui->pages[ui->pageCurrent].slipsAddr[i]->color, false, 40));
							if (ui->UIS.inp_mouseState->rgbButtons[0] == RSKey0)
								ui->pages[ui->pageCurrent].slipsAddr[i]->MouseDown = false;
						}
						else
						{
							ui->pages[ui->pageCurrent].slipsAddr[i]->MouseDown = true;

							ui->p_bControlsBrush->SetColor(TurnDarkOrShallow(ui->pages[ui->pageCurrent].slipsAddr[i]->color, true, 40));

							ui->pages[ui->pageCurrent].slipsAddr[i]->rect2.left = ui->UIS.pt->x / ui->dpi;
							ui->pages[ui->pageCurrent].slipsAddr[i]->rect2.left = ui->pages[ui->pageCurrent].slipsAddr[i]->rect2.left - slipWidth / 2;
							ui->pages[ui->pageCurrent].slipsAddr[i]->rect2.right = ui->pages[ui->pageCurrent].slipsAddr[i]->rect2.left + slipWidth;

							sliplong = (int)(ui->pages[ui->pageCurrent].slipsAddr[i]->rect.right - ui->pages[ui->pageCurrent].slipsAddr[i]->rect.left);
							stemp = (int)(ui->pages[ui->pageCurrent].slipsAddr[i]->rect.right - ui->pages[ui->pageCurrent].slipsAddr[i]->rect2.left);
							stemp = sliplong - stemp;
							value = (float)stemp / (float)sliplong;
							value *= 100;
							value += 5;
							ui->pages[ui->pageCurrent].slipsAddr[i]->value = (int)value;

						}
					}
					ui->hwndren->FillRectangle(ui->pages[ui->pageCurrent].slipsAddr[i]->rect2, ui->p_bControlsBrush);
					//swprintf_s((wchar_t*)pages[pageCurrent].slipsAddr[i]->text,20, L"%d",value);
					//hwndren->DrawTextW(pages[pageCurrent].slipsAddr[i]->text,sizeof(pages[pageCurrent].slipsAddr[i]->text),m_pText,pages[pageCurrent].slipsAddr[i]->rect,p_bControlsBrush);
				}

				//

				D2D1_ROUNDED_RECT rrect = { 0 };
				bool b = false;
				for (size_t j = 0; j < ui->pages[ui->pageCurrent].editsAddr.size(); j++)
				{
					if (ui->pages[ui->pageCurrent].editsAddr[j]->focus)
					{
						b = true;
					}
				}
				if (!b)
				{
					ui->UIS.wParam->clear();
					ui->editInputChar.clear();
				}
				for (size_t j = 0; j < ui->pages[ui->pageCurrent].editsAddr.size(); j++)
				{

					rrect.rect = ui->pages[ui->pageCurrent].editsAddr[j]->rect;
					rrect.radiusX = (ui->pages[ui->pageCurrent].editsAddr[j]->rect.right - ui->pages[ui->pageCurrent].editsAddr[j]->rect.left) / 50;
					rrect.radiusY = rrect.radiusX;

					if (ui->UIS.pt->x == -1 || ui->UIS.pt->y == -1)
						result = false;
					else
						result = PtInControl(ui->pages[ui->pageCurrent].editsAddr[j]->rect, ui->dpi, *ui->UIS.pt);

					DIMOUSESTATE mousestate = *ui->UIS.inp_mouseState;

					if (!result)
					{
						if (!ui->pages[ui->pageCurrent].editsAddr[j]->focus)
						{
							ui->p_bControlsBrush->SetColor(ui->pages[ui->pageCurrent].editsAddr[j]->color);
							ui->p_bControlsBrush->SetOpacity(1.0f);
						}
						else
						{
							ui->p_bControlsBrush->SetColor(ui->pages[ui->pageCurrent].editsAddr[j]->color);
							ui->p_bControlsBrush->SetOpacity(0.8f);
						}
						if (mousestate.rgbButtons[0] == RSKeyDown)
							ui->pages[ui->pageCurrent].editsAddr[j]->focus = false;
					}
					else
					{
						if (!ui->pages[ui->pageCurrent].editsAddr[j]->focus)
						{
							if (mousestate.rgbButtons[0] == RSKeyDown)
							{
								ui->pages[ui->pageCurrent].editsAddr[j]->focus = true;
							}
							ui->p_bControlsBrush->SetColor(ui->pages[ui->pageCurrent].editsAddr[j]->color);
							ui->p_bControlsBrush->SetOpacity(1.0f);
						}
						else
						{
							ui->p_bControlsBrush->SetColor(ui->pages[ui->pageCurrent].editsAddr[j]->color);
							ui->p_bControlsBrush->SetOpacity(0.7f);
						}
					}
					ui->hwndren->FillRoundedRectangle(&rrect, ui->p_bControlsBrush);

					DWRITE_TEXT_METRICS metrics = { 0 };

					ui->pages[ui->pageCurrent].editsAddr[j]->textLayout->GetMetrics(&metrics);

					float width = ui->pages[ui->pageCurrent].editsAddr[j]->rect.right - ui->pages[ui->pageCurrent].editsAddr[j]->rect.left;

					if (ui->pages[ui->pageCurrent].editsAddr[j]->focus)
					{
						ui->editInputChar = *ui->UIS.wParam;
						if (0 != ui->editInputChar.size())
						{
							for (size_t i = 0; i < ui->editInputChar.size(); i++)
							{
								if (ui->editInputChar[i] == 8)
								{
									if (0 != ui->pages[ui->pageCurrent].editsAddr[j]->text.size())
									{
										ui->pages[ui->pageCurrent].editsAddr[j]->text.erase(ui->pages[ui->pageCurrent].editsAddr[j]->text.end() - 1);
									}
								}
								else if (ui->editInputChar[i] == 13)
								{
									ui->pages[ui->pageCurrent].editsAddr[j]->text+=L"\r\n";
								}
								else
								{
									ui->pages[ui->pageCurrent].editsAddr[j]->text.push_back(ui->editInputChar[i]);
									layoutLock.lock();

									if (ui->pages[ui->pageCurrent].editsAddr[j]->textLayout)
										ReleaseCom(ui->pages[ui->pageCurrent].editsAddr[j]->textLayout);

									dWriteFactory->CreateTextLayout(ui->pages[ui->pageCurrent].editsAddr[j]->text.c_str(), ui->pages[ui->pageCurrent].editsAddr[j]->text.size(), editTextFormat, 0.5f, 0.5f, &ui->pages[ui->pageCurrent].editsAddr[j]->textLayout);
									ui->pages[ui->pageCurrent].editsAddr[j]->textLayout->GetMetrics(&metrics);
									layoutLock.unlock();
									if ((metrics.width / width) >= 0.95)
									{
										ui->pages[ui->pageCurrent].editsAddr[j]->text.insert(ui->pages[ui->pageCurrent].editsAddr[j]->text.end() - 2, L'\r\n');
									}
								}
							}
							ui->UIS.wParam->clear();
							ui->editInputChar.clear();
						}
						if (ui->pages[ui->pageCurrent].editsAddr[j]->textLayout)
							ReleaseCom(ui->pages[ui->pageCurrent].editsAddr[j]->textLayout);
						dWriteFactory->CreateTextLayout(ui->pages[ui->pageCurrent].editsAddr[j]->text.c_str(), ui->pages[ui->pageCurrent].editsAddr[j]->text.size(), editTextFormat, 0.5f, 0.5f, &ui->pages[ui->pageCurrent].editsAddr[j]->textLayout);
					}


					ui->p_bControlsBrush->SetColor({ 1.0f,1.0f,1.0f,1.0f });
					ui->p_bControlsBrush->SetOpacity(1.0f);
					layoutLock.lock();

					//ui->hwndren->PushAxisAlignedClip(ui->pages[ui->pageCurrent].editsAddr[i]->rect , D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
					ui->hwndren->DrawTextLayout({ ui->pages[ui->pageCurrent].editsAddr[j]->rect.left ,ui->pages[ui->pageCurrent].editsAddr[j]->rect.top }
					, ui->pages[ui->pageCurrent].editsAddr[j]->textLayout, ui->p_bControlsBrush);
					//ui->hwndren->PopAxisAlignedClip();
					layoutLock.unlock();
					DWRITE_HIT_TEST_METRICS dhtm = { 0 };
					BOOL inside = false;
					BOOL outside = false;
					ui->textLayout->HitTestPoint(ui->UIS.pt->x / ui->dpi, ui->UIS.pt->y / ui->dpi, &inside, &outside, &dhtm);
					ui->textLayout->SetUnderline(FALSE, { 0,MAXINT });
					ui->textLayout->SetUnderline(TRUE, { dhtm.textPosition,1 });
				}



				ui->hwndren->EndDraw();
			}
		}
		end = GetTickCount64();
		temp = end - start;
		if (temp < PerFrameTime)
		{
			Sleep(PerFrameTime - temp);
		}
		ui->frame == 60 ? ui->frame = 0 : ++ui->frame;

	}
	_endthreadex(0);
}



bool UI::CreateRenderThread()
{
	int result = 0;
	if (!bRenderThread)
		result = _beginthread(DrawPage, 0, this);
	if (result == -1)
		return false;

	this->bRenderThread = true;
	return true;
}

HWND UI::RCreateWindow(int width, int height, DWORD winStyle, HINSTANCE hInstance, WNDPROC mwWndProc, const wchar_t* ClassName, const wchar_t* icoPath)
{
	WNDCLASSEX wnd = { 0 };
	wnd.cbClsExtra = 0;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon = 0/*LoadIcon(hInstance,MAKEINTRESOURCE())*/;
	wnd.hIconSm = 0/*LoadIcon(0, L"ico.png")*/;
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = mwWndProc;
	wnd.lpszClassName = ClassName;
	wnd.style = CS_VREDRAW | CS_HREDRAW;
	wnd.lpszMenuName = NULL;
	if (!RegisterClassEx(&wnd))
	{
		MessageBox(0, L"Register Class Failed!", L"error", 0);
		return false;
	}
	HDC hdc = GetDC(0);
	int ScreenWidth = GetDeviceCaps(hdc, DESKTOPHORZRES);
	int ScreenHeight = GetDeviceCaps(hdc, DESKTOPVERTRES);
	ReleaseDC(NULL, hdc);
	int x = (ScreenWidth - width) / 2,
		y = (ScreenHeight - height) / 2;
	HWND hwnd;
	hwnd = CreateWindow(ClassName, L"", winStyle, x, y, width, height, 0, 0, hInstance, 0);

	return hwnd;
}
BYTE UI::CreateButton(D2D1_COLOR_F color, D2D1_COLOR_F Textcolor, const wchar_t* text, D2D1_RECT_F rect, void(*oc)(void))
{
	ButtonStruct bs = { 0 };
	RID rid = { 0 };
	rid.conTrol_type = RControlType_Button;
	rid.id = RIds.size();
	rid.pointId = buttons.size();
	bs.bitmapOpacity = 1.0f;
	bs.color = color;
	bs.Textcolor = Textcolor;
	bs.id = rid.pointId;
	bs.oc = oc;
	bs.rect = rect;
	bs.text = text;
	bs.MouseOncolor = TurnDarkOrShallow(color, false, 40);
	bs.OnClickcolor = TurnDarkOrShallow(color, true, 40);
	this->buttons.push_back(bs);
	this->RIds.push_back(rid);
	return rid.id;
}
bool UI::LoadBitmapForControl(BYTE id, const wchar_t* FilePath)
{
	for (size_t i = 0; i < RIds.size(); i++)
	{
		if (RIds[i].id == id)
		{
			if (RIds[i].conTrol_type == RControlType_Button)
			{
				buttons[RIds[i].pointId].bitmap = D2DCreateBitmap(hwndren, imageFactory, FilePath);
				if (buttons[RIds[i].pointId].bitmap)
					return true;
			}

		}
	}
	return false;
	//return false;
}
BYTE UI::CreateSlip(D2D1_COLOR_F color, D2D1_RECT_F rect, const wchar_t* text/*, bool vertical*/)
{
	SlipStruct ss = { 0 };
	RID rid = { 0 };
	ss.color = color;
	ss.rect = rect;
	ss.text = text;
	ss.id = slips.size();
	rid.conTrol_type = RControlType_Slip;
	rid.id = RIds.size();
	rid.pointId = ss.id;

	//ss.vertical = vertical;
	int width = (int)(ss.rect.right - ss.rect.left);
	width /= 10;
	ss.Width = width;
	ss.rect2.left = ss.rect.left;
	ss.rect2.right = ss.rect.left + width;
	ss.rect2.bottom = ss.rect.bottom;
	ss.rect2.top = ss.rect.top;
	slips.push_back(ss);
	RIds.push_back(rid);
	return rid.id;
}
void UI::SetEditText(BYTE id, wstring text)
{
	for (size_t i = 0; i < RIds.size(); i++)
	{
		if (RIds[i].id == id)
		{
			edits[RIds[i].pointId].text = text;
			ReleaseCom(edits[RIds[i].pointId].textLayout);
			if (dWriteFactory)
			{
				dWriteFactory->CreateTextLayout(edits[RIds[i].pointId].text.c_str(), edits[RIds[i].pointId].text.size(), editTextFormat, 0.5f, 0.5f, &edits[RIds[i].pointId].textLayout);
			}
		}
	}
}
bool UI::LoadControlFromFile(const wchar_t* filePath)
{

	ifstream ifile(filePath, std::ios::binary);
	if (!ifile.is_open())
		return false;
	string line = "";
	vector<string> lines;
	while (getline(ifile, line))
	{
		lines.push_back(line);
	}
	vector<int> signStart;
	vector<int> signEnd;
	int numTemp = 0;

	for (size_t i = 0; i < lines.size(); i++)//�ҳ����еĳ���־��ĩ��־
	{
		if (signStart.size() <= signEnd.size()) //���β��־���������ڻ��ߵ��ڳ���־������ ��ô�Ͳ��ҳ���־
		{
			numTemp = lines[i].find("[RC]");
			if (-1 != numTemp)
			{
				signStart.push_back(i);
			}
		}
		else   //����г���־  ��ôѰ��β��־
		{
			numTemp = lines[i].find("[&RC]");
			if (-1 != numTemp)
			{
				signEnd.push_back(i);
			}
		}
	}
	if (signStart.size() == 0)//���û���ҵ���־ �򷵻�false
	{
		return false;
	}
	if (signEnd.size() != signStart.size())//�������־��ĩ��־��������  �ͱ��� ����ʧ��
	{
		MessageBox(0, L"The number of the start and end signs is not the same!", L"Load Control Failed!", 0 | MB_ICONERROR);
		return false;
	}


	string varName = ""; //�����洢�Ⱥ�ǰ���ֵ������
	string var = "";  //�����洢=�ź����ֵ
	int search = 0;
	for (size_t i = 0; i < lines.size(); i++)//��ȥ�����
	{
		search = 0;
		while (search != -1)
		{
			search = lines[i].find('\r');
			if (search != -1)lines[i].erase(lines[i].begin() + search);
			search = lines[i].find('\n');
			if (search != -1)lines[i].erase(lines[i].begin() + search);
			search = lines[i].find('\t');
			if (search != -1)lines[i].erase(lines[i].begin() + search);
		}
		
	}
	for (size_t i = 0; i < lines.size(); i++)  //��ȥ����
	{
		if (lines[i] == "")
			lines.erase(lines.begin() + i);
	}
	search = -1;
	for (size_t i = 0; i < signStart.size(); i++)//����ÿһ�Ա�ʶ������
	{

		for (size_t j = signStart[i]; j < signEnd[i]; j++)
		{
			search = lines[j].find('=');
			if (-1 != search)
			{
				varName = lines[j].substr(0, search);//��ͷ��ȡ���Ⱥ�֮ǰ
				var = lines[j].substr(search + 1, lines[j].size() - search); //�ӵȺ�֮���ȡ���ַ�����β��
				//varName.compare();
			}
		}
	}
	return false;
}
void UI::DeleteControl(BYTE id)
{

	for (size_t i = 0; i < RIds.size(); i++)
	{
		if (RIds[i].id == id)
		{

			switch (RIds[i].conTrol_type)
			{
			case RControlType_Button:
				if (RIds[i].pointId < buttons.size())
				{
					buttons.erase(buttons.begin() + RIds[i].pointId);
					for (size_t j = 0; j < pages.size(); j++)
					{
						for (size_t k = 0; k < pages[j].buttonsAddr.size(); k++)
						{
							if (pages[j].buttonsAddr[k]->id == RIds[i].pointId)
							{
								pages[j].buttonsAddr.erase(pages[j].buttonsAddr.begin() + k);
							}
						}
					}
				}
				return;
			case RControlType_Slip:
				if (RIds[i].pointId < slips.size())
				{
					slips.erase(slips.begin() + RIds[i].pointId);
					for (size_t j = 0; j < pages.size(); j++)
					{
						for (size_t k = 0; k < pages[j].slipsAddr.size(); k++)
						{
							if (pages[j].slipsAddr[k]->id == RIds[i].pointId)
							{
								pages[j].slipsAddr.erase(pages[j].slipsAddr.begin() + k);
							}
						}
					}
				}
				return;
			case RControlType_Edit:
				if (RIds[i].pointId < edits.size())
				{
					edits.erase(edits.begin() + RIds[i].pointId);
					for (size_t j = 0; j < pages.size(); j++)
					{
						for (size_t k = 0; k < pages[j].editsAddr.size(); k++)
						{
							if (pages[j].editsAddr[k]->id == RIds[i].pointId)
							{
								pages[j].editsAddr.erase(pages[j].editsAddr.begin() + k);
							}
						}
					}
				}
				return;
			}
		}
	}

}
BYTE UI::CreateEdit(D2D1_RECT_F rect, D2D1_COLOR_F color)
{
	EditStruct es = { 0 };
	RID rid = { 0 };
	es.bitmapOpacity = 1.0f;
	es.cursorPos = 0;
	es.rect = rect;
	es.id = edits.size();
	rid.conTrol_type = RControlType_Edit;
	rid.pointId = es.id;
	rid.id = RIds.size();
	es.text = L"";
	es.color = color;
	dWriteFactory->CreateTextLayout(L"", lstrlenW(L""), editTextFormat, 0.5f, 0.5f, &es.textLayout);
	edits.push_back(es);
	RIds.push_back(rid);
	return rid.id;
}
int* UI::GetSlipValueAddress(BYTE id)
{
	for (size_t i = 0; i < RIds.size(); i++)
	{
		if (RIds[i].id == id)
		{
			return &slips[RIds[i].pointId].value;
		}
	}
	return nullptr;
}

bool UI::SetPageForControl(BYTE id, BYTE pageid)
{
	if (pageid >= this->pages.size())
	{
		return false;
	}
	//order = this->GetOrderById(id, Control_Type);//����id��ȡ�ؼ���vector�����Ԫ��˳��  
	for (size_t i = 0; i < RIds.size(); i++)
	{
		if (RIds[i].id == id)
		{
			switch (RIds[i].conTrol_type)
			{
			case RControlType_Button:
				pages[pageid].buttonsAddr.push_back(&buttons[RIds[i].pointId]);  //�������ť�ĵ�ַ������page		
				return true;
			case  RControlType_Slip:
				pages[pageid].slipsAddr.push_back(&slips[RIds[i].pointId]);
				return true;
			case RControlType_Edit:
				pages[pageid].editsAddr.push_back(&edits[RIds[i].pointId]);
				return true;
			default:
				break;
			}
		}
	}

	return true;
}
void UI::LoadBitmapForPageBackgournd(BYTE Pageid, const wchar_t* FilePath)
{
	if (Pageid >= 20)
		return;

	pages[Pageid].bitmap = D2DCreateBitmap(hwndren, imageFactory, FilePath);
}
UI::~UI()
{
	bRenderThread = false;

	for (size_t i = 0; i < pages.size(); i++)
	{
		for (size_t j = 0; j < pages[i].editsAddr.size(); j++)
		{
			ReleaseCom(pages[i].editsAddr[j]->textLayout);
			pages[i].editsAddr.~vector();
		}
		for (size_t j = 0; j < pages[i].buttonsAddr.size(); j++)
		{
			pages[i].buttonsAddr.~vector();
		}
		for (size_t j = 0; j < pages[i].slipsAddr.size(); j++)
		{
			pages[i].slipsAddr.~vector();
		}
	}
	pages.~vector();
	this->editInputChar.clear();
	//ReleaseCom(editTextFormat);
	//ReleaseCom(d2dFactory);
	//ReleaseCom(dWriteFactory);
	//ReleaseCom(imageFactory);
	ReleaseCom(this->m_pText);
	ReleaseCom(this->hwndren);
	ReleaseCom(this->ren);
	ReleaseCom(this->p_bControlsBrush);
}

