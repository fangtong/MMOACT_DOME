#pragma once
#include <vector>
#include "Singleton.h"
#include <memory>
#include <hash_map>
#include "ORMTypedef.h"
#include "LockedSet.h"

using namespace SevenSmile::Util::Container;

namespace SevenSmile{

	namespace DBCPFramework
	{
		class DBCP;
	}

	namespace ORM{
		class DBDateManage
		{
		public:
			typedef LockedSet<SPtrAbsDBProxy> UpDateList;
			typedef std::tr1::shared_ptr<SevenSmile::DBCPFramework::DBCP> SPtrDBCP;
			typedef stdext::hash_map<std::string,SptrAbsTable> HashNameToTable;
			DBDateManage(void);
			~DBDateManage(void);
		
			SevenSmile::ORM::DBDateManage::SPtrDBCP GetSPtrDBCP() const { return _sPtrDBCP; }
			void SetSPtrDBCP(SevenSmile::ORM::DBDateManage::SPtrDBCP val) { _sPtrDBCP = val; }
			void InitDataBase();
			void UpdateDataBase();
			SptrAbsTable GetTableByName(std::string i_name);
		private:
			UpDateList _setUpDataList;
			SPtrDBCP _sPtrDBCP;
			
			HashNameToTable _hashNameToTable;
		public:
			void	AddToUpDateList(SPtrAbsDBProxy i_add);
			void	DelToUpDateList(SPtrAbsDBProxy i_del);
			SPtrAbsDBProxy	FindUpDateList(SPtrAbsDBProxy i_find);
			void	UpDate();
			void	ClearUpDateList();
		};
	}
}


#define DBDateManager SevenSmile::Util::DesignPatterns::Singleton<DBDateManage>::instance()
