#include "MsgData.h"

namespace SevenSmile{
	namespace Net{
		MsgData::MsgData(void)
			:length(0),action(0),content(0)
		{

		}
		MsgData::~MsgData(void){

		}

		char* MsgData::GetByteArray(void){
			if (0!=content){
				return content;
			}
			else {
				return NULL;
			}
		}

		int MsgData::GetData(void){
			int intLength=this->length-ConstInt_Length_Of_MsgDataHead;
			if (intLength==1){
				return this->GetByte();
			}
			else if (intLength==2){
				return this->GetShort();
			}
			else if (intLength==4){
				return this->GetInt();
			}
			else{
				return NULL;
			}
		}

		UINT MsgData::GetDataU(void){
			int intLength=this->length-ConstInt_Length_Of_MsgDataHead;
			if (intLength==1){
				return this->GetByteU();
			}
			else if (intLength==2){
				return this->GetShortU();
			}
			else if (intLength==4){
				return this->GetIntU();
			}
			else{
				return NULL;
			}
		}

		char MsgData::GetByte(void) {
			if (NULL!=content){
				return content[0];
			}
			else {
				return NULL;
			}
		}

		UCHAR MsgData::GetByteU(void) {
			if (NULL!=content){
				return (UCHAR)content[0];
			}
			else {
				return NULL;
			}
		}

		short MsgData::GetShort(void) {
			if (NULL!=content){
				return Util::BytesToShortForAs(content);
			}
			else {
				return NULL;
			}
		}

		USHORT MsgData::GetShortU(void) {
			if (NULL!=content){
				return Util::BytesToUShortForAs(content);
			}
			else {
				return NULL;
			}
		}

		int MsgData::GetInt(void) {
			if (NULL!=content){
				return Util::BytesToIntForAs(content);
			}
			else {
				return NULL;
			}
		}

		UINT MsgData::GetIntU(void) {
			if (NULL!=content){
				return Util::BytesToUIntForAs(content);
			}
			else {
				return NULL;
			}
		}

		std::string MsgData::GetDataStr( void )
		{
			std::string res;
			if(length > 0){
				res =  std::string(GetByteArray(),length-2);
			}
			return res;
		}
	}
}