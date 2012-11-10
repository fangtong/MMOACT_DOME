#include "MsgPackage.h"
#include <assert.h>
#include <time.h>
#include <math.h>
//#include <iostream>

namespace SevenSmile{
	namespace Net{
		MsgPackage::MsgPackage(void)
//			:_vMsgData(0)
		{
			_ptrNilMsgData = new MsgData();
			//_bIsPackaged=false;
			//this->Init();
		}

		MsgPackage::MsgPackage( const MsgPackage &packet ) 
			:_strRetuanPackagedToInt(packet._strRetuanPackagedToInt)
			//,_usPackageTotalLength(packet._usPackageTotalLength)
			,_usPackageAction(packet._usPackageAction)
			//,_vMsgData(packet._vMsgData)
//			,_vMsgData(0)
			,_lpCharPackaged(0)
			,_bIsPackaged(packet._bIsPackaged)
			,_index(packet._index)
		{
			//_lpCharPackaged=0;
			//_vMsgData=0;

			//_lpCharReturnPackaged=0;

			//if (0!=packet._lpCharReturnPackaged){
			//	_lpCharReturnPackaged=new char[_usPackageTotalLength];
			//	memcpy_s(_lpCharReturnPackaged,_usPackageTotalLength,packet._lpCharReturnPackaged,packet._usPackageTotalLength);
			//}

			//if (_bIsPackaged)
			//{
			//	memcpy(_cArrRes,packet._cArrRes,MAX_LENGTH);
			//}
			memcpy(_cArrRes,packet._cArrRes,MAX_LENGTH);
		}
		//MsgPackage::MsgPackage(char* i_lpChar,USHORT i_usLength)
		//{
		//	this->Init();
		//	_lpCharPackaged=i_lpChar;
		//	_usUnverifiedPackageTotalLength=i_usLength;
		//	//_usPackageTotalLength=i_usLength;

		//	//_bIsUnpackage=true;
		//	this->UnPackage();
		//}

		void MsgPackage::Init(void){
			//_usPackageTotalLength=0;
			_usPackageAction=0;
			_lpCharPackaged=0;
			//_lpCharReturnPackaged=0;
			_strRetuanPackagedToInt.clear();
			_usUnverifiedPackageTotalLength=0;
			//_bIsUnpackage=false;
			_bIsPackaged=false;
//			_vMsgData=0;

			_index=0;

			memset(_cArrRes,0,MAX_LENGTH);
		}

		MsgPackage::~MsgPackage(void)
		{
			//this->Dispose();
			ClearVectorMsgData();

//			if (0!=_vMsgData)
//			{
//				delete _vMsgData;
//				_vMsgData=0;
//			}
			if(0 != _ptrNilMsgData)
			{
				delete _ptrNilMsgData;
				_ptrNilMsgData = 0;
			}
		}

		void MsgPackage::Clear(void){
			//this->Dispose();
			Init();
		}
		
		void MsgPackage::InitVectorMsgData(void){
			//if (0!=_lpCharReturnPackaged){
			//	delete[] _lpCharReturnPackaged;
			//	_lpCharReturnPackaged=0;
			//}

			//if (0!=_lpStrRetuanPackagedToInt)
			//{
			//	delete _lpStrRetuanPackagedToInt;
			//	_lpStrRetuanPackagedToInt=NULL;
			//}

			ClearVectorMsgData();
			//_vMsgData=new vMsgData();

//			if (0==_vMsgData)
//			{
//				_vMsgData=new vMsgData();
//			}
		}


		void MsgPackage::ClearVectorMsgData( void )
		{
			unsigned int size=_vMsgData.size();
			if(size!=0)
			{
				for (unsigned int i=0;i<size;i++)
				{
					MsgData* lpMsgData=_vMsgData[i];
					if( 0 == lpMsgData){
						continue;
					}
					if(0!=lpMsgData->content){
						delete[] lpMsgData->content;
					}

					delete lpMsgData;
				}
				_vMsgData.clear();
			}
		}
		/************************************************************************/
		/* 以下是解包用到的                                                     */
		/************************************************************************/
		unsigned short MsgPackage::GetPackageTotalLength(void) {
			//if (0==_usPackageTotalLength){
			if (0==_index)
			{
				if (0==_lpCharPackaged)
				{
					return 0;
				}

				_index=Util::BytesToUShortForAs(_lpCharPackaged);
				
			}

			if (0!=_usUnverifiedPackageTotalLength){
				if(_index>=_usUnverifiedPackageTotalLength){
					_index=_usUnverifiedPackageTotalLength;
				}

			}

			
			return _index;
		}

		unsigned short MsgPackage::GetPackageAction(void) {
			//if (0==_usPackageAction){
			//if(!_bIsPackaged){
			//	_usPackageAction=Util::BytesToUShortForAs(_lpCharPackaged+LENGTH_Of_PACKAGE_CONTENT_LENGTH);
			//}

			return _usPackageAction;
		}

