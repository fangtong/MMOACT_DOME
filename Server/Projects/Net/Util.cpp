#include "Util.h"

namespace SevenSmile{
	namespace Net{
		//USHORT Util::ByteToUShortForAs(char i_Char){
		//	USHORT r=i_Char & 0xFF;
		//	return r;
		//}

		//short Util::BytesToShortForAs(char* i_lpChar){
		//	short shortRes=i_lpChar[1] & 0xFF;
		//	shortRes |= ((i_lpChar[0] << 8) & 0xFF00);
		//	return shortRes;
		//}

		USHORT Util::ByteToUShortForAs(char i_Char){
			USHORT r=i_Char & 0xFF;
			return r;
		}

		short Util::BytesToShortForAs(char* i_lpChar){
			short shortRes=i_lpChar[1] & 0xFF;
			shortRes |= ((i_lpChar[0] << 8) & 0xFF00);
			return shortRes;
		}


		USHORT Util::BytesToUShortForAs(char* i_lpChar){
			USHORT usRes = i_lpChar[1] & 0xFF;
			usRes |= ((i_lpChar[0] << 8) & 0xFF00);
			return usRes;
		}

		int Util::BytesToIntForAs(char* i_lpChar){
			int intRes=i_lpChar[3] & 0xFF;
			intRes |= ((i_lpChar[2] << 8) & 0xFF00);
			intRes |= ((i_lpChar[1] << 16) & 0xFF0000);
			intRes |= ((i_lpChar[0] << 24) & 0xFF000000);

			return intRes;
		}

		UINT Util::BytesToUIntForAs(char* i_lpChar){
			//int uintRes = i_lpChar[3] & 0xFF;
			UINT uintRes = i_lpChar[3] & 0xFF;
			uintRes |= ((i_lpChar[2] << 8) & 0xFF00);
			uintRes |= ((i_lpChar[1] << 16) & 0xFF0000);
			uintRes |= ((i_lpChar[0] << 24) & 0xFF000000);
			return uintRes;
		}

		//////////////////////////////////////////////////////////////////////////

		char Util::UShortToCharForAs(USHORT i_us){
			//byte[] abyte0 = new byte[4];
			//abyte0[0] = (byte) (0xff & i);
			//abyte0[1] = (byte) ((0xff00 & i) >> 8);
			//abyte0[2] = (byte) ((0xff0000 & i) >> 16);
			//abyte0[3] = (byte) ((0xff000000 & i) >> 24);
			//return abyte0;
			UCHAR c=(UCHAR) (0xff & i_us);
			return c;
		}

		void Util::UShortToTowCharForAs(USHORT i_us,char* r_lpChar){
			r_lpChar[1]=(UCHAR)(i_us);
			r_lpChar[0]=(UCHAR)(i_us >> 8);
		}

		void Util::UIntToBytesForAs(UINT i_uint,char* r_lpChar){
			r_lpChar[3] = (UCHAR) (0xff & i_uint);
			r_lpChar[2] = (UCHAR) ((0xff00 & i_uint) >> 8);
			r_lpChar[1] = (UCHAR) ((0xff0000 & i_uint) >> 16);
			r_lpChar[0] = (UCHAR) ((0xff000000 & i_uint) >> 24);
		}
		void Util::IntToBytesForAs(int i_int,char* r_lpChar){
			r_lpChar[3] = (byte) (0xff & i_int);
			r_lpChar[2] = (byte) ((0xff00 & i_int) >> 8);
			r_lpChar[1] = (byte) ((0xff0000 & i_int) >> 16);
			r_lpChar[0] = (byte) ((0xff000000 & i_int) >> 24);
		}
		void Util::UShortToBytesForAs(USHORT i_ushort,char* r_lpChar){
			r_lpChar[1] = (UCHAR) (0xff & i_ushort);
			r_lpChar[0] = (UCHAR) ((0xff00 & i_ushort) >> 8);
		}
		void Util::ShortToBytesForAs(short i_short,char* r_lpChar){
			r_lpChar[1] = (byte) (0xff & i_short);
			r_lpChar[0] = (byte) ((0xff00 & i_short) >> 8);
		}

	}
}