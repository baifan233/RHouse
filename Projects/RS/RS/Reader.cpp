#include "Reader.h"



Reader::Reader()
{
	WNDCLASSEX wnd;
	wnd.lpfnWndProc = this->WndProc;
}

Reader::~Reader()
{
}
