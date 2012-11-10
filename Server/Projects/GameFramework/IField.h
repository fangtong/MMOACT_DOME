#pragma once
#include <memory>
#include <string>

using namespace std::tr1;

namespace SevenSmile{
	namespace ORM{
		class IField
			:public enable_shared_from_this<IField>
		{
		public:
			virtual ~IField(){};
			virtual std::string GetTableName() const = 0;
			virtual std::string GetFieldName() const = 0;
			virtual bool		FieldIsPriKey() const = 0;
		};
	}
}