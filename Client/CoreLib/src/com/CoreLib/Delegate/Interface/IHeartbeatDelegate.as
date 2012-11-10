package com.CoreLib.Delegate.Interface
{
	import flash.display.Stage;

	public interface IHeartbeatDelegate 
	{
		function StartHeartbeat(stage:Stage):void;

		function StopHeartbeat():void;

		function UpHeartbeatHandle(time:int):void;

		function RegisterTimeHandle(time:int, handle:Function):void;

		function RemoveTimeHandle(time:int, handle:Function):void;
		
		function HasTimeHandle(time:int, handle:Function):Boolean;
	}
}