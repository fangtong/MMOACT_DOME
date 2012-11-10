#pragma once
#include <string>
#include "ICallBackHandle.h"
#include <hash_map>
#include "FinalClass.h"
#include <sys\utime.h>
#include <xstring>
#include "iobject.h"
#include "imanageobject.h"
#include "ieventobject.h"

//namespace SevenSmile{
//	namespace Util{
//		namespace DesignPatterns
//		{
//			enum BroadcastEvent;
//		}
//	}
//}


using namespace SevenSmile::Util::DesignPatterns;

namespace SevenSmile
{
	namespace Core
	{
		class AbsObject
			:public virtual SevenSmile::Core::IObject,
			public virtual SevenSmile::Core::IManagedObject,
			public virtual SevenSmile::Core::IEventObject
		{
		public:
			AbsObject():_isDestroy(false){};
			virtual ~AbsObject(){ if(!_isDestroy) Destroy(); };
			virtual void Destroy();
			virtual	void UpTime(time_t i_diff);

			template<class T>
			bool CompareByFlag(unsigned int i_flag,T i_value);

#pragma region 对象管理
		public:
			/*template<class T>
			virtual void AddToObjectManage();
			template<class T>
			virtual void DelFromObjectManage();*/

			virtual void AddToObjectManage(){};
			virtual void DelFromObjectManage(){};

			virtual void SetObjectKey(std::string i_key){ _key = i_key; }
			virtual std::string GetObjectKey() { return _key; };
#pragma endregion 对象管理
		
		
#pragma region 事件相关
		public:
			virtual void InitEventHandle();
			virtual void DistroyEventHandle();
			virtual VoidShardPtr GetSPtrThis();
			//************************************
			// Method:    NotifyToWorld	向监听者发送通知
			// FullName:  SevenSmile::Core::AbsObject::NotifyToListener
			// Access:    protected 
			// Returns:   void
			// Qualifier:
			// Parameter: BroadcastEvent i_event
			// Parameter: void * i_param
			//************************************
			virtual void NotifyToListener(BroadcastEvent i_event,void* i_param);
#pragma endregion 事件相关
	
#pragma region 属性
		protected:
			typedef stdext::hash_map<BroadcastEvent,SharPtrICallBack> HashCallBack;
			HashCallBack _eventHandles;
			std::string _key;
			bool			_isDestroy;
#pragma endregion 属性

		};

		template<class T>
		bool AbsObject::CompareByFlag(unsigned int i_flag,T i_value)
		{
			return false;
		}

		//template<class T>
		//void SevenSmile::Core::AbsObject<T>::AddToObjectManage()
		//{
		//	ObjectManage<T>::AddObj(
		//		GetObjectKey(),
		//		static_pointer_cast<T,void>(GetSPtrThis()));
		//}

		//template<class T>
		//void SevenSmile::Core::AbsObject<T>::DelFromObjectManage()
		//{
		//	ObjectManage<T>::DelObj(
		//		GetObjectKey(),
		//		static_pointer_cast<T,void>(GetSPtrThis()));
		//}
	}
}
