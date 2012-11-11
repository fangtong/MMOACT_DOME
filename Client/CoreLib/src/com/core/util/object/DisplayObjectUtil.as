package com.core.util.object
{
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.Loader;
	import flash.geom.Point;

	/**
	 *  显示对象 工具
	 * @author fangtong
	 * 
	 */
	public class DisplayObjectUtil
	{
		/**
		 * 获取显示对象相对其父节点的中心点 
		 * @param val
		 * @return 
		 * 
		 */
		public static function GetCenterPoint(val:DisplayObject):Point
		{
			var p:Point = null;
			if(val)
			{
				p = new Point(
					val.x + (val.width/2),
					val.y + (val.height/2)
				);
			}
			return p;
		}
		
		/**
		 * 从其父节点删除显示对象
		 * @param val
		 * 
		 */
		public static function RemoveFromParent(val:DisplayObject):void
		{
			if(!val) return;
			var parent:DisplayObjectContainer = val.parent;
			if(!parent||parent is Loader) return;
			parent.removeChild(val);
		}

// 拷贝显示对象 未能实现
//		public static function DuplicateDisplayObject(target:DisplayObject):DisplayObject {
//			// create duplicate (constructor in quotes to bypass strict mode)
//			var targetClass:Class = target["constructor"];
//			var duplicate:DisplayObject = new targetClass();
//			
//			// duplicate properties
//			duplicate.transform = target.transform;
//			duplicate.filters = target.filters;
//			duplicate.cacheAsBitmap = target.cacheAsBitmap;
//			duplicate.opaqueBackground = target.opaqueBackground;
//			
//			//duplicate Sprite
//			if(target is Sprite)
//			{
//				var spTarget:Sprite = target as Sprite;
//				var spDuplicate:Sprite = duplicate as Sprite;
//				spDuplicate.graphics.copyFrom(spTarget.graphics);
//			}
//			
//			if (target.scale9Grid) {
//				var rect:Rectangle = target.scale9Grid;
//				// Flash 9 bug where returned scale9Grid is 20x larger than assigned
//				rect.x /= 20, rect.y /= 20, rect.width /= 20, rect.height /= 20;
//				duplicate.scale9Grid = rect;
//			}
//			
//			var conTarget:DisplayObjectContainer = target as DisplayObjectContainer;
//			if(conTarget)
//			{
//				var conDuplicate:DisplayObjectContainer = duplicate as DisplayObjectContainer;
//				for(var i:int = 0; i < conTarget.numChildren; i++)
//				{
//					var child:DisplayObject = conTarget.getChildAt(i);
//					conDuplicate.addChildAt(DuplicateDisplayObject(child),i);
//				}
//			}
//
//			return duplicate;
//		}
//		
//		public static function DuplicateSpriteByBitmapData(target:Sprite):Sprite 
//		{
//
//			var myBitmapData:BitmapData = new BitmapData(target.width, target.height,true,0xffffff);
//			// 然後繪製一個相同的clip_mc
//			myBitmapData.draw(target);
//			
//			var duplicate:Sprite = new Sprite();
//			duplicate.graphics.beginBitmapFill(myBitmapData);
//			duplicate.graphics.drawRect(target.transform.matrix.tx,target.transform.matrix.ty,myBitmapData.width,myBitmapData.height);
//			duplicate.graphics.endFill();
//			return duplicate;
//		}
	}
}