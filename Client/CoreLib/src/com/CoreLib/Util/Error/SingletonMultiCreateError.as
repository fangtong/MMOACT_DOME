package com.CoreLib.Util.Error
{
    import flash.utils.*;

	/**
	 * 单例多次创建错误 
	 * @author fangtong
	 * 
	 */
    public class SingletonMultiCreateError extends Error {

        private var m_singletonClass:Class;

        public function SingletonMultiCreateError(i_cls:Class, i_info:String=null)
		{
            this.m_singletonClass = i_cls;
			i_info = ((i_info) || (("can't create mulitple instance of singleton class: " + getQualifiedClassName(i_cls))));
            super(i_info);
        }
		
        public function get singletonClass():Class
		{
            return (this.m_singletonClass);
        }

    }
}
