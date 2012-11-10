#include "BehaviorCreateMainThread.h"
#include "Exception.h"

using namespace SevenSmile::Util::Debug;
namespace SevenSmile{
	namespace Net{
		namespace IOCPFramework{
			namespace Behavior{
				BehaviorCreateMainThread::BehaviorCreateMainThread(void)
					:_hMainThread(0)
					,_initRes(0)
					,_bDestroy(false)
				{
					//this->_lpBaseBehaviorMsgDeal=0;
					//this->_lpIBMsgDealManage=0;

					//_hMainThread=NULL;
					//_dwMainThreadId=NULL;
					//_hEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
					//_initRes=0;

					Init();
					strcpy_s(ADDR,"127.0.0.1");
					PORT=5555;
					_cpuMask=0;

					SYSTEM_INFO	sys_info;
					GetSystemInfo( &sys_info );
					_intMaxExecuteThreadNum = 
						sys_info.dwNumberOfProcessors > MAXTHREAD_COUNT ? MAXTHREAD_COUNT : sys_info.dwNumberOfProcessors;
					//_intPostNum=POST_NUMBER;

					//创建主线程
					_hMainThread = CreateThread(
						NULL,
						0,
						&ServerRoutine,
						(LPVOID)this,
						CREATE_SUSPENDED,
						//&_dwMainThreadId
						NULL
						);
				}

				BehaviorCreateMainThread::BehaviorCreateMainThread(
					char* i_lpCharIpAddr,
					int i_intPort,
					int i_intExecuteThreadNum
					):_bDestroy(false)
				{
					Init();

					strcpy_s(ADDR,i_lpCharIpAddr);
					PORT =i_intPort;
					_intMaxExecuteThreadNum=i_intExecuteThreadNum;

					//创建主线程
					_hMainThread = CreateThread(
						NULL, 
						0, 
						&ServerRoutine, 
						(LPVOID)this, 
						CREATE_SUSPENDED,
						//&_dwMainThreadId
						NULL
						);
				}

				void BehaviorCreateMainThread::Init(void)
				{
					_lpBaseBehaviorMsgDeal	=0;
					_lpMainListenIOCPKey	=0;
					_lpBehaviorMsgSend		=0;
					_lpIBMsgDealManage		=0;
				}

				BehaviorCreateMainThread::~BehaviorCreateMainThread(void)
				{
					Close();
					if (this->_initRes<=4)
					{
						delete[] _hArrThread;
						_hArrThread=NULL;
					}

					if (this->_initRes==0)
					{
						TerminateThread(_hMainThread,NULL);
					}

					if (NULL!=_lpMainListenIOCPKey){
						delete _lpMainListenIOCPKey;
						_lpMainListenIOCPKey=NULL;
					}

					if (NULL!=_lpBehaviorMsgSend){
						delete _lpBehaviorMsgSend;
						_lpBehaviorMsgSend=NULL;
					}
				}

				int BehaviorCreateMainThread::Close()
				{
					if(_bDestroy) return 0;
					_bDestroy = true;
					closesocket( _soketMainListenSoket );

					do
					{
						Lock lock(*this);
						if(_intMaxExecuteThreadNum<= 0) break;
						PostQueuedCompletionStatus( _hIOCP, 0, NULL, NULL );
					}while(_condCompletionThreadClose.Wait());

					//关闭完成端口处理线程
					//PostQueuedCompletionStatus( _hIOCP, 0, NULL, NULL );
					CloseHandle( _hIOCP );

					////关闭线程，这里要换用让线程退出的方法。
					//if(NULL!=this->_hArrThread && this->_initRes>4){
					//	for (int i=0;i<this->_intMaxExecuteThreadNum;i++)
					//	{
					//		if(NULL!=_hArrThread[i]){
					//			TerminateThread(_hArrThread[i],NULL);
					//			CloseHandle(_hArrThread[i]);
					//		}
					//	}
					//}
					return 1;
				}

				void BehaviorCreateMainThread::SetListenSoketIPAddr(const char* i_lpChar){
					strcpy_s(ADDR,i_lpChar);
				}

				void BehaviorCreateMainThread::SetListenSoketPort(int i_intPort){
					//strcpy_s(ADDR,i_lpChar);
					PORT=i_intPort;
				}
				void BehaviorCreateMainThread::SetExecuteThreadNum(int i_intThreadNum){
					this->_intMaxExecuteThreadNum=i_intThreadNum;
				}

				//void SetPostNum(int i_intPostNum);

