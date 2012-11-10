#pragma once
#include "irunnable.h"

class ClearSessionThreadJob :
	public SevenSmile::Util::ThreadHelper::IJob
{
public:
	ClearSessionThreadJob(void);
	~ClearSessionThreadJob(void);

public:
	virtual unsigned int Do(void* i_lpVoid =0 );
};
