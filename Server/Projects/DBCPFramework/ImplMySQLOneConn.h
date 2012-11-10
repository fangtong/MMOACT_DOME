#ifndef DBCP_IMPL_MYSQL_ONE_CONN_H
#define DBCP_IMPL_MYSQL_ONE_CONN_H
#include "winsock.h"
#include "mysql.h"
#include "Condition.h"
#include "IDBConnection.h"

namespace SevenSmile{
	namespace DBCPFramework{
		namespace ImplBehaviors{
			using namespace SevenSmile::DBCPFramework::Interface;
			using namespace SevenSmile::DBCPFramework::ImplBehaviors;
			using namespace SevenSmile::Util::ThreadHelper;

			class ImplMySQLOneConn
				:public IDBConnection<MYSQL_RES>
			{
			public:
				ImplMySQLOneConn(WaitRes i_bEnableWaitRes=DISABLE_WAIT_RES);
				~ImplMySQLOneConn(void);

				virtual bool				Connect(
												const char* i_clpCharIPAddr,
												int i_intPort,
												const char* i_clpCharUserName,
												const char* i_clpCharPwd,
												const char* i_clpCharDBName,
												const char* i_strCharSetName
												) throw(...);//throw(const char*);
				virtual void				Query(const char* i_strQuery);
				virtual IResult<MYSQL_RES>* Execute(unsigned long long*	r_uint64Newid=0);
				virtual bool				Close(void)throw();
				virtual void				FreeRes(IResult<MYSQL_RES>* i_lpRes);
				virtual void				FreeResWithThreads(IResult<MYSQL_RES>* i_lpImplResult);

				virtual unsigned int		Do(void* i_lpVoid=0);

				void						SetConnectionPool(IConnectionPool<MYSQL_RES>* i_lpImplConnectionPool);

				//virtual Condition*			GetCondition()	{ return &_condJob; }
				virtual IResult<MYSQL_RES>*	GetResult(void)	{ return _lpResultTemp; }
				virtual unsigned long long	GetNewId(void)	{ return _uint64NewId; }

				bool BAutoReconnect() const { return _bAutoReconnect; }
				void BAutoReconnect(bool val) { _bAutoReconnect = val; }

				virtual	void				SetAutoReconnect(bool i_bAutoReconnect);

				//是否需要返回结果
				virtual void				NeedResult(bool i_b=false){_bNeedRes=i_b;}
			private:
				char*						_strSQL;

				const char*					_lpCharIPAddr;
				int							_intPort;
				const char*					_lpCharUserName;
				const char*					_lpCharPwd;
				const char*					_lpCharDBName;
				const char*					_strCharSetName;

				MYSQL*						_lpMysql;

				WaitRes						_bEnableWaitRes;		//是否等待结果
				//Condition					_condJob;

				IResult<MYSQL_RES>*			_lpResultTemp;
				unsigned long long			_uint64NewId;
				IResult<MYSQL_RES>*			_lpResultFree;

				IConnectionPool<MYSQL_RES>* _lpConnectionPool;

				OPERATIONS					_opt;
				bool						_bAutoReconnect;
				bool						_bNeedRes;
			};

		}
	}
}
#endif