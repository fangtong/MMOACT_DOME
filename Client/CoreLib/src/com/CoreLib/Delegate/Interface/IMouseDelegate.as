package com.CoreLib.Delegate.Interface
{
	import flash.display.Stage;

	public interface IMouseDelegate
	{
		function StartDelegate(stage:Stage):void;
		function StopDelegate():void;
		
		/**
		 * 
		 * @param handle	回调
		 * @param target	0:传递和当前 1:只当前 2:只传递
		 * @param priority 	优先级
		 * 
		 */
		function RegisterMouseHandle(
			handle:Function,			
			target:int = 0,				
			priority:int = 0):void;	
		function RemoveMouseHandle(
			handle:Function,
			target:int = 0):void;
	}
}