		bool MsgPackage::UnPackage(char* i_lpChar,unsigned short i_usLength){

			//ClearVectorMsgData();
			InitVectorMsgData();

			_lpCharPackaged=i_lpChar;
			_usUnverifiedPackageTotalLength=i_usLength;
			_index=0;
			int intTotalLength=GetPackageTotalLength();
			
		

			if (intTotalLength==i_usLength)
			{
				assert(i_usLength <= MAX_LENGTH);
				memcpy(_cArrRes,i_lpChar,i_usLength);
				_bIsPackaged = true;

				_usPackageAction=Util::BytesToUShortForAs(_lpCharPackaged+LENGTH_Of_PACKAGE_CONTENT_LENGTH);
				int intContentLength=intTotalLength-LENGTH_Of_PACKAGE_HEAD;
				if(intContentLength <= 0) return false;
				char* chArrActions=new char[intContentLength];
				memcpy(chArrActions,_lpCharPackaged+LENGTH_Of_PACKAGE_HEAD,intContentLength);

				this->UnPackageMsgData(chArrActions,intContentLength);

				delete[] chArrActions;
				chArrActions=0;
				return true;
			}
			return false;
		}

		void MsgPackage::UnPackageMsgData(char* i_lpChar,int i_intTotalLength){
			//if (i_intTotalLength>0){
			//	unsigned short usMsgDataLength = Util::ByteToUShortForAs(i_lpChar[0]);
			//	if (usMsgDataLength<=i_intTotalLength && usMsgDataLength>=LENGTH_Of_MSG_DATA_HEAD){
			//		unsigned short usMsgDataAction = Util::ByteToUShortForAs(i_lpChar[LENGTH_Of_MSG_DATA_CONTENT_LENGTH]);
			//		if (usMsgDataAction==JOINT_MSG_DATA_ACTION){	//链接超长包
			//			unsigned int size=_vMsgData.size();
			//			if (size>0){
			//				JoinMsgData(_vMsgData[size-1],usMsgDataLength,i_lpChar);
			//			}
			//		}
			//		else{
			//			MsgData* md = new MsgData();

			//			char* lpCharMsgData=0;

			//			if (usMsgDataLength>LENGTH_Of_MSG_DATA_HEAD)	//有内容的小包
			//			{
			//				lpCharMsgData = new char[usMsgDataLength-LENGTH_Of_MSG_DATA_HEAD];
			//				memcpy(
			//					lpCharMsgData,
			//					i_lpChar+LENGTH_Of_MSG_DATA_HEAD,
			//					usMsgDataLength-LENGTH_Of_MSG_DATA_HEAD);
			//			}

			//			md->length = usMsgDataLength;
			//			md->action = usMsgDataAction;
			//			md->content = lpCharMsgData;
			//			_vMsgData.push_back(md);
			//		}

			//		UnPackageMsgData(
			//			i_lpChar+usMsgDataLength,
			//			i_intTotalLength-usMsgDataLength
			//			);
			//	}
			//}
			char* data = i_lpChar;
			unsigned short usMsgDataLength = Util::ByteToUShortForAs(data[0]);
			int intTotalLength = i_intTotalLength;
			while (intTotalLength>0 && usMsgDataLength<=intTotalLength && usMsgDataLength>=LENGTH_Of_MSG_DATA_HEAD)
			{
				unsigned short usMsgDataAction = Util::ByteToUShortForAs(data[LENGTH_Of_MSG_DATA_CONTENT_LENGTH]);
				if (usMsgDataAction == JOINT_MSG_DATA_ACTION){	//链接超长包
					unsigned int size=_vMsgData.size();
					if (size>0){
						JoinMsgData(_vMsgData[size-1],usMsgDataLength,data);
					}
				}
				else{
					MsgData* md = new MsgData();

					char* lpCharMsgData=0;

					if (usMsgDataLength>LENGTH_Of_MSG_DATA_HEAD)	//有内容的小包
					{
						lpCharMsgData = new char[usMsgDataLength-LENGTH_Of_MSG_DATA_HEAD];
						memcpy(
							lpCharMsgData,
							data+LENGTH_Of_MSG_DATA_HEAD,
							usMsgDataLength-LENGTH_Of_MSG_DATA_HEAD);
					}

					md->length = usMsgDataLength;
					md->action = usMsgDataAction;
					md->content = lpCharMsgData;
					_vMsgData.push_back(md);
				}
				data += usMsgDataLength,
				intTotalLength -= usMsgDataLength;
				usMsgDataLength = Util::ByteToUShortForAs(data[0]);
			}
		}

