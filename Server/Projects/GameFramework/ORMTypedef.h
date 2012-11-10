#pragma once
#include <memory>
#include <vector>
#include <string>
#include <hash_map>

namespace SevenSmile{
	namespace ORM{

		class AbsTable;
		typedef std::tr1::shared_ptr<AbsTable> SptrAbsTable;

		class AbsRow;
		typedef std::tr1::shared_ptr<AbsRow> SPtrAbsRow;

		class AbsProxyDBData;
		typedef std::tr1::shared_ptr<AbsProxyDBData> SPtrAbsDBProxy;

		class IField;
		typedef std::tr1::shared_ptr<IField> SPtrIField;

		class IFields;
		typedef std::tr1::shared_ptr<IFields> SPtrIFields;

		typedef unsigned long long PRIMARY;

		typedef std::vector<SPtrIField> DBFields;
		typedef std::vector<std::string> DBDatas;
		typedef stdext::hash_map<std::string,size_t> FieldNameToPos;

		enum DATA_STATE
		{
			DATA_STATE_NOCHANGE = 0,
			DATA_STATE_NEW		= 1,
			DATA_STATE_DELETE	= 2,
			DATA_STATE_UPDATE	= 3,
		};
	}
}
