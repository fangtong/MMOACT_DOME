#ifndef DBCP_IB_RESULT_H
#define DBCP_IB_RESULT_H
#include "Condition.h"

namespace SevenSmile{
	namespace DBCPFramework{
		namespace Interface{
			using namespace SevenSmile::Util::ThreadHelper;

			typedef char*	Col;
			typedef Col*	Row;		//array Cols

			template<typename QUERY_RESULT>
			class IResult
			{
			public:
				IResult(QUERY_RESULT* i_lpRes=0)
					:_lpRes(i_lpRes)
					,_i32Error(0)
				{}

				virtual ~IResult(void){}

				Row NextRow(void){
					if (0!=_lpRes)
					{
						return this->FetchRow(_lpRes);
					}
					return 0;
				}

				QUERY_RESULT* GetRes(void) const { return _lpRes; }
				void SetRes(QUERY_RESULT* val) { _lpRes = val; }

				virtual unsigned long long RowCount(void) const=0;
				//virtual void Free(void)=0;

				void Wait(void){ _condRes.Wait(); }
				void Signal(void){_condRes.Signal();}

				int GetErrorCode() const { return _i32Error; }
				void SetErrorCode(int val) { _i32Error = val; }

				//Condition* GetCond(void) { return &_condRes; }
			private:
				QUERY_RESULT*	_lpRes;
				Condition		_condRes;
				int				_i32Error;
				

				virtual Row FetchRow(QUERY_RESULT* i_lpQueryRes)=0;
			};
		}
	}
}

#endif