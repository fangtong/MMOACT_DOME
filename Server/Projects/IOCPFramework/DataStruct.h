#pragma once
#include "DoubleList.h"

class CPlayer;

namespace SevenSmile{
	namespace Net{
		namespace IOCPFramework{
			//////////////////////////////////////////////////////////////////////////

#define		BUFFER_SIZE			4096
			//#define		MAXTHREAD_COUNT		8
#define		POST_NUMBER			10

			//#define    PORT               7000
			//#define    ADDR				"127.0.0.1"

			//#define		IPSTR				"http://localhost/game1/"

			//////////////////////////////////////////////////////////////////////////

			//
			//完成端口完成的事件
			//
			typedef enum
			{
				IOCP_COMPLETE_READ,
				IOCP_COMPLETE_ACCEPT_READ,
				IOCP_COMPLETE_WRITE,
				IOCP_COMPLETE_ACCEPT
			}IOCP_COMPLETE_STATE;

			//
			//自定义枚举数据类型，用来标识套接字IO动作类型
			//
			typedef enum
			{
				IOCP_ACCEPT, //AcceptEx/accept
				IOCP_READ,   //WSARecv/recv/ReadFile
				IOCP_WRITE,   //WSASend/send/WriteFile
				IOCP_END
			}IOCP_OPERATION, *IOCP_OPERATION_PTR;


			//
			//自定义结构，即“完成键”(单句柄数据)
			//
			typedef struct
			{
				SOCKET                     socket;
			}IOCP_KEY;

			//
			//标志SOCKET的当前状态
			//
			enum
			{
				SOCKET_STATE_NOT_CONNECT,
				SOCKET_STATE_CONNECT,
				SOCKET_STATE_CONNECT_AND_READ,
				SOCKET_STATE_TIMEOUT
			};

			//template<typename T>
			//class IocpData{
			//public:
			//	static T* GetData(){
			//		return _tmplData;
			//	}

			//	static void SetData(T* i_T){
			//		_tmplData=i_T;
			//	}
			//private:
			//	static T* _tmplData;
			//};


			//单IO数据，扩展的WSAOVERLAPPED
			/*typedef*/ struct IOCP_IO
			//class IOCP_IO
			{
			public:
				IOCP_IO()
				{
					//memset(&ol,0,sizeof(WSAOVERLAPPED));
					//memset(&buf,0,BUFFER_SIZE);
					//wsaBuf.buf	= buf;
					//wsaBuf.len	= BUFFER_SIZE;
					Clear();
				}

				void Clear(void){
					memset(&ol,0,sizeof(WSAOVERLAPPED));
					memset(&buf,0,BUFFER_SIZE);
					wsaBuf.buf	= buf;
					wsaBuf.len	= BUFFER_SIZE;
					//error=false;
				}

				WSAOVERLAPPED				ol;

				char						buf[BUFFER_SIZE];

				WSABUF						wsaBuf;
				SOCKET						socket;

				IOCP_OPERATION				operation;

				//char*						ip;
				int							port;
				volatile int				state;
				int							pipe_id;
				CPlayer*					player;
				//volatile bool				error;
			};

			//template<class T>
			//class IOCP_IO {
			//public:
			//	WSAOVERLAPPED				ol;
			//	char						buf[BUFFER_SIZE];
			//	WSABUF						wsaBuf;
			//	SOCKET						socket;
			//	IOCP_OPERATION				operation;
			//	
			//	//template<typename T>
			//	//T*							data;
			//	//char*						ip;
			//	int							port;
			//	volatile int				state;

			//	template<typename T>
			//	T* Get(){
			//		return IocpData<T>::GetData();
			//	}

			//	template<typename T>
			//	void Set(T* i_T){
			//		IocpData<T>::SetData(i_T);
			//	}
			//	
			//	IOCP_IO(){
			//		operation=IOCP_END;
			//	}
			//};

			//using namespace SevenSmile::IOCPFramework::Util;

			typedef CDoubleList<IOCP_IO,POST_NUMBER>	IO_GROUP;
			typedef CDoubleList<IOCP_KEY,POST_NUMBER>	KEY_GROUP;

			typedef IO_GROUP::ITERATOR	IO_POS;
			typedef KEY_GROUP::ITERATOR KEY_POS;

		}
	}
}