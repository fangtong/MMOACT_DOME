#pragma once
#include "abstable.h"

namespace SevenSmile{
	namespace ORM{

		class MySqlTable :
			public SevenSmile::ORM::AbsTable
		{
		public:
			explicit MySqlTable(std::string i_tableName);
			~MySqlTable(void);

			void InitTable();
			void LoadTable();
		};
	}
}