		void MsgPackage::JoinMsgData(MsgData*& r_lpMd,unsigned short i_usMsgDataLength,char* i_lpChar){
			unsigned int uiNewContentLength=
				r_lpMd->length-LENGTH_Of_MSG_DATA_HEAD+i_usMsgDataLength-LENGTH_Of_MSG_DATA_HEAD;
			unsigned int uiOldContentLength=r_lpMd->length-LENGTH_Of_MSG_DATA_HEAD;
			
			char* lpChar=new char[uiNewContentLength];
			memcpy(lpChar,r_lpMd->content,uiOldContentLength);
			if (NULL!=r_lpMd->content){
				delete r_lpMd->content;
				r_lpMd->content=NULL;
			}

			memcpy
				(
				lpChar+uiOldContentLength
				,i_lpChar+LENGTH_Of_MSG_DATA_HEAD
				,i_usMsgDataLength-LENGTH_Of_MSG_DATA_HEAD
				);

			r_lpMd->length=uiNewContentLength+LENGTH_Of_MSG_DATA_HEAD;
			r_lpMd->content=lpChar;
		}

		unsigned short MsgPackage::GetMsgDatasNum(void) const{
			return _vMsgData.size();
		}

		MsgData& MsgPackage::GetMsgDataByAction(unsigned char i_ucAction) const{
			MsgData* res = _ptrNilMsgData;
			unsigned int size=_vMsgData.size();
			for (unsigned int i=0;i<size;i++){
				MsgData* lpMsgData=_vMsgData[i];
				if (lpMsgData->action==i){
					res = lpMsgData;
				}
			}
			return *res;
		}

		MsgData& MsgPackage::GetMsgDataByPos(unsigned int i) const{
			MsgData* res = _ptrNilMsgData;
			if (i<_vMsgData.size()){
				res = _vMsgData[i];
			}
			return *res;
		}

		//vMsgData& MsgPackage::GetMsgDatas(void) {
		//	return _vMsgData;
		//}

		/************************************************************************/
		/* 以下是封包用到的                                                     */
		/************************************************************************/
		void MsgPackage::SetPackageAction(unsigned short i_ucHeadAction){
			//this->Clear();
			//_usPackageTotalLength=ConstInt_Length_Of_PackageHead;
			//_usPackageAction=i_usHeadAction;
#ifdef _DEBUG
//			printf("发包 包号：%d\n",i_ucHeadAction);
#endif // _DEBUG
			this->Clear();

			_usPackageAction=i_ucHeadAction;

			_index = LENGTH_Of_PACKAGE_CONTENT_LENGTH -1;
			Util::UShortToBytesForAs(i_ucHeadAction,_cArrRes+LENGTH_Of_PACKAGE_CONTENT_LENGTH);
			_index += LENGTH_Of_PACKAGE_ACTION;
		}

		void MsgPackage::AddMsgData(unsigned char i_usAction){

			assert(_index < MAX_LENGTH-2);
			if (_index>=MAX_LENGTH-2)
			{
				return;
			}

			_cArrRes[++_index]=2;
			_cArrRes[++_index]=i_usAction;

		}


		void MsgPackage::AddMsgData(unsigned char i_usAction,int i_intData){
			if (i_intData>=SCHAR_MIN && i_intData<=SCHAR_MAX){	//-128~127之间
				AddMsgDataChar(i_usAction,(char)i_intData);
			}
			else if(i_intData>SCHAR_MAX && i_intData<=UCHAR_MAX){	//128~255之间
				AddMsgDataCharU(i_usAction,(unsigned char)i_intData);
			}
			else if(i_intData>=SHRT_MIN && i_intData<=SHRT_MAX){	//–32768~32767
				AddMsgDataShort(i_usAction,(short)i_intData);
			}
			else if(i_intData>SHRT_MAX && i_intData<=USHRT_MAX){	//32768~65535
				AddMsgDataShortU(i_usAction,(unsigned short)i_intData);
			}
			else if(i_intData>=INT_MIN && i_intData<=INT_MAX){	//–2147483648~2147483647
				AddMsgDataInt(i_usAction,i_intData);
			}

			//else if(i_intData>INT_MAX && i_intData<=UINT_MAX){	//2147483647~4294967295
			//	AddMsgDataUInt(i_usAction,(UINT)i_intData);
			//}
			//else{
			//	//超出长度限制；
			//}
		}

		void MsgPackage::AddMsgData(unsigned char i_ucAction,unsigned int i_uintData){
			if(i_uintData>=0 && i_uintData<=UCHAR_MAX){	//0~255之间
				AddMsgDataCharU(i_ucAction,(unsigned char)i_uintData);
			}
			else if(i_uintData>UCHAR_MAX && i_uintData<=USHRT_MAX){	//256~65535
				AddMsgDataShortU(i_ucAction,(unsigned short)i_uintData);
			}
			else if(i_uintData>USHRT_MAX && i_uintData<=UINT_MAX){	//65536~4294967295
				AddMsgDataIntU(i_ucAction,i_uintData);
			}
			else{
				//超出长度限制；
			}
		}