				/*-------------------------------------------------------------------------------------------
				函数功能：主循环
				函数说明：
				函数返回：成功，TRUE；失败，FALSE
				-------------------------------------------------------------------------------------------*/
				bool BehaviorCreateMainThread::MainLoop(void)
				{
					DWORD	dwRet;

					while(1)
					{

						dwRet = WaitForSingleObject(_hAcceptEvent,1000);
						if(_bDestroy) break;
						switch( dwRet )
						{
						case WAIT_FAILED:
							{
								PostQueuedCompletionStatus(_hIOCP,0,0,NULL);
								return FALSE;
							}
							break;
						case WAIT_TIMEOUT:
							{
								//CheckForInvalidConnection();
							}
							break;
						case WAIT_OBJECT_0:	//接收到了所有发出的连接都用光了的消息，再次发出连接
							{

								if(  !PostAcceptEx() )
								{
									PostQueuedCompletionStatus(_hIOCP,0,0,NULL);
									return FALSE;
								}
							}
							break;
						}

					}

					return TRUE;
				}

				DWORD BehaviorCreateMainThread::ServerRoutine(LPVOID lp){
					//这里运行主线程初始化工作
					BehaviorCreateMainThread* lpBehaviorCreateMainThread=(BehaviorCreateMainThread*) lp;
					//lpBehaviorCreateMainThread->_initRes = lpBehaviorCreateMainThread->InitIOCP();
					//SetEvent(lpBehaviorCreateMainThread->_hEvent);
					//if(lpBehaviorCreateMainThread->_initRes==0){
						lpBehaviorCreateMainThread->MainLoop();
					//}
					return 0;
				}

				int BehaviorCreateMainThread::Start(int i_intStartSeconds){
					if (0!=i_intStartSeconds)
					{
						Sleep(i_intStartSeconds*1000);
					}

					this->_initRes = this->InitIOCP();

					if(this->_initRes==0){
						ResumeThread(_hMainThread);
					}
					//WaitForSingleObject(_hEvent,INFINITE);
					return _initRes;
				}

				int BehaviorCreateMainThread::InitIOCP(){
					WSAData data;
					if(WSAStartup( MAKEWORD(2,2),&data)){
						//throw "WSAStartup fail!";
						return 1;
					}

					_lpBehaviorMsgSend=new BaseBehavior::BaseBehaviorMsgSend();
					this->_lpBaseBehaviorMsgDeal->SetBehaviorMsgSend(this->_lpBehaviorMsgSend);

					_hIOCP=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,0);
					if( NULL == _hIOCP ){
						return 2;
					}

					if( !InitSocket() ){
						PostQueuedCompletionStatus( _hIOCP, 0, NULL, NULL );
						//AddMessage("Init sociket fail!");
						CloseHandle( _hIOCP );
						return 3;
					}

					//启动处理线程
					if( !StartThread() ){
						//AddMessage("start thread fail!");
						PostQueuedCompletionStatus( _hIOCP, 0, NULL, NULL );
						CloseHandle( _hIOCP );
						return 4;
					}

					if( !BindAndListenSocket() ){
						PostQueuedCompletionStatus( _hIOCP, 0, NULL, NULL );
						CloseHandle( _hIOCP );
						closesocket( _soketMainListenSoket );
						return 5;
					}


					//函数指针化
					if( !GetFunPointer() ){
						//cout<<"GetFunPointer fail!"<<endl;
						PostQueuedCompletionStatus( _hIOCP, 0, NULL, NULL );
						CloseHandle( _hIOCP );
						closesocket( _soketMainListenSoket );
						return 6;
					}

					//发送acceptEx
					if( !PostAcceptEx() ){
						PostQueuedCompletionStatus( _hIOCP, 0, NULL, NULL );
						//cout<<"PostAcceptEx fail!"<<endl;
						CloseHandle( _hIOCP );
						closesocket( _soketMainListenSoket );
						return 7;
					}

					//注册事件
					if( !RegAcceptEvent() ){
						PostQueuedCompletionStatus( _hIOCP, 0, NULL, NULL );
						//cout<<"RegAcceptEvent fail!"<<endl;
						CloseHandle( _hIOCP );
						closesocket( _soketMainListenSoket );
						return 8;
					}

					return 0;
				}

