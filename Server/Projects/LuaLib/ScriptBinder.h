
#ifndef _SCRIPT_BINDER_H_
#define _SCRIPT_BINDER_H_

#include <string>
#include "../Util/_UTIL_DEFINE.h"
#include <vector>
#include "StringHelper.h"
using namespace std;
using namespace SevenSmile::Util::StringHelper;

extern "C" 
{
	#include "lua.h"
	#include "lauxlib.h"


}
//! �ű�����
/*! */
class ScriptBinder
{
	public:
		//! ����
		ScriptBinder( lua_State* _L ): m_L( _L ) {}
		//! push ��
		void PushNumber( double _n )
		{
			 lua_pushnumber(m_L, _n );
		}
		//! ����
		double CheckNumber( int _idx )
		{
			 return luaL_checknumber( m_L, _idx );
		}
		//! push bool
		void PushBool( bool _b )
		{
			lua_pushboolean( m_L,  _b );
		}
		//! ��bool
		bool CheckBool( int _idx )
		{
			if( lua_isboolean( m_L, _idx ) == LUA_TBOOLEAN )
			{
				bool ret = lua_toboolean( m_L , _idx ) == 0 ? false : true;
				return ret;
			}
			return false;
		}
		//! push Nil
		void PushNil()
		{
			lua_pushnil( m_L );
		}
		//! push �ַ���
		void PushString( char const* _s )
		{
			lua_pushstring( m_L, _s );
		}
		//! �����ַ���
		char const* CheckString( int _idx )
		{
			return luaL_checkstring( m_L, _idx );
		}

		uint64 toUint64( string const& _str )
		{
			uint64 k;
			sscanf_s( _str.c_str(), "%llu", &k );
			return k;
		}

		//! ����id
		uint64 CheckEntityId( int _idx )
		{
			char const* str = CheckString( _idx );
			return toUint64( str );
		}

		string& toString( uint64 _id, string& _out )
		{
			string str = ToString(_id);
			_out = str;
			return _out;
		}
		
		void PushEntityId( uint64 _idx )
		{
			string str;
			toString(  _idx , str );
			lua_pushstring( m_L, str.c_str() );
		}

		
		//void GetTableItem(const char* lpszTableName,std::vector<string>& r_table)
		void GetTableItem(int idx,std::vector<string>& r_table)
		{
			//lua_getglobal(m_L, lpszTableName);		// 获取Table
			//if (!lua_istable(m_L, -1)) { return;  }
			//int idx = lua_gettop(m_L);                         // 获取栈顶，此时Table肯定在栈�?
			if (!lua_istable(m_L, idx)) { return;  }
			lua_pushnil(m_L);
			while (lua_next(m_L, idx) != 0)					  // 开始枚举，并把枚举到的值压入栈
			{			
				/* table的键�?2位置，值在-1位置 */
				//lua_tostring(m_L, -2);

				r_table.push_back(lua_tostring(m_L, -1));
				lua_pop(m_L, 1);     
			}
			lua_pop(m_L, 1); 
		}
		void Create_lib( char const* _tname, luaL_Reg const* _flist,int (*destroy)(lua_State* ) );
		//! ��ʼ��
		void Init( char const* _tname, char const* _bname1, char const* _bname2, luaL_Reg const* _flist, int (*destroy)(lua_State*) );
		//! ���÷���
		void SetOperator( char const* _tname, char const* _operator, int (*opt)(lua_State*) );
		//! push �û�����
		void PushUserType( void* _udata, char const* _tname );
		//! �� �û�����
		void* CheckUserType( int _idx, char const* _tname );
		//
public:
		//! lua ״̬
		lua_State* m_L;
};

#endif
