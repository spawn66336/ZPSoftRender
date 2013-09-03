#ifndef ZP_CAMERA
#define ZP_CAMERA

#include "ZPDependency.h"
#include "ZPMathDependency.h"

class ZPEXPORT Camera
{
public:
	Camera(void);
	virtual ~Camera(void);

	/**
	* @brief ������������up����תtheta��
	* @param theta ��ת�Ķ���������ֵ��
	*/
	void RotateWithUp( const Real theta );

	/**
	* @brief ������������t����תtheta��
	* @param theta ��ת�Ķ���������ֵ��
	*/
	void RotateWithT( const Real theta );

	/**
	* @brief �����������ĳ�������תtheta��
	* @param theta ��ת�Ķ���������ֵ��
	*/
	void RotateWithDir( const Real theta );

	/**
	* @brief �����������λ�ƶ�len����λ
	* @param len Ҫ�ƶ��ĳ���
	*/
	void MoveAlongDir( const Real len );

	/**
	* @brief ���������up��ָ��λ�ƶ�len����λ
	* @param len Ҫ�ƶ��ĳ���
	*/
	void MoveAlongUp( const Real len );

	/**
	* @brief ���������t��ָ��λ�ƶ�len����λ
	* @param len Ҫ�ƶ��ĳ���
	*/
	void MoveAlongT( const Real len );

	/**
	* @brief  ��ȡ���λ������
	*/
	inline Math::Vec3 GetPos(void) const { return m_pos; }

	/**
	* @brief  ���������λ��
	* @param pos ���λ��
	*/
	inline void SetPos( const Math::Vec3& pos ){ m_pos = pos; }

	/**
	* @brief ��������ķ�λ
	*/
	void Reset( void );

	/**
	* @brief ��������OpenGL������
	*/
	Math::Matrix4 GetGLCameraMatrix( void );

	/**
	* @brief ����������
	*/
	Math::Matrix4 GetCameraMatrix( void );
	
	/**
	* @brief ������׵���Ž�
	* @param fov ��׵���Žǣ��Ƕ�ֵ��
	*/
	void SetFov( const Real fov ){ m_fov = fov; }

	/**
	* @brief �����׵���Ž�
	*/
	Real GetFov( void ) const { return m_fov; }

protected:
	Math::Vec3 m_up;    ///>�Ϸ�������
	Math::Vec3 m_t;	   ///>ˮƽ��������
	Math::Vec3 m_dir;   ///>������������λ������
	Math::Vec3 m_pos; ///>�������λ��
	Real			 m_fov;	  ///>�ӿ��Ž�(���Ƕȼ�)
};

#endif