				/*-------------------------------------------------------------------------------------------
				函数功能：初始化侦听SOCKET端口，并和完成端口连接起来。
				函数说明：
				函数返回：成功，TRUE；失败，FALSE
				-------------------------------------------------------------------------------------------*/
				bool BehaviorCreateMainThread::InitSocket(){
					_soketMainListenSoket = WSASocket(
						AF_INET,
						SOCK_STREAM,
						IPPROTO_TCP,
						NULL,
						0,
						WSA_FLAG_OVERLAPPED
						);

					if( INVALID_SOCKET == _soketMainListenSoket ){
						return FALSE;
					}

					//IOCP_KEY_PTR  lp_key = m_key_group.GetBlank();

					_lpMainListenIOCPKey = new IOCP_KEY;
					_lpMainListenIOCPKey->socket = _soketMainListenSoket;
					
					setsockopt(_soketMainListenSoket,IPPROTO_TCP,TCP_NODELAY,&chOpt,sizeof(char));

					HANDLE hRet = CreateIoCompletionPort(
						(HANDLE)_soketMainListenSoket,
						_hIOCP,
						(DWORD)_lpMainListenIOCPKey,
						0
						);

					if(hRet == NULL){
						closesocket( _soketMainListenSoket );
						//m_key_group.RemoveAt( lp_key );
						return FALSE;
					}

					return TRUE;
				}

				/*-------------------------------------------------------------------------------------------
				函数功能：将侦听端口和自己的IP，PORT绑定，并开始侦听
				函数说明：
				函数返回：成功，TRUE；失败，FALSE
				-------------------------------------------------------------------------------------------*/
				bool BehaviorCreateMainThread::BindAndListenSocket(){
					SOCKADDR_IN addr;
					memset( &addr, 0, sizeof(SOCKADDR_IN) );
					addr.sin_family			= AF_INET;
					addr.sin_addr.s_addr	= inet_addr( ADDR );
					addr.sin_port			= htons( PORT );
					int nRet;
					nRet = bind( _soketMainListenSoket, (SOCKADDR*)&addr, sizeof( SOCKADDR ) );
					if( SOCKET_ERROR == nRet ){
						return FALSE;
					}
					nRet = listen( _soketMainListenSoket, 20 );
					if( SOCKET_ERROR == nRet ){
						return FALSE;
					}

					return TRUE;
				}

				void BehaviorCreateMainThread::SetThreadCPU(int _mask){
					this->_cpuMask=_mask;
				}
				/*-------------------------------------------------------------------------------------------
				函数功能：根据CPU的数目，启动相应数量的数据处理线程
				函数说明：
				函数返回：成功，TRUE；失败，FALSE
				-------------------------------------------------------------------------------------------*/
				bool BehaviorCreateMainThread::StartThread()
				{
					int i;

					//SYSTEM_INFO	sys_info;
					//GetSystemInfo( &sys_info );
					//int intThreadCount = sys_info.dwNumberOfProcessors > MAXTHREAD_COUNT ? MAXTHREAD_COUNT : sys_info.dwNumberOfProcessors;
					int intThreadCount = this->_intMaxExecuteThreadNum;
					_hArrThread=new HANDLE[intThreadCount];
					//_dwArrThreadIds=new DWORD[intThreadCount];

					for( i = 0; i < intThreadCount; i++ )
					{
						_hArrThread[i] = CreateThread(
							NULL,
							0,
							CompletionRoutine,
							(LPVOID)this,
							0,
							//&(_dwArrThreadIdsl[i])
							NULL
							);
						
						if(_cpuMask!=0){
							SetThreadAffinityMask(_hArrThread[i],_cpuMask);
						}

						if( NULL == _hArrThread[i] ){
							CloseThreadHandle( i );
							CloseHandle( _hIOCP );
							return FALSE;
						}
					}
					return TRUE;
				}

