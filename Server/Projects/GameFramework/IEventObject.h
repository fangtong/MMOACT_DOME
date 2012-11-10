#pragma once
#include "FinalClass.h"
#include "IBroadcast.h"

//namespace SevenSmile{
//	namespace Util{
//		namespace DesignPatterns
//		{
//			//extern enum BroadcastEvent;
//			typedef unsigned int BroadcastEvent;
//		}
//	}
//}

using namespace SevenSmile::Util::DesignPatterns;

namespace SevenSmile
{
	namespace Core
	{
		struct IEventObject
		{
		public:
			virtual ~IEventObject(){};

#pragma region �¼����
		public:
			virtual void InitEventHandle()	= 0;
			virtual void DistroyEventHandle() = 0;
			virtual VoidShardPtr GetSPtrThis() = 0;
			//************************************
			// Method:    NotifyToWorld	������߷���֪ͨ
			// FullName:  SevenSmile::Core::AbsObject::NotifyToListener
			// Access:    protected 
			// Returns:   void
			// Qualifier:
			// Parameter: BroadcastEvent i_event
			// Parameter: void * i_param
			//************************************
			virtual void NotifyToListener(BroadcastEvent i_event,void* i_param) = 0;
#pragma endregion �¼����
		};
	}
}
