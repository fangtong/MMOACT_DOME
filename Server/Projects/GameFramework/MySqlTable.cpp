#include "MySqlTable.h"
#include "DBDateManage.h"
#include "MySqlField.h"
#include "DBCP.h"
#include "AbsRow.h"
#include <sstream>
#include "AbsTable.h"
#include "ifields.h"

using namespace SevenSmile::ORM;
using namespace SevenSmile::DBCPFramework;

SevenSmile::ORM::MySqlTable::MySqlTable( std::string i_tableName )
:AbsTable(i_tableName)
{

}

MySqlTable::~MySqlTable(void)
{
}

void SevenSmile::ORM::MySqlTable::InitTable()
{
	DBDateManage::SPtrDBCP sPtrDBCP = DBDateManager->GetSPtrDBCP();


	{
		//初始化字段信息
		std::stringstream sql;
		sql << "SELECT * FROM " << _tableName << " LIMIT 0,1";
		Result* result = sPtrDBCP->Query(sql.str().c_str());

		if(_sPtrFields->GetFieldSize() == 0)
		{
			MYSQL_FIELD*  field=0;
			while((field=mysql_fetch_field(result->GetRes())) != 0)
			{
				SPtrIField iField = SPtrIField(new MySqlField(field));
				_sPtrFields->PushField(iField);
			}
		}
		sPtrDBCP->FreeResult(result);
	}

	do{
		//初始化自增量
		assert(_sPtrFields->GetPriFieldPos() != _sPtrFields->GetFieldSize());
		std::stringstream sql;
		sql << "SELECT MAX("<< _sPtrFields->GetPriFieldName() <<") FROM " << _tableName ;
		Result* result = sPtrDBCP->Query(sql.str().c_str());
		if (result && result->RowCount() > 0) 
		{
			Row row=result->NextRow();
			_keyValueCount = row[0]?(PRIMARY)atoi(row[0]):0;
		}
		sPtrDBCP->FreeResult(result);
	}while(0);

}

void SevenSmile::ORM::MySqlTable::LoadTable()
{
	DBDateManage::SPtrDBCP sPtrDBCP = DBDateManager->GetSPtrDBCP();
	std::string sql = GenerateLoadSql();
	Result* result = sPtrDBCP->Query(sql.c_str());
	
#ifdef _DEBUG
	printf("Load Table :%s MaxCount: %I64u! Start\n" ,_tableName.c_str(),result->RowCount());
	MEMORYSTATUSEX statex;
	DWORDLONG ullAvailPhys = 0;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);
	ullAvailPhys = statex.ullAvailPhys;
#endif // _DEBUG
	do 
	{
		size_t priPos =_sPtrFields->GetPriFieldPos();
		if(priPos == _sPtrFields->GetFieldSize())
		{
			printf("Error Table: %s not has PRIMARY!\n",_tableName.c_str());
			break;
		}
		//取出数据
		PRIMARY key = 0;
		if (0!=result && result->RowCount()!=0)
		{
			Row row=result->NextRow();
			while (row)
			{
				key = (PRIMARY)atoi(row[priPos]);

				SPtrAbsRow sPtrAbsRow = GetRowByPriKey(key);
				if(!sPtrAbsRow) sPtrAbsRow = GenerateRow();

				DBDatas& datas = sPtrAbsRow->GetDatas();
				//SPtrIField iField;
				for (unsigned int i=0;i<_sPtrFields->GetFieldSize();i++)
				{
					//iField = _fields[i];
					datas[i] = row[i]?row[i]:"";

				}
				//sPtrAbsRow->Init(true);
				_hashKeyToRow[key] = sPtrAbsRow;
				row=result->NextRow();
			}
		}
	} while (0);
	sPtrDBCP->FreeResult(result);
#ifdef _DEBUG
	GlobalMemoryStatusEx (&statex);
	ullAvailPhys -= statex.ullAvailPhys;
	ullAvailPhys = ullAvailPhys/1024;
	printf("Load Table :%s Complete! \n" ,_tableName.c_str());
	printf("	--Use %*I64d Kbytes of physical memory.\n",7, ullAvailPhys);
	if(_keyValueCount != 0)
	printf("	--Use %*I64d Kbytes by count:%*I64u!\n",3,ullAvailPhys/_keyValueCount,7,_keyValueCount);
#endif // _DEBUG

}
