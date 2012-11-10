#include "GenerateTool.h"
#include <sstream>

using namespace std;
GenerateTool::GenerateTool(void)
{
}

GenerateTool::~GenerateTool(void)
{
}

void GenerateTool::Generate(
	const char* i_cDBAddress, 
	unsigned int i_uiDBPort, 
	const char* i_cDBUser, 
	const char* i_cDBPass, 
	const char* i_cDBName, 
	const char* i_cDBSetName,
	const char* i_savePath)
{

	DBCP loadDBCP;
	loadDBCP.SetConnect(
		i_cDBAddress,
		i_uiDBPort,
		i_cDBUser,
		i_cDBPass,
		i_cDBName,
		i_cDBSetName);/*连接池连接属性*/
	//loadDBCP.SetThreadCPU(1);
	loadDBCP.OpenConnections(1);  /*线程数*/

	//查询所有表名
	std::vector<string> tableNames;
	{
		string sqlQueryTables = "show tables";
		Result* resQueryTables = loadDBCP.Query(sqlQueryTables.c_str());
		Row rowQueryTables = 0;
		while ((rowQueryTables = resQueryTables->NextRow()) != 0)
		{
			tableNames.push_back(string(rowQueryTables[0]));
		}
		loadDBCP.FreeResult(resQueryTables);
	}
	
	string tableName;
	for (size_t i = 0; i < tableNames.size(); i++)
	{
		tableName = tableNames[i];
		string sqlQueryFields = "select * from ";
		sqlQueryFields+=tableName;
		Result* resQueryFields = loadDBCP.Query(sqlQueryFields.c_str());

		//取出字段配置
		MYSQL_FIELD*  field=0;
		while((field=mysql_fetch_field(resQueryFields->GetRes())) != 0)
		{
			string tableName = field->table;
			string fieldName = field->name;
			unsigned int fieldType = field->type;
			unsigned int fieldFlags = field->flags;
			GenerateHead(tableName,fieldName,fieldType,fieldFlags);
			GenerateCpp(tableName,fieldName,fieldType,fieldFlags);
		}
		
		loadDBCP.FreeResult(resQueryFields);

		//写入文件
		string strFileNameh(i_savePath);
		strFileNameh+=tableName;
		strFileNameh+=".h";

		string strFileNamecpp(i_savePath);
		strFileNamecpp+=tableName;
		strFileNamecpp+=".cpp";
	}
}

std::string GenerateTool::GenerateHead( string tableName,string fieldName,unsigned int fieldType,unsigned int fieldFlags )
{
	stringstream ssHead;
	///*引进头*/
	//ssHead  << "#pragma once\n"
	//		<< "#include \"AbsRow.h\" \n\n"
	///*命名空间设定*/
	//		<< "namespace Tennis{\n";
	//		<< " namespace DBDataProxy{\n"
	///*声明class*/
	//		<< "  class " << tableName <<"\n";
	//		<< "  	:public AbsRow\n";
	//		<< "  { \n";
	//		<<"private: \n";
	//for (UINT ii=0;ii<childForeignKey.size();ii++)
	//{
	//	ssHead <<"	";	ssHead <<"DB"<<childForeignKey[ii]._strReferencedTableName;	ssHead <<"*";
	//	ssHead <<"	m_lp";  ssHead <<"DB"<<childForeignKey[ii]._strReferencedTableName;	ssHead <<";\n\n";
	//}
	//ssHead <<"\npublic: \n";
	//ssHead << "	";ssHead <<"DB"<<field.DBTableName.c_str() << "(void);\n";
	//ssHead << "	~";ssHead <<"DB"<<field.DBTableName.c_str() <<"(void);\n\n";

	//for (UINT ii=0;ii<field.m_type.size();ii++)
	//{
	//	ssHead <<"	" << getType(&field,ii) ;
	//	ssHead <<" " << field.m_name[ii];
	//	ssHead <<";\n";
	//}
	//ssHead <<"\n	static SQL_Base* CreateClass();\n";
	//for (UINT ii=0;ii<childForeignKey.size();ii++)
	//{
	//	ssHead <<"	";	ssHead << "DB"<<childForeignKey[ii]._strReferencedTableName << "*";
	//	ssHead <<" _lp";  ssHead <<"DB"<<childForeignKey[ii]._strReferencedTableName <<"();\n";
	//	ssHead <<"	void";
	//	ssHead <<" _lp";  ssHead <<"DB"<<childForeignKey[ii]._strReferencedTableName <<"(";
	//	ssHead <<"DB"<<childForeignKey[ii]._strReferencedTableName <<"* i_lp" <<"DB"<<childForeignKey[ii]._strReferencedTableName;
	//	ssHead <<");\n";
	//}

	//ssHead <<"\npublic:\n";
	//ssHead <<"	bool init(DBCP* db,char* i_charPrimaryKeyValue=0);\n";
	//ssHead <<"	string const toString();\n";

	//ssHead <<"};\n";

	//ssHead << endl;
	//fprintf_s(fp,ssHead.str().c_str());
	//ssHead.str("");
	return ssHead.str();
}

std::string GenerateTool::GenerateCpp( string tableName,string fieldName,unsigned int fieldType,unsigned int fieldFlags )
{
	string cpp;
	return cpp;
}