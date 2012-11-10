package com.CoreLib.Delegate.Interface
{
	
	import com.CoreLib.Sound.SSSound;
	
	import flash.events.Event;
	import flash.events.IEventDispatcher;
	import flash.media.SoundTransform;

	[Event(name="complete", type="flash.events.Event")]

	public interface ISoundDelegate extends IEventDispatcher
	{
		
		/**
		 * 增加声音资源地址 
		 * @param i_soundName
		 * @param i_sourceUrl
		 * @param layer
		 * 
		 */
		function AddSoundBySourceUrl(i_soundName:String,i_sourceUrl:String,i_layer:uint):void;
		
		/**
		 * 增加声音资源
		 * @param i_soundName
		 * @param i_sound
		 * @param layer
		 * 
		 */
		function AddSoundBySource(i_soundName:String,i_sound:SSSound,i_layer:uint):void;
		
		/**
		 * 删除声音资源 
		 * @param i_soundName
		 * 
		 */
		function DelSound(i_soundName:String):void;
		/**
		 * 播放声音
		 * @param i_soundName
		 * @param startTime
		 * @param loops
		 * @param sndTransform
		 * 
		 */
		function PlaySound(i_soundName:String,startTime:Number=0, loops:int=0,inQueue:Boolean = false, sndTransform:SoundTransform=null):void;
		/**
		 * 声音停止 
		 * @param i_soundName
		 * 
		 */
		function StopSound(i_soundName:String):void;
		/**
		 * 停止某层所有声音 
		 * @param layer
		 * 
		 */
		function StopLayerSounds(i_soundLayer:uint):void;
		
		/**
		 * 设置某一层声音 
		 * @param i_layer
		 * @param i_volume
		 * 
		 */
		function SetLayerVolume(i_layer:uint,i_volume:uint):void
			
		/**
		 * 停止所有声音 
		 * 
		 */
		function StopAllSound():void;
		
		/**
		 * 获取声音
		 * @param i_soundName
		 * @return 
		 * 
		 */
		function GetSound(i_soundName:String):SSSound;
		
		/**
		 * 初始化配置 
		 * @param url
		 * 
		 */
		function InitConfig(url:String):void;
		
		/**
		 * 解析xml 
		 * @param xml
		 * 
		 */
		function ResolveConfig(xml:XML):void;
	}
}