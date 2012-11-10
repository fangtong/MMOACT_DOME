#pragma once
#include <memory>
#include <string>
#include "ORMTypedef.h"

using namespace std::tr1;

namespace SevenSmile{
	namespace ORM{
		class IFields
			:public enable_shared_from_this<IFields>
		{
		public:
			virtual ~IFields(){};

			virtual std::string		GetTableName() const										= 0;
			virtual void				SetTableName(std::string& i_tableName)			= 0;

			virtual size_t				GetPosByFieldName(const std::string& i_fieldName)	= 0;
			virtual std::string		GetNameByFieldPos(int i_fieldPos) const			= 0;
			virtual void				PushField(SPtrIField i_field)								= 0;
			virtual size_t				GetFieldSize() const											= 0;

			virtual size_t				GetPriFieldPos()					= 0;
			virtual std::string		GetPriFieldName() const		= 0;
		};
	}
}