				//执行线程的回调函数
				DWORD BehaviorCreateMainThread::CompletionRoutine(LPVOID lp_param){
					BehaviorCreateMainThread* lp_this = (BehaviorCreateMainThread*)lp_param;

					int				nRet;
					BOOL			bRet;
					DWORD			dwBytes			= 0;
					HANDLE			hRet;

					IOCP_KEY*		lp_key			= NULL;
					IOCP_IO*		lp_io			= NULL;
					LPWSAOVERLAPPED	lp_ov			= NULL;


					IOCP_KEY*		lp_new_key		= NULL;
					
					while(1)
					{
						if(lp_this->_bDestroy) break;

						bRet = GetQueuedCompletionStatus(
							lp_this->_hIOCP,
							&dwBytes,
							//(LPDWORD)&lp_key,
							(PULONG_PTR)&lp_key,
							&lp_ov,
							INFINITE
							);

						lp_io	= (IOCP_IO*)lp_ov;
						
						//关闭线程
						if(0 == lp_key && 0 == lp_io){ 
							break; 
						}
						if(0 == lp_key){
							printf("lp_key Is Null!\n");
							continue;
						}

						if(0 == lp_io){
							//cout<<"recv a null CIoContext!"<<endl;
							continue;
						}

						if( false == bRet )
						{
							//cout << "GetQueuedCompletionStatus() failed: " << GetLastError() << endl;
							printf(
								"File:%s\nFunction:%s\nLine:%d\nErrorCode:%d\n",
								__FILE__,
								__FUNCTION__,
								__LINE__,
								GetLastError()
								);
							//lp_io->error=false;

							DWORD errorCode = GetLastError();
							//程序退出时IO遗失
							if(ERROR_OPERATION_ABORTED == errorCode 
							&& lp_this->_bDestroy)
							{
								break;
							}
							if(lp_this->_lpBaseBehaviorMsgDeal)
							lp_this->_lpBaseBehaviorMsgDeal->MsgQuit(lp_io);

							//正常退出
							closesocket( lp_io->socket );

							//lp_this->Remove(lp_io,lp_key);
							if (lp_io->operation==IOCP_WRITE)
							{
								if(lp_this->_lpBehaviorMsgSend)
								lp_this->_lpBehaviorMsgSend->Remove(lp_io);
							}
							else{
								lp_this->Remove(lp_io,lp_key);
							}

							continue;
						}

						//客户端主动退出
						if	(IOCP_ACCEPT != lp_io->operation && 0 == dwBytes)
						{
							//printf("GetQueuedCompletionStatus() failed:%d\n",GetLastError());
#ifdef _DEBUG
							printf("Secket:%d 客户端主动退出\n",lp_io->socket);
#endif // _DEBUG
							
							lp_this->_lpBaseBehaviorMsgDeal->MsgQuit(lp_io);

							//正常退出
							closesocket( lp_io->socket );
							//lp_io->error=false;

							if (lp_io->operation==IOCP_WRITE)
							{
								lp_this->_lpBehaviorMsgSend->Remove(lp_io);
							}
							else{
								lp_this->Remove(lp_io,lp_key);
							}
							
							//lp_this->_lpBaseBehaviorMsgDeal->MsgQuit(lp_io->socket);
							

							continue;
						}


						switch( lp_io->operation )
						{
						case IOCP_ACCEPT:
						{
							//	cout<<"接收到一个连接"<<endl;

							//lp_this->GetAddrAndPort(lp_io);
							lp_io->state = SOCKET_STATE_CONNECT;

							if( dwBytes > 0 )
							{
								lp_io->state = SOCKET_STATE_CONNECT_AND_READ;

								//cout<<"读取到一条数据"<<endl;
							}

							nRet = setsockopt( 
								lp_io->socket, 
								SOL_SOCKET, 
								SO_UPDATE_ACCEPT_CONTEXT, 
								( char* )&lp_this->_soketMainListenSoket,
								sizeof( lp_this->_soketMainListenSoket ) 
								);

							if( SOCKET_ERROR == nRet ){
								closesocket( lp_io->socket );
								//lp_io->error=false;
								//lp_this->_lpBaseBehaviorMsgDeal->MsgQuit(lp_io);
								lp_this->_ioGroup.RemoveAt( lp_io );
								//lp_this->_ioGroup.RemoveObj(lp_io);
								//PostQueuedCompletionStatus(lp_this->_hIOCP,0,(ULONG_PTR)lp_key,&lp_io->ol);

								printf(
									"IOCP_ACCEPT update setsockopt fail! WSAGetLastError:%d\n",
									WSAGetLastError()
									);

								continue;
							}

							nRet=setsockopt(
								lp_io->socket,
								IPPROTO_TCP,TCP_NODELAY,
								&chOpt,sizeof(char)
								);
							if( SOCKET_ERROR == nRet ){
								closesocket( lp_io->socket );
								//lp_io->error=false;
								lp_this->_ioGroup.RemoveAt( lp_io );
								//PostQueuedCompletionStatus(lp_this->_hIOCP,0,(ULONG_PTR)lp_key,&lp_io->ol);

								printf(
									"File:%s\nFunction:%s\nLine:%d\nErrorCode:%d\n",
									__FILE__,
									__FUNCTION__,
									__LINE__,
									GetLastError()
									);

								continue;
							}
							

// 							Err = setsockopt( 
// 								lp_io->socket, 
// 								SOL_SOCKET, 
// 								SO_KEEPALIVE, 
// 								&chOpt,
// 								sizeof(char)
// 								);		//超时保持连接

							lp_new_key = lp_this->_keyGroup.GetBlank();

							if( lp_new_key == NULL ){
								closesocket( lp_io->socket );
								//lp_io->error=false;
								lp_this->_ioGroup.RemoveAt( lp_io );
								//PostQueuedCompletionStatus(lp_this->_hIOCP,0,(ULONG_PTR)lp_key,&lp_io->ol);
								printf(
									"File:%s\nFunction:%s\nLine:%d\nErrorCode:%d\n",
									__FILE__,
									__FUNCTION__,
									__LINE__,
									GetLastError()
									);
								//cout<<"get a handle fail!"<<GetLastError()<<endl;
								continue;
							}

							lp_new_key->socket = lp_io->socket;

							//将新建立的SOCKET同完成端口关联起来。
							hRet = CreateIoCompletionPort( 
								( HANDLE )lp_io->socket,
								lp_this->_hIOCP,
								(DWORD)lp_new_key,
								0 
								);

							if( NULL == hRet ){

								closesocket( lp_io->socket );
								//lp_io->error=false;
								lp_this->_keyGroup.RemoveAt( lp_new_key );
								lp_this->_ioGroup.RemoveAt( lp_io );

								//直接从双链表里移除这个lp_io
								//lp_this->_keyGroup.RemoveObj( lp_new_key );
								//lp_this->_ioGroup.RemoveObj( lp_io );
								
								//PostQueuedCompletionStatus(lp_this->_hIOCP,0,(ULONG_PTR)lp_new_key,&lp_io->ol);

								//printf("link to iocp fail! GetLastError:%d\n",GetLastError());
								//printf("link to iocp fail! WSAGetLastError:%d\n",WSAGetLastError());
								//FormatMessage();
								//cout<<"link to iocp fail!"<<WSAGetLastError()<<endl;
								printf(
									"CreateIoCompletionPort fail! GetLastError:%d\n",
									GetLastError()
									);
								printf(
									"CreateIoCompletionPort fail! WSAGetLastError:%d\n",
									WSAGetLastError()
									);

								

								continue;
							}

							//处理读取到的数据
							if( dwBytes > 0 ){
								//lp_io->wsaBuf.len = dwBytes;
								//lp_this->HandleData( lp_io, IOCP_COMPLETE_ACCEPT_READ );
								//bRet = lp_this->DataAction( lp_io, lp_new_key );
								//if( FALSE == bRet )
								//{
								//	continue;
								//}
							}
							else{
								//lp_this->HandleData( lp_io, IOCP_COMPLETE_ACCEPT );
								//bRet = lp_this->DataAction( lp_io, lp_new_key );
								lp_this->InitIoContext( lp_io );
								lp_io->operation	= IOCP_READ;
								lp_this->ContinueRecv(lp_io, lp_new_key);
								lp_io->player=NULL;
								if( FALSE == bRet ){
									continue;
								}
							}
						}
						break;
						case IOCP_READ:
						{
							if( SOCKET_STATE_CONNECT_AND_READ != lp_io->state )
							{
								lp_io->state = SOCKET_STATE_CONNECT_AND_READ;
							}

							lp_io->wsaBuf.len = dwBytes;

							//处理用户发送的信息
							//lp_this->otherClass.translateMsg(lp_io->socket,lp_io->buf,len);

							//发送给用户信息
							//lp_this->_lpBaseBehaviorMsgDeal->
							//if (lp_this->MsgSend(lp_io->socket,"12345678",3)==FALSE){

							//这里是游戏逻辑处理。
							//if (lp_this->_lpBaseBehaviorMsgDeal->MsgDeal(lp_io,lp_io->buf,dwBytes)==FALSE)
							//if (lp_this->_lpIBMsgDealManage->MsgDealManage(
							//	&BaseBehavior::BaseBehaviorMsgDeal::MsgDeal,
							//	lp_io,
							//	lp_io->buf,
							//	dwBytes
							//	)==FALSE)
							if (lp_this->_lpIBMsgDealManage->MsgDealManage(lp_this,lp_io,lp_io->buf,dwBytes)==FALSE)
							{
#ifdef _DEBUG
								printf("Secket:%d 服务器逻辑控制客户端退出\n",lp_io->socket);
#endif // _DEBUG
								lp_this->_lpBaseBehaviorMsgDeal->MsgQuit(lp_io);
								closesocket( lp_io->socket );
								//lp_io->error=false;
								lp_this->_ioGroup.RemoveAt(lp_io);
								lp_this->_keyGroup.RemoveAt(lp_key);
								
								//PostQueuedCompletionStatus(lp_this->_hIOCP,0,(ULONG_PTR)lp_key,&lp_io->ol);

								continue;
							}
							else{
								lp_this->ContinueRecv(lp_io,lp_key);
							}
						}

						break;
						case IOCP_WRITE:
						{


							lp_this->_lpBehaviorMsgSend->Remove(lp_io);
							continue;
							//cout<<lp_io->buf<<endl;
							//lp_this->_ioSendGroup.RemoveAt( lp_io );

						}

						break;

						default:
							{
								continue;
							}
							break;
						}
					}

					//lp_this->AddMessage("io over");
					printf("Close CompletionThread \n");
					lp_this->CondCompletionThreadClose();
					return 0;
				}