		void MsgPackage::AddMsgDataIntU(unsigned char i_ucAction,unsigned int i_uintData){
			if (_index>=MAX_LENGTH-6)
			{
				return;
			}

			_cArrRes[++_index]=6;
			_cArrRes[++_index]=i_ucAction;

			_cArrRes[++_index]=(unsigned char)((0xff000000&i_uintData)>>24);
	 		_cArrRes[++_index]=(unsigned char)((0xff0000&i_uintData)>>16);
	 		_cArrRes[++_index]=(unsigned char)((0xff00&i_uintData)>>8);
	 		_cArrRes[++_index]=(unsigned char)(0xff&i_uintData);
		}
		void MsgPackage::AddMsgDataInt(unsigned char i_ucAction,int i_intData){

			if (_index>=MAX_LENGTH-6)
			{
				return;
			}

			_cArrRes[++_index]=6;
			_cArrRes[++_index]=i_ucAction;

			_cArrRes[++_index]=(char)((0xff000000&i_intData)>>24);
			_cArrRes[++_index]=(char)((0xff0000&i_intData)>>16);
			_cArrRes[++_index]=(char)((0xff00&i_intData)>>8);
			_cArrRes[++_index]=(char)(0xff&i_intData);
		}
		void MsgPackage::AddMsgDataShortU(unsigned char i_ucAction,unsigned short i_ushortData){

			if (_index>=MAX_LENGTH-4)
			{
				return;
			}

			_cArrRes[++_index]=4;
			_cArrRes[++_index]=i_ucAction;

			_cArrRes[++_index]=(unsigned char)((0xff00&i_ushortData)>>8);
			_cArrRes[++_index]=(unsigned char)(0xff&i_ushortData);
		}
		void MsgPackage::AddMsgDataShort(unsigned char i_ucAction,short i_shortData){
			if (_index>=MAX_LENGTH-4)
			{
				return;
			}

			_cArrRes[++_index]=4;
			_cArrRes[++_index]=i_ucAction;

			_cArrRes[++_index]=(char)((0xff00&i_shortData)>>8);
			_cArrRes[++_index]=(char)(0xff&i_shortData);
		}

		void MsgPackage::AddMsgDataCharU(unsigned char i_ucAction,unsigned char i_ubyteData){
			//char* charArrIntData=new char[1];
			////char charArrIntData[1];
			//charArrIntData[0]=i_ubyteData;
			//AddMsgData(i_usAction,charArrIntData,1);
			//delete[] charArrIntData;

			if (_index>=MAX_LENGTH-3)
			{
				return;
			}

			_cArrRes[++_index]=3;
			_cArrRes[++_index]=i_ucAction;
			_cArrRes[++_index]=i_ubyteData;

		}
		void MsgPackage::AddMsgDataChar(unsigned char i_ucAction,char i_byteData){
			//char* charArrIntData=new char[1];
			////char charArrIntData[1];
			//charArrIntData[0]=i_byteData;
			//AddMsgData(i_ucAction,charArrIntData,1);
			//delete[] charArrIntData;

			if (_index>=MAX_LENGTH-3)
			{
				return;
			}

			_cArrRes[++_index]=3;
			_cArrRes[++_index]=i_ucAction;
			_cArrRes[++_index]=i_byteData;
		}

		//void MsgPackage::AddMsgData(unsigned char i_usAction,char* i_lpCharContent,unsigned short i_usContentLength){
		//	if (NULL==i_lpCharContent){
		//		return;
		//	}
		//	if (i_usContentLength<=0){
		//		return;
		//	}

		//	//if (i_usAction==255){
		//	//	return;
		//	//}

		//	MsgData* lpMdAction=new MsgData();
		//	lpMdAction->length=i_usContentLength+ConstInt_Length_Of_MsgDataHead;
		//	lpMdAction->action=i_usAction;

		//	UINT uintSpliteContentLen=0;
		//	if (lpMdAction->length>255){
		//		lpMdAction->length=255;
		//		uintSpliteContentLen=255-ConstInt_Length_Of_MsgDataHead;
		//		//截取数据
		//		char* lpCharTemp=new char[uintSpliteContentLen];	//这个char* 在析构时释放
		//		memcpy(lpCharTemp,i_lpCharContent,uintSpliteContentLen);
		//		lpMdAction->content=lpCharTemp;
		//		AddMsgData(lpMdAction);
		//		AddMsgData(ConstUs_Joint_MsgDataAction,i_lpCharContent+uintSpliteContentLen,i_usContentLength-uintSpliteContentLen);
		//		//SplitMsgData(i_lpCharContent+uintSpliteContentLen,i_usContentLength-uintSpliteContentLen);
		//	}
		//	else{
		//		char* lpCharTemp=new char[i_usContentLength];	//这个char* 在析构时释放
		//		memcpy(lpCharTemp,i_lpCharContent,i_usContentLength);
		//		lpMdAction->content=lpCharTemp;
		//		AddMsgData(lpMdAction);
		//	}
		//}


