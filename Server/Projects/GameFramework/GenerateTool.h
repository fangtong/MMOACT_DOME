#pragma once
#include "DBCP.h"
using namespace SevenSmile::DBCPFramework;

class GenerateTool
{
public:
	GenerateTool(void);
	~GenerateTool(void);

public:
	void Generate(const char* i_cDBAddress, 
			  unsigned int i_uiDBPort, 
			  const char* i_cDBUser, 
			  const char* i_cDBPass, 
			  const char* i_cDBName, 
			  const char* i_cDBSetName,
			  const char* i_savePath);
private:
	std::string GenerateHead(string tableName,string fieldName,unsigned int fieldType,unsigned int fieldFlags);
	std::string GenerateCpp(string tableName,string fieldName,unsigned int fieldType,unsigned int fieldFlags);
};