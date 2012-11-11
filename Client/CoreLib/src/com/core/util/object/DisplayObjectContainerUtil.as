package com.core.util.object
{
	import flash.display.DisplayObjectContainer;

	/**
	 *  
	 * @author fangtong
	 * 
	 */
	public class DisplayObjectContainerUtil
	{
		/**
		 * 移除 DisplayObjectContainer 上所有子节点
		 * @param parent
		 * @param layerNum
		 * 
		 */
		public static function RemoveChildren(
			parent:DisplayObjectContainer,
			layerNum:int = -1
		):void
		{
			if(layerNum == 0 || 
				!parent ||
				parent.numChildren == 0 
			)
			{
				return;
			}
			while(parent.numChildren > 0)
			{
				parent.removeChildAt(0);
			}
			layerNum--;
			RemoveChildren(
				parent as DisplayObjectContainer,
				layerNum);
		}
	}
}