		void MsgPackage::AddMsgData( unsigned char i_usAction,const char* i_lpCharContent,unsigned short i_usContentLength )
		{

			if (0==i_lpCharContent){
				return;
			}

			unsigned int uintLength=i_usContentLength+LENGTH_Of_MSG_DATA_HEAD;

			//unsigned int uintLength = i_usContentLength + ceil(i_usContentLength/255.0f) * LENGTH_Of_MSG_DATA_HEAD;
			
			assert(_index < MAX_LENGTH-uintLength);
			if (_index>=MAX_LENGTH-uintLength)	//如果超过最大长度，就截掉
			{
				return;
			}


			 if (uintLength>255)
			 {
			 	unsigned int uintSpliteContentLen=255-LENGTH_Of_MSG_DATA_HEAD;
			 	_cArrRes[++_index]=255;
			 	_cArrRes[++_index]=i_usAction;
			 	memcpy(_cArrRes+(++_index),i_lpCharContent,uintSpliteContentLen);
			 	_index+=uintSpliteContentLen-1;
			 	AddMsgData(
			 		JOINT_MSG_DATA_ACTION
			 		,i_lpCharContent+uintSpliteContentLen
			 		,i_usContentLength-uintSpliteContentLen
			 		);
			 }else{
			 	_cArrRes[++_index]=uintLength;
			 	_cArrRes[++_index]=i_usAction;
			 	if (i_usContentLength>0)
			 	{
			 		memcpy(_cArrRes+(++_index),i_lpCharContent,i_usContentLength);
			 		_index+=i_usContentLength-1;
			 	}
			 }

			//unsigned int uintCurLength 	= 0;
			//unsigned int uintCurPos 	= 0;

			//while(uintCurPos < i_usContentLength)
			//{
			//	uintCurLength = i_usContentLength - uintCurPos; 
			//	//超过255 则截断
			//	if(uintCurLength > 255 - LENGTH_Of_MSG_DATA_HEAD)
			//	{
			//		uintCurLength = 255 - LENGTH_Of_MSG_DATA_HEAD;
			//	}
			//	//头为数据长度+头长度
			//	_cArrRes[++_index]=uintCurLength + LENGTH_Of_MSG_DATA_HEAD;
			//	_cArrRes[++_index]=i_usAction;
			//	//数据
			//	if (uintCurLength>0)
			// 	{
			// 		memcpy(_cArrRes+(++_index),i_lpCharContent+uintCurPos,uintCurLength);
			// 		_index+=uintCurLength-1;
			// 	}
			// 	//将当前游标向后移动
			// 	uintCurPos += uintCurLength;
			//}

		}

		//void MsgPackage::AddMsgData(USHORT i_usAction,std::string* i_lpStr){
		//	if (0==i_lpStr){
		//		return;
		//	}

		//	unsigned int ui32ContentLength=i_lpStr->length();

		//	if (ui32ContentLength<=0){
		//		return;
		//	}

		//	//if (i_usAction==255){
		//	//	return;
		//	//}

		//	MsgData* lpMdAction=new MsgData();
		//	lpMdAction->length=ui32ContentLength+ConstInt_Length_Of_MsgDataHead;
		//	lpMdAction->action=i_usAction;

		//	char* i_lpCharContent=(char*)i_lpStr->c_str();

		//	UINT uintSpliteContentLen=0;
		//	if (lpMdAction->length>255){
		//		lpMdAction->length=255;
		//		uintSpliteContentLen=255-ConstInt_Length_Of_MsgDataHead;

		//		//截取数据
		//		//char* lpCharTemp=new char[uintSpliteContentLen];
		//		//memcpy(lpCharTemp,i_lpCharContent,uintSpliteContentLen);

		//		std::string* lpCharTemp=new std::string(i_lpStr,0,uintSpliteContentLen);

		//		lpMdAction->content=(char*)lpCharTemp.c_str();
		//		AddMsgData(lpMdAction);

		//		std::string strNext(i_lpStr,uintSpliteContentLen,ui32ContentLength-uintSpliteContentLen);
		//		AddMsgData(ConstUs_Joint_MsgDataAction,&strNext);
		//		//AddMsgData(ConstUs_Joint_MsgDataAction,i_lpCharContent+uintSpliteContentLen,ui32ContentLength-uintSpliteContentLen);
		//		//SplitMsgData(i_lpCharContent+uintSpliteContentLen,i_usContentLength-uintSpliteContentLen);
		//	}
		//	else{
		//		lpMdAction->content=i_lpCharContent;
		//		AddMsgData(lpMdAction);
		//	}
		//}

		//void MsgPackage::SplitMsgData(char* i_lpChar,USHORT i_usLength){
		//	if (NULL==i_lpChar){
		//		return;
		//	}
		//	if (i_usLength<=0){
		//		return ;
		//	}

		//	UINT uintSpliteContentLen=0;

		//	MsgData* lpMdAction=new MsgData();
		//	lpMdAction->length=i_usLength+ConstInt_Length_Of_MsgDataHead;
		//	lpMdAction->action=ConstUs_Joint_MsgDataAction;
		//	if (lpMdAction->length>256){
		//		lpMdAction->length=256;
		//		
		//		//截取数据
		//		uintSpliteContentLen=256-ConstInt_Length_Of_MsgDataHead;

