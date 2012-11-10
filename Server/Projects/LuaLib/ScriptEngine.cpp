#include <stdio.h>
#include <map>
#include "scriptEngine.h"
#include "tinyxml.h"


extern "C"
{
}


#include <string>
using namespace std;

//
ScriptEngine::ScriptEngine()
{
	m_vm = 0;
}
//
ScriptEngine::~ScriptEngine()
{
	if( m_vm != 0 )
	{
		lua_close( m_vm );
	}
}
//

bool ScriptEngine::InitPool( std::string const& _path, std::string const& _file ,lua_CFunction _InitScriptEngine)
{
	//
	if( m_vm == 0 )
	{
		m_vm = lua_open();
		if( m_vm == NULL )
		{
			return false;
		}
	}

	lua_pushcfunction( m_vm , _InitScriptEngine );

	if( lua_pcall( m_vm,0,0,0) ) 
	{
		//GameInstance::getSingleton().log().write( "./script", "ScripEngine:: InitScriptEngine failed. %s \n", lua_tostring(m_vm, -1) );
		printf("ScripEngine:: InitScriptEngine failed. %s \n" , lua_tostring(m_vm, -1));
		return 1;
	}
	//
	TiXmlDocument  mapf;
	string file = _path + _file;

	if( !mapf.LoadFile( file.c_str() ) )
	{
		return false;
	}
	TiXmlElement* 	cvalue = 0;
	TiXmlElement* 	root = mapf.RootElement();
	cvalue = root->FirstChildElement();
	
	std::string key;
	std::string code;
	for( ; cvalue != 0; cvalue = cvalue->NextSiblingElement() )
	{
		key = cvalue->Attribute("key");
		if( Load( _path+cvalue->Attribute("scriptfile"), code ) )
		{
			m_scPool[key] = code;
		}
		else
		{
			printf("ScripEngine:: %s failed.\n", cvalue->Attribute("scriptfile"));
			//GameInstance::getSingleton().log().write( "./script", "ScripEngine:: %s failed.\n", cvalue->Attribute("file") );
		}	
	}
	return true;
}

//
bool ScriptEngine::Load( std::string const& _file, std::string& code )
{
	FILE* file = 0;
	file = fopen( _file.c_str(), "r" );
	if( file != 0 )
	{
		long length = 0;
		fseek( file, 0, SEEK_END );
		length = ftell( file );
		fseek( file, 0 , SEEK_SET );
		if( length > 0 )
		{
			char *buf = new char[length+1];
			memset( buf, 0 ,sizeof(char)*(length+1) );
			//if( fread( buf, length, 1, file) == 1 )
			if( fread( buf, 1, length, file) != 0 )
			{
				code = buf;
				delete [] buf;
				fclose( file );
				return true;
			}
			delete [] buf;
		}
		fclose(file);
	}
	printf("ScripEngine:: %s Load.err\n", _file.c_str());
	return false;
}
//
bool ScriptEngine::Exec( std::string const& _script )
{
	std::string code;
	if( !_script.empty() )
	{
		//code.clear();
		code = m_scPool[_script];//从容器中取出lua脚本内容
		if( code.length() > 0 )
		{
			int error = luaL_loadbuffer( m_vm, code.c_str(), code.length(), _script.c_str() ) ||
				lua_pcall( m_vm, 0, LUA_MULTRET, 0 );
			if( error )
			{
				//GameInstance::getSingleton().log().write( "./script", "ScripEngine:: %s.\n", lua_tostring(m_vm, -1) );
				lua_pop( m_vm, 1 );
				return false;
			}	
			return true;
		}
	}
	return false;
}
//
bool ScriptEngine::ExecFile( std::string const& _path, std::string const& _file )
{
	std::string path;
	if( !_file.empty() )
	{
		path = _path +_file;
		int error = luaL_loadfile(m_vm, path.c_str()) || lua_pcall(m_vm, 0, LUA_MULTRET, 0);
		if( error ) 
		{
			//GameInstance::getSingleton().log().write( "./script", "ScripEngine:: %s.\n", lua_tostring(m_vm, -1) );
			lua_pop( m_vm, 1 );
			return false;
		}
		return true;
	}
	return false;
}
//加载函数
bool ScriptEngine::FuncEnv( std::string const& _script, std::string const& _func  )
{
	std::string code;
	if( !_script.empty() && !_func.empty() )
	{
		//		code.clear();
		code = m_scPool[_script]; 
		if( code.length() > 0 )
		{

			int error = luaL_loadbuffer( m_vm, code.c_str(), code.length(), _func.c_str() ) ;

			if (error !=0)
			{
				//GameInstance::getSingleton().log().write( "./script", "ScriptEngine.cpp:198 In %s, ScripEngine:: %s.\n", 
				//		_script.c_str(), lua_tostring(m_vm, -1) );
				printf("%s\n",lua_tostring(m_vm, -1));
				lua_pop( m_vm, 1 );
			}

			error = lua_pcall(m_vm, 0, 0, 0);

			if( error )
			{
				//GameInstance::getSingleton().log().write( "./script", "ScriptEngine.cpp In %s, ScripEngine:: %s.\n", 
				//		_script.c_str(), lua_tostring(m_vm, -1) );
				printf("%s\n",lua_tostring(m_vm, -1));
				lua_pop( m_vm, 1 );
			}
			else
			{
				lua_getglobal( m_vm, _func.c_str() );
				if( lua_isfunction(m_vm,-1) )
					return true;
				else
				{
					assert(0);
					lua_pop(m_vm,1);
				}
			}
		}
	}
	return false; 
}
//加载函数文件
bool ScriptEngine::FuncFileEnv(std::string const& _path, std::string const& _file, std::string const& _func  )
{
	std::string path;
	if( !_file.empty() )
	{
		path = _path+_file;
		int error = luaL_loadfile(m_vm, path.c_str());
		if( error ) 
		{
			//GameInstance::getSingleton().log().write( "./script", "ScripEngine:: %s.\n", lua_tostring(m_vm, -1) );

			lua_pop( m_vm, 1 );
		}
		else
		{
			lua_getglobal( m_vm, _func.c_str() );
			return true;
		}
	}
	return false;
}	
//执行函数
bool ScriptEngine::ExecFunc( int _param_c, int _ret_c )
{
	//printf("  ====>top=%d\n",lua_gettop(m_vm));
	int error = lua_pcall( m_vm, _param_c, _ret_c, 0);
	//printf("  ====>top=%d\n",lua_gettop(m_vm));	 
	if( error )
	{
		//GameInstance::getSingleton().log().write( "./script", "ScripEngine:: %s.\n", lua_tostring(m_vm, -1)  );
		printf("ScripEngine:: %s.\n", lua_tostring(m_vm, -1) );

		lua_pop( m_vm, 1 );
		return false;
	}
	return true;
}

