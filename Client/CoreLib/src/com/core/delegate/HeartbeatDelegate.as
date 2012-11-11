package com.core.delegate
{
	
	import com.core.util.time.IntervalTime;
	
	import flash.display.Stage;
	import flash.events.Event;
	import flash.utils.Dictionary;
	import flash.utils.getTimer;

	public class HeartbeatDelegate implements IHeartbeatDelegate
	{
		/**
		 * this:[key:<v>time int</v>,value:<v>dic dictionary</v>]
		 * <br>
		 * dic: [key:<v>handle function</v>,value:<v>interval IntervalTime</v>] 
		 */
		private var _heratbeatDic:Dictionary;
		private var _stage:Stage;
		private var _clstime:int = 0;
		private static var _instance:HeartbeatDelegate;
		
		public static function GetInstance():IHeartbeatDelegate
		{
			if(_instance == null) _instance = new HeartbeatDelegate();
			return _instance;
		}
		
		public function HeartbeatDelegate()
		{
			_heratbeatDic = new Dictionary();
		}
		
		public function StartHeartbeat(stage:Stage):void
		{
			StopHeartbeat()
			_clstime = getTimer();
			if(stage == null) return;
			_stage = stage;
			_stage.addEventListener(Event.ENTER_FRAME,onFrameHandle);
		}
		
		public function StopHeartbeat():void
		{
			if(_stage == null) return;
			_stage.removeEventListener(Event.ENTER_FRAME,onFrameHandle);
			_stage = null;
		}
		
		private function onFrameHandle(e:Event):void
		{
			var diff:int = getTimer() - _clstime;
			_clstime = getTimer();
 			UpHeartbeatHandle(diff);
		}
		
		public function UpHeartbeatHandle(time:int):void
		{
			var interval:IntervalTime = null;
			for each(var dic:Dictionary in _heratbeatDic)
			{
				for(var handle:Object in dic)
				{
					interval = dic[handle];
					interval.Update(time);
					while(interval.Passed())
					{
						handle();
						interval.Reset();
					}
				}
			}
		}
		
		public function RegisterTimeHandle(time:int, handle:Function):void
		{
			if(_heratbeatDic[time] == null)
			{
				_heratbeatDic[time] = new Dictionary();
			}
			var dic:Dictionary = _heratbeatDic[time]
			var interval:IntervalTime =  new IntervalTime();
			interval.SetCurrent(0);
			interval.SetInterval(time);
			dic[handle] = interval;
		}
		
		public function RemoveTimeHandle(time:int, handle:Function):void
		{
			if(_heratbeatDic[time] == null)
			{
				return;
			}
			
			var dic:Dictionary = _heratbeatDic[time];
			if(dic[handle] != null)
			{
				dic[handle] = null;
				delete dic[handle];
			}
		}
		
		public function HasTimeHandle(time:int, handle:Function):Boolean
		{
			var res:Boolean = false;
			do
			{
				var dic:Dictionary = _heratbeatDic[time];
				if(!dic) break;
				if(!dic[handle]) break;
				res = true
			}while(0);
			return res;
		}
	}
}