#pragma once
#include <string>
#include "ifields.h"

namespace SevenSmile{
	namespace ORM{
		class AbsFields
			:public IFields
		{
		public:
			AbsFields();
			~AbsFields();

			std::string			GetTableName() const;
			void					SetTableName(std::string& i_tableName);

			size_t				GetPosByFieldName(const std::string& i_fieldName);
			std::string			GetNameByFieldPos(int i_fieldPos) const;
			void					PushField(SPtrIField i_field);
			size_t				GetFieldSize() const;

			size_t				GetPriFieldPos();
			std::string			GetPriFieldName() const;
		private:
			std::string					_tableName;
			FieldNameToPos		_fieldNameToPos;
			DBFields					_fields;
			SPtrIField					_priField;
		};
	}
}