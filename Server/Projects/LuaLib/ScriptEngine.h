
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
//! 脚本Engine
/*! */
class ScriptEngine
{
	public:
		//!构造
		ScriptEngine();
		//!析构
		virtual ~ScriptEngine();
		//! 初始化池子
		bool InitPool( std::string const& _path, std::string const& _file ,lua_CFunction _InitScriptEngine);
		//! 运行池子
		bool Exec( std::string const& _script );
		//! 运行脚本
		bool ExecFile(std::string const& _path, std::string const& _file );
		//! 加载函数
		bool FuncEnv(  std::string const& _script, std::string const& _func  );
		//! 加载函数文件
		bool FuncFileEnv( std::string const& _path, std::string const& _file, std::string const& _func  );	
		//! 执行函数
		bool ExecFunc( int _param_c, int _ret_c );
		//! push 数
		template <class T>
		void PushNumber( char const* _vname, T _value )
		{
			lua_pushnumber( m_vm, _value );
			lua_setglobal( m_vm, _vname );
		}
		//! push 数
		template <class T>
		void PushNumber( T _value )
		{
			lua_pushnumber( m_vm, _value );
		}
		//! push 字符串
		inline void PushString( char const* _vname, char const* _value )
		{
			lua_pushstring( m_vm, _value );
			lua_setglobal( m_vm, _vname );
		}
		//! push 字符
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
		//!  pop 数
		template <class T>
		T PopNumber(T _t)
		{
			 T t ;
			 t = (T)(lua_tonumber(m_vm, -1 ));
			 lua_pop( m_vm, 1 );
			 return t;
		}
		//! pop 字符
		inline std::string PopString()
		{
			std::string t;
			t = lua_tostring( m_vm, -1 );
			lua_pop( m_vm, 1 );
			return t;
		}

		void GetTableItem(const char* lpszTableName,std::vector<string>& r_table)
		{
			lua_getglobal(m_vm, lpszTableName);		// 鑾峰彇Table
			if (!lua_istable(m_vm, -1)) { return;  }
			int it = lua_gettop(m_vm);                         // 鑾峰彇鏍堥《锛屾鏃禩able鑲畾鍦ㄦ爤椤?
			lua_pushnil(m_vm);
			while (lua_next(m_vm, it) != 0)					  // 寮�濮嬫灇涓撅紝骞舵妸鏋氫妇鍒扮殑鍊煎帇鍏ユ爤
			{			
				/* table鐨勯敭鍦?2浣嶇疆锛屽�煎湪-1浣嶇疆 */
				r_table.push_back(lua_tostring(m_vm, -1));
				lua_pop(m_vm, 1);     
			}
			lua_pop(m_vm, 1); 
		}
	protected:
		//! 加载数据
		bool Load( std::string const& _file, std::string& _code );
					
	protected:
		//! lua state
		lua_State* 	m_vm;
		//! 映射
		std::map<std::string, std::string> m_scPool;
		//
};

typedef std::tr1::shared_ptr<ScriptEngine>	ScriptEngine_ptr;

#endif
