#pragma once
#include "FinalClass.h"

using namespace std::tr1;

namespace SevenSmile
{
	namespace Manage
	{
		template<class T,class PropertyType>
		class TempletePropertyManage
		{
		public:
			TempletePropertyManage(){};
			~TempletePropertyManage(){};

			typedef LockedHashMap<shared_ptr<T>, shared_ptr<PropertyType>,hashSPtr<T>> ImplToProperty;

			shared_ptr<PropertyType> static GetProperty(shared_ptr<T> i_object)
			{
				//同步
				shared_ptr<PropertyType> res = NullShardPtr(PropertyType);
				ImplToProperty::Lock lock(_hashMap);
				ImplToProperty::iterator it = _hashMap.find(i_object);
				if (it != _hashMap.end())
				{
					res = it->second;
				}
				return res;
			}

			static void AddProperty(shared_ptr<T> i_obj,shared_ptr<PropertyType> i_property)
			{
				//同步
				if ( !i_property || !i_obj ) return;
				ImplToProperty::Lock lock(_hashMap);
				ImplToProperty::iterator it = _hashMap.find(i_obj);
				if (it != _hashMap.end()) return;
				_hashMap[i_obj] = i_property;
			}

			static void DelProperty(shared_ptr<T> i_obj)
			{
				//同步
				ImplToProperty::Lock lock(_hashMap);
				ImplToProperty::iterator it = _hashMap.find(i_obj);
				if (it == _hashMap.end()) return;
				_hashMap.erase(it);
			}

			static void Destroy()
			{
				_hashMap.clear();
			}
		private:
			static ImplToProperty _hashMap;
		};
	}
}