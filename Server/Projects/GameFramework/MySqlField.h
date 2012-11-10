#pragma once
#include <string>
#include "winsock.h"
#include "mysql.h"
#include "StringHelper.h"
#include "IField.h"


namespace SevenSmile{
	namespace ORM{

		namespace FieldUtil
		{
			 /*
			  * 是否是数字
			 */
			 bool IsNum(enum_field_types i_type);
			 /* 
			  * 是否是日期 
			 */
			 bool IsDate(enum_field_types i_type);
			 /* 
			  * 是否是字符 
			 */
			 bool IsChar(enum_field_types i_type);
			 /*
			  * 是否为二进制数据
			 */
			 bool IsBlob(enum_field_types i_type);
		}

		class MySqlField
			:public IField
		{
		public:
			explicit MySqlField(MYSQL_FIELD* _field);
			~MySqlField(){};

			std::string GetTableName() const { return _tableName; }
			std::string GetFieldName() const { return _fieldName; }
			bool		FieldIsPriKey() const { return _isPriKey; }
		private:
			std::string			_tableName;
			std::string			_fieldName;
			enum_field_types	_types;
			bool				_isPriKey;
		};
	}
}