				//bool BehaviorCreateMainThread::MsgSend(SOCKET s,char* buf,int len)
				//{
				//	DWORD	dwBytes;
				//	int		nRet;
				//	
				//	//IOCP_IO io;

				//	////WSABUF wsaBuf;
				//	////WSAOVERLAPPED ol;

				//	//io.operation=IOCP_WRITE;
				//	//memset( &io.ol,  0, sizeof( WSAOVERLAPPED ) );

				//	//char msg[BUFFER_SIZE];
				//	//memset( &msg, 0, BUFFER_SIZE );
				//	//io.wsaBuf.buf	= msg;
				//	//io.wsaBuf.len	= BUFFER_SIZE;
				//	////SOCKET socket;
				//	//io.socket=s;
				//	//
				//	//memcpy(msg,buf,len);
				//	//io.wsaBuf.buf =msg;
				//	//io.wsaBuf.len=len;

				//	//nRet = WSASend(
				//	//	io.socket, 
				//	//	&io.wsaBuf,
				//	//	1,
				//	//	&dwBytes,
				//	//	0,
				//	//	&io.ol,NULL);
				//	

				//	WSABUF wsaBuf;
				//	WSAOVERLAPPED ol;

				//	
				//	memset( &ol,  0, sizeof( WSAOVERLAPPED ) );

