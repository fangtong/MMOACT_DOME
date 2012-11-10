#pragma once
#include <windows.h>
#include <vector>
#include "msgdata.h"
#include <limits.h>
#include "Util.h"
//#include <string>
#pragma warning(push)
#pragma warning(disable:4251)
//using namespace std;
namespace SevenSmile{
	namespace Net{
		typedef std::vector<MsgData*> vMsgData;

		using namespace std;

		//extern "C" 
		class __declspec(dllexport) MsgPackage
		{
		public:
			MsgPackage(void);
			~MsgPackage(void);

			MsgPackage(const MsgPackage &packet);

			/************************************************************************/
			/* 解包                                                        */
			/************************************************************************/
			bool UnPackage(char* i_lpChar,unsigned short i_usLength);

			unsigned short GetPackageTotalLength(void);
			unsigned short GetPackageAction(void);
			unsigned short GetMsgDatasNum(void) const;
			MsgData& GetMsgDataByPos(unsigned int i_pos) const;
			MsgData& GetMsgDataByAction(unsigned char i_ucAction) const;
			//vMsgData& GetMsgDatas(void) ;
			//__declspec(dllexport) char*& ToStringUnPackage();

			/************************************************************************/
			/*打包																	*/
			/************************************************************************/
			inline void SetPackageAction(unsigned short i_usHeadAction);

			inline void AddMsgData(unsigned char i_ucAction,int i_intData);
			inline void AddMsgData(unsigned char i_ucAction,unsigned int i_intData);
#ifdef WIN64
			void AddMsgData(unsigned char i_ucAction,size_t i_sizetData)
			{
				AddMsgData(i_ucAction,(unsigned int)i_sizetData);
			}
#endif // WIN64

			inline void AddMsgData(unsigned char i_ucAction,const string& i_strData);
			inline void AddMsgData( unsigned char i_ucAction,const string* i_strData );

			inline void AddMsgDataIntU(unsigned char i_ucAction,unsigned int i_uintData);
			inline void AddMsgDataInt(unsigned char i_ucAction,int i_intData);

			inline void AddMsgDataShortU(unsigned char i_ucAction,unsigned short i_ushortData);
			inline void AddMsgDataShort(unsigned char i_ucAction,short i_shortData);

			inline void AddMsgDataCharU(unsigned char i_ucAction,unsigned char i_ubyteData);
			inline void AddMsgDataChar(unsigned char i_ucAction,char i_byteData);

			//inline void AddMsgData(unsigned char i_usAction,char* i_lpCharContent,unsigned short i_usContentLength);
			//此函数比较特殊，用来打很长的数据   strdup
			inline void AddMsgData(unsigned char i_ucAction,const char* i_lpCharContent,unsigned short i_usContentLength);

			void AddMsgData(unsigned char i_ucAction);

			inline int Package(char* &msg);		//打包
			inline char* Package(void);			//打包
			inline const char* ToStringPackage(void);

		private:
			void Init(void);
			void InitVectorMsgData(void);
			void ClearVectorMsgData(void);

			void Clear(void);

			//void AddMsgData(MsgData* i_lpMd);
			
			void SetMsgDataById(unsigned int i_idx,MsgData* i_lpMd);

			//数据长度超过255，则用此方法分包
			//void SplitMsgData(char* i_lpChar,USHORT i_usLength);
			void JoinMsgData(MsgData*& r_lpMd,unsigned short i_usLength,char* i_lpCharContent);

			////void PackageHead(char* *r_lpCharMsg);
			//void PackageHead(char* &r_lpCharMsg);
			////void PackageMsgData(char* *r_lpCharMsg);
			//void PackageMsgData(char* &r_lpCharMsg);

			//void PackageHead();
			//void PackageMsgData();

			//int PackageData(MsgData* i_lpMd,char* i_lpChar);
			
			void UnPackageMsgData(char* i_lpChar,int i_intTotalLength);

			bool IsPackaged() const { return _bIsPackaged; }
		
		public:
			static const int			LENGTH_Of_PACKAGE_HEAD=4;
			static const int			LENGTH_Of_PACKAGE_CONTENT_LENGTH=2;
			static const int			LENGTH_Of_PACKAGE_ACTION=2;

			static const int			LENGTH_Of_MSG_DATA_HEAD=2;
			static const int			LENGTH_Of_MSG_DATA_CONTENT_LENGTH=1;
			static const int			LENGTH_Of_MSG_DATA_ACTION=1;
			static const unsigned char	JOINT_MSG_DATA_ACTION=255;

			
		private:
			//unsigned short			_usPackageTotalLength;
			unsigned short				_usUnverifiedPackageTotalLength;
			unsigned short				_usPackageAction;

			//char*						_lpCharReturnPackaged;
			vMsgData					_vMsgData;
			MsgData*					_ptrNilMsgData;
			bool						_bIsPackaged;
			char*						_lpCharPackaged;

			std::string					_strRetuanPackagedToInt;
			//bool _bIsUnpackage;

			// 修改后的打包算法，需要的变量 [9/9/2010 ericwang]
			unsigned int				_index;
			static const short			MAX_LENGTH=4096;
			char						_cArrRes[MAX_LENGTH];

		};
	}
}
#pragma warning(pop)