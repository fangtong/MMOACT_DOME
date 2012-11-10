package com.CoreLib.Util.Object
{
	import flash.display.LoaderInfo;
	import flash.system.ApplicationDomain;

	/**
	 * 有关类的工具
	 * @author fangtong
	 * 
	 */
	public class ClassUtil
	{
		/**
		 * 根据名字及其加载info 获取类对象
		 * @param i_clsName
		 * @param i_info
		 * @return 
		 * 
		 */
		public static function GetClassByNameAndInfo(
			i_clsName : String, 
			i_info : LoaderInfo = null) : Class
		{
			var res:Class = null;
			try
			{
				if (i_info == null)
				{
					res = ApplicationDomain.currentDomain.getDefinition(i_clsName) as Class;
				}else{
					res =  i_info.applicationDomain.getDefinition(i_clsName) as Class;
				}
			} catch (e : ReferenceError)
			{
				trace("定义 " + i_clsName + " 不存在");
			}
			return res;
		}
	}
}