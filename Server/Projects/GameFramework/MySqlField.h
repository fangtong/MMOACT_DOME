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
			  * �Ƿ�������
			 */
			 bool IsNum(enum_field_types i_type);
			 /* 
			  * �Ƿ������� 
			 */
			 bool IsDate(enum_field_types i_type);
			 /* 
			  * �Ƿ����ַ� 
			 */
			 bool IsChar(enum_field_types i_type);
			 /*
			  * �Ƿ�Ϊ����������
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