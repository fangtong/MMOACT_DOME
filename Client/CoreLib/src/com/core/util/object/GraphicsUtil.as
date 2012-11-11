package com.core.util.object
{
	import flash.display.Graphics;
	
	/**
	 * 绘制工具 
	 * @author fangtong
	 * 
	 */
	public class GraphicsUtil
	{
		/**
		 * 获知多边形 
		 * @param graphics
		 * @param radius 长度为多边形边数 数值为每点绘制半径
		 * 
		 */
		public static function DrawPolygon(graphics:Graphics,radius:Array):void
		{
			if(!radius || radius.length <=0) return;
			graphics.moveTo(Math.cos (Math.PI*2)*radius[0], Math.sin (Math.PI*2)*radius[0]);
			var n:uint = radius.length;
			var v:uint = n;
			//radius.push(radius[0]);
			while (n--) {
				graphics.lineTo(Math.cos (n/v*Math.PI*2) * radius[n], Math.sin (n/v*Math.PI*2) * radius[n]);
			}
		}
	}
}