package com.core.util.object
{
	import flash.display.MovieClip;
	import flash.events.Event;

	/**
	 * 影片简介工具
	 * @author fangtong
	 * 
	 */
	public class MovieClipUtil
	{
		/**
		 * 影片播放到指定帧数后执行回调 
		 * @param mc
		 * @param handle	回调函数
		 * @param frame 	如果其值为0则播放到totalFrames
		 * 
		 */
		public static function MovieClipPlayComplete(mc:MovieClip,handle:Function,frame:uint = 0):void
		{
			if(frame ==0) frame = mc.totalFrames;
			mc.addEventListener(Event.ENTER_FRAME,
				function fun(e:Event):void
				{
					if(mc.currentFrame == frame)
					{
						handle();
						mc.removeEventListener(Event.ENTER_FRAME,fun);
					}
				});
		}
	}
}