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
#pragma region 对象相关
			virtual void Destroy() = 0;
			virtual	void UpTime(time_t i_diff) = 0;
#pragma endregion 对象相关

		};
	}
}
