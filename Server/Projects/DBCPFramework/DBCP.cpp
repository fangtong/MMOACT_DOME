#include "DBCP.h"
#include "ImplMySQLConnection.h"
#include "ImplMySQLOneConn.h"
#include "CheckMemoryLeak.h"

namespace SevenSmile{
	namespace DBCPFramework{
		using namespace SevenSmile::DBCPFramework::ImplBehaviors;
		using namespace SevenSmile::Util::Container;

		DBCP::DBCP()
			:_lpCharIPAddr(0)
			,_lpCharDBName(0)
			,_lpCharUserName(0)
			,_lpCharPwd(0)
			,_strCharSetName(0)
			,_lpImplDBConn(0)
			,_intCpuMask(0)
			,_bAutoReconnect(true)
		{
		}

		DBCP::~DBCP()
		{
			_tp.TerminateAll();
			//for_each(_vecImplDBConnections.begin(),_vecImplDBConnections.end(),&DBCP::Disconnection);
			//_dlImplDBConnections.Clear(&DBCP::ClearDoubleQueue);

			if (0!=_lpImplDBConn)
			{
				delete _lpImplDBConn;
				_lpImplDBConn=0;
			}
		}

		void DBCP::SetConnect( 
			const char* i_clpCharIPAddr, 
			int			i_intPort, 
			const char* i_clpCharUserName, 
			const char* i_clpCharPwd, 
			const char* i_clpCharDBName, 
			const char* i_cStrCharSetName 
			)
		{
			_lpCharIPAddr=i_clpCharIPAddr;
			_intPort=i_intPort;
			_lpCharUserName=i_clpCharUserName;
			_lpCharPwd=i_clpCharPwd;
			_lpCharDBName=i_clpCharDBName;
			_strCharSetName=i_cStrCharSetName;
		}

		void DBCP::OpenConnections( unsigned int i_intThreadNum/*=10*/ )
		{
			_uintThreadsNum=i_intThreadNum;

			if (InitConnections())
			{
				InitThreads();
			}
		}

		bool DBCP::InitConnections( void )
		{
			bool bRes=false;
			bRes = InitConns(_dlImplDBConnections);
			//bRes &=InitConns(_dlImplDBConnsForFree);

			//初始化单线程
			_lpImplDBConn=new ImplMySQLOneConn(ENABLE_WAIT_RES);
			_lpImplDBConn->SetAutoReconnect(_bAutoReconnect);
			if(_lpImplDBConn->Connect(
				this->_lpCharIPAddr,
				this->_intPort,
				this->_lpCharUserName,
				this->_lpCharPwd,
				this->_lpCharDBName,
				this->_strCharSetName
				))
			{
				_lpImplDBConn->SetConnectionPool(this);
				printf(".");
			}
			else{
				printf("x");
			}

			printf(" completed. \r\n");

			return bRes;
		}


		bool DBCP::InitConns( DoubleList<IDBConnection<MYSQL_RES>>& i_dl )
		{
			bool bRes=true;
			printf("Open DB connections:");

			for (size_t i=0;i<_uintThreadsNum;i++)
			{
				IDBConnection<MYSQL_RES>* lpImplDBConnection=new ImplMySQLConnection();
				lpImplDBConnection->SetAutoReconnect(_bAutoReconnect);
				if(lpImplDBConnection->Connect(
					this->_lpCharIPAddr,
					this->_intPort,
					this->_lpCharUserName,
					this->_lpCharPwd,
					this->_lpCharDBName,
					this->_strCharSetName
					))
				{
					lpImplDBConnection->SetConnectionPool(this);
					//_dlImplDBConnections.AppendToIdleList(lpImplDBConnection);
					i_dl.AppendToIdle(lpImplDBConnection);
					printf(".");
					bRes&=true;
				}
				else{
					bRes&=false;
					printf("x");
				}
			}

			return bRes;
		}

		bool DBCP::InitThreads( void )
		{
			_tp.SetThreadCPU(_intCpuMask);
			_tp.SetInitThreadNum(_uintThreadsNum);
			_tp.Start();
			return true;
		}

		IResult<MYSQL_RES>* DBCP::QueryUseAllConnsWithRes( 
			const char* i_strSql, 
			unsigned long long* r_uint64NewId/*=0 */ 
			)
		{
			{
				MutexLock<Mutex> lock(_mutex);

				//从空闲列表中取出一个数据库连接
				IDBConnection<MYSQL_RES>* lpImplDBConnection
					=_dlImplDBConnections.GetEle_MoveEleFromIdleToBusy();

				//设置用此连接执行的语句
				lpImplDBConnection->Query(i_strSql);
				lpImplDBConnection->NeedResult(true);
				//执行
				//这里是不等待结果的
				//sql执行完成后，把当前使用的数据库连接放回空闲列表
				_tp.Run(lpImplDBConnection);

				IResult<MYSQL_RES>* lpRes=lpImplDBConnection->GetResult();
				lpRes->Wait();

				if (0!=r_uint64NewId)
				{
					*r_uint64NewId=lpImplDBConnection->GetNewId();
				}

				IResult<MYSQL_RES>* lpRes2=new MySQLResult();
				lpRes2->SetRes(lpRes->GetRes());
				return lpRes2;
			}
		}

