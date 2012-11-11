package com.core.util.object
{
	import flash.utils.ByteArray;

	/**
	 * 对象工具 
	 * @author fangtong
	 * 
	 */
	public class ObjectUtil
	{
		/**
		 * 获取属性根据属性名字数组
		 * @param obj
		 * @param arr
		 * @return 
		 * 
		 */
		public static function GetObjectPropertyByNames(obj:Object,arr:Array):Object
		{
			
			var res:Object = obj;
			var index:int = 0;
			for(index; index < arr.length; index++)
			{
				var name:String = arr[index];
				res = GetObjectPropertyByName(res,name);
				if(!res) break;
			}
			return res;
		}
		
		/**
		 * 获取属性根据属性名
		 * @param obj
		 * @param name
		 * @return 
		 * 
		 */
		public static function GetObjectPropertyByName(obj:Object,name:String):Object
		{
			var res:Object = null;
			if(obj.hasOwnProperty(name))
			{
				res = obj[name];
			}
			return res;
		}
		
		/**
		 *
		 * 拷贝的对象obj最好是非DisplayObject，因为 我自己试过
		 * 直接传一个DisplayObject进来，拷贝出来的ByteArray.length==1，也就是拷贝失败。
		 * 最好先将其转成ByteArray数据
		 * @param obj
		 * @return 
		 * 
		 */
		public static function Copy(obj:Object):*
		{
			var byte:ByteArray = new ByteArray();
			byte.writeObject(obj);
			byte.position = 0;
			var res:* = byte.readObject();
			trace("Copy " + obj + " to " + res);
			return res;
		}
		
		/**
		 * 
		 * @param _args
		 * @return 
		 * 
		 */
		public static function bitAND(... _args):uint
		{
			var res:uint = 4294967295;
			var i:uint = 0;
			while (i < _args.length) {
				res = (res & _args[i]);
				i++;
			};
			return res;
		}
		/**
		 * 
		 * @param _args
		 * @return 
		 * 
		 */
		public static function bitOR(... _args):uint
		{
			var res:uint 	= 0;
			var i:uint 		= 0;
			while (i < _args.length) {
				res = (res | _args[i]);
				i++;
			};
			return res;
		}
	}
}