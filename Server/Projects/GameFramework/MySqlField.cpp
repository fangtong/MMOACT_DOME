#include "MySqlField.h"

using namespace SevenSmile::ORM::FieldUtil;

 /*
  * 是否是数字
 */
 bool IsNum(enum_field_types i_type)
 {
	 if(IS_NUM(i_type)){
		 return true;
	 }
	 return false;
 }
 /* 
  * 是否是日期 
 */
 bool IsDate(enum_field_types i_type)
 {
	if( FIELD_TYPE_DATE == i_type || 
		FIELD_TYPE_DATETIME == i_type ){
		return true;
	}
	return false; 
 }
 /* 
  * 是否是字符 
 */
 bool IsChar(enum_field_types i_type)
 {
	 if(i_type==FIELD_TYPE_STRING  ||
	   i_type==FIELD_TYPE_VAR_STRING ||
	   i_type==FIELD_TYPE_CHAR ){
		   return true;
	 }
	 return false;
 }
 /*
  * 是否为二进制数据
 */
 bool IsBlob(enum_field_types i_type)
 {
	 if(IS_BLOB(i_type)){
		return true;
	 }
	 return false;
 }


 SevenSmile::ORM::MySqlField::MySqlField( MYSQL_FIELD* _field )
 {
	 _tableName	= _field->table;
	 _fieldName	= _field->name;
	 _types		= _field->type;
	 _isPriKey	= IS_PRI_KEY(_field->flags) != 0;
 }