package com.CoreLib.Delegate.NetWork
{
	public interface INetWorkDelegate
	{
		function RegisterReceiveHandle(action:int, handle:Function):void;
		function RemoveReceiveHandle(action:int, handle:Function):void;
		function ExecuteReceiveHandle(action:int,data:Array):void;
		function HasReceiveHandle(action:int):Boolean;
	}
}