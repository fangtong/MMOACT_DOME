#pragma once
#include <windows.h>

namespace SevenSmile{
	namespace Net{
		class __declspec(dllexport)  Util
		{
		public:
			static USHORT ByteToUShortForAs(char i_Char);
			//static USHORT BytesToUShortForAs(char* i_lpChar);
			//static UINT BytesToUIntForAs(char* i_lpChar);
			static short BytesToShortForAs(char* i_lpChar);
			static USHORT BytesToUShortForAs(char* i_lpChar);
			static int BytesToIntForAs(char* i_lpChar);
			static UINT BytesToUIntForAs(char* i_lpChar);
			
			static char UShortToCharForAs(USHORT i_us);
			static void UShortToTowCharForAs(USHORT i_us,char* r_lpChar);

			static void UIntToBytesForAs(UINT i_us,char* r_lpChar);
			static void IntToBytesForAs(int i_int,char* r_lpChar);
			static void UShortToBytesForAs(USHORT i_ushort,char* r_lpChar);
			static void ShortToBytesForAs(short i_short,char* r_lpChar);
		};
	}
}