		//		char lpCharTemp[uintSpliteContentLen];
		//		memcpy(lpCharTemp,i_lpChar,uintSpliteContentLen);
		//		lpMdAction->content=lpCharTemp;
		//		//lpMdAction->content=i_lpCharContent;
		//		AddMsgData(lpMdAction);
		//		SplitMsgData(i_lpChar+uintSpliteContentLen,i_usLength-uintSpliteContentLen);
		//	}
		//	else{
		//		lpMdAction->content=i_lpChar;
		//		AddMsgData(lpMdAction);
		//	}
		//}

		//void MsgPackage::SetMsgDataById(UINT i_idx,USHORT i_usAction, char* i_lpCharContent,USHORT i_usContentLength){
		//	if (i_idx<_vMsgData.size()){
		//		_vMsgData[i_idx]->length=i_usContentLength;
		//		_vMsgData[i_idx]->action=i_usAction;
		//		_vMsgData[i_idx]->content=i_lpCharContent;
		//	}
		//}

		//void MsgPackage::AddMsgData(MsgData* i_lpMd){
		//	_vMsgData.push_back(i_lpMd);
		//	_usPackageTotalLength+=i_lpMd->length;
		//}

		//void MsgPackage::AddMsgData( USHORT i_usAction,string i_strData )
		//{
		//	int l=i_strData.length();
		//	if (i_strData.length()>0)
		//	{
		//		char* lpCharTemp=new char[i_strData.length()];
		//		memcpy(lpCharTemp,i_strData.c_str(),i_strData.length());
		//		this->AddMsgData(i_usAction,lpCharTemp,i_strData.length());
		//	}
		//}

		void MsgPackage::AddMsgData( unsigned char i_ucAction,const string& i_strData )
		{
			int l=i_strData.length();
			if (l==0){
				//char* c=new char[1];
				//c[0]='\0';
				//this->AddMsgData(i_ucAction,i_strData.c_str(),1);
				this->AddMsgData(i_ucAction);
			}

			if (l>0)
			{
				//char* lpCharTemp=new char[l];
				//memcpy(lpCharTemp,i_strData.c_str(),l);
				this->AddMsgData(i_ucAction,i_strData.c_str(),l);
				//delete[] lpCharTemp;
			}
		}

		void MsgPackage::AddMsgData( unsigned char i_ucAction,const string* i_strData )
		{
			int l=i_strData->length();

			if (l==0){
				//this->AddMsgData(i_ucAction,i_strData->c_str(),1);
				this->AddMsgData(i_ucAction);
			}

			if (l>0)
			{
				//char* lpCharTemp=new char[l];
				//memcpy(lpCharTemp,i_strData->c_str(),l);
				this->AddMsgData(i_ucAction,i_strData->c_str(),l);
				//delete[] lpCharTemp;
			}
		}

		//void MsgPackage::SetMsgDataById(UINT i_idx,MsgData* i_lpMd){
		//	if (i_idx<_vMsgData.size()){
		//		_vMsgData[i_idx]->length=i_lpMd->length;
		//		_vMsgData[i_idx]->action=i_lpMd->action;
		//		_vMsgData[i_idx]->content=i_lpMd->content;
		//	}
		//}

		//int MsgPackage::Package(char* *r_lpCharMsg){
		//	PackageMsgData(r_lpCharMsg);
		//	PackageHead(r_lpCharMsg);
		//	for (UINT i=0;i<_vMsgData.size();i++)
		//	{
		//		delete _vMsgData[i];
		//		_vMsgData[i]=NULL;
		//	}
		//	_vMsgData.clear();
		//	return _usPackageTotalLength;
		//}

		int MsgPackage::Package(char* &r_lpCharMsg){
			r_lpCharMsg=this->Package();
			return _index;
			//if (this->_bIsPackaged==false)
			//{
			//	this->_bIsPackaged=true;
			//	//PackageMsgData(r_lpCharMsg);
			//	//PackageHead(r_lpCharMsg);

			//	++_index;
			//	if (_index<=65535)
			//	{
			//		cArrRes[0]=(char)((_index&0xFF00)>>8);
			//		cArrRes[1]=(char)(_index&0xFF);
			//		//return ret;
			//		r_lpCharMsg=cArrRes;
			//	}
			//	else{
			//		//return 0;
			//		r_lpCharMsg=0;
			//	}

			//	//for (UINT i=0;i<_vMsgData.size();i++)
			//	//{
			//	//	MsgData* lpMp=_vMsgData[i];
			//	//	if (0!=lpMp)
			//	//	{
			//	//		if (0!=lpMp->content)
			//	//		{
			//	//			delete lpMp->content;
			//	//			lpMp->content=0;
			//	//		}
			//	//		delete lpMp;
			//	//		lpMp=0;
			//	//	}
			//	//}
			//	//_vMsgData.clear();
			//}
			//else{
			//	//r_lpCharMsg=_lpCharReturnPackaged;
			//	r_lpCharMsg=cArrRes;
			//}

			////return _usPackageTotalLength;
			//return _index;
		}

