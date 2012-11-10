package com.CoreLib.Sound
{
	import com.CoreLib.IObject;
	
	import flash.display.Loader;
	import flash.media.Sound;
	import flash.media.SoundLoaderContext;
	import flash.media.SoundTransform;
	import flash.net.URLRequest;

	internal interface ISSSound extends IObject
	{
		function Play(startTime:Number = 0, loops:int = 0, sndTransform:SoundTransform = null):void;
		function Stop():void;
		function SetSound(i_snd:Sound):void;
		function Load(stream:URLRequest, context:SoundLoaderContext = null):void;
	}
}