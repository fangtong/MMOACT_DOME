package com.core.util.time
{
	/**
	 * 间隔时间 
	 * @author fangtong
	 * 
	 */
	public class IntervalTime
	{
		private var _interval:int;
		private var _current:int;
		
		public function IntervalTime()
		{
			_interval = 0;
			_current = 0;
		}
		/**
		 * 更新时间 
		 * @param diff
		 * 
		 */
		public function Update(diff:int):void 
		{ 
			_current += diff;
			if(_current<0) 
				_current=0;
		}
		/**
		 * 是否已过间隔时间 
		 * @return 
		 * 
		 */
		public function Passed():Boolean 
		{ 
			return _current >= _interval; 
		}
		/**
		 * 间隔时间过后的重置 
		 * 
		 */
		public function Reset():void 
		{ 
			if(_current >= _interval) 
				_current -= _interval;  
		}
		
		/**
		 * 设置当前时间 
		 * @param current
		 * 
		 */
		public function SetCurrent(current:int):void 
		{
			_current = current; 
		}
		/**
		 * 设置间隔时间 
		 * @param interval
		 * 
		 */
		public function SetInterval(interval:int):void 
		{ 
			_interval = interval; 
		}
		/**
		 * 获取间隔时间 
		 * @return 
		 * 
		 */
		public function GetInterval():int
		{ 
			return _interval; 
		}
		/**
		 * 获取当前时间 
		 * @return 
		 * 
		 */
		public function GetCurrent():int
		{ 
			return _current; 
		}
		/**
		 * 清除所有 
		 * 
		 */
		public function Clear():void
		{
			_interval = 0;
			_current = 0;
		}
	}
}