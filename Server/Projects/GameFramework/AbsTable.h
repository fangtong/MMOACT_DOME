#pragma once
#include "ORMTypedef.h"
#include <hash_map>
#include <string>

namespace SevenSmile{
	namespace ORM{
		

		class AbsTable
		{
		public:
			typedef stdext::hash_map<PRIMARY,SPtrAbsRow> HashKeyToRow;
			explicit AbsTable(std::string i_tableName);
			~AbsTable(void);
			
		protected:
			std::string				_tableName;
			HashKeyToRow		_hashKeyToRow;
			PRIMARY				_keyValueCount;
			SPtrIFields				_sPtrFields;

		public:
			virtual void LoadTable() = 0;
			virtual void InitTable() = 0;
			virtual std::string GenerateLoadSql();
			virtual SPtrAbsRow GenerateRow();
			virtual SPtrAbsRow GenerateNewRow();
			virtual unsigned int GetPriFieldPos();
			PRIMARY GenerateNewPrimary();
			HashKeyToRow& GetHashKeyToRow() { return _hashKeyToRow; };
			SPtrAbsRow GetRowByPriKey(PRIMARY i_key);
		};


	}
}
