#ifndef SEVENSMILE_DBCP_DEFINE_H
#define SEVENSMILE_DBCP_DEFINE_H

#include "_Util_Define.h"

#ifdef DBCPFRAMEWORK_EXPORTS
#define DBCP_DLL_DECL SEVENSMILE_DLL_EXPORT //__declspec(dllexport)
#else
#define DBCP_DLL_DECL SEVENSMILE_DLL_IMPORT //__declspec(dllimport)
#endif

namespace SevenSmile{
	namespace DBCPFramework{
	}
}

#endif