#ifndef DBCP_H
#define DBCP_H
#include "_Util_Define.h"
#include "_DBCP_DEFINE.h"
#include "winsock.h"
#include "mysql.h"

#include <vector>
#include "IDBConnection.h"
#include "ThreadPool.h"
#include "DoubleList_.h"
#include "ThreadWorker.h"
#include "MySQLResult.h"

#pragma warning(push)
#pragma warning(disable:4251)
namespace SevenSmile{
	namespace DBCPFramework{
		using namespace std;
		using namespace SevenSmile::DBCPFramework::Interface;
		//using namespace SevenSmile::DBCPFramework::ImplBehaviors;
		using namespace SevenSmile::Util::ThreadHelper;

		//typedef MySQLResult Result;
		typedef IResult<MYSQL_RES> Result;

		class DBCP_DLL_DECL DBCP
			:public IConnectionPool<MYSQL_RES>
		{
		public:
			DBCP();
			~DBCP();

			void SetConnect(
				const char* i_clpCharIPAddr,
				int			i_intPort,
				const char* i_clpCharUserName,
				const char* i_clpCharPwd,
				const char* i_clpCharDBName,
				const char* i_cStrCharSetName
				);

			//�򿪶��ٴ����߳�
			void OpenConnections(unsigned int i_intThreadNum=10);

			//������������д���ݣ����߶�����,
			//���ؽ��
			//ע�⣺��ѯ���Ҫ�ͷš�
			IResult<MYSQL_RES>* QueryUseAllConnsWithRes(
				const char* i_strSql,
				unsigned long long* r_uint64NewId=0
				);

			//�����������ȴ����
			void QueryUseAllConnsWithoutRes(const char* i_strSql);

			//˳��ִ�У�һ������˳�������ݻ���ִ������
			//ע�⣺��ѯ���Ҫ�ͷš�
			IResult<MYSQL_RES>* Query(
				const char* i_strSql,
				unsigned long long* r_uint64NewId=0
				);

			//˳��ִ�У��޷���ֵ��
			void QueryWithoutRes(const char* i_strSql);

			//�������Ӷ�Ҫִ�е���䣬����set name
			void QueryWithAllIdleConns(
				const char* i_strSql,
				unsigned long long* r_uint64NewId=0
				);

			void FreeResult(IResult<MYSQL_RES>* &i_lpRes);

			virtual DoubleList<IDBConnection<MYSQL_RES>>*	GetDBConnections();
			virtual Condition*								GetCondition(void);

			void SetThreadCPU(int i_intCpuMask=0);
			void SetAutoReconnect(bool val);
		private:
			const char*		_lpCharIPAddr;
			int				_intPort;
			const char*		_lpCharUserName;
			const char*		_lpCharPwd;
			const char*		_lpCharDBName;
			const char*		_strCharSetName;

			unsigned int	_uintThreadsNum;

			DoubleList<IDBConnection<MYSQL_RES>> _dlImplDBConnections;
			//DoubleList<IDBConnection<MYSQL_RES>> _dlImplDBConnsForFree;
			
			ThreadPool		_tp;

			IDBConnection<MYSQL_RES>*	_lpImplDBConn;

			Condition		_conJobs;
			Mutex			_mutex;

			int				_intCpuMask;
			bool			_bAutoReconnect;
			
			bool			InitConnections(void);
			bool			InitConns(DoubleList<IDBConnection<MYSQL_RES>>& i_dl);
			bool			InitConnsForFree(void);
			bool			InitThreads(void);
		};
	}
}
#pragma warning(pop)
#endif