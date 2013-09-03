#ifndef ZP_TEXTUREMANAGER
#define ZP_TEXTUREMANAGER

#include "ZPDependency.h"
#include "Singleton.h"

namespace Resource
{ 
	class ITextureLoader;
	class Texture2D;

	class  TextureManager : public Util::SingletonT<TextureManager>
	{
	public:
		TextureManager(void);
		virtual ~TextureManager(void);

		/**
		* @brief �������ȡ����
		* @param name �������ƣ�һ��Ϊ����·������
		* @remark �˺�������������������в���ͬ��������������
		*				 ���أ�����᳢�Դ�����·���ж�ȡ��������ȡ�ɹ�
		*				 �򷵻ش�����������󡣷��صĶ���Ҫɾ�����������
		*				��������������
		*/
		Texture2D* CreateOrRetrieveTexture2D( const String& name );

		/**
		* @brief �������ȡ����
		* @param name ��������
		* @remark  �˺�������������������в���ͬ��������������
		*				   ���أ�����᳢�Դ�Ĭ��·���ж�ȡ��������ȡ�ɹ�
		*				   �򷵻ش�����������󡣷��صĶ���Ҫɾ�������������
		*					�����������ڡ�
		*/
		Texture2D* CreateOrRetrieveTexture2DFromDefaultLocation( const String& name );

		/**
		* @brief ��ȡ����
		* @param name ��������
		* @remark  �˺�����������������в���ͬ��������������
		*				   ���أ����򷵻ؿա����صĶ���Ҫɾ�������������
		*					�����������ڡ�
		*/
		Texture2D* RetrieveTexture2D( const String& name );

		/**
		* @brief ɾ��ָ������
		* @param name ��������
		* @remark  �˺�����������������в���ͬ��������������
		*				   ɾ��������true�����򷵻�false�� 
		*/
		bool DeleteTexture2DByName( const String& name );

		/**
		* @brief ɾ�����ж�ά����
		*/
		void DeleteAllTexture2D( void ); 

	protected:

		typedef std::map<String,Texture2D*> textureTable_t;

		String m_strDefaultLocation;
		ITextureLoader* m_pTextureLoader;
		textureTable_t m_textures;
	};

}//namespace Resource

#endif//ZP_TEXTUREMANAGER
