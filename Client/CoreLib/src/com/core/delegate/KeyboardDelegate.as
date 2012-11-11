package com.core.delegate
{
	
	import flash.display.Stage;
	import flash.events.KeyboardEvent;
	import flash.utils.Dictionary;
	
	public class KeyboardDelegate implements IKeyboardDelegate
	{
		private static var _instance:IKeyboardDelegate;
		
		private var _stage:Stage;
		/**
		 *  
		 */
		private var _dic:Dictionary;
		
		public static function GetInstance():IKeyboardDelegate
		{
			if(_instance == null) _instance = new KeyboardDelegate();
			return _instance;
		}
		
		
		public function KeyboardDelegate()
		{
			_dic = new Dictionary();
		}
		
		public function StartKeyboard(stage:Stage):void
		{
			StopKeyboard()
			if(stage == null) return;
			_stage = stage;
			_stage.addEventListener(KeyboardEvent.KEY_DOWN,onKeyDown);
			_stage.addEventListener(KeyboardEvent.KEY_UP,onKeyUp);
		}
		
		public function StopKeyboard():void
		{
			if(_stage == null) return;
			_stage.removeEventListener(KeyboardEvent.KEY_DOWN,onKeyDown);
			_stage.removeEventListener(KeyboardEvent.KEY_UP,onKeyUp);
			_stage = null;
		}
		
		private function onKeyDown(e:KeyboardEvent):void
		{
			UpKeyboardHandle(
				e.keyCode,true,false,
				e.ctrlKey,e.altKey,e.shiftKey);
			e.stopPropagation();
		}
		
		private function onKeyUp(e:KeyboardEvent):void
		{
			UpKeyboardHandle(
				e.keyCode,false,true,
				e.ctrlKey,e.altKey,e.shiftKey);
			e.stopPropagation();
		}
		
		public function UpKeyboardHandle(
			keycode:int,
			keydown:Boolean,
			keyup:Boolean,
			ctrl:Boolean,
			alt:Boolean,
			shift:Boolean):void
		{
			var key:String = GetKeyboardKey(
				keycode, keydown, keyup,
				ctrl, alt, shift);
			var funs:Vector.<Function> = _dic[key];
			if(funs != null)
			{
				for each(var fun:Function in funs)
				{
					fun();
				}
			}
		}
		
		public function RegisterKeyboardHandle(
			handle:Function,
			keycode:int,
			keydown:Boolean = false,
			keyup:Boolean	= false, 
			ctrl:Boolean	= false,
			alt:Boolean		= false,
			shift:Boolean	= false
		):void
		{
			var key:String = GetKeyboardKey(
				keycode, keydown, keyup,
				ctrl, alt, shift);
			var funs:Vector.<Function> = _dic[key];
			if(funs == null)
			{
				funs = new Vector.<Function>;
				_dic[key] = funs;
			};
			
			funs.push(handle);
		}
		
		public function RemoveKeyboardHandle(
			handle:Function,
			keycode:int,
			keydown:Boolean = false,
			keyup:Boolean	= false,
			ctrl:Boolean	= false,
			alt:Boolean		= false,
			shift:Boolean	= false
		):void
		{
			var key:String = GetKeyboardKey(
				keycode, keydown, keyup,
				ctrl, alt, shift);
			var funs:Vector.<Function> = _dic[key];
			
			if(funs == null) return;
			var i:int = 0;
			for each(var fun:Function in funs)
			{
				if(fun ==handle)
				{
					funs.splice(i,1);
					break;
				}
				i++;
			}
			if(funs.length == 0)
			{
				_dic[key] = null;
				delete _dic[key];
			}
		}
		
		private function GetKeyboardKey(
			keycode:int,
			keydown:Boolean,
			keyup:Boolean,
			ctrl:Boolean,
			alt:Boolean,
			shift:Boolean):String
		{
			var str:String = "key_";
			str += keycode.toString() + "_" 
			str	+= keydown.toString() + "_"
			str	+= keyup.toString() + "_";
			str	+= ctrl.toString() + "_"
			str	+= shift.toString() + "_";
			str += alt.toString();
			return str;
		}
	}
}