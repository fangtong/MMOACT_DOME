#include "AbsTable.h"
#include <sstream>
#include "DBCP.h"
#include "DBDateManage.h"
#include "IField.h"
#include "AbsRow.h"
#include "StringHelper.h"
#include "FinalClass.h"
#include "ifields.h"
#include "absfields.h"

using namespace SevenSmile::Util::StringHelper;
using namespace SevenSmile::ORM;
using namespace SevenSmile::DBCPFramework;

AbsTable::~AbsTable(void)
{
	_hashKeyToRow.clear();
	_sPtrFields = NullShardPtr(IFields);
}

SevenSmile::ORM::AbsTable::AbsTable( std::string i_tableName )
:_tableName(i_tableName),
_keyValueCount(0)
{
	_sPtrFields = SPtrIFields(new AbsFields());
	_sPtrFields->SetTableName(_tableName);
}

//void SevenSmile::ORM::AbsTable::LoadTable()
//{
//	DBDateManage::SPtrDBCP sPtrDBCP = DBDateManager->GetSPtrDBCP();
//	std::string sql = GenerateLoadSql();
//	Result* result = sPtrDBCP->Query(sql.c_str());
//
//	//取出字段配置
//
//	MYSQL_FIELD*  field=0;unsigned int pos = 0;
//	DBFields dbFields;
//	while((field=mysql_fetch_field(result->GetRes())) != 0)
//	{
//		spt
//	}
//	
//	//取出数据
//	if (0!=result && result->RowCount()!=0)
//	{
//		Row row=result->NextRow();
//		while (row)
//		{
//			SPtrAbsRow sPtrAbsRow = GenerateRow();
//			PRIMARY key ;
//			for (unsigned int i=0;i<pos;i++)
//			{
//				field = fields[i];
//				SPtrIField sPtrIField = sPtrAbsRow->_hashFields[field->name];
//				if(sPtrIField) sPtrIField->SetFieldValue(row[i],field->type);
//				if(IS_PRI_KEY(field->flags))
//				{
//					key = (PRIMARY)atoi(row[i]);
//					if(key > _keyValueCount) _keyValueCount = key;
//				}
//			}
//			_hashKeyToRow[key] = sPtrAbsRow;
//			row=result->NextRow();
//		}
//	}
//	sPtrDBCP->FreeResult(result);
//}

std::string SevenSmile::ORM::AbsTable::GenerateLoadSql()
{
	std::stringstream str;
	str << "SELECT * FROM " << _tableName ;
	return str.str();
}

SevenSmile::ORM::SPtrAbsRow SevenSmile::ORM::AbsTable::GenerateRow()
{
	SPtrAbsRow res = SPtrAbsRow(new AbsRow());
	res->SetTableName(_tableName);
	res->SetFields(_sPtrFields);
	return res;
}


//SevenSmile::ORM::SPtrAbsRow SevenSmile::ORM::AbsTable::CreateNewRow()
//{
//	SPtrAbsRow res = GenerateRow();
//	int priKeyPos = -1;
//	SPtrIField iField;
//	for(size_t pos = 0; pos < _fields.size();pos++)
//	{
//		iField = _fields[pos];
//		if(iField->FieldIsPriKey())
//		{
//			priKeyPos = pos;
//			break;
//		}
//	}
//	if (priKeyPos != -1)
//	{
//		_keyValueCount++;
//		res->_datas[priKeyPos] = SevenSmile::Util::StringHelper::ToString(_keyValueCount);
//		_hashKeyToRow[_keyValueCount] = res;
//	}
//	res->ChangeDataState(DATA_STATE_NEW);
//	return res;
//}

SevenSmile::ORM::PRIMARY SevenSmile::ORM::AbsTable::GenerateNewPrimary()
{
	_keyValueCount++;
	return _keyValueCount;
}

unsigned int SevenSmile::ORM::AbsTable::GetPriFieldPos()
{
	return _sPtrFields->GetPriFieldPos();
}

SevenSmile::ORM::SPtrAbsRow SevenSmile::ORM::AbsTable::GetRowByPriKey( PRIMARY i_key )
{
	SPtrAbsRow res = NullShardPtr(AbsRow);
	HashKeyToRow::iterator it = _hashKeyToRow.find(i_key);
	if(it != _hashKeyToRow.end())
	{
		res = it->second;
	}
	return res;
}

SevenSmile::ORM::SPtrAbsRow SevenSmile::ORM::AbsTable::GenerateNewRow()
{
	SPtrAbsRow sPtrAbsRow = GenerateRow();
	unsigned int priPos = GetPriFieldPos();
	PRIMARY key = GenerateNewPrimary();
	DBDatas& datas = sPtrAbsRow->GetDatas();
	datas[priPos] = ToString(key).c_str();
	_hashKeyToRow[key] = sPtrAbsRow;
	return sPtrAbsRow;
}
