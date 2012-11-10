#pragma once
#include <hash_map>
#include <deque>
#include "LockedHashMap.h"
#include "ThreadingModel.h"
#include "FinalClass.h"
#include <memory>


namespace SevenSmile{
	namespace Manage{
		using namespace SevenSmile::Util::Container;
		using namespace stdext;
		using namespace std;
		using namespace std::tr1;

		template<class T>
		class GroupManager
		{
		public:
			typedef LockedHashMap<unsigned int,shared_ptr<T>> MapGroups;
			typedef std::deque<unsigned int> DequeIndex;
		public:
			GroupManager(void);
			~GroupManager(void);

			unsigned int	AddGroupRtKey(shared_ptr<T> i_implGroup);
			bool			DelGroupByKey(unsigned int i_key);
			bool			DelGroupByKey(shared_ptr<T> i_implGroup);
			int				GetGroupTotalNum(void);
			shared_ptr<T>	GetGroupByKey(unsigned int i_ui32GroupId);
			MapGroups&		GetGroups(void) { return _mapGroups; }
			bool			FindGroupByKey( shared_ptr<T> i_implGroup );

		private:
			MapGroups _mapGroups;
			unsigned int _ui32GroupIndex;
			DequeIndex _dequeIndex;
		};

		template<class T>
		int SevenSmile::Manage::GroupManager<T>::GetGroupTotalNum( void )
		{
			MapGroups::Lock lock(_mapGroups);
			return _mapGroups.size();
		}
		template<class T>
		shared_ptr<T> SevenSmile::Manage::GroupManager<T>::GetGroupByKey( unsigned int i_key )
		{
			shared_ptr<T> res = NullShardPtr(T);
			MapGroups::Lock lock(_mapGroups);
			MapGroups::iterator it=_mapGroups.find(i_key);
			if(it != _mapGroups.end()) res = it->second;
			return res;
		}
		template<class T>
		bool SevenSmile::Manage::GroupManager<T>::FindGroupByKey( shared_ptr<T> i_implGroup )
		{
			bool res = false;
			MapGroups::Lock lock(_mapGroups);
			MapGroups::iterator it;
			for ( it = _mapGroups.begin() ; it != _mapGroups.end() ; it++ )
			{
				if (i_implGroup == it->second)
				{
					res = true;
					break;
				}
			}
			return res;
		}
		template<class T>
		bool SevenSmile::Manage::GroupManager<T>::DelGroupByKey( unsigned int i_key )
		{
			bool res = false;
			MapGroups::Lock lock(_mapGroups);
			MapGroups::iterator it=this->_mapGroups.find(i_key);
			if (it!=this->_mapGroups.end())
			{
				unsigned int index=it->first;
				_dequeIndex.push_back(index);
				_mapGroups.erase(it);
				res = true;
			}
			return res;
		}
		template<class T>
		bool SevenSmile::Manage::GroupManager<T>::DelGroupByKey( shared_ptr<T> i_implGroup )
		{
			bool res = false;
			MapGroups::Lock lock(_mapGroups);
			MapGroups::iterator it;
			for ( it = _mapGroups.begin() ; it != _mapGroups.end() ; it++ )
			{
				if (i_implGroup == it->second)
				{
					unsigned int index=it->first;
					_dequeIndex.push_back(index);
					_mapGroups.erase(it);
					res = true;
					break;
				}
			}
			return res;
		}
		template<class T>
		unsigned int SevenSmile::Manage::GroupManager<T>::AddGroupRtKey( shared_ptr<T> i_implGroup )
		{
			unsigned int index = 0;
			if(!_dequeIndex.empty()){
				index=_dequeIndex.front();
				_dequeIndex.pop_front();
			}else{
				_ui32GroupIndex++;
				index = _ui32GroupIndex;
			}
			_mapGroups[index]=i_implGroup;
			return index;
		}
		template<class T>
		SevenSmile::Manage::GroupManager<T>::~GroupManager( void )
		{
			MapGroups::Lock lock(_mapGroups);
			_mapGroups.clear();
			_dequeIndex.clear();
		}
		template<class T>
		SevenSmile::Manage::GroupManager<T>::GroupManager( void )
		{
			_ui32GroupIndex=0;
		}
	}
}