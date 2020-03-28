#pragma once
#include<Windows.h>
#include<thread>
#include<vector>
#pragma comment(lib,"winmm.lib")
using namespace std;
typedef struct RTimer_t
{
	BYTE id;
	BYTE var;
};
enum RTimerStructType
{
	RTimer_type_Normal = 0,
	RTimer_type_SelfReset = 1,
}; 
enum RTimerState
{
	RTimer_State_NOCALL = 0,
	RTimer_State_ISTIME = 1
};
//type:
//ms:timer 
//cycle:how many times will timer call you
struct RTimerStruct 
{
	BYTE type;
	long long ms;
	long long sendTime;
	int cycle;
	RTimer_t rtimer_t;
};
class RTimer
{
private:	
	thread timeThread;
	thread eventThread;
	bool bthreadWork=false;
	long long ltime=0;
	std::vector<RTimerStruct> RTS;
	
public:		
	void resetTimerNOCall(BYTE id);
	long long GetTime() { return ltime; };
	vector<RTimerStruct>* GetRTS() { return &RTS; };
	void AddTime(long long ms) { ltime += ms; };
	bool* GetThreadState() { return &bthreadWork; };
	void ReSetTimer() { ltime = 0; };
	RTimer_t* CreateTimer(BYTE type,long long ms,int cycle);
	void Start();
	void Stop();
	~RTimer();
};
void RTimer::resetTimerNOCall(BYTE id)
{
	if (id < RTS.size())
	{
		RTS[id].rtimer_t.var = RTimer_State_NOCALL;
		RTS[id].sendTime = ltime;
		RTS[id].cycle > 0 ? RTS[id].cycle-- : RTS[id].cycle;
	}
}
RTimer_t* RTimer::CreateTimer(BYTE type, long long ms, int cycle)
{
	RTimerStruct rts = {0};
	rts.cycle = cycle;
	rts.ms = ms;
	rts.rtimer_t.id = RTS.size();
	rts.rtimer_t.var =RTimer_State_NOCALL;
	rts.type = type;
	rts.sendTime = ltime;
	RTS.push_back(rts);
	int temp = 0;
	if (RTS.size() != 0)
	{
		temp = RTS.size();
		return &RTS[temp - 1].rtimer_t;
	}
	return nullptr;
}

void _cdecl EventFunc(void* param)
{
	RTimer* rtimer = (RTimer*)param;
	bool* b = rtimer->GetThreadState();
	vector<RTimerStruct> *RTS = rtimer->GetRTS();
	long long ltime=rtimer->GetTime();
	while (*b)
	{
		ltime = rtimer->GetTime();
		for (vector<RTimerStruct>::iterator i = RTS->begin(); i !=RTS->end(); i++)
		{
			if (i->rtimer_t.var == RTimer_State_ISTIME)
			{
				if (i->cycle==-1)
				{
					i->sendTime = ltime;
				}
				
				if (i->type == RTimer_type_Normal)
				{
					i->rtimer_t.var = RTimer_State_NOCALL;
				}
				else if (i->type == RTimer_type_SelfReset)
				{

				}
			}
			if (i->ms + i->sendTime <= ltime&&(i->cycle>0||i->cycle==-1))
			{
				i->rtimer_t.var = RTimer_State_ISTIME;
			}
		}
		
		Sleep(1);
	}
	_endthreadex(0);
	return;
}


void _cdecl UpdateTime(void* param)
{
	RTimer* rtimer = (RTimer*)param;
	bool* b = rtimer->GetThreadState();
	while (*b)
	{
		rtimer->AddTime(20);
		Sleep(19);
	}
	_endthreadex(0);
	return;
}
void RTimer::Start()
{
	bthreadWork = _beginthread(UpdateTime,0,this);
	bthreadWork = _beginthread(EventFunc,0,this);
}
void RTimer::Stop()
{
	bthreadWork = false;	
}
RTimer::~RTimer()
{
	
}