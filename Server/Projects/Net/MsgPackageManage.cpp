
#include "MsgPackageManage.h"

namespace SevenSmile{
	namespace Net{
		MsgPackageManage::MsgPackageManage(char* i_lpChar,unsigned int i_uiPackageLength)
		{
			//this->_lpMpArr=new MsgPackage[10];
			//this->_lpMpArr=new MsgPackageArr;

			if (0 != i_lpChar) {
				_lpChar = i_lpChar;
				this->ExeMsgPackage(_lpChar,i_uiPackageLength);
			}
			else {
				_lpChar = new char[i_uiPackageLength];
			}
		}

		MsgPackageManage::~MsgPackageManage(void)
		{
			for (unsigned int i=0;i<_lpMpArr.size();i++)
			{
				if (0!=_lpMpArr[i]){
					if (0!=_lpMpArr[i]->lpcMsgPackage){
						delete[] _lpMpArr[i]->lpcMsgPackage;
						_lpMpArr[i]->lpcMsgPackage=0;
					}

					delete _lpMpArr[i];
					_lpMpArr[i]=0;
				}
			}

			_lpMpArr.clear();
			//if (0!=_lpMpArr)
			//{
			//	delete _lpMpArr;
			//	_lpMpArr=0;
			//}
			
		}

		MsgPackageArr& MsgPackageManage::GetMsgPackages()
		{
			return _lpMpArr;
		}

		void MsgPackageManage::ExeMsgPackage(char* i_lpChar,unsigned int i_uiPackageLength)
		{
			/*
			if (i_uiPackageLength>0 ){
				//char* lpcForRead=new char[i_uiPackageLength];
				//memcpy_s(lpcForRead,i_uiPackageLength,i_lpChar,i_uiPackageLength);

				unsigned int uintPackageTotalLength = Util::BytesToUShortForAs(i_lpChar);

				if (uintPackageTotalLength > i_uiPackageLength 
				|| uintPackageTotalLength == 0)
				{
					return;
				}

				char* lpcTemp=new char[uintPackageTotalLength];
				//memcpy_s(lpcTemp,uintPackageTotalLength,i_lpChar,i_uiPackageLength);
				memcpy_s(lpcTemp,uintPackageTotalLength,i_lpChar,uintPackageTotalLength);

				StructMsgPackage* lpSmp=new StructMsgPackage();
				lpSmp->lpcMsgPackage=lpcTemp;
				lpSmp->uiPackageLength=uintPackageTotalLength;
				//this->_lpMpArr->push_back(lpcTemp);
				this->_lpMpArr.push_back(lpSmp);

				if (uintPackageTotalLength < i_uiPackageLength) {
					int i=i_uiPackageLength-uintPackageTotalLength;
					char* lpcTemp2=new char[i];
					memcpy_s(lpcTemp2,i,i_lpChar+uintPackageTotalLength,i);
					ExeMsgPackage(lpcTemp2,i);

					if (0!=lpcTemp2){
						delete[] lpcTemp2;
						lpcTemp2=0;
					}
				}
			}*/

			while(i_uiPackageLength>0)
			{
				unsigned int uintPackageTotalLength = Util::BytesToUShortForAs(i_lpChar);

				if (uintPackageTotalLength > i_uiPackageLength || uintPackageTotalLength == 0)
				{
					return;
				}

				char* lpcTemp=new char[uintPackageTotalLength];
				memcpy_s(lpcTemp,uintPackageTotalLength,i_lpChar,uintPackageTotalLength);
				
				StructMsgPackage* lpSmp=new StructMsgPackage;
				lpSmp->lpcMsgPackage=lpcTemp;
				lpSmp->uiPackageLength=uintPackageTotalLength;
				
				this->_lpMpArr.push_back(lpSmp);

				i_lpChar += uintPackageTotalLength;
				i_uiPackageLength -= uintPackageTotalLength;
			}
		}
	}
}