package com.CoreLib.Sound
{
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.media.Sound;
	import flash.media.SoundChannel;
	import flash.media.SoundLoaderContext;
	import flash.media.SoundMixer;
	import flash.media.SoundTransform;
	import flash.net.URLRequest;
	
	public class SSSound extends EventDispatcher implements ISSSound
	{
		
		private var _channel:SoundChannel;
		private var _sound:Sound;
		
		private var _change:Boolean = false;
		private var _stream:URLRequest = null;
		private var _context:SoundLoaderContext = null;
		private var _volume:int = 1;
		
		static public function Volume(i_vol:Number):void{
			SoundMixer.soundTransform.volume =i_vol;
		}
		
		public function SSSound()
		{
			//this.SetSound(i_snd);
			//_channel=new SoundChannel();
		}
		
		public function Load(stream:URLRequest, context:SoundLoaderContext = null):void{
			if(_sound==null){
				this.SetSound(new Sound());
			}
			_stream = stream;
			_context = context; 
			_change = true;
		}
		
		public function set volume(i_vol:Number):void{
			_volume = i_vol;
			if(!_channel) return;
			var soundTransform:SoundTransform = _channel.soundTransform
			soundTransform.volume = i_vol;
			_channel.soundTransform = soundTransform;
		}
		
		public function Play(startTime:Number=0, loops:int=0, sndTransform:SoundTransform=null):void
		{
			if(_sound==null) return;
			
			if (SoundMixer.soundTransform.volume == 0) return;
			
			if(_change)
			{
				_sound.load(_stream,_context);
				_change = false;
			}
			_channel = _sound.play(startTime, loops, sndTransform);
			volume = _volume;
			_channel.addEventListener(Event.SOUND_COMPLETE,onComplete);
		}
		
		public function Stop():void
		{
			if(_sound==null) return;
			if(_channel == null) return;
			_channel.stop();
			
		}
		
		public function SetSound(i_snd:Sound):void
		{
			this.Clear();
			_sound=i_snd;

		}
		
		private function onComplete(e:Event):void{
			if(!_channel) return;
			_channel.removeEventListener(Event.SOUND_COMPLETE,onComplete);
			dispatchEvent(e);
		}
		
		public function Destructor():void
		{
			this.Clear();
		}
		
		private function Clear():void {
			this.Stop();
			
			if (null!=_sound) {
				_sound.removeEventListener(Event.SOUND_COMPLETE, onComplete);
				_sound = null;
			}
			
			_channel = null;
		}
	}
}