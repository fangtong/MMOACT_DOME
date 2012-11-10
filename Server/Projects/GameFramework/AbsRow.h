#pragma once
#include <string>
#include "IField.h"
#include <vector>
#include <hash_map>
#include <string>
#include <memory>
#include "ORMTypedef.h"
#include "StringHelper.h"
#include <string.h>

using namespace SevenSmile::Util::StringHelper;
namespace SevenSmile{
	namespace ORM{

		class AbsTable;

		class AbsRow
			:public std::tr1::enable_shared_from_this<AbsRow>
		{
			friend class AbsTable;
		public:
			AbsRow(void);
			~AbsRow(void);

			virtual std::string GenerateNewSql();
			virtual std::string GenerateDeleteSql();
			virtual std::string GenerateUpdateSql();
			std::string GenerateSql(DATA_STATE i_state);

			void SetFields(SPtrIFields i_fields) ;
			SPtrIFields GetFields() { return _fields; }
			DBDatas& GetDatas() { return _datas; }

			void SetTableName(std::string i_name) { _tableName = i_name; }
			std::string GetTbaleName() { return _tableName; }

			const char* GetValueByFieldName(const std::string& i_fieldName) const;
			void SetValueByFieldName(const std::string& i_fieldName,const std::string& i_fieldValue);
			void InitValues();

			//bool Init() const { return _init; }
			//void Init(bool val) { _init = val; }

			//void ChangeDataState(DATA_STATE i_state);

			template<typename T>
			bool CompareValueByField(T i_val,std::string i_fieldName)
			{
				const char* rowValue = GetValueByFieldName(i_fieldName);
				std::string val = ToString(i_val);
				return !strcmp(val.c_str() ,rowValue);
			}
		protected:
			DBDatas _datas;
			SPtrIFields _fields;
			std::string	_tableName;

			//DATA_STATE _state;
			//bool _init;
		};
	}
}
