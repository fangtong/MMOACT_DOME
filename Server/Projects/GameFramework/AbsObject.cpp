#include "AbsObject.h"
#include "ApplicationBase.h"
#include "IBroadcast.h"
#include "FinalClass.h"
using namespace SevenSmile::Util::DesignPatterns;
void SevenSmile::Core::AbsObject::Destroy()
{
	DistroyEventHandle();
	DelFromObjectManage();
	_isDestroy = true;
}

void SevenSmile::Core::AbsObject::InitEventHandle()
{
	SharPtrIBroadcast sPtrBroadcastDelegate = ApplicationBase::sPtrBroadcastDelegate;
	do 
	{
		if(!sPtrBroadcastDelegate) break;
		VoidShardPtr sPtrthis = GetSPtrThis();
		if(!sPtrthis) break;
		HashCallBack::iterator it = _eventHandles.begin();
		for (it; it!= _eventHandles.end(); it++)
		{
			sPtrBroadcastDelegate->Listen(sPtrthis,it->first,it->second);
		}
	} while (0);
}

void SevenSmile::Core::AbsObject::DistroyEventHandle()
{
	SharPtrIBroadcast sPtrBroadcastDelegate = ApplicationBase::sPtrBroadcastDelegate;
	do 
	{
		if(!sPtrBroadcastDelegate)break;
		VoidShardPtr sPtrthis = GetSPtrThis();
		if(!sPtrthis) break;
		HashCallBack::iterator it = _eventHandles.begin();
		for (it; it!= _eventHandles.end(); it++)
		{
			sPtrBroadcastDelegate->UnListen(sPtrthis,it->first,it->second);
		}
	} while (0);
	_eventHandles.clear();
}

VoidShardPtr SevenSmile::Core::AbsObject::GetSPtrThis()
{
	return NullShardPtr(AbsObject);
}

void SevenSmile::Core::AbsObject::NotifyToListener( BroadcastEvent i_event,void* i_param )
{
	SharPtrIBroadcast sPtrBroadcastDelegate = ApplicationBase::sPtrBroadcastDelegate;
	do 
	{	
		if(!sPtrBroadcastDelegate) break;
		VoidShardPtr sPtrthis = GetSPtrThis();
		if(!sPtrthis) break;
		sPtrBroadcastDelegate->Notify(sPtrthis,i_event,i_param);
	} while (0);

}

void SevenSmile::Core::AbsObject::UpTime( time_t t )
{

}
