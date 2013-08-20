#ifndef ZP_SINGLETON
#define ZP_SINGLETON

namespace Util
{
	/**
	* @brief SingletonT ����Ϊ����ģ���� 		
	*/
	template<typename T>
	class SingletonT
	{
	public:
		SingletonT(){}
		~SingletonT(){}

		/**
		* @brief ���������е�ʵ����ȫ��ֻ����ϵͳ��ʼ��ʱ����һ��
		* @warning �˺���ȫ��ֻ�ɵ���һ�Σ�����ᴥ������
		*/
		static void CreateInstance(void)
		{
			//ֻ�������һ�θú���
			ZP_ASSERT( NULL == g_lp_inst );
			ZP_SAFE_NEW( g_lp_inst , T ); 
		}

		/**
		* @brief ���ٵ����е�ʵ����ȫ�������ٵ���ʱ����һ��
		* @warning �˺���������CreateInstance���ú���ã�����ᴥ������
		*/
		static void DestroyInstance(void)
		{
			//�����ȵ���CreateInstance
			ZP_ASSERT( NULL != g_lp_inst );
			ZP_SAFE_DELETE( g_lp_inst );
		}

		/**
		* @brief ���ʵ��ָ��
		* @return �õ�����ȫ��Ψһʵ��ָ��
		* @return ����ȫ�ָö����Ψһʵ�����˺���������CreateInstance���ú����
		*				����ᴥ������
		*/
		static T* GetInstance(void)
		{ 
			ZP_ASSERT( NULL != g_lp_inst );
			return g_lp_inst;
		}

	private:
		static T* g_lp_inst;		///>������ȫ��ʵ��ָ��
	};

	 template<typename T>
	T* Util::SingletonT<T>::g_lp_inst = NULL;

}// namespace ZPUTIL

#endif//ZP_SINGLETON