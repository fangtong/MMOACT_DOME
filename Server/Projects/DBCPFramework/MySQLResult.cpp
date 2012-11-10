#include "MySQLResult.h"
#include "mysql.h"

namespace SevenSmile{
	namespace DBCPFramework{
		namespace ImplBehaviors{
			MySQLResult::MySQLResult(MYSQL_RES* i_lpRes)
				:IResult(i_lpRes)
			{
			}
			MySQLResult::~MySQLResult(void)
			{
			}

			Row MySQLResult::FetchRow(MYSQL_RES* i_lpMySqlRes)
			{
				MYSQL_ROW row=mysql_fetch_row(i_lpMySqlRes);
				return row;
			}

			unsigned long long MySQLResult::RowCount( void ) const
			{
				MYSQL_RES* lpRes=this->GetRes();
				if (0==lpRes)
				{
					return 0;
				}

				return lpRes->row_count;
			}

			//void MySQLResult::Free( void )
			//{
			//	if (0!=_lpRes)
			//	{
			//		mysql_free_result(_lpRes);
			//	}
			//}
		}
	}
}