		char* MsgPackage::Package(void)
		{
			if (_bIsPackaged==false)
			{
				//AddMsgData(254,(unsigned int)time(0));
				//PackageMsgData();
				//PackageHead();

				++_index;
				if (_index<=MAX_LENGTH)
				{
					_bIsPackaged=true;
					_cArrRes[0]=(char)((_index&0xFF00)>>8);
					_cArrRes[1]=(char)(_index&0xFF);
					//return ret;
					return _cArrRes;
				}
				else{
					return 0;
				}

				//for (UINT i=0;i<_vMsgData.size();i++)
				//{
				//	MsgData* lpMp=_vMsgData[i];
				//	if (0!=lpMp)
				//	{
				//		if (0!=lpMp->content)
				//		{
				//			delete lpMp->content;
				//			lpMp->content=0;
				//		}
				//		delete lpMp;
				//		lpMp=0;
				//	}
				//}
				//_vMsgData.clear();
			}

			//return _lpCharReturnPackaged;
			return _cArrRes;
		}

		//void MsgPackage::PackageHead(char* *r_lpCharMsg){
		//	Util::UShortToTowCharForAs(_usPackageTotalLength,*r_lpCharMsg);
		//	(*r_lpCharMsg)[2]=Util::UShortToCharForAs(_usPackageAction);
		//}

		//void MsgPackage::PackageHead(char* &r_lpCharMsg){
		//	Util::UShortToTowCharForAs(_usPackageTotalLength,r_lpCharMsg);
		//	r_lpCharMsg[2]=Util::UShortToCharForAs(_usPackageAction);
		//}

		//void MsgPackage::PackageHead()
		//{
		//	Util::UShortToTowCharForAs(_usPackageTotalLength,_lpCharReturnPackaged);
		//	_lpCharReturnPackaged[2]=Util::UShortToCharForAs(_usPackageAction);
		//}

		//void MsgPackage::PackageMsgData(char* *r_lpCharMsg){
		//	char* _lpTempPackaged=new char[_usPackageTotalLength];
		//	int intPos=ConstInt_Length_Of_PackageHead;
		//	int vLength=_vMsgData.size();
		//	for(int i=0;i<vLength;i++){
		//		char lpCharContent[256];
		//		int len=PackageData(_vMsgData[i],lpCharContent);
		//		//USHORT j=_vMsgData[i]->action;
		//		memcpy(_lpTempPackaged+intPos,lpCharContent,len);
		//		intPos+=len;
		//	}
		//	(*r_lpCharMsg)=_lpTempPackaged;
		//}

		//void MsgPackage::PackageMsgData(char* &r_lpCharMsg){
		//	if (0!=_lpCharReturnPackaged){
		//		r_lpCharMsg=_lpCharReturnPackaged;
		//	}
		//	else{
		//		this->_lpCharReturnPackaged=new char[_usPackageTotalLength];
		//		//char* _lpTempPackaged=new char[_usPackageTotalLength];
		//		int intPos=ConstInt_Length_Of_PackageHead;
		//		int vLength=_vMsgData.size();
		//		for(int i=0;i<vLength;i++){
		//			char lpCharContent[256];
		//			int len=PackageData(_vMsgData[i],lpCharContent);
		//			//USHORT j=_vMsgData[i]->action;
		//			memcpy(_lpCharReturnPackaged+intPos,lpCharContent,len);
		//			intPos+=len;
		//		}
		//		r_lpCharMsg=_lpCharReturnPackaged;
		//	}
		//}

		//void MsgPackage::PackageMsgData()
		//{
		//	this->_lpCharReturnPackaged=new char[_usPackageTotalLength];
		//	//char* _lpTempPackaged=new char[_usPackageTotalLength];
		//	int intPos=ConstInt_Length_Of_PackageHead;
		//	int vLength=_vMsgData.size();
		//	for(int i=0;i<vLength;i++){
		//		char lpCharContent[256];
		//		int len=PackageData(_vMsgData[i],lpCharContent);
		//		//USHORT j=_vMsgData[i]->action;
		//		memcpy(_lpCharReturnPackaged+intPos,lpCharContent,len);
		//		intPos+=len;
		//	}
		//}

		//int MsgPackage::PackageData(MsgData* i_lpMd,char*& i_lpChar){
		//	i_lpChar[0]=UShortToCharForAs(i_lpMd->length);
		//	i_lpChar[1]=UShortToCharForAs(i_lpMd->action);
		//	if (NULL!=i_lpMd->content){
		//		memcpy(i_lpChar+ConstInt_Length_Of_MsgDataHead,i_lpMd->content,i_lpMd->length-ConstInt_Length_Of_MsgDataHead);
		//	}
		//	return i_lpMd->length;
		//}

