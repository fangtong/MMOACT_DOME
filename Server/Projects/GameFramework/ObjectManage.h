#pragma once
#include "LockedHashMap.h"
#include <memory>
#include "FinalClass.h"
#include <assert.h>
#include "CChineseCode.h"

using namespace SevenSmile::Util::Container;
using namespace SevenSmile::Util::StringHelper::CChineseCode;
using namespace std::tr1;
using namespace std;

namespace SevenSmile
{
	namespace Manage
	{
		template<class T>
		class ObjectManage
		{
		public:
			ObjectManage(void){};
			~ObjectManage(void){};

		public:
			typedef LockedHashMap<std::string, std::tr1::shared_ptr<T>> KeyToObject;
			shared_ptr<T> static GetObj(std::string i_key)
			{
				//ͬ��
				shared_ptr<T> res = NullShardPtr(T);
				KeyToObject::Lock lock(_hashMap);
				KeyToObject::iterator it = _hashMap.find(i_key);
				if (it != _hashMap.end())
				{
					res = it->second;
				}
				return res;
			}

			static void AddObj(std::string i_key,shared_ptr<T> i_object)
			{
				//ͬ��
				if ( !i_object ) return;
				KeyToObject::Lock lock(_hashMap);
				KeyToObject::iterator it = _hashMap.find(i_key);
				if (it != _hashMap.end()) 
				{
					string sGBK;
					UTF_8ToGBK(sGBK,i_key);
					printf("%s �Ѿ�����!\n",sGBK.c_str());
					//assert(false);
					return;
				}
				_hashMap[i_key] = i_object;
			}

			static void DelObj(std::string i_key)
			{
				//ͬ��
				KeyToObject::Lock lock(_hashMap);
				KeyToObject::iterator it = _hashMap.find(i_key);
				if (it == _hashMap.end()) return;
				_hashMap.erase(it);
			}

			//************************************
			// Method:    GetObjHashMap ���ض����б�
			// FullName:  SevenSmile::Manage::ObjectManage<T>::GetObjHashMap
			// Access:    public static 
			// Returns:   KeyToObject&
			// Qualifier:
			//************************************
			static KeyToObject& GetObjHashMap(){
				return _hashMap;
			}

			static void Destroy()
			{
				_hashMap.clear();
			}
		private:
			static KeyToObject _hashMap;
		};

		template<class T> LockedHashMap<std::string, std::tr1::shared_ptr<T>> SevenSmile::Manage::ObjectManage<T>::_hashMap;

	}
}
