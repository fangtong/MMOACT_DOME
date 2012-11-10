#pragma once
#include "Util.h"
#include <xstring>
// #ifdef _DEBUG
// #define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
// #endif
namespace SevenSmile{
	namespace Net{
		class __declspec(dllexport) MsgData{
		private:
			static const int ConstInt_Length_Of_MsgDataHead=2;
			static const int ConstInt_Length_Of_MsgDataContentLength=1;
			static const int ConstInt_Length_Of_MsgDataAction=1;
		public:
			USHORT length;
			USHORT action;
			char* content;
		public:
			MsgData(void);
			~MsgData(void);

			char* GetByteArray(void);
			int MsgData::GetData(void);
			UINT MsgData::GetDataU(void);
			std::string GetDataStr(void);

			char GetByte(void);
			UCHAR GetByteU(void);
			
		private:
			short GetShort(void);
			USHORT GetShortU(void);
			int GetInt(void);
			UINT GetIntU(void);
		};
	}
}