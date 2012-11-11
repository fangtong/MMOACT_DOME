package com.core.util.text
{
	public class StringExt
	{
		// 函数：空格检验，是-true，否-false
		public static function isSpace (str : String) : Boolean 
		{
			switch (str)
			{
				case String.fromCharCode (32) :
					// 英文空格
					//trace("English Spacebar Blank");
					break;
				case String.fromCharCode (9) :
					// Tab键
					//trace("Tab Blank");
					break;
				case String.fromCharCode (12288) :
					// 中文双字节空格
					//trace("Chinese Spacebar Blank");
					break;
				case String.fromCharCode (13) :
					// 换行符
					//trace("Enter");
					break;
				default :
					//trace("No Blank");
					return false;
			}
			return true;
		}
		
		//
		// 函数：计算从开头以后连续空格的数量
		public static function countStartSpace (str : String) : Number 
		{
			var numLoop : Number = str.length;
			for (var i:int = 0; i < numLoop; i ++)
			{
				// 遇到非空格马上退出
				if (isSpace (str.charAt (i)) == false)
				{
					return i;
				}
			}
			// 全部是空格
			return i;
		}
		
		//
		// 函数：计算从末尾以前连续空格的数量
		public static function countEndSpace (str : String) : Number 
		{
			var numLoop : Number = str.length;
			for (var i:int = numLoop - 1; i >= 0; i --)
			{
				// 遇到非空格马上退出
				if (isSpace (str.charAt (i)) == false)
				{
					return i;
				}
			}
			// 全部是空格
			return i;
		}
		
		//
		// 函数：去除字符串前端的空格，返回裁减后的字符串
		public static function leftTrim (str : String) : String 
		{
			var newStart:Number = countStartSpace (str);
			return str.slice (newStart);
		}
		
		//
		// 函数：去除字符串末端的空格，返回裁减后的字符串
		public static function rightTrim (str : String) : String 
		{
			var newEnd:Number = countEndSpace (str) + 1;
			return str.slice (0, newEnd);
		}
		
		//
		// 函数：去除字符串两端的空格，返回裁减后的字符串
		public static function allTrim (str : String) : String 
		{
			var rightStr : String = leftTrim (str);
			if (rightStr.length == 0)
			{
				return "";
				//return null;
			}
			else 
			{
				return rightTrim (rightStr);
			}
		}
		
		//
		// 测试
		//var str:String = new String(" 　 123 5 　 ");
		//trace(leftTrim(str));
		//trace(rightTrim(str));
		//trace(allTrim(str));
	}
}