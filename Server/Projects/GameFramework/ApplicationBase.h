#pragma once
#include "ThreadPool.h"
#include <memory>
#include "Timer.h"
#include <hash_map>
#include <vector>


namespace SevenSmile{
	namespace Util{
		namespace DesignPatterns
		{
			class IBroadcast;
		}
		namespace ThreadHelper
		{
			class ThreadWorker;
			class IJob;
		}
		namespace Config{
			class Configuration;
		}
		class IntervalTimer;
	}
	namespace DBCPFramework
	{
		class DBCP;
	};
	namespace Net{
		namespace IOCPFramework
		{
			class IOCP;
		}
	}
	namespace GameFramework
	{
		class NetWorkDelegate;
	};
}

using namespace SevenSmile::Net;
using namespace SevenSmile::Net::IOCPFramework;
using namespace SevenSmile::DBCPFramework;
using namespace SevenSmile::GameFramework;
using namespace SevenSmile::Util::Config;
using namespace SevenSmile::Util::ThreadHelper;
using namespace SevenSmile::Util;
using namespace SevenSmile::Util::DesignPatterns;
using namespace std;
using namespace std::tr1;
using namespace stdext;

namespace SevenSmile
{
	namespace GameFramework
	{
		class ApplicationBase
		{
		public:
			typedef stdext::hash_map<IntervalTimer*,shared_ptr<IJob>> HashIntervalToWorkJobs;
			typedef stdext::hash_map<IntervalTimer*,shared_ptr<ThreadWorker>> HashIntervalToWorker;
			ApplicationBase(void);
			virtual ~ApplicationBase(void);

		public:
			bool Start();
			bool Run();
			void Stop() {_bRunMark = false;};
			virtual bool Release();
			
			static const char* GetStrPropertyByName(const char* _str);
			static unsigned int GetIntPropertyByName(const char* _str);
		protected:
			virtual bool StartDBCP(
				const char*		i_cDBAddress,
				unsigned int	i_uiDBPort,
				const char*		i_cDBUser,
				const char*		i_cDBPass,
				const char*		i_cDBName,
				const char*		i_cDBSetName);
			virtual bool StartIOCP(
				const char*		i_cIOCPAddress,
				unsigned int	i_uiIOCPPort);
			virtual bool StartThread();
			virtual bool StartGame();
			virtual bool LoadConfig();
		protected:
			HashIntervalToWorker			_hashThreadWorker;								//�߳� ���� ӳ�����
			static Configuration*				_lpConfig ;											//�����ļ�
			bool										_bRunMark;											//����״̬
			
			HashIntervalToWorkJobs		_hashIntervalToThreadWorkJobs;			//���̹߳�������
			HashIntervalToWorkJobs		_hashIntervalToWorkJobs;						//���̹߳�������
			HANDLE								_hEventSocketRecive;							

		public:
			static unsigned int						uiFrame;
			static time_t									mainDeltaTime;								//��ѭ��ʱ���
			static time_t									mainRunLTime;									//��ѭ������ʱ��

			static shared_ptr<DBCP>				sPtrDBCPDelegate;
			static shared_ptr<NetWorkDelegate>	sPtrNetWorkDelegate;
			static shared_ptr<IBroadcast>		sPtrBroadcastDelegate;
		};
	}
}



