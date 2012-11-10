#pragma once
#include "BaseBehaviorMsgSend.h"

namespace SevenSmile{
	namespace Net{
		namespace IOCPFramework{
			namespace BaseBehavior{
				BaseBehaviorMsgSend::BaseBehaviorMsgSend(){
					//_lpIocpIO=new IOCP_IO();
					//_lpIocpIO->operation=IOCP_WRITE;
					//memset(&_lpIocpIO->ol,0,sizeof(WSAOVERLAPPED));
					//memset(&_lpIocpIO->buf,0,BUFFER_SIZE);
					//_lpIocpIO->wsaBuf.buf	= _lpIocpIO->buf;
					//_lpIocpIO->wsaBuf.len	= BUFFER_SIZE;
				}
				BaseBehaviorMsgSend::~BaseBehaviorMsgSend(){
					//if (0!=_lpIocpIO)
					//{
					//	delete _lpIocpIO;
					//	_lpIocpIO=0;
					//}
				}

				void  BaseBehaviorMsgSend::Remove(IOCP_IO* lp_io){
					//lp_io->error=false;
					this->_ioSendGroup.RemoveAt(lp_io);
				}

				bool BaseBehaviorMsgSend::Send(
					IOCP_IO* i_lp_io,
					char* i_lpChar,
					int i_charArrLength
					)
				{

					if (i_charArrLength > BUFFER_SIZE)
					{
						return false;
					}

					//if (i_lp_io->error==true){ 
					//	//i_lp_io->error=true;
					//	return false;
					//}
					if(!i_lp_io || !i_lp_io->socket)
					{
						printf("send iocp is null\n");
						return false;
					}
					DWORD	dwBytes;
					int		nRet;

					//WSABUF wsaBuf;
					//WSAOVERLAPPED ol;

					IOCP_IO* lp_io = _ioSendGroup.GetBlank();
					lp_io->Clear();

					lp_io->operation=IOCP_WRITE;

					//memset(&lp_io->ol,0,sizeof(WSAOVERLAPPED));
					//memset(&lp_io->buf,0,BUFFER_SIZE);
					//lp_io->wsaBuf.buf	= lp_io->buf;
					//lp_io->wsaBuf.len	= BUFFER_SIZE;

					lp_io->socket=i_lp_io->socket;
					memcpy(lp_io->buf,i_lpChar,i_charArrLength);
					lp_io->wsaBuf.buf =lp_io->buf;
					lp_io->wsaBuf.len=i_charArrLength;

//					printf("wsabuf length:%d!\n",i_charArrLength);

					nRet = WSASend(
						lp_io->socket, 
						&lp_io->wsaBuf,
						1,
						&dwBytes,
						0,
						&lp_io->ol,NULL);

					if((nRet == SOCKET_ERROR)&&(WSAGetLastError() != WSA_IO_PENDING))
					{
#ifdef _DEBUG
						printf(
						"File:%s\nFunction:%s\nLine:%d\nErrorCode:%d\n",
							__FILE__,
							__FUNCTION__,
							__LINE__,
							WSAGetLastError()
							);
#endif // _DEBUG
						//_ioSendGroup.RemoveAt(lp_io);

						//lp_io->error=true;
						//i_lp_io->error=true;
						_ioSendGroup.RemoveAt(lp_io);

						
						//PostQueuedCompletionStatus(_hIOCP,0,(ULONG_PTR)i_lp_io->socket,(LPOVERLAPPED)&i_lp_io->ol);
						return false;
					}
					
					/*
					IOCP_IO _lpIocpIO;
					_lpIocpIO.operation=IOCP_WRITE;
					memset(&_lpIocpIO.ol,0,sizeof(WSAOVERLAPPED));
					memset(&_lpIocpIO.buf,0,BUFFER_SIZE);
					_lpIocpIO.wsaBuf.buf	= _lpIocpIO.buf;
					_lpIocpIO.wsaBuf.len	= BUFFER_SIZE;

					_lpIocpIO.socket=i_lp_io->socket;

					memcpy(_lpIocpIO.buf,i_lpChar,i_charArrLength);
					_lpIocpIO.wsaBuf.buf =_lpIocpIO.buf;
					_lpIocpIO.wsaBuf.len=i_charArrLength;

					//_lpIocpIO->socket=i_lp_io->socket;

					//memcpy(_lpIocpIO->buf,i_lpChar,i_charArrLength);
					//_lpIocpIO->wsaBuf.buf =_lpIocpIO->buf;
					//_lpIocpIO->wsaBuf.len=i_charArrLength;

					//nRet = WSASend(
					//	_lpIocpIO->socket
					//	,&_lpIocpIO->wsaBuf
					//	,1
					//	,&dwBytes
					//	,0
					//	,&_lpIocpIO->ol
					//	,NULL
					//	);

					nRet = WSASend(
						_lpIocpIO.socket
						,&_lpIocpIO.wsaBuf
						,1
						,&dwBytes
						,0
						,&_lpIocpIO.ol
						,NULL
						);

					if((nRet == SOCKET_ERROR)&&(WSAGetLastError() != WSA_IO_PENDING))
					{
						return FALSE;
					}
					*/

					return true;
				}

			}
		}
	}
}