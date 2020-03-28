#include<Windows.h>
#include<fstream>
#include<vector>
#include<any>

using namespace std;

template <class T1>
T1 AnySafeConvert(any a)
{
	T1 t1;
	try
	{
		t1 = any_cast<T1>(a);
	}
	catch (exception & e)
	{
		return 0;
	}
	return t1;
}

template <class T1>
struct rvstruct
{
	string varName;
	T1 var;
};
class Rvar
{
public:
	void aRvar(string name, any a)
	{
		RAny.push_back({ name,a });
	}
	void ResetVar(string name,any a)
	{
		int i = GetValueOrder(name);
		RAny[i].var.reset();
		RAny[i].var=a;		
	}

	template <class T1>
	any Operator(string name, string name2, char op)
	{
		int i1 = GetValueOrder(name);
		int i2 = GetValueOrder(name2);
		T1 itemp1;
		T1 itemp2;
		any result;
		try
		{
			itemp1 = AnySafeConvert<T1>(RAny[i1].var);
			itemp2 = AnySafeConvert<T1>(RAny[i2].var);

			switch (op)
			{
			case '+':
				result = itemp1 + itemp2;
				break;
			case '-':
				result = itemp1 - itemp2;
				break;
			case '*':
				result = itemp1 * itemp2;
				break;
			case '/':
				result = itemp1 / itemp2;
				break;
			case '^':
				result = itemp1 + itemp2;
				break;
			}
		}
		catch (exception & e)
		{
			return 0;
		}
		return result;
	}
	any GetValue(string name)
	{
		for (size_t i = 0; i < RAny.size(); i++)
		{
			if (RAny[i].varName.compare(name) == 0)
			{
				return RAny[i].var;
			}
		}
	}
	int GetValueOrder(string name)
	{
		for (size_t i = 0; i < RAny.size(); i++)
		{
			if (RAny[i].varName.compare(name) == 0)
			{
				return i;
			}
		}
	}
	any* GetValueAddr(string name)
	{
		for (size_t i = 0; i < RAny.size(); i++)
		{
			if (RAny[i].varName.compare(name) == 0)
			{
				return &RAny[i].var;
			}
		}
	}
private:
	vector<rvstruct<any>> RAny;
};


struct RConstruction
{
	int RfNum;
	any* t1;
	any* t2;
	any* t3;
	int tt1;
	int tt2;
	int tt3;

	int ro;  //  1 +  2 -  3 * 4 /  5 % 6^  7& 
	any* addVar;
	any* var;
	bool empty;
};
struct RCSStruct
{
	vector<RConstruction> rcs;
	int conditionType;      //0 if  1 while  
	int conditionSign;   //0 ==   1!=  2>   3<  4>=   5<=
	int compareVar;
	int conditionVar;
	bool empty;
};
struct RCSST2
{
	RConstruction rcs;
	RCSStruct rcss;
};
//void RFuncExecuter(RConstruction rcs)
//{
//	if (rcs.ro != 0)
//	{
//		switch (rcs.ro)
//		{   //  1 +  2 -  3 * 4 /  5 % 6^  7& 
//		case 1:
//			rcs.var += rcs.addVar;
//		default:
//			break;
//		}
//	}
//	switch (rcs.RfNum)
//	{
//	case 0:    //模拟鼠标左键按下
//		if (any_cast<int>(rcs.t1) == 0)
//		{
//			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
//			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
//		}
//		else if (any_cast<int>(rcs.t1) == 1)
//		{
//			mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
//			mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
//		}
//		else
//		{
//			mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
//			mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
//		}
//
//		break;
//	case 1:
//
//		break;
//	default:
//		break;
//	}
//}
class RCSS
{
public:
	void AddRConstruction(RConstruction rcs)
	{
		RCSStruct rsstemp;
		rsstemp.empty = true;
		rcss.push_back({ rcs,rsstemp });
	}
	void AddRCSStruct(RCSStruct rcs)
	{
		RConstruction rc = { 0 };
		rc.empty = true;
		rcss.push_back({ rc,rcs });
	}
	void RunRConstruction(RConstruction rcs)
	{
		if (!rcs.empty)
		{
			if (rcs.RfNum != -1)  //如果有函数标志
			{
				switch (rcs.RfNum)
				{
				case 0:    //模拟鼠标左键按下   RF :  MouseDown();//param: 0  L   1R   2M
					if (*any_cast<int>(rcs.t1) == 0)
					{
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}
					else if (*any_cast<int>(rcs.t1) == 1)
					{
						mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
					}
					else
					{
						mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
					}

					break;
				case 1:

					break;
				default:
					break;
				}
			}
			else  //如果没有函数标志  那应该就是  RV的operator 
			{
				string temp;
				BYTE compareVar = 0;
				switch (rcs.ro)
				{
				case 1: //  1 +  2 -  3 * 4 /  5 % 6^  7& 
					temp = rcs.var->type().name();
					compareVar = temp.compare(rcs.addVar->type().name());
					if (compareVar == 0 || compareVar == 1)
					{
						if (temp.compare(""));
					}
					break;
				default:
					break;
				}
			}
		}
		else
		{

		}
	}


