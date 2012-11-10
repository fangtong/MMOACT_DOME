#include "ApplicationBase.h"

#include "Log.h"
#include "Configuration.h"
#include "NetWorkDelegate.h"
#include "DBCP.h"
#include "Exception.h"

#include "Mmsystem.h"
#pragma comment(lib,"Winmm")
#include "IRunnable.h"
#include "FinalClass.h"
#include "Timer.h"
#include "ThreadWorker.h"
#include "ClearSessionThreadJob.h"
#include "DBDateManage.h"
#include "Broadcast.h"
#include "HttpUtil.h"

using namespace SevenSmile::LogNameSpace;
using namespace SevenSmile::Util::Debug;
using namespace SevenSmile::ORM;
using namespace SevenSmile::Util::TimeNameSpace;

ApplicationBase::ApplicationBase(void)
{
	sPtrBroadcastDelegate = SharPtrIBroadcast(new Broadcast());
	_lpConfig = 0;
	_hEventSocketRecive = CreateEvent(NULL, FALSE, FALSE, NULL);
}

ApplicationBase::~ApplicationBase(void)
{
	if(_bRunMark)
	{
		Release();
	}
	if(_lpConfig) {
		_lpConfig->Destory();
		_lpConfig = 0; 
	}

	sPtrNetWorkDelegate = NullShardPtr(NetWorkDelegate);
	sPtrDBCPDelegate = NullShardPtr(DBCP);
	sPtrBroadcastDelegate = NullShardPtr(IBroadcast);
	CloseHandle(_hEventSocketRecive);
}

bool 
ApplicationBase::Start()
{
	Exception::InitTranslateSEHToCE();
	bool res = false;
	do 
	{
		if(!LoadConfig()) break;

		uiFrame				= _lpConfig->GetIntPropertyByName("Frame");
	
		LOG("Master >>> 启动数据库\n");
		const char* cDBAddress	= _lpConfig->GetStrPropertyByName("DBAddress");
		unsigned int cDBPort	= _lpConfig->GetIntPropertyByName("DBPort");
		const char* cDBUser		= _lpConfig->GetStrPropertyByName("DBUser");
		const char* cDBPass		= _lpConfig->GetStrPropertyByName("DBPass");
		const char* cDBName		= _lpConfig->GetStrPropertyByName("DBName");
		const char* cDBSetName	= _lpConfig->GetStrPropertyByName("DBSetName");
		if (!StartDBCP(cDBAddress,	cDBPort,cDBUser,cDBPass,cDBName,cDBSetName)){
			LOG("Master Error >>> 启动数据库失败\n");
			break;	
		}
		LOG("Master <<< 启动数据库成功\n");

		LOG("Master >>> 初始化游戏\n");
		if (!StartGame())
		{
			LOG("Master Error >>> 初始化游戏失败\n");
			break;
		}
		LOG("Master <<< 初始化游戏成功\n");

		LOG("Master >>> 启动各处理线程\n");
		if (!StartThread())
		{
			LOG("Master Error >>> 启动各处理线程失败\n");
			break;
		}
		LOG("Master <<< 启动各处理线程成功\n");

		LOG("Master >>> 启动网络连接\n");
		//LoadIOCPConfig
		const char* cIOCPAddress	= _lpConfig->GetStrPropertyByName("IOCPAddress");
		unsigned int cIOCPPort		= _lpConfig->GetIntPropertyByName("IOCPPort");
		if (!StartIOCP(cIOCPAddress,cIOCPPort))
		{
			LOG("Master Error >>> 启动网络连接失败\n");
			break;
		}
		LOG("Master <<< 启动网络连接成功\n");
		res = true;
	} while (0);
	
	return res;
}

bool ApplicationBase::LoadConfig()
{
	bool res = true;
	
	LOG("Master >>> 读取配置文件\n");
	_lpConfig = Configuration::Configure(DEFAULT_CFG_PATH);
	if (!_lpConfig)
	{
		LOG("Master Error >>> 读取配置文件失败\n");
		res = false;
	}
	if(res) LOG("Master <<< 读取配置文件完成\n");
	return res;
}

