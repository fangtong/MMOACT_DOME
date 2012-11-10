#pragma once
#include <string>
#include <sys\utime.h>

namespace SevenSmile
{
	namespace Core
	{
		struct IObject
		{
		public:
			virtual ~IObject(){};
		public:
#pragma region �������
			virtual void Destroy() = 0;
			virtual	void UpTime(time_t i_diff) = 0;
#pragma endregion �������

		};
	}
}
