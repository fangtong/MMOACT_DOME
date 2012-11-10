#include "AbsRow.h"
#include <sstream>
#include "DBDateManage.h"
#include <assert.h>
#include "ifields.h"
#include "FinalClass.h"

using namespace SevenSmile::ORM;
SevenSmile::ORM::AbsRow::AbsRow()
//:_state(DATA_STATE_NOCHANGE),
//_init(false)
{
	_fields = NullShardPtr(IFields);
}

SevenSmile::ORM::AbsRow::~AbsRow( void )
{
	_fields = NullShardPtr(IFields);
	_datas.clear();
	//DBDateManager->DelToUpDateList(shared_from_this());
}

std::string SevenSmile::ORM::AbsRow::GenerateUpdateSql()
{
	std::stringstream str;
	str << " UPDATE " << _tableName.c_str() << " SET ";
	unsigned int le = _fields->GetFieldSize();
	std::string data = "";
	size_t priKeyPos = _fields->GetPriFieldPos();
	for(unsigned int i = 0 ; i < le; i++)
	{
		data = _datas[i];
		str << _fields->GetNameByFieldPos(i).c_str() << " = '" << data.c_str() << "' ";
		if(i != le - 1) str << ", ";
	}
	if(priKeyPos == le) return "";
	str << " WHERE ";
	str << _fields->GetPriFieldName().c_str() << " = '" << _datas[priKeyPos].c_str() << "'";
	return str.str();
}

std::string SevenSmile::ORM::AbsRow::GenerateNewSql()
{
	std::stringstream str;
	assert(_datas.size() == _fields->GetFieldSize());
	str << "INSERT INTO " << _tableName.c_str() << " (";
	//fields
	unsigned int le = _fields->GetFieldSize();
	for(unsigned int i = 0 ; i < le; i++)
	{
		str << _fields->GetNameByFieldPos(i).c_str()  ;
		if(i != le - 1) str << ", ";
	}

	str << " )VALUES( ";
	//values
	le = _datas.size();
	std::string data = "";
	for(unsigned int i = 0 ; i < le; i++)
	{
		data = _datas[i];
		str << "'" << data.c_str() << "'" ;
		if(i != le - 1) str << ", ";
	}
	str << ")";
	return str.str();
}

std::string SevenSmile::ORM::AbsRow::GenerateDeleteSql()
{
	std::stringstream str;
	assert(_datas.size() == _fields->GetFieldSize());
	size_t le = _fields->GetFieldSize();
	size_t priKeyPos = _fields->GetPriFieldPos();
	if (priKeyPos == le) return "";
	str << "DELETE FROM " << _tableName.c_str() << " ";
	str << " WHERE ";
	str << _fields->GetPriFieldName().c_str() << " = '" << _datas[priKeyPos].c_str() << "'";
	return str.str();
}

std::string SevenSmile::ORM::AbsRow::GenerateSql(DATA_STATE i_state)
{
	std::string sql("");
	switch(i_state)
	{
	case DATA_STATE_NEW:
		{
			sql = GenerateNewSql();
		}break;
	case DATA_STATE_DELETE:
		{
			sql = GenerateDeleteSql();
		}break;
	case DATA_STATE_UPDATE:
		{
			sql = GenerateUpdateSql();
		}break;
	}
	return sql;
}

//
//void SevenSmile::ORM::AbsRow::ChangeDataState( DATA_STATE i_state )
//{
//	//保持新数据 
//	if (!_init && i_state != DATA_STATE_NEW) return;
//	//保持删除数据
//	if(_state == DATA_STATE_DELETE && i_state != DATA_STATE_NOCHANGE) return;
//	_state = i_state;
//	if(_state != DATA_STATE_NOCHANGE){
//		DBDateManager->AddToUpDateList(shared_from_this());
//	}
//}

const char* SevenSmile::ORM::AbsRow::GetValueByFieldName(const std::string& i_fieldName ) const
{
	const char* res = "";
	size_t pos = _fields->GetPosByFieldName(i_fieldName);
	do 
	{
		if(pos == _datas.size()) 
		{
			printf("Field Not Have!%s!\n",i_fieldName.c_str());
			break;
		}
		res = _datas[pos].c_str();
	} while (0);
	return res;
}

void SevenSmile::ORM::AbsRow::SetValueByFieldName(const std::string& i_fieldName,const std::string& i_fieldValue )
{
	size_t pos = _fields->GetPosByFieldName(i_fieldName);
	if(pos != _datas.size())
	{
		_datas[pos] = i_fieldValue;
	}
	//ChangeDataState(DATA_STATE_UPDATE);
}

void SevenSmile::ORM::AbsRow::SetFields( SPtrIFields i_fields )
{
	_fields = i_fields;
	InitValues();
}

void SevenSmile::ORM::AbsRow::InitValues()
{
	//init data
	_datas.clear();
	size_t le = _fields?_fields->GetFieldSize():0;
	if(le != 0) 
	{
		_datas.insert(_datas.begin(),le,"");
	}
}