		void DBCP::QueryUseAllConnsWithoutRes(const char* i_strSql)
		{
			{
				MutexLock<Mutex> lock(_mutex);

				//从空闲列表中取出一个数据库连接
				IDBConnection<MYSQL_RES>* lpImplDBConnection
					=_dlImplDBConnections.GetEle_MoveEleFromIdleToBusy();

				//设置用此连接执行的语句
				lpImplDBConnection->Query(i_strSql);
				lpImplDBConnection->NeedResult(false);
				//执行
				//这里是不等待结果的
				//sql执行完成后，把当前使用的数据库连接放回空闲列表
				_tp.Run(lpImplDBConnection);
			}
		}

		IResult<MYSQL_RES>* DBCP::Query( 
			const char* i_strSql, 
			unsigned long long* r_uint64NewId/*=0 */ 
			)
		{
			{
				MutexLock<Mutex> lock(_mutex);
				_lpImplDBConn->Query(i_strSql);
				_lpImplDBConn->NeedResult(true);
				//_tw.SetJob(_lpImplDBConn);
				_tp.Run(_lpImplDBConn);
				_conJobs.Wait();
				//return _lpImplDBConn->GetResult();

				IResult<MYSQL_RES>* lpRes=_lpImplDBConn->GetResult();

				if (0!=r_uint64NewId)
				{
					*r_uint64NewId=_lpImplDBConn->GetNewId();
				}
				
				IResult<MYSQL_RES>* lpRes2=new MySQLResult();
				lpRes2->SetRes(lpRes->GetRes());
				return lpRes2;
			}
		}

		void DBCP::QueryWithoutRes( const char* i_strSql )
		{
			{
				MutexLock<Mutex> lock(_mutex);
				_lpImplDBConn->Query(i_strSql);
				_lpImplDBConn->NeedResult(false);
				//_tw.SetJob(_lpImplDBConn);
				_tp.Run(_lpImplDBConn);
				_conJobs.Wait();
				IResult<MYSQL_RES>* lpRes=_lpImplDBConn->GetResult();
				_lpImplDBConn->FreeRes(lpRes);
			}
		}

		void DBCP::QueryWithAllIdleConns( const char* i_strSql, unsigned long long* r_uint64NewId/*=0 */ )
		{
			////这里要让所有空闲连接都执行一遍sql语句；
			//LIST_T* lpList= _dlImplDBConnections.GetIdleList();
			//ITERATOR_T it=lpList->begin();
			//for (;it!=lpList->end();it++)
			//{
			//	IDBConnection<MYSQL_RES>* lpRes=*it;
			//	lpRes->Query(i_strSql);
			//	lpRes->Execute(r_uint64NewId);
			//}
		}

		void DBCP::FreeResult( IResult<MYSQL_RES>* &i_lpRes )
		{
			if(0 == i_lpRes) return; 
			{
				MutexLock<Mutex> lock(_mutex);
				//从空闲列表中取出一个数据库连接
				IDBConnection<MYSQL_RES>* lpImplDBConnection
					=_dlImplDBConnections.GetEle_MoveEleFromIdleToBusy();
				//IDBConnection<MYSQL_RES>* lpImplDBConnection
				//	=_dlImplDBConnsForFree.GetEle_MoveEleFromIdleToBusy();
				lpImplDBConnection->FreeResWithThreads(i_lpRes);
				_tp.Run(lpImplDBConnection);

				if (0!=i_lpRes && lpImplDBConnection->GetResult()!=i_lpRes)
				{
					i_lpRes->Wait();

					delete i_lpRes;
					i_lpRes=0;
				}
			}
		}

		void DBCP::SetThreadCPU( int i_intCpuMask )
		{
			_intCpuMask=i_intCpuMask;
		}

		void DBCP::SetAutoReconnect( bool val )
		{
			_bAutoReconnect = val;
		}

		DoubleList<IDBConnection<MYSQL_RES>>* DBCP::GetDBConnections()
		{
			return &_dlImplDBConnections;
		}

		Condition* DBCP::GetCondition( void )
		{
			return &_conJobs;
		}

		//void DBCP::ClearDoubleQueue( IDBConnection<MYSQL_RES>* i_lpConnection )
		//{
		//	if (0!=i_lpConnection)
		//	{
		//		delete i_lpConnection;
		//		i_lpConnection=0;
		//	}
		//}
	}
}