		//int MsgPackage::PackageData(MsgData* i_lpMd,char* i_lpChar){
		//	i_lpChar[0]=Util::UShortToCharForAs(i_lpMd->length);
		//	i_lpChar[1]=Util::UShortToCharForAs(i_lpMd->action);
		//	if (NULL!=i_lpMd->content){
		//		memcpy(
		//			i_lpChar+ConstInt_Length_Of_MsgDataHead,
		//			i_lpMd->content,
		//			i_lpMd->length-ConstInt_Length_Of_MsgDataHead
		//			);
		//	}
		//	return i_lpMd->length;
		//}

		const char* MsgPackage::ToStringPackage()
		{
			if (0!=_strRetuanPackagedToInt.length()){
				//return _lpStrRetuanPackagedToInt->c_str();
				return _strRetuanPackagedToInt.c_str();
			}

			char* lpc=0;
			char* lpcSplit="|";
			for (int i=0;i<this->GetPackageTotalLength();i++)
			{
				//char c=_lpCharReturnPackaged[i];
				char c=_cArrRes[i];
				int j=c;
				//char* lpc=new char[10];
				lpc=new char[10];
				//char lpc[10];
				_itoa_s(j,lpc,10,10);
				_strRetuanPackagedToInt.append(lpc);
				_strRetuanPackagedToInt.append(lpcSplit);
			}

			if (0!=lpc)
			{
				delete[] lpc;
				lpc=0;
			}

			return _strRetuanPackagedToInt.c_str();
		}

		//char*& MsgPackage::ToStringUnPackage()
		//{
		//	return NULL;
		//}

		//char MsgPackage::UShortToCharForAs(USHORT i_us){
		//	//byte[] abyte0 = new byte[4];
		//	//abyte0[0] = (byte) (0xff & i);
		//	//abyte0[1] = (byte) ((0xff00 & i) >> 8);
		//	//abyte0[2] = (byte) ((0xff0000 & i) >> 16);
		//	//abyte0[3] = (byte) ((0xff000000 & i) >> 24);
		//	//return abyte0;
		//	UCHAR c=(UCHAR) (0xff & i_us);
		//	return c;
		//}

		//void MsgPackage::UShortToTowCharForAs(USHORT i_us,char* r_lpChar){
		//	//r_lpChar[1]=(UCHAR)(0xff & i_us);
		//	//r_lpChar[0]=(UCHAR)((0xff00 & i_us) >> 8);
		//	//char r_lpChar[2];

		//	r_lpChar[1]=(UCHAR)(i_us);
		//	r_lpChar[0]=(UCHAR)(i_us >> 8);

		//	//return r_lpChar;

		//	//byte[] abyte0 = new byte[4];
		//	//abyte0[0] = (byte) (0xff & i);
		//	//abyte0[1] = (byte) ((0xff00 & i) >> 8);
		//	//abyte0[2] = (byte) ((0xff0000 & i) >> 16);
		//	//abyte0[3] = (byte) ((0xff000000 & i) >> 24);
		//	//return abyte0;
		//}

		//void MsgPackage::UINTToBytesForAs(UINT i_us,char* r_lpChar){
		//	//r_lpChar[1]=(UCHAR)(0xff & i_us);
		//	//r_lpChar[0]=(UCHAR)((0xff00 & i_us) >> 8);

		//	r_lpChar[3] = (UCHAR) (0xff & i_us);
		//	r_lpChar[2] = (UCHAR) ((0xff00 & i_us) >> 8);
		//	r_lpChar[1] = (UCHAR) ((0xff0000 & i_us) >> 16);
		//	r_lpChar[0] = (UCHAR) ((0xff000000 & i_us) >> 24);

		//	//byte[] abyte0 = new byte[4];
		//	//abyte0[0] = (byte) (0xff & i);
		//	//abyte0[1] = (byte) ((0xff00 & i) >> 8);
		//	//abyte0[2] = (byte) ((0xff0000 & i) >> 16);
		//	//abyte0[3] = (byte) ((0xff000000 & i) >> 24);
		//	//return abyte0;
		//}
		//void MsgPackage::IntToBytesForAs(int i_int,char* r_lpChar){
		//	r_lpChar[3] = (byte) (0xff & i_int);
		//	r_lpChar[2] = (byte) ((0xff00 & i_int) >> 8);
		//	r_lpChar[1] = (byte) ((0xff0000 & i_int) >> 16);
		//	r_lpChar[0] = (byte) ((0xff000000 & i_int) >> 24);
		//}
		//void MsgPackage::UShortToBytesForAs(USHORT i_ushort,char* r_lpChar){
		//	r_lpChar[1] = (UCHAR) (0xff & i_ushort);
		//	r_lpChar[0] = (UCHAR) ((0xff00 & i_ushort) >> 8);
		//}
		//void MsgPackage::ShortToBytesForAs(short i_short,char* r_lpChar){
		//	r_lpChar[1] = (byte) (0xff & i_short);
		//	r_lpChar[0] = (byte) ((0xff00 & i_short) >> 8);
		//}

	}
}