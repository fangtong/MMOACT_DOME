#include "DBDateManage.h"
#include "DBCP.h"
#include "AbsRow.h"
#include "ORMTypedef.h"
#include "MySqlTable.h"
#include "FinalClass.h"
#include "AbsProxyDBData.h"

using namespace SevenSmile::DBCPFramework;
using namespace SevenSmile::ORM;

SevenSmile::ORM::DBDateManage::DBDateManage(void)
{
	ClearUpDateList();
}

SevenSmile::ORM::DBDateManage::~DBDateManage(void)
{
	ClearUpDateList();
	_sPtrDBCP = NullShardPtr(DBCP);
}

void SevenSmile::ORM::DBDateManage::AddToUpDateList( SPtrAbsDBProxy i_add )
{
	UpDateList::Lock lock(_setUpDataList);
	_setUpDataList.insert(i_add);
}

void SevenSmile::ORM::DBDateManage::DelToUpDateList( SPtrAbsDBProxy i_del )
{
	UpDateList::Lock lock(_setUpDataList);
	UpDateList::iterator it =  _setUpDataList.find(i_del);
	if(it == _setUpDataList.end()) return;
	_setUpDataList.erase(it);
}

SevenSmile::ORM::SPtrAbsDBProxy SevenSmile::ORM::DBDateManage::FindUpDateList( SPtrAbsDBProxy i_find )
{
	SPtrAbsDBProxy res;
	UpDateList::Lock lock(_setUpDataList);
	UpDateList::iterator it = _setUpDataList.find(i_find);
	if(it != _setUpDataList.end()) { res = *it; }
	return res;
}

void SevenSmile::ORM::DBDateManage::UpDate()
{
	UpDateList copyUpDateList;

	{
		UpDateList::Lock lock(_setUpDataList);
		if(_setUpDataList.size() == 0) return;
		copyUpDateList.swap(_setUpDataList);
	}
	

	_sPtrDBCP->QueryWithoutRes("set autocommit=0");
	_sPtrDBCP->QueryWithoutRes("START TRANSACTION");

	SPtrAbsDBProxy sPtrAbsDBProxy;
	SPtrAbsRow sPtrAbsRow;
	for (UpDateList::iterator it = copyUpDateList.begin();
		it != copyUpDateList.end(); it ++)
	{
		sPtrAbsDBProxy = (*it);
		if(!sPtrAbsDBProxy) continue;
		sPtrAbsRow = sPtrAbsDBProxy->GetRow();
		if(!sPtrAbsRow) continue;
		sPtrAbsDBProxy->DBDataFromData();
		std::string sql = sPtrAbsRow->GenerateSql(sPtrAbsDBProxy->GetDataState());
		if(sql == "" || !_sPtrDBCP) continue;
		if(!sPtrAbsDBProxy->Init()) sPtrAbsDBProxy->Init(true);
		sPtrAbsDBProxy->ChangeDataState(DATA_STATE_NOCHANGE);
		sPtrAbsDBProxy->InitRowValues();
#ifdef _DEBUG
		//printf("DBDateManage UpData >> %s\n",sql.c_str());
#endif // _DEBUG
		//_sPtrDBCP->QueryUseAllConnsWithoutRes(sql.c_str());
		_sPtrDBCP->QueryWithoutRes(sql.c_str());
	}
	//Result* resQueryTable = _sPtrDBCP->Query("COMMIT");
	_sPtrDBCP->QueryWithoutRes("COMMIT");
	_sPtrDBCP->QueryWithoutRes("set autocommit=1");
	copyUpDateList.clear();
	//ClearUpDateList();
}

void SevenSmile::ORM::DBDateManage::ClearUpDateList()
{
	UpDateList::Lock lock(_setUpDataList);
	_setUpDataList.clear();
}

void SevenSmile::ORM::DBDateManage::InitDataBase()
{
	printf("Load DataBase\n");
	const char* sqlQueryTables = "show tables";
	Result* resQueryTables = _sPtrDBCP->Query(sqlQueryTables);
	Row rowQueryTables = 0;
	while ((rowQueryTables = resQueryTables->NextRow()) != 0)
	{
		std::string tableName = rowQueryTables[0];
		SptrAbsTable table = SptrAbsTable(new MySqlTable(tableName));
		table->InitTable();
		//table->LoadTable();
		_hashNameToTable[tableName] = table;
	}
	_sPtrDBCP->FreeResult(resQueryTables);
}

SevenSmile::ORM::SptrAbsTable SevenSmile::ORM::DBDateManage::GetTableByName( std::string i_name )
{
	SptrAbsTable res;
	HashNameToTable::iterator it = _hashNameToTable.find(i_name);
	if(it != _hashNameToTable.end()) res = it->second;
	return res;
}

void SevenSmile::ORM::DBDateManage::UpdateDataBase()
{
	HashNameToTable::iterator it = _hashNameToTable.begin();
	SptrAbsTable table;
	for (it; it!= _hashNameToTable.end(); it++)
	{
		table = it->second;
		if(!table) continue;
		table->LoadTable();
	}
}