				//	char msg[BUFFER_SIZE];
				//	memset( &msg, 0, BUFFER_SIZE );
				//	wsaBuf.buf	= msg;
				//	wsaBuf.len	= BUFFER_SIZE;
				//	SOCKET socket;
				//	socket=s;

				//	memcpy(msg,buf,len);
				//	wsaBuf.buf =msg;
				//	wsaBuf.len=len;

				//	nRet = WSASend(
				//		socket, 
				//		&wsaBuf,
				//		1,
				//		&dwBytes,
				//		0,
				//		&ol,NULL);

				//	if( ( nRet == SOCKET_ERROR ) && ( WSAGetLastError() != WSA_IO_PENDING ) )
				//	{
				//		return FALSE;
				//	}
				//	return TRUE;
				//}

				//bool BehaviorCreateMainThread::MsgSend()
				//{
				//	return TRUE;
				//}

				bool BehaviorCreateMainThread::ContinueRecv(IOCP_IO* lp_io,IOCP_KEY* lp_key)
				{
					DWORD	dwBytes;
					int		nRet;
					DWORD	dwFlags;

					InitIoContext( lp_io );
					lp_io->operation	= IOCP_READ;
					dwFlags = 0;

					nRet = WSARecv(
						lp_io->socket,
						&lp_io->wsaBuf,
						1,
						&dwBytes,
						&dwFlags,
						&lp_io->ol,NULL);


					if(nRet==SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING )
					{
#ifdef _DEBUG
						printf(
							"File:%s\nFunction:%s\nLine:%d\nErrorCode:%d\n",
							__FILE__,
							__FUNCTION__,
							__LINE__,
							GetLastError()
							);
#endif // _DEBUG
						_lpBaseBehaviorMsgDeal->MsgQuit(lp_io);
						closesocket( lp_io->socket );
						//lp_io->error=false;
						_ioGroup.RemoveAt( lp_io );
						_keyGroup.RemoveAt( lp_key );
						//PostQueuedCompletionStatus(_hIOCP,0,(ULONG_PTR)lp_key,&lp_io->ol);

						return FALSE;
					}
					return TRUE;
				}

				void BehaviorCreateMainThread::Remove(IOCP_IO* lp_io,IOCP_KEY* lp_key)
				{

					_ioGroup.RemoveAt( lp_io );
					//if(lp_key!=NULL){
					_keyGroup.RemoveAt( lp_key );
					//}
					//UpdPlayerList();
				}

				///*-------------------------------------------------------------------------------------------
				//函数功能：得到连接上来的客户端IP和PORT
				//函数说明：
				//函数返回：成功，TRUE；失败，FALSE
				//-------------------------------------------------------------------------------------------*/
				//bool BehaviorCreateMainThread::GetAddrAndPort(IOCP_IO* lp_io)
				//{
				//	int nLocalLen, nRmoteLen;

				//	SOCKADDR_IN *pLocalAddr, *pRemoteAddr;

				//	lpGetAcceptExSockaddrs(lp_io->buf,   
				//		0,
				//		sizeof(SOCKADDR_IN)   +   16,                           //同上   
				//		sizeof(SOCKADDR_IN)   +   16,                           //同上   
				//		(SOCKADDR **)&pLocalAddr,
				//		&nLocalLen,
				//		(SOCKADDR **)&pRemoteAddr,
				//		&nRmoteLen);   