	void Run()
	{

		for (size_t i = 0; i < rcss.size(); i++)
		{
			if (rcss[i].rcs.empty == true)  //如果单句句式是空  那么执行带关键词的
			{
				if (rcss[i].rcss.conditionType == 0)
				{
					switch (rcss[i].rcss.conditionSign)
					{
					case 0:   //==
						if (rcss[i].rcss.compareVar == rcss[i].rcss.conditionVar)
						{

						}
						break;
					case 1:  //!=
						break;
					case 2:  //>
						break;
					case 3:  //<
						break;
					case 4:  //>=
						break;
					case 5:   //<=
						break;
					}

				}
			}
			else
			{

			}
		}
	}

private:
	vector<RCSST2> rcss;
};

vector<string> split(string str, string pattern)
{
	string::size_type pos;
	vector<string> result;

	str += pattern;//扩展字符串以方便操作
	int size = str.size();

	for (int i = 0; i < size; i++) {
		pos = str.find(pattern, i);
		if (pos < size) {
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}
Rvar rvar;
RCSS rcss;
vector<string> RFunc = {
	"RGetForegroundWindow"
		"RGetMouseX",
		"RGetMouseY" };

//给出函数名 根据函数名返回值
std::any ValueofFunc(string funcName)
{
	if (funcName == "")
		return 0;
	int funcNum = -1;
	int searchtemp = -1;
	for (size_t i = 0; i < RFunc.size(); i++)
	{
		searchtemp = funcName.compare(RFunc[i]);
		if (searchtemp == 0 || searchtemp == 1)
		{
			funcNum = i;
			break;
		}
	}
	any returnTemp;
	POINT returnPointtemp = { 0 };
	switch (funcNum)
	{
	case 0:  //RGetForegroundWindow
		return GetForegroundWindow();
		break;
	case 1:  //RGetMouseX
		GetCursorPos(&returnPointtemp);
		return returnPointtemp.x;
		break;
	case 2:  //RGetMouseY
		GetCursorPos(&returnPointtemp);
		return returnPointtemp.y;
		break;
	default:
		return nullptr;
	}
	return nullptr;
}
void rx(const char* path)
{
	string rvl = "";
	if (path == nullptr)
		return;
	ifstream fin(path, std::ios::binary);
	if (!fin.is_open())
		return;
	vector<char> buf(fin.seekg(0, std::ios::end).tellg());
	fin.seekg(0, std::ios::beg).read(&buf[0], static_cast<std::streamsize>(buf.size()));
	fin.close();
	vector<string> line;
	line = split(&buf[0], "\r\n");
	buf.~vector();
	int search = -1;
	for (size_t i = 0; i < line.size(); i++)//除去分号和分号后面的多余字符
	{
		search = line[i].rfind(';');
		if (search != -1)line[i].erase(search, line[i].size() - search);

	}

	int rvsearch = -1;
	vector<string> var;
	vector<string> varName;
	int endDef = -1;
	for (size_t i = 0; i < line.size(); i++)
	{
		rvsearch = line[i].find("rv");
		if (rvsearch != -1)//如果找到rv标识符就认为这是声明变量
		{
			search = line[i].find('=');
			if (search != -1)
			{
				var.push_back(line[i].substr(search + 1, line[i].size() - search));
				varName.push_back(line[i].substr(rvsearch + 2, search - 2));
			}
		}
		else
		{
			endDef = line[i].find("&Rendefine");
			if (endDef != -1)
			{
				endDef = i;
				break;
			}
		}
	}
	bool bnum;
	int searchend = -1;
	string funcName = "";
	int temp = 0;

	for (size_t i = 0; i < var.size(); i++)
	{
		bnum = true;
		search = var[i].find("\"");
		searchend = var[i].rfind("\"");
		if (search != searchend && search != -1)
		{
			rvar.aRvar(varName[i], static_cast<string>(var[i].substr(search + 1, searchend - search - 1)));
		}
		else
		{
			search = var[i].find("rf:");
			if (search != -1)  //如果找到函数标识符
			{
				funcName = var[i].substr(search + 3, var[i].size() - search);
				rvar.aRvar(varName[i], ValueofFunc(funcName));

			}
			else  //如果var既不是字符串也不是函数返回值 则进行数字判断
			{
				for (size_t j = 0; j < var[i].size(); j++)
				{
					if (isdigit(var[i][j]) == 0)//如果存在字符 
					{
						bnum = false;
					}
				}
				if (bnum)  //如果经过一轮检查还是没有存在字符 那么就判断它为数字
				{
					rvar.aRvar(varName[i], atoi(var[i].c_str()));
				}
			}
		}

	}

	var.~vector();
	varName.~vector();
	string strTemp = "";
	string strparamTemp = "";
	vector<string> params;
	RConstruction rcs;

	for (size_t i = endDef + 1; i < line.size(); i++)
	{
		search = line[i].find("rk:");
		if (search != -1)
		{
			rcs = { -1 };
			//funcName = line[i].substr(search+3,line[i].size()-search+3);
			search = line[i].find("while");
			if (search != -1)
			{
				//rcss.conditionType = 1;
				search = line[i].find('(');
				searchend = line[i].find(')');
				if (search != -1 && searchend != -1)
				{
					strTemp = line[i].substr(search + 1, searchend - search - 1);
				}

			}
		}
		search = line[i].find("rf:");
		if (search != -1)
		{
			rcs = { -1 };
			searchend = line[i].find('(');
			funcName = line[i].substr(search + 3, searchend - search - 3);
			search = searchend;
			searchend = line[i].find(')');
			strparamTemp = line[i].substr(search + 1, searchend - search - 1);
			params = split(strparamTemp, ",");
			strparamTemp.clear();
			int searchvar = -1;
			for (size_t i = 0; i < params.size(); i++)
			{
				searchvar = params[i].find("&");
				if (searchvar != -1)
				{
					switch (i)
					{
					case 0:
						rcs.t1 = rvar.GetValueAddr(params[i].substr(searchvar, params[i].size() - searchvar));
						break;
					case 1:
						rcs.t2 = rvar.GetValueAddr(params[i].substr(searchvar, params[i].size() - searchvar));
						break;
					case 2:
						rcs.t3 = rvar.GetValueAddr(params[i].substr(searchvar, params[i].size() - searchvar));
					default:
						break;
					}
				}
				else
				{
					switch (i)
					{
					case 0:
						rcs.tt1 = atoi(params[i].c_str());
						break;
					case 1:
						rcs.tt2 = atoi(params[i].c_str());
						break;
					case 2:
						rcs.tt3 = atoi(params[i].c_str());
					default:
						break;
					}
				}
			}
			rcss.AddRConstruction(rcs);

			if (funcName == "mouseDown")
			{
				rcs.RfNum = 0;
			}
		}
		search = line[i].find("ro:");
		if (search != -1)
		{
			rcs = { -1 };
			strTemp = line[i].substr(search + 3, line[i].size() - search - 3);

		}
		//search=;
	}
}

void sgamma(int r, int g, int b)
{
	HDC hdc = GetDC(0);
	WORD GammaArray[3][256] = { 0 };
	for (int i = 0; i < 256; i++)
	{
		GammaArray[0][i] = min(65535, i * (r + 128));
		GammaArray[1][i] = min(65535, i * (g + 128));
		GammaArray[2][i] = min(65535, i * (b + 128));
	}
	bool bRet = false;
	//bRet = SetDeviceGammaRamp(hdc,GammaArray);
	if (hdc)
	{
		ReleaseDC(0, hdc);
	}
}


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrehIntance,
	_In_ LPWSTR nCmdLine, _In_ int nCmdShow)
{

	rvar.aRvar("i1", L"h");
	rvar.aRvar("i2",L"i");
	rvar.ResetVar("i1",23);
	rvar.ResetVar("i2",23);
	rvar.ResetVar("i1",rvar.Operator<int>("i1", "i2", '+'));
	int a = AnySafeConvert<int>(rvar.GetValue("i1"));


	//any a = rvar.Operator<const wchar_t*>("i1", "i2", '+');
	

	//any a = rvar.GetValue("i1");

	//const wchar_t* ws = AnySafeConvert<const wchar_t* >(rvar.GetValue("i1"));


	//SetForegroundWindow(c.cast());

	//fstream file;

	/*sgamma(128,128,80);
	void* a = 0;
	int b = 1050;
	a = reinterpret_cast<void*>(b);
	int i = reinterpret_cast<int>(a);
	string name = typeid(a).name();*/


	rx("C:\\re.txt");


	return 0;
}