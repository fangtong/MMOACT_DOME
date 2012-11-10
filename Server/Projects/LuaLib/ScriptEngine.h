
#ifndef _SCRIPT_ENGINE_H_
#define _SCRIPT_ENGINE_H_

#include <map>
#include "../Util/_UTIL_DEFINE.h"
#include <string>
#include <hash_map>
#include "StringHelper.h"
using namespace std;
using namespace stdext;
using namespace SevenSmile::Util::StringHelper;


extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"


}

//typedef int (*lua_CFunction) (lua_State *L);
//! �ű�Engine
/*! */
class ScriptEngine
{
	public:
		//!����
		ScriptEngine();
		//!����
		virtual ~ScriptEngine();
		//! ��ʼ������
		bool InitPool( std::string const& _path, std::string const& _file ,lua_CFunction _InitScriptEngine);
		//! ���г���
		bool Exec( std::string const& _script );
		//! ���нű�
		bool ExecFile(std::string const& _path, std::string const& _file );
		//! ���غ���
		bool FuncEnv(  std::string const& _script, std::string const& _func  );
		//! ���غ����ļ�
		bool FuncFileEnv( std::string const& _path, std::string const& _file, std::string const& _func  );	
		//! ִ�к���
		bool ExecFunc( int _param_c, int _ret_c );
		//! push ��
		template <class T>
		void PushNumber( char const* _vname, T _value )
		{
			lua_pushnumber( m_vm, _value );
			lua_setglobal( m_vm, _vname );
		}
		//! push ��
		template <class T>
		void PushNumber( T _value )
		{
			lua_pushnumber( m_vm, _value );
		}
		//! push �ַ���
		inline void PushString( char const* _vname, char const* _value )
		{
			lua_pushstring( m_vm, _value );
			lua_setglobal( m_vm, _vname );
		}
		//! push �ַ�
		inline void PushString( char const* _value )
		{
			lua_pushstring( m_vm, _value );
		}

		string& toString( uint64 _id, string& _out )
		{
			char str[21] = {0};
			sprintf_s( str,"%I64x", _id );
			_out = str;
			return _out;
		}

		//! push id
		inline void PushEntityId( uint64 _id )
		{
			string out;
			toString( _id, out );
			PushString( out.c_str() );
		}
		//!  pop ��
		template <class T>
		T PopNumber(T _t)
		{
			 T t ;
			 t = (T)(lua_tonumber(m_vm, -1 ));
			 lua_pop( m_vm, 1 );
			 return t;
		}
		//! pop �ַ�
		inline std::string PopString()
		{
			std::string t;
			t = lua_tostring( m_vm, -1 );
			lua_pop( m_vm, 1 );
			return t;
		}

		void GetTableItem(const char* lpszTableName,std::vector<string>& r_table)
		{
			lua_getglobal(m_vm, lpszTableName);		// 获取Table
			if (!lua_istable(m_vm, -1)) { return;  }
			int it = lua_gettop(m_vm);                         // 获取栈顶，此时Table肯定在栈�?
			lua_pushnil(m_vm);
			while (lua_next(m_vm, it) != 0)					  // 开始枚举，并把枚举到的值压入栈
			{			
				/* table的键�?2位置，值在-1位置 */
				r_table.push_back(lua_tostring(m_vm, -1));
				lua_pop(m_vm, 1);     
			}
			lua_pop(m_vm, 1); 
		}
	protected:
		//! ��������
		bool Load( std::string const& _file, std::string& _code );
					
	protected:
		//! lua state
		lua_State* 	m_vm;
		//! ӳ��
		std::map<std::string, std::string> m_scPool;
		//
};

typedef std::tr1::shared_ptr<ScriptEngine>	ScriptEngine_ptr;

#endif
