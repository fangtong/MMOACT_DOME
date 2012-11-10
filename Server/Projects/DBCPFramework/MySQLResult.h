#ifndef DBCP_IMPL_MYSQL_RESULT_H
#define DBCP_IMPL_MYSQL_RESULT_H
#include "_DBCP_DEFINE.h"
#include "IResult.h"
#include "winsock.h"
#include "mysql.h"
#pragma warning(push)
#pragma warning(disable:4251)
namespace SevenSmile{
	namespace DBCPFramework{
		namespace ImplBehaviors{
			using namespace SevenSmile::DBCPFramework::Interface;

			class DBCP_DLL_DECL MySQLResult
				:public IResult<MYSQL_RES>
			{
			public:
				MySQLResult(MYSQL_RES* i_lpRes=0);
				~MySQLResult();

				virtual unsigned long long RowCount(void) const;
				//virtual void Free(void);
			private:
				virtual Row FetchRow(MYSQL_RES* i_lpMySqlRes);
			};
		}
	}
}
#pragma warning(pop)
#endif