				//	lp_io->ip=inet_ntoa(pRemoteAddr->sin_addr);
				//	lp_io->port=ntohs(pRemoteAddr->sin_port);

				//	return TRUE;
				//}

				///*-------------------------------------------------------------------------------------------
				//函数功能：处理数据函数
				//函数说明：
				//函数返回：成功，TRUE；失败，FALSE
				//-------------------------------------------------------------------------------------------*/
				//bool BehaviorCreateMainThread::HandleData(IOCP_IO* lp_io, int nFlags)
				//{
				//	switch( nFlags )
				//	{
				//	case IOCP_COMPLETE_ACCEPT:
				//		{
				//			//cout<<"Accept a link!*****************************"<<endl;


				//			InitIoContext( lp_io );

				//			lp_io->operation	= IOCP_READ;
				//		}
				//		break;
				//	case IOCP_COMPLETE_ACCEPT_READ:
				//		{
				//			//cout<<"read a data!*******************************data length is:"<<endl;

				//			lp_io->operation	= IOCP_WRITE;

				//			memset( &lp_io->ol, 0, sizeof(lp_io->ol) );
				//		}
				//		break;
				//	case IOCP_COMPLETE_READ:
				//		{
				//			//cout<<"IOCP_COMPLETE_READ!*******************************"<<endl;

				//			lp_io->operation	= IOCP_WRITE;

				//			memset( &lp_io->ol, 0, sizeof(lp_io->ol) );

				//		}
				//		break;
				//	case IOCP_COMPLETE_WRITE:
				//		{
				//			//cout<<"IOCP_COMPLETE_WRITE!******************************"<<endl;

				//			InitIoContext( lp_io );
				//			lp_io->operation	= IOCP_READ;
				//		}
				//		break;
				//	default:
				//		{
				//			//cout<<"handleData do nothing!*********************"<<endl;

				//			return FALSE;
				//		}
				//	}

				//	return TRUE;
				//}


				/*-------------------------------------------------------------------------------------------
				函数功能：关闭所有线程
				函数说明：
				函数返回：
				-------------------------------------------------------------------------------------------*/
				void BehaviorCreateMainThread::CloseThreadHandle(int count)
				{
					if( count <= 0 )
					{
						return;
					}

					for( int i= 0; i < count; i++ )
					{
						CloseHandle( _hArrThread[i] );

						_hArrThread[i] = INVALID_HANDLE_VALUE;
					}
				}

				/*-------------------------------------------------------------------------------------------
				函数功能：得到MS封装的SOCKET函数指针，这样可以提高速度
				函数说明：
				函数返回：成功，TRUE；失败，FALSE
				-------------------------------------------------------------------------------------------*/
				bool BehaviorCreateMainThread::GetFunPointer()
				{
					DWORD dwRet,nRet;

					nRet = WSAIoctl(
						_soketMainListenSoket, 
						SIO_GET_EXTENSION_FUNCTION_POINTER, 
						&g_GUIDAcceptEx, 
						sizeof(g_GUIDAcceptEx), 
						&lpAcceptEx,
						sizeof(lpAcceptEx),
						&dwRet,NULL,NULL);

					if( SOCKET_ERROR == nRet )
					{
						closesocket( _soketMainListenSoket );

						//AddMessage("get acceptex fail!");

						return FALSE;
					}

					//nRet = WSAIoctl( 
					//	m_listen_socket, 
					//	SIO_GET_EXTENSION_FUNCTION_POINTER,
					//	&g_GUIDTransmitFile,
					//	sizeof(g_GUIDTransmitFile),
					//	&lpTransmitFile,
					//	sizeof(lpTransmitFile),
					//	&dwRet,NULL,NULL);

					//if(nRet == SOCKET_ERROR )
					//{
					//	closesocket( m_listen_socket );

					//	AddMessage("get transmitfile fail!");

					//	return FALSE;
					//}

					//nRet = WSAIoctl( 
					//	m_listen_socket, 
					//	SIO_GET_EXTENSION_FUNCTION_POINTER, 
					//	&g_GUIDGetAcceptExSockaddrs, 
					//	sizeof(g_GUIDGetAcceptExSockaddrs), 
					//	&lpGetAcceptExSockaddrs,
					//	sizeof(lpGetAcceptExSockaddrs),
					//	&dwRet,NULL,NULL);

					return TRUE;
				}

