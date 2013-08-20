#ifndef ZP_SINGLETON
#define ZP_SINGLETON

namespace Util
{
	/**
	* @brief SingletonT 此类为单例模板类 		
	*/
	template<typename T>
	class SingletonT
	{
	public:
		SingletonT(){}
		~SingletonT(){}

		/**
		* @brief 创建单例中的实例，全局只可在系统初始化时调用一次
		* @warning 此函数全局只可调用一次，否则会触发断言
		*/
		static void CreateInstance(void)
		{
			//只允许调用一次该函数
			ZP_ASSERT( NULL == g_lp_inst );
			ZP_SAFE_NEW( g_lp_inst , T ); 
		}

		/**
		* @brief 销毁单例中的实例，全局在销毁单例时调用一次
		* @warning 此函数必须在CreateInstance调用后调用，否则会触发断言
		*/
		static void DestroyInstance(void)
		{
			//必须先调用CreateInstance
			ZP_ASSERT( NULL != g_lp_inst );
			ZP_SAFE_DELETE( g_lp_inst );
		}

		/**
		* @brief 获得实例指针
		* @return 该单例的全局唯一实例指针
		* @return 返回全局该对象的唯一实例，此函数必须在CreateInstance调用后调用
		*				否则会触发断言
		*/
		static T* GetInstance(void)
		{ 
			ZP_ASSERT( NULL != g_lp_inst );
			return g_lp_inst;
		}

	private:
		static T* g_lp_inst;		///>单例的全局实例指针
	};

	 template<typename T>
	T* Util::SingletonT<T>::g_lp_inst = NULL;

}// namespace ZPUTIL

#endif//ZP_SINGLETON