package com.CoreLib.Util.Time
{
	/**
	 * 时间工具 
	 * @author fangtong
	 * 
	 */
	public class TimeUtil
	{
		/**
		 * 根据当前本地时间 获取 
		 * @param i_time 秒级本地时间
		 * @return 
		 * 
		 */
		public static function LimitLocalTimeDay(i_localTime:int):uint
		{
			var data:Date= new Date();
			var res:int =  Math.ceil(i_localTime - ((data.getTime()/1000) + data.timezoneOffset*60))/86400
			return res>0?res:0;
		}
	}
}