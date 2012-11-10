#pragma once
#include "MsgPackage.h"
#include <vector>
#pragma warning(push)
#pragma warning(disable:4251)
namespace SevenSmile{
	namespace Net{
		struct StructMsgPackage{
			char* lpcMsgPackage;
			unsigned int uiPackageLength;
			StructMsgPackage():
			lpcMsgPackage(0),uiPackageLength(0){}
		};

		typedef std::vector<StructMsgPackage*> MsgPackageArr;

		extern "C" class __declspec(dllexport) MsgPackageManage
		{
		public:
			MsgPackageManage(char* i_lpChar,unsigned int i_uiPackageLength);
			~MsgPackageManage(void);

			MsgPackageArr& GetMsgPackages();
			//MsgPackage* GetMsgPackages();

		private:
			MsgPackageArr _lpMpArr;
			char*		_lpChar;

			void ExeMsgPackage(char* i_lpChar,unsigned int i_uiPackageLength);
		};
	}
}
#pragma warning(pop)
