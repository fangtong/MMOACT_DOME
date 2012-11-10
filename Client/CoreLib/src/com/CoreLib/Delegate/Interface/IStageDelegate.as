package com.CoreLib.Delegate.Interface
{
	
	import flash.display.Stage;
	import flash.events.Event;

	public interface IStageDelegate
	{
		function get hearbeatDelegate():IHeartbeatDelegate;
		function get keyboradDelegate():IKeyboardDelegate;
		function get mouseDelegate():IMouseDelegate;
		function get soundDelegate():ISoundDelegate;
		function get stage():Stage;
		
		function set mouseLeaveHandle(fun:Function):void;
		function get mouseLeaveHandle():Function;
	}
}