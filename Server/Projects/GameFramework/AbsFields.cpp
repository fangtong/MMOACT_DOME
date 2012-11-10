#include "AbsFields.h"
#include "IField.h"
#include "FinalClass.h"

using namespace SevenSmile::ORM;

SevenSmile::ORM::AbsFields::AbsFields()
{
	_priField = NullShardPtr(IField);
}

SevenSmile::ORM::AbsFields::~AbsFields()
{
	_priField = NullShardPtr(IField);
	_fields.clear();
	_fieldNameToPos.clear();
}

std::string SevenSmile::ORM::AbsFields::GetTableName() const
{
	return _tableName;
}

void SevenSmile::ORM::AbsFields::SetTableName( std::string& i_tableName )
{
	_tableName = i_tableName;
}

size_t SevenSmile::ORM::AbsFields::GetPosByFieldName(const std::string& i_fieldName )
{
	size_t res = _fields.size();
	FieldNameToPos::iterator it = _fieldNameToPos.find(i_fieldName);
	if(it != _fieldNameToPos.end())
	{
		res = it->second;
	}
	return res;
}

std::string SevenSmile::ORM::AbsFields::GetNameByFieldPos( int i_fieldPos ) const
{
	if(i_fieldPos <0 || i_fieldPos > _fields.size()) return "";
	return _fields[i_fieldPos]->GetFieldName();
}

 void	 SevenSmile::ORM::AbsFields::PushField(SPtrIField i_field)	
{
	_fields.push_back(i_field);	
	_fieldNameToPos[i_field->GetFieldName()] = _fields.size()-1;
	if (i_field->FieldIsPriKey()) _priField = i_field;
 }

 size_t SevenSmile::ORM::AbsFields::GetPriFieldPos()
 {
	size_t res = _fields.size();
	if(_priField)
	{
		res = GetPosByFieldName(_priField->GetFieldName());
	}
	return res;
 }

 size_t SevenSmile::ORM::AbsFields::GetFieldSize() const
 {
	return _fields.size();
 }
 std::string SevenSmile::ORM::AbsFields::GetPriFieldName() const
 {
	if(!_priField) return "";
	return _priField->GetFieldName();
 }