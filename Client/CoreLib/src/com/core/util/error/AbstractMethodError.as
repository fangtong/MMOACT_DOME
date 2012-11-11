package com.core.util.error
{
    import flash.utils.*;

    public class AbstractMethodError extends Error {

        public function AbstractMethodError(i_cls:Object = null, i_info:String = null)
		{
            super(((("can't call abstract method directly: " + (i_cls) ? getQualifiedClassName(i_cls) : "unknown") + "'s method ") + i_info));
        }
    }
}
