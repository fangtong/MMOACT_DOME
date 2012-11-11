package com.core.delegate
{
	
	import flash.display.Stage;
	import flash.events.Event;
	import flash.events.EventDispatcher;
	
	public class StageDelegate extends EventDispatcher implements IStageDelegate
	{
		private static var _instance:StageDelegate;
		
		private var _stage:Stage;
		
		/**************************************************************************************************************
		 * components
		 **************************************************************************************************************/
		private var _iHearbeatDelegate:IHeartbeatDelegate		= null;
		private var _iKeyboardDelegate:IKeyboardDelegate;
		private var _iMouseDelegate:IMouseDelegate;
		private var _iSoundDelegate:ISoundDelegate;

		
		/**************************************************************************************************************
		 * event handle
		 **************************************************************************************************************/
		private var _mouseLeaveHandle:Function 				= null;
		
		public static function get instance():StageDelegate
		{
			throw new Error("StageDelegate Not Instance!");
			return StageDelegate._instance;
		}
		
		public function StageDelegate(i_stage:Stage)
		{
			if(StageDelegate._instance) return;
			StageDelegate._instance = this;
			
			_stage = i_stage;
			
			_iHearbeatDelegate = new HeartbeatDelegate();
			_iHearbeatDelegate.StartHeartbeat(_stage);
			
			_iKeyboardDelegate = new KeyboardDelegate();
			_iKeyboardDelegate.StartKeyboard(_stage);
			
			_iSoundDelegate = new SoundDelegate();
			
			InitStageEvent();
		}

		public function get hearbeatDelegate():IHeartbeatDelegate
		{
			return _iHearbeatDelegate;
		}
		
		public function get keyboradDelegate():IKeyboardDelegate
		{
			return _iKeyboardDelegate;
		}
		
		public function get mouseDelegate():IMouseDelegate
		{
			return _iMouseDelegate;
		}
		
		public function get stage():Stage
		{
			return _stage;
		}


		public function get soundDelegate():ISoundDelegate
		{
			return _iSoundDelegate;
		}
		
		/**************************************************************************************************************
		 * event handle proxy
		 **************************************************************************************************************/
		public function get mouseLeaveHandle():Function
		{
			return _mouseLeaveHandle;
		}

		/**
		 * @private
		 */
		public function set mouseLeaveHandle(value:Function):void
		{
			_mouseLeaveHandle = value;
		}
		
		/**************************************************************************************************************
		 * event
		 **************************************************************************************************************/
		private function InitStageEvent():void
		{
			_stage.addEventListener(Event.MOUSE_LEAVE,OnStageEventHandle);
		}
		
		private function OnStageEventHandle(e:Event):void
		{
			switch(e.type)
			{
				case Event.MOUSE_LEAVE:
				{
					if(null != _mouseLeaveHandle) _mouseLeaveHandle();
					break;
				}
					
				default:
				{
					break;
				}
			}
		}
	}
}