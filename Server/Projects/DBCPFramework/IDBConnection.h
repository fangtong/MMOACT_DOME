#ifndef DBCP_IB_CONNECTION_H
#define DBCP_IB_CONNECTION_H
#include "IResult.h"
#include "IRunnable.h"
#include "DoubleList_.h"

namespace SevenSmile{
	namespace DBCPFramework{
		namespace Interface{
			using namespace SevenSmile::Util::ThreadHelper;
			using namespace SevenSmile::Util::Container;

			enum WaitRes{
				DISABLE_WAIT_RES=0,
				ENABLE_WAIT_RES=1
			};

			enum OPERATIONS{
				EXECUTE=0,
				FREE_RES=1
			};

			template<typename T>
			class IConnectionPool;

			template<typename T>
			class IDBConnection
				:public IJob
			{
			public:
				IDBConnection(void){}
				virtual ~IDBConnection(void){}

				virtual bool				Connect
												(const char* i_clpCharIPAddr
												,int i_intPort
												,const char* i_clpCharUserName
												,const char* i_clpCharPwd
												,const char* i_clpCharDBName
												,const char* i_cStrCharSetName
												)=0;
				virtual void				Query(const char* i_clpCharQueryStr)=0;
				virtual IResult<T>*			Execute(unsigned long long* i_lpInt64Newid=0)=0;
				virtual bool				Close(void)=0;
				virtual void				FreeRes(IResult<T>* i_lpImplResult)=0;
				virtual void				FreeResWithThreads(IResult<T>* i_lpImplResult)=0;

				virtual void				SetConnectionPool(
												IConnectionPool<T>* i_lpImplConnectionPool
												)=0;
				//virtual Condition*			GetCondition(void)=0;
				virtual IResult<T>*			GetResult(void)=0;
				virtual unsigned long long	GetNewId(void)=0;
				virtual	void				SetAutoReconnect(bool i_bAutoReconnect)=0;
				//virtual int				GetErrorCode()=0;
				//virtual bool				GetResultRowCount(void)=0;

				//是否需要返回结果
				virtual void				NeedResult(bool i_b)=0;
			};

			template<typename T>
			class IConnectionPool{
			public:
				virtual DoubleList<IDBConnection<T>>*	GetDBConnections()=0;
				virtual Condition*						GetCondition(void)=0;
			};
		}
	}
}
#endif