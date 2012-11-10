package com.CoreLib.Util.Object 
{
    import flash.utils.*;

	/**
	 * 
	 * @author fangtong
	 * 
	 */
    public final class DictionaryUtil {

        private static var m_tempArrayToStoreDict:Array = [];

		/**
		 * 拷贝 Dictionary
		 * @param i_dic
		 * @param r_toDic
		 * @return 
		 * 
		 */
        public static function copyDictionary(i_dic:Dictionary, r_toDic:Dictionary=null):Dictionary{
            var key:Object;
			r_toDic = r_toDic || new Dictionary();
            clearDictionary(r_toDic);
            
            for (key in i_dic) 
            {
				r_toDic[key] = i_dic[key];
            };
            return r_toDic;
        }
		
		/**
		 * 清除 Dictionary内所有元素 
		 * @param r_dic
		 * 
		 */
        public static function clearDictionary(r_dic:Dictionary):void{
            var key:Object;
            m_tempArrayToStoreDict.length = 0;
            for (key in r_dic) {
                m_tempArrayToStoreDict.push(key);
            };
            for each (key in m_tempArrayToStoreDict) 
            {
				r_dic[key] = null;
                delete r_dic[key];
            };
        }
		
		/**
		 * 是否为空 Dictionary
		 * @param i_dic
		 * @return 
		 * 
		 */
        public static function isDictionaryEmpty(i_dic:Dictionary):Boolean
        {
            if (!i_dic)
            {
                return true;
            };
            ;
            for (var key:Object in i_dic) 
            {
                return false;
            };
            return true;
        }

		/**
		 * 获得Dictionary元素数量 
		 * @param i_dic
		 * @return 
		 * 
		 */
        public static function getDictionaryElemCount(i_dic:Dictionary):uint
        {
           
            if (!i_dic){
                return 0;
            };
            var count:uint;
            for (var key:Object in i_dic) 
            {
                count++;
            };
            return count;
        }
    }
}
