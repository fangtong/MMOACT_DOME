#pragma once
#include <string>

namespace SevenSmile
{
	namespace Core
	{
		struct IManagedObject
		{

		public:
			virtual ~IManagedObject(){};
#pragma region �������
		public:
			virtual void AddToObjectManage() = 0;
			virtual void DelFromObjectManage() = 0;

			virtual void SetObjectKey(std::string i_key) = 0;
			virtual std::string GetObjectKey() = 0;
#pragma endregion �������
		};
	}
}