bool ApplicationBase::StartDBCP( 
	const char* i_cDBAddress, unsigned int i_uiDBPort, 
	const char* i_cDBUser, const char* i_cDBPass, 
	const char* i_cDBName, const char* i_cDBSetName )
{
	
	do 
	{
		if (!i_cDBAddress || 
			!i_uiDBPort || 
			!i_cDBUser ||
			!i_cDBPass ||
			!i_cDBName ||
			!i_cDBSetName)
		{
			break;
		}

		sPtrDBCPDelegate = shared_ptr<DBCP>(new DBCP());
		sPtrDBCPDelegate->SetConnect(
			i_cDBAddress,
			i_uiDBPort,
			i_cDBUser,
			i_cDBPass,
			i_cDBName,
			i_cDBSetName);
		//sPtrDBCPDelegate->SetThreadCPU(2);
		sPtrDBCPDelegate->OpenConnections(10);  /*线程数*/
		DBDateManager->SetSPtrDBCP(sPtrDBCPDelegate);
		DBDateManager->InitDataBase();
		return true;
	} while (0);

	return false;
}

bool 
ApplicationBase::StartIOCP( 
				  const char* i_cIOCPAddress, 
				  unsigned int i_uiIOCPPort )
{
	if (!i_cIOCPAddress ||
		!i_uiIOCPPort)
	{
		return false;
	}
	sPtrNetWorkDelegate = shared_ptr<NetWorkDelegate>(new NetWorkDelegate(_hEventSocketRecive));
	return sPtrNetWorkDelegate->Connect(i_cIOCPAddress,i_uiIOCPPort);
}

bool 
ApplicationBase::StartThread()
{
	IntervalTimer* interval1 = new IntervalTimer();
	interval1->SetInterval(10000);
	_hashIntervalToThreadWorkJobs[interval1] = shared_ptr<IJob>(new ClearSessionThreadJob());
	HashIntervalToWorkJobs::iterator jobIt = _hashIntervalToThreadWorkJobs.begin();
	for(jobIt; jobIt != _hashIntervalToThreadWorkJobs.end();jobIt++)
	{
		shared_ptr<ThreadWorker> worker = shared_ptr<ThreadWorker>(new ThreadWorker());
		//worker->SetThreadCPU(2);
		worker->Start();
		_hashThreadWorker[jobIt->first] = worker;
		
	}
	return true;
}

bool 
ApplicationBase::StartGame()
{
	return true;
}
const char* ApplicationBase::GetStrPropertyByName(const char* _str)
{
	return _lpConfig->GetStrPropertyByName(_str);
}
unsigned int ApplicationBase::GetIntPropertyByName(const char* _str)
{
	return _lpConfig->GetIntPropertyByName(_str);
}

