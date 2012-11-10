package com.CoreLib.Util.Log
{
    import com.CoreLib.Util.Error.SingletonMultiCreateError;
    
    import flash.events.*;
    import flash.system.*;

    public class LogManager extends EventDispatcher 
	{

		/**
		 *日志事件 
		 */
        public static const 	LOG_ADDED:String = "LogAdded";
        public static const 	LOG_CLEANED:String = "LogCleaned";
		
		/**
		 * 日志等级名字 
		 */
        private static const 	LOG_LEVEL_NAMES:Vector.<String> = new Vector.<String>(LogLevel.COUNT, true);
;

		/**
		 * 日志单例 
		 */
        private static var _instance:LogManager;

        private var _logContent:String = "";
        private var _recordLogLevel:uint;
        private var _enable:Boolean;

        public function LogManager()
		{
			if(_instance)
			{
				throw new SingletonMultiCreateError(LogManager);
			}
            LOG_LEVEL_NAMES[LogLevel.DEBUG_ONLY] = "(DebugOnly)";
            LOG_LEVEL_NAMES[LogLevel.FATAL] = "(FatalError)";
            LOG_LEVEL_NAMES[LogLevel.INFORMATIVE] = "(Informative)";
            LOG_LEVEL_NAMES[LogLevel.IMPORTANT] = "(Important)";
            this._enable = Capabilities.isDebugger;
        }
        public static function get instance():LogManager{
			_instance = _instance || new LogManager();
            return _instance;
        }

        public function get recordLogLevel():uint
		{
            return _recordLogLevel;
        }
		
        public function set recordLogLevel(i_level:uint):void
		{
            this._recordLogLevel = Math.min((LogLevel.COUNT - 1), i_level);
        }
		
        public function get enable():Boolean
		{
            return _enable;
        }
		
        public function set enable(val:Boolean):void
		{
            _enable = val;
        }
		
		public function get allLog():String
		{
			return _logContent;
		}
		
		
        public function Log(i_level:uint, ... _args):void{
            if (!this._enable){
                return;
            };
			
            trace.apply(null, _args);
            if (i_level < this._recordLogLevel){
                return;
            };
            var date:Date = new Date();
            var str:String = int(date.hours).toString();
			str = str + ":";
			str = str + int(date.minutes).toString();
			str = str + ":";
			str = str + int(date.seconds).toString();
			str = str + LOG_LEVEL_NAMES[i_level];
			
            var le:uint = _args.length;
            var i:uint = 0;
            while (i < le) {
                if (!_args[i]){
                } else {
                    str = str + _args[i].toString();
                    str = str + " ";
                };
                i++;
            };
            str = str + "\n";
            if (i_level >= LogLevel.FATAL){
                str = str + "\n";
                str = str + new Error().getStackTrace();
            };
            _logContent = _logContent + str;
			
            if (hasEventListener(LOG_ADDED)){
                dispatchEvent(new DataEvent(LOG_ADDED, false, false, str));
            };
        }
        public function clearLog():void
		{
            _logContent = "";
            if (hasEventListener(LOG_CLEANED)){
                dispatchEvent(new Event(LOG_CLEANED));
            };
        }
       

    }
}
