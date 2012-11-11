package com.core.delegate
{
	import flash.display.Stage;

	public interface IKeyboardDelegate 
	{

		function StartKeyboard(stage:Stage):void;

		function StopKeyboard():void;

		function UpKeyboardHandle(
			keycode:int, keydown:Boolean, keyup:Boolean,
			ctrl:Boolean, alt:Boolean, shift:Boolean):void;

		function RegisterKeyboardHandle(
			handle:Function, keycode:int,
			keydown:Boolean	= false,
			keyup:Boolean	= false,
			ctrl:Boolean	= false,
			alt:Boolean		= false,
			shift:Boolean	= false):void;

		function RemoveKeyboardHandle(
			handle:Function,keycode:int,
			keydown:Boolean	= false,
			keyup:Boolean	= false,
			ctrl:Boolean	= false,
			alt:Boolean		= false,
			shift:Boolean	= false):void;
	}
}