#include "scriptBinder.h"

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

static int SearchParent( lua_State* _L )
{
		lua_getfield( _L, -2, "_base" );
		lua_getfield( _L, -1,"__index");
		lua_pushvalue( _L, 2 );
		lua_gettable( _L, -2 );
		if( lua_isnil( _L, -1 ) )
		{
			lua_settop( _L, 2 );
			lua_getfield( _L, -2, "_base1");
			lua_getfield( _L, -1,"__index");
			lua_pushvalue( _L, 2 );
			lua_gettable( _L , -2 );
		}
		lua_replace( _L, 1 );
		lua_settop( _L, 1 );
		return 1;
		
			
		/*
		lua_getfield( _L, -2, "_base" );
		lua_pushvalue( _L, -2 );
		lua_rawget( 	_L, -2 );	
		if( lua_isnil( _L, -1 ) == LUA_TNIL )
		{
			lua_settop( _L, 2 );
			lua_getfield( _L, -2, "__base1" );
			lua_pushvalue( _L, -2 );
			lua_rawget( 	_L, -2 );	
		}
		lua_replace( _L, 1 );
		lua_settop( _L, 1 );
		return 1; 
		*/
}


void ScriptBinder::Create_lib( char const* _tname, luaL_reg const* _flist,int (*destroy)(lua_State* ) )
{

	
	if( m_L != 0 )
	{

		lua_newtable( m_L );
		
		lua_getglobal(m_L,"_G");
		lua_setfield( m_L, -2, "__index" );
		
		lua_pushstring( m_L, "v");
		lua_setfield( m_L, -2, "__mode" );
		
		lua_replace( m_L, LUA_ENVIRONINDEX );
		
		luaL_register(m_L, _tname, _flist );  //t

		luaL_newmetatable( m_L, _tname );      //t y
		lua_pushvalue( m_L, -2 );               //tyt 
		lua_setfield( m_L, -2, "__index" );    //ty y.__index =t. 
		
		lua_pushcfunction( m_L, destroy );   // tyf
		lua_setfield( m_L, -2, "__gc" );
		lua_pop(m_L,2);
	}
	
}


//
void ScriptBinder::Init( char const* _tname,char const* _bname, char const* _bname1, luaL_reg const* _flist, int (*destroy)(lua_State* ) )
{
	if( m_L != 0 )
	{
		lua_newtable( m_L );
		lua_pushstring( m_L, "v");
		lua_setfield( m_L, -2, "__mode" );
		lua_pushvalue( m_L, -1 );
		lua_setmetatable( m_L, -2 );
		lua_replace( m_L, LUA_ENVIRONINDEX );
		
		luaL_register(m_L, _tname, _flist );  //t

		luaL_newmetatable( m_L, _tname );
		lua_pushvalue( m_L, -2 );
		lua_setfield( m_L, -2, "__index" );
		lua_pushcfunction( m_L, destroy );
		lua_setfield( m_L, -2, "__gc" );
		if( _bname != 0 )
		{
				luaL_getmetatable( m_L, _bname );
				lua_setfield( m_L, -2, "_base");
		}
		if( _bname1 != 0 )
		{
				luaL_getmetatable( m_L, _bname1 );
				lua_setfield( m_L, -2, "_base1" );
		}
		lua_pop( m_L, 1 );
		if( _bname != 0 && _bname1 != 0 )
		{
			luaL_getmetatable( m_L, _bname );
			lua_setfield( m_L, -2, "_base" );
			luaL_getmetatable( m_L, _bname1 );
			lua_setfield( m_L, -2, "_base1" ); 
			lua_pushcfunction( m_L, SearchParent );
			lua_setfield( m_L, -2, "__index");
			lua_pushvalue( m_L, -1 );
			lua_setmetatable( m_L, -2 );
		}
		else if( _bname != 0 )
		{
			lua_getfield( m_L, LUA_GLOBALSINDEX, _bname );
			lua_setfield( m_L, -2, "__index" );
			lua_pushvalue( m_L, -1 );
			lua_setmetatable( m_L, -2 );
		}
		else if( _bname1 != 0 )
		{
			lua_getfield( m_L, LUA_GLOBALSINDEX, _bname1 );
			lua_setfield( m_L, -2, "__index" );
			lua_pushvalue( m_L, -1 );
			lua_setmetatable( m_L, -2 );
		}
	}
}
//
void ScriptBinder::SetOperator( char const* _tname, char const* _operator, int (*opt)(lua_State*) )
{
		if( _tname != 0 )
		{
			luaL_getmetatable( m_L, _tname );
			lua_pushcfunction( m_L, opt );
			lua_setfield( m_L, -2, _operator);
			lua_pop( m_L, 1 );
		}
}


//
void ScriptBinder::PushUserType( void* _udata, char const* _tname )
{
	if( m_L == 0 )
			return ;
	lua_pushlightuserdata( m_L, _udata );
	lua_rawget( m_L, LUA_ENVIRONINDEX );
	if( lua_isnil( m_L, -1 ) )
	{
		 void** ubox = (void**)lua_newuserdata( m_L, sizeof(void*));
		 *ubox = _udata;
		 luaL_getmetatable( m_L, _tname );
		 lua_setmetatable( m_L, -2 );
		 lua_pushlightuserdata( m_L, _udata );
		 lua_pushvalue( m_L, -2 );
		 lua_rawset( m_L, LUA_ENVIRONINDEX );
	}
}
//
void* ScriptBinder::CheckUserType( int _idx, char const* _tname )
{
		if( m_L == NULL )
			return NULL;
			
	  void  **udata = (void **)luaL_checkudata(m_L, _idx, _tname);
	  return *udata;

}
