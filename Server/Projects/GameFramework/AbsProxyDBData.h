#pragma once
#include "ORMTypedef.h"
#include "AbsRow.h"
#include "FinalClass.h"
#include "imanageobject.h"
#include "DBDateManage.h"
#include "AbsTable.h"
#include <memory>
#include "AbsObject.h"

using namespace SevenSmile::ORM;
using namespace std::tr1;
namespace SevenSmile
{
	namespace ORM
	{
		class AbsProxyDBData
			:public enable_shared_from_this<AbsProxyDBData>
			,public SevenSmile::Core::AbsObject
		{
		public:
			AbsProxyDBData():_row(NullShardPtr(AbsRow)),_init(false),_state(DATA_STATE_NOCHANGE){}
			virtual ~AbsProxyDBData(){}
		public:
			virtual void SetRow(SPtrAbsRow i_row){
				_row = i_row;
			}
			SPtrAbsRow GetRow() const {
				return _row;
			}
			
			//************************************
			// Method:    InitRowValues 初始化数据库取出的数据
			// FullName:  SevenSmile::ORM::AbsProxyDBData::InitRowValues
			// Access:    public 
			// Returns:   void
			// Qualifier:
			//************************************
			void InitRowValues() 
			{
				if(!_row) return;
				_row->InitValues();
			}
			virtual void DataFromDBData() = 0;
			virtual void DBDataFromData() = 0;

			void ChangeDataState(DATA_STATE i_state)
			{
				//保持新数据 
				if (!_init && i_state == DATA_STATE_UPDATE) return;
				//保持删除数据
				if(_state == DATA_STATE_DELETE && i_state != DATA_STATE_NOCHANGE) return;
				//删除数据完成后 释放对象
				if(_state == DATA_STATE_DELETE && i_state == DATA_STATE_NOCHANGE)
				{
					Destroy();
				}
				_state = i_state;
				if(_state != DATA_STATE_NOCHANGE){
					DBDateManager->AddToUpDateList(shared_from_this());
				}
			}
			DATA_STATE GetDataState()
			{
				return _state;
			}

			bool Init() const { return _init; }
			void Init(bool val) { _init = val; }

		protected:
			SPtrAbsRow	_row;
			DATA_STATE _state;
			bool _init;
		};

		template<typename ProxyClass,typename DataClass>
		unsigned int GenerateNew( const char* i_table,const char* i_key )
		{
			SptrAbsTable sPtrItemTable = DBDateManager->GetTableByName(i_table);
			assert(sPtrItemTable);
			SPtrAbsRow row = sPtrItemTable->GenerateNewRow();
			assert(row);
			const char* id = row->GetValueByFieldName(i_key);
			assert(id);
			row->SetValueByFieldName(i_key,ToString(id));

			shared_ptr<ProxyClass> proxy = shared_ptr<ProxyClass>(new ProxyClass());
			shared_ptr<DataClass> data = shared_ptr<DataClass>(new DataClass());
			proxy->SetData(data);
			proxy->SetRow(row);
			proxy->SetObjectKey(ToString(id));
			proxy->AddToObjectManage();
			proxy->DataFromDBData();
			proxy->ChangeDataState(DATA_STATE_NEW);
			return atoi(id);
		}
	}
}