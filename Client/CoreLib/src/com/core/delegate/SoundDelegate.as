package com.core.delegate
{
	
	import com.core.sound.SSSound;
	import com.core.sound.SoundLayer;
	
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.media.SoundMixer;
	import flash.media.SoundTransform;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import flash.utils.Dictionary;
	
	public class SoundDelegate extends EventDispatcher implements ISoundDelegate
	{
		private var _dic:Dictionary;
		private var _layerSounds:Vector.<Vector.<SSSound>> ;
		private var _layerVolum:Vector.<int>;
		private var _soundQueue:Vector.<Array>;
		
		public function SoundDelegate()
		{
			_dic = new Dictionary();
			_layerSounds = new Vector.<Vector.<SSSound>>;
			_layerVolum = new Vector.<int>;
			_soundQueue = new Vector.<Array>;
			for(var i:int = 0; i < SoundLayer.ENUM_SOUNDLAYER_MAX; i++)
			{
				_layerSounds.push(new Vector.<SSSound>);
				_layerVolum.push(1);
			}
		}
		
		public function InitConfig(url:String):void
		{
			var urlLoader:URLLoader = new URLLoader(new URLRequest(url));
			urlLoader.addEventListener(Event.COMPLETE,onLoadConfigComplete,false,0,true);
		}
		public function ResolveConfig(xml:XML):void
		{
			var sounds:XMLList = xml.child("sound");
			for each(var sound:XML in sounds)
			{
				if(!(sound.attribute("key") && sound.attribute("url") && sound.attribute("layer"))) return;
				AddSoundBySourceUrl(sound.attribute("key"), sound.attribute("url"),sound.attribute("layer"));
				trace(sound.attribute("key"),sound.attribute("url"),sound.attribute("layer"))
			}
			dispatchEvent(new Event(Event.COMPLETE));
		}
		
		public function AddSoundBySourceUrl(i_soundName:String, i_sourceUrl:String,i_layer:uint):void
		{
			if(!i_sourceUrl || !i_soundName) return;
			var urlRequest:URLRequest = new URLRequest(i_sourceUrl);
			var sound:SSSound = new SSSound();
			sound.Load(urlRequest);
			AddSoundBySource(i_soundName,sound,i_layer);
		}
		
		public function AddSoundBySource(i_soundName:String, i_sound:SSSound,i_layer:uint):void
		{
			if(!i_sound || !i_soundName) return;
			if(i_layer >= SoundLayer.ENUM_SOUNDLAYER_MAX) return;
			DelSound(i_soundName);
			_dic[i_soundName] 		= i_sound;
			_layerSounds[i_layer].push(i_sound);
			i_sound.volume = _layerVolum[i_layer];
		}
		
		public function DelSound(i_soundName:String):void
		{
			var sound:SSSound = _dic[i_soundName];
			if(!sound) return;
			_dic[i_soundName] = null;
			delete _dic[i_soundName];
			//删除层里的声音
			for each(var vct:Vector.<SSSound> in _layerSounds)
			{
				var index:int = vct.indexOf(sound);
				if(index != -1) vct.splice(index,1);
			}
		}
		
		public function PlaySound(i_soundName:String, startTime:Number=0, loops:int=0,inQueue:Boolean = false,sndTransform:SoundTransform=null):void
		{
			var sound:SSSound = _dic[i_soundName];
			if(!sound) return;
			
			//音乐 替换
			{
				var musics:Vector.<SSSound> = _layerSounds[SoundLayer.ENUM_SOUNDLAYER_MUSIC];
				if(musics.indexOf(sound) != -1)
				{
					StopLayerSounds(SoundLayer.ENUM_SOUNDLAYER_MUSIC);
				}
			}
			//音效 不替换
			//进入队列 等待播放
			if(inQueue)
			{
				_soundQueue.push([sound,i_soundName,startTime,loops,sndTransform]);
			}
			if(inQueue && _soundQueue.length > 1) return;
			
			//不在队列或者为队列头 那么直接播放
			sound.addEventListener(Event.SOUND_COMPLETE,onSoundComplete);
			sound.Play(startTime,loops,sndTransform);
			
		}
		
		
		/**
		 * 停止某层所有声音 
		 * @param layer
		 * 
		 */
		public function StopLayerSounds(i_layer:uint):void
		{
			if(i_layer >= SoundLayer.ENUM_SOUNDLAYER_MAX) return;
			var sounds:Vector.<SSSound> = _layerSounds[i_layer];
			for each(var sound:SSSound in sounds)
			{
				sound.Stop();
			}
		}
		
		public function StopSound(i_soundName:String):void
		{
			var sound:SSSound = _dic[i_soundName];
			if(!sound) return;
			sound.Stop();
		}
		
		public function SetLayerVolume(i_layer:uint,i_volume:uint):void
		{
			if(i_layer >= SoundLayer.ENUM_SOUNDLAYER_MAX) return;
			var sounds:Vector.<SSSound> = _layerSounds[i_layer];
			for each(var sound:SSSound in sounds)
			{
				sound.volume = i_volume;
			}
			_layerVolum[i_layer] = i_volume;
		}
		
		public function GetSound(i_soundName:String):SSSound
		{
			return _dic[i_soundName];
		}
		
		public function StopAllSound():void
		{
			SoundMixer.stopAll();
		}

		/****************************************************************
		 * event
		 ****************************************************************/
		private function onLoadConfigComplete(e:Event):void
		{
			var urlLoader:URLLoader = URLLoader(e.currentTarget);
			var xml:XML = XML(urlLoader.data);
			ResolveConfig(xml);
		}
		private function onSoundComplete(e:Event):void
		{
			var sound:SSSound = SSSound(e.currentTarget);
			sound.removeEventListener(Event.SOUND_COMPLETE,onSoundComplete);
			//不是队列头元素 退出
			if(_soundQueue.length == 0 || sound != _soundQueue[0][0]) return;
			//队列中元素 进行队列播放
			_soundQueue.shift();
			if(_soundQueue.length == 0) return;
			PlaySound(_soundQueue[0][1],_soundQueue[0][2],_soundQueue[0][3],_soundQueue[0][4]);

		}
	}
}