bool
ApplicationBase::Run()
{
		_bRunMark = true;

		//timeBeginPeriod(1);
		const DWORD frameTime = 30;
		DWORD  waitTime = frameTime;
		DWORD  lastTime = timeGetTime();
		while(_bRunMark)
		{
			if (waitTime)
			{
				WaitForSingleObject(_hEventSocketRecive,waitTime);
			}
			
			if (!_bRunMark)
			{
				break;
			}

			sPtrNetWorkDelegate->ReciveAll();
			sPtrNetWorkDelegate->SendAll();
			DWORD thisTime = timeGetTime();
			DWORD elapseTime = thisTime - lastTime; 

			//设置间隔时间 和当前主线程时间
			ApplicationBase::mainDeltaTime = elapseTime;
			ApplicationBase::mainRunLTime = GetLocalTimeSecond(time(0));

			if ( elapseTime >= frameTime )
			{
#pragma region 执行工作
				{
					HashIntervalToWorkJobs::iterator it = _hashIntervalToWorkJobs.begin();
					shared_ptr<IJob> job = NullShardPtr(IJob);
					IntervalTimer* interval = 0;

					for (it; it != _hashIntervalToWorkJobs.end(); it++)
					{
						job = it->second;
						job->Do(&elapseTime);
					}
				}
#pragma endregion 执行工作

#pragma region 执行线程工作
				{
					HashIntervalToWorkJobs::iterator it = _hashIntervalToThreadWorkJobs.begin();
					HashIntervalToWorkJobs::iterator itEnd = _hashIntervalToThreadWorkJobs.end();
					shared_ptr<IJob> job = NullShardPtr(IJob);
					IntervalTimer* interval = 0;
					unsigned int i = 0;
					shared_ptr<ThreadWorker> sPtrWorker;

					for (; it != itEnd; ++it)
					{
						interval = it->first;
						job = it->second;
						sPtrWorker =_hashThreadWorker[interval];
						interval->Update((time_t)elapseTime);
						if (sPtrWorker->GetThreadState() == IDLESSE 
							&& interval->Passed())
						{
#ifdef _DEBUG
							//printf("Job time %d Running %s\n",interval->GetCurrent(),typeid(*job).name());
#endif // _DEBUG
							sPtrWorker->SetJob(job.get(),this);

							while (interval->Passed())
							{
								interval->Reset();
							}
						}
						i++;
					}
				}
				elapseTime = timeGetTime() - lastTime;
				lastTime = thisTime;
#pragma endregion 执行线程工作
			}
			
			if (elapseTime < frameTime)
			{
				waitTime = frameTime - elapseTime;
			}
			else
			{
				waitTime = 0;
			}

			/*
			nTime=static_cast<double>(1000*(nStopCounter.QuadPart-nStartCounter.QuadPart)/nFrequency.QuadPart);
			//if(nTime<_uiFrame){
				//Sleep(static_cast<DWORD>(_uiFrame-nTime));
			//if(nTime<ApplicationBase::uiFrame){
			//	Sleep(0);
				//Sleep(static_cast<DWORD>(ApplicationBase::uiFrame-nTime));
			//	::QueryPerformanceCounter(&nStopCounter);
			//}else{
				::QueryPerformanceCounter(&nStartCounter);

#pragma region 执行工作
				{
					HashIntervalToWorkJobs::iterator it = _hashIntervalToWorkJobs.begin();
					shared_ptr<IJob> job = NullShardPtr(IJob);
					IntervalTimer* interval = 0;

					for (it; it != _hashIntervalToWorkJobs.end(); it++)
					{
						interval = it->first;
						job = it->second;
						interval->Update((time_t)nTime);
						if (interval->Passed())
						{
							job->Do(interval);
							interval->SetCurrent(0);
						}
					}
				}
#pragma endregion 执行工作

#pragma region 执行线程工作
				{
					HashIntervalToWorkJobs::iterator it = _hashIntervalToThreadWorkJobs.begin();
					shared_ptr<IJob> job = NullShardPtr(IJob);
					IntervalTimer* interval = 0;
					unsigned int i = 0;
					shared_ptr<ThreadWorker> sPtrWorker;

					for (it; it != _hashIntervalToThreadWorkJobs.end(); it++)
					{
						interval = it->first;
						job = it->second;
						sPtrWorker =_hashThreadWorker[interval];
						interval->Update((time_t)nTime);
						if (sPtrWorker->GetThreadState() == IDLESSE 
							&& interval->Passed())
						{
#ifdef _DEBUG
							//printf("Job time %d Running %s\n",interval->GetCurrent(),typeid(*job).name());
#endif // _DEBUG
							sPtrWorker->SetJob(job.get(),this);

							while (interval->Passed())
							{
								interval->Reset();
							}
						}
						i++;
					}
				}
#pragma endregion 执行线程工作
				Sleep(1);
				::QueryPerformanceCounter(&nStopCounter);
				
			//}
		}
		*/
	}
	//catch (...){};
	//timeEndPeriod(1);
	return true;
}


bool SevenSmile::GameFramework::ApplicationBase::Release()
{
	Stop();
	shared_ptr<ThreadWorker> worker;
	HashIntervalToWorker::iterator workerIt = _hashThreadWorker.begin();
	for(workerIt;workerIt != _hashThreadWorker.end();workerIt++)
	{
		worker = workerIt->second;
		worker->Close();
		worker->Join();
		printf("%s Worker Close\n",typeid(*worker).name());
	}
	_hashThreadWorker.clear();

	{
		HashIntervalToWorkJobs::iterator it = _hashIntervalToThreadWorkJobs.begin();
		for (it; it != _hashIntervalToThreadWorkJobs.end(); it++)
		{
			if( it->first ) delete it->first;
		}
		_hashIntervalToThreadWorkJobs.clear();
	}

	{
		HashIntervalToWorkJobs::iterator it = _hashIntervalToWorkJobs.begin();
		for (it; it != _hashIntervalToWorkJobs.end(); it++)
		{
			if( it->first ) delete it->first;
		}
		_hashIntervalToWorkJobs.clear();
	}

	if(sPtrNetWorkDelegate){
		sPtrNetWorkDelegate->CloseAllSession();
	}

	HttpInstance->BeginShutdown();
	return true;
}

unsigned int ApplicationBase::uiFrame		= 0;
time_t ApplicationBase::mainDeltaTime		= 0;
time_t ApplicationBase::mainRunLTime		= GetLocalTimeSecond(time(0));

shared_ptr<DBCP> ApplicationBase::sPtrDBCPDelegate;
shared_ptr<NetWorkDelegate> ApplicationBase::sPtrNetWorkDelegate;
shared_ptr<IBroadcast> SevenSmile::GameFramework::ApplicationBase::sPtrBroadcastDelegate;
Configuration*	SevenSmile::GameFramework::ApplicationBase::_lpConfig ;