				/*-------------------------------------------------------------------------------------------
				函数功能：发出一定数量的连接
				函数说明：
				函数返回：成功，TRUE；失败，FALSE
				-------------------------------------------------------------------------------------------*/
				bool BehaviorCreateMainThread::PostAcceptEx()
				{
					int		count = POST_NUMBER;
					DWORD	dwBytes;
					BOOL	bRet;

					for( int i = 0; i < count; i++ )
					{
						SOCKET socket = WSASocket(
							AF_INET,
							SOCK_STREAM,
							IPPROTO_TCP,
							NULL,
							0,
							WSA_FLAG_OVERLAPPED);

						if( INVALID_SOCKET == socket )
						{
							std::cout<<"INVALID_SOCKET"<<std::endl;
							continue;
						}

						IOCP_IO* lp_io = _ioGroup.GetBlank();

						InitIoContext( lp_io );

						lp_io->socket			= socket;
						lp_io->operation		= IOCP_ACCEPT;
						lp_io->state			= SOCKET_STATE_NOT_CONNECT;
						lp_io->pipe_id=-1;

						/////////////////////////////////////////////////


						bRet = lpAcceptEx(
							_soketMainListenSoket,
							lp_io->socket,
							lp_io->buf,
							//			lp_io->wsaBuf.len   -   ((sizeof(SOCKADDR_IN)   +   16)   *   2),   

							0,
							sizeof(SOCKADDR_IN) + 16,
							sizeof(SOCKADDR_IN) + 16,
							&dwBytes,&lp_io->ol);

						if( ( bRet == FALSE ) && ( WSA_IO_PENDING != WSAGetLastError() ) )
						{
							closesocket( socket );

							_ioGroup.RemoveAt( lp_io );

							//AddMessage("post acceptex fail:");

							continue;
						}
					}
					return TRUE;
				}

				/*-------------------------------------------------------------------------------------------
				函数功能：初始化IO结点
				函数说明：
				函数返回：
				-------------------------------------------------------------------------------------------*/
				void BehaviorCreateMainThread::InitIoContext(IOCP_IO* lp_io)
				{
					memset( &lp_io->ol,  0, sizeof( WSAOVERLAPPED ) );

					memset( &lp_io->buf, 0, BUFFER_SIZE );



					lp_io->wsaBuf.buf		= lp_io->buf;

					lp_io->wsaBuf.len		= BUFFER_SIZE;
					//lp_io->pipe_id=-1;
					//lp_io->player=0;

				}


				/*-------------------------------------------------------------------------------------------
				函数功能：注册FD_ACCEPTG事件到m_h_accept_event事件，以便所有发出去的连接耗耗尽时，得到通知。
				函数说明：
				函数返回：成功，TRUE；失败，FALSE
				-------------------------------------------------------------------------------------------*/
				bool BehaviorCreateMainThread::RegAcceptEvent()
				{
					int		nRet;

					_hAcceptEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

					if( NULL == _hAcceptEvent )
					{
						return FALSE;
					}

					nRet = WSAEventSelect( _soketMainListenSoket, _hAcceptEvent, FD_ACCEPT );

					if( nRet != 0 )
					{
						CloseHandle( _hAcceptEvent );

						return FALSE;
					}

					return TRUE;
				}

				void BehaviorCreateMainThread::SetBehaviorMsgDeal(BaseBehavior::BaseBehaviorMsgDeal* i_lpBaseBehaviorMsgDeal){
					this->_lpBaseBehaviorMsgDeal=i_lpBaseBehaviorMsgDeal;
				}

				void BehaviorCreateMainThread::SetIBMsgDealManage( BaseBehavior::IBMsgDealManage* i_lpIBMsgDealManage )
				{
					this->_lpIBMsgDealManage=i_lpIBMsgDealManage;
				}


				//void BehaviorCreateMainThread::SetBehaviorMsgSend(BaseBehavior::BaseBehaviorMsgSend* i_lpBaseBehaviorMsgSend){
				//	//this->_lpBaseBehaviorMsgSend=i_lpBaseBehaviorMsgSend;
				//	//this->_lpBaseBehaviorMsgSend=i_lpBaseBehaviorMsgSend;
				//}

				//void BehaviorCreateMainThread::SetBehaviorQuit(BaseBehavior::BaseBehaviorQuit* i_lpBaseBehaviorBehaviorQuit){
				//	this->_lpBaseBehaviorQuit=i_lpBaseBehaviorBehaviorQuit;
				//}

				//void BehaviorCreateMainThread::SetBehaviorMsgSend(SOCKET s,char* buf,int len){
				//	this->_lpBaseBehaviorMsgDeal=i_lpBaseBehaviorMsgDeal;
				//}

				//void BehaviorCreateMainThread::SetBehaviorQuit(){

				//}

			}
		}
	}
}