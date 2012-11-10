#include "ImplMySQLConnection.h"
#include <stdio.h>
#include "MySQLResult.h"

namespace SevenSmile{
	namespace DBCPFramework{
		namespace ImplBehaviors{
			ImplMySQLConnection::ImplMySQLConnection(WaitRes i_bEnableWaitRes)
				:_lpMysql(0)
				,_strSQL(0)
				,_lpCharIPAddr(0)
				,_intPort(0)
				,_lpCharUserName(0)
				,_lpCharPwd(0)
				,_lpCharDBName(0)
				,_strCharSetName(0)
				,_lpConnectionPool(0)
				,_bEnableWaitRes(i_bEnableWaitRes)
				,_lpResultTemp(0)
				,_lpResultFree(0)
				,_uint64NewId(0)
				,_opt(EXECUTE)
				,_bAutoReconnect(false)
				,_bNeedRes(false)
			{
				_lpResultTemp=new MySQLResult();
			}

			ImplMySQLConnection::~ImplMySQLConnection(void)
			{
				if (0!=_strSQL)
				{
					free(_strSQL);
					_strSQL=0;
				}

				if (0!=_lpResultTemp)
				{
					delete _lpResultTemp;
					_lpResultTemp=0;
				}

				this->Close();
			}

			bool ImplMySQLConnection::Connect( 
				const char* i_clpCharIPAddr ,
				int			i_intPort ,
				const char* i_clpCharUserName ,
				const char* i_clpCharPwd ,
				const char* i_clpCharDBName ,
				const char* i_strCharSetName 
				) throw(...)// throw(const char*)
			{
				_lpCharIPAddr=i_clpCharIPAddr;
				_intPort=i_intPort;
				_lpCharUserName=i_clpCharUserName;
				_lpCharPwd=i_clpCharPwd;
				_lpCharDBName=i_clpCharDBName;
				_strCharSetName=i_strCharSetName;
				//初始化
				if(mysql_library_init(0,NULL,NULL)!=0)
				{
					return false;
				}

				//初始化musql对象
				//if(mysql_init(_lpMysql)==0)
				if(0==(_lpMysql=mysql_init(0)))
				{
					return false;
				}

				//if (NULL==i_clpCharMySqlCharSetName){

				//}

				//if(mysql_options(&_mysql,MYSQL_SET_CHARSET_NAME,"utf-8")!=0)
				if(mysql_options(_lpMysql,MYSQL_SET_CHARSET_NAME,i_strCharSetName)!=0)
				{
					return false;
				}

				if(mysql_options(_lpMysql,MYSQL_OPT_RECONNECT,&_bAutoReconnect)!=0)
				{
					return false;
				}

				// 最后一个参数 CLIENT_MULTI_RESULTS ，使得调用存储过程后得到返回结果集
				if(0==mysql_real_connect(
					_lpMysql,
					i_clpCharIPAddr,
					i_clpCharUserName,
					i_clpCharPwd,
					i_clpCharDBName,
					i_intPort,
					NULL,
					CLIENT_MULTI_RESULTS
					))
				{
					int i=mysql_errno(_lpMysql);
					printf("mysql_real_connect mysql error code:%d \r\n",i);
					return false;
				}

				//mysql_real_query(_lpMysql,"SET AUTOCOMMIT=1",17); // 自动提交
				//mysql_set_server_option(_lpMysql,MYSQL_OPTION_MULTI_STATEMENTS_ON); //允许多语句支持

				return true;
			}

			bool ImplMySQLConnection::Close( void ) throw()
			{
				bool res=false;
				try
				{
					if (0!=_lpMysql)
					{
						mysql_close(_lpMysql);
						_lpMysql=0;
					}
					
					res= true;
				}
				catch (...)
				{
					printf("close fail \r\n");
					return false;
				}

				return res;
			}

			void ImplMySQLConnection::FreeRes( IResult<MYSQL_RES>* i_lpRes )
			{
				if (0==i_lpRes)
				{
					return;
				}
				mysql_free_result(i_lpRes->GetRes());
			}

			void ImplMySQLConnection::FreeResWithThreads( IResult<MYSQL_RES>* i_lpImplResult )
			{
				_opt=FREE_RES;
				_lpResultFree=i_lpImplResult;
			}

			void ImplMySQLConnection::Query( const char* i_strQuery )
			{
				_opt=EXECUTE;

				if (0!=_strSQL)
				{
					free(_strSQL);
					_strSQL=0;
				}
				_strSQL=_strdup(i_strQuery);
			}

			IResult<MYSQL_RES>* ImplMySQLConnection::Execute(
				unsigned long long* r_uint64Newid/*=0 */ 
				)
			{
				int r=mysql_query(_lpMysql,_strSQL);
				//int res=mysql_real_query(_lpMysql,_lpCharSQL,100);
				if(0==r){
					if (0!=r_uint64Newid){
						*r_uint64Newid=mysql_insert_id(_lpMysql);
					}

					MYSQL_RES* lpRes=mysql_store_result(_lpMysql);
					_lpResultTemp->SetRes(lpRes);
					_lpResultTemp->SetErrorCode(0);
					return _lpResultTemp;
				}
				else{
					r=mysql_errno(_lpMysql);
					_lpResultTemp->SetRes(0);
					_lpResultTemp->SetErrorCode(r);
#if _DEBUG
					printf("%s \r\n",_strSQL);
					printf("ExecReturnNewId error code:%d \r\n",r);
#endif // _DEBUG
					return 0;
				}
			}

			unsigned int ImplMySQLConnection::Do( void* i_lpVoidParams/*=0*/ )
			{
				if (EXECUTE==_opt)
				{
					this->Execute(&_uint64NewId);

					if (_bEnableWaitRes)	//要等待执行结果的
					{
						_lpConnectionPool->GetCondition()->Signal();
					}
					else{	//不需要等待执行结果的
						if (_bNeedRes)
						{
							_lpResultTemp->Signal();	//返回结果集，不需要结果集的查询是不需要这行的
						}
					}
				}
				else if (FREE_RES==_opt && 0 != _lpResultFree)
				{
					this->FreeRes(_lpResultFree);
					_lpResultFree->Signal();
				}

				//这里执行完了要告诉外面，然后把自己从空闲列表中移除
				if (0==_lpConnectionPool)
				{
					return 0;
				}
				DoubleList<IDBConnection<MYSQL_RES>>* dl=_lpConnectionPool->GetDBConnections();
				dl->MoveEleFromBusyToIdle(this);
				return 0;
			}

			void ImplMySQLConnection::SetConnectionPool( IConnectionPool<MYSQL_RES>* i_lpImplConnectionPool )
			{
				_lpConnectionPool=i_lpImplConnectionPool;
			}

			void ImplMySQLConnection::SetAutoReconnect( bool i_bAutoReconnect )
			{
				_bAutoReconnect=i_bAutoReconnect;
			}

		}
	}
}