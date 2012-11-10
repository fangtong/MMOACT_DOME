#include "ClearSessionThreadJob.h"
#include "ApplicationBase.h"
#include "NetWorkDelegate.h"
#include "..\com.sevensmile.tennis.core\coreconfig.h"

using namespace SevenSmile::GameFramework;
using namespace Tennis::CoreModule;

ClearSessionThreadJob::ClearSessionThreadJob(void)
{
}

ClearSessionThreadJob::~ClearSessionThreadJob(void)
{
}

unsigned int ClearSessionThreadJob::Do( void* i_lpVoid /*=0 */ )
{
	//static int number = time(0);
	//static int begin = 0;
	//static int count = 0;
	//unsigned int start = GetTickCount();
	ApplicationBase::sPtrNetWorkDelegate->ClearExitSession();
	//begin += GetTickCount()-start;
	//count++;
	//if (time(0)-number >60)
	//{
	//	number=time(0);
	//	m_ThreadJob.write("Threadtime","ClearSession Thread time = %f ,count = %d",(float)begin/count,count);
	//	begin = 0;
	//	count = 0;
	//}
	return 1;
}
