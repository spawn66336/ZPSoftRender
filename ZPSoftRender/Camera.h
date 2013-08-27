#ifndef ZP_CAMERA
#define ZP_CAMERA

#include "ZPDependency.h"
#include "ZPMathDependency.h"

class ZPEXPORT Camera
{
public:
	Camera(void);
	virtual ~Camera(void);

	void RotateWithUp( const Real theta );

	void RotateWithT( const Real theta );

	void RotateWithDir( const Real theta );

	void MoveAlongDir( const Real len );

	void MoveAlongUp( const Real len );

	void MoveAlongT( const Real len );

	Math::Vec3 GetPos(void) const { return m_pos; }

	void SetPos( const Math::Vec3& pos ){ m_pos = pos; }

	void Reset( void );

	Math::Matrix4 GetGLCameraMatrix( void );

	Math::Matrix4 GetCameraMatrix( void );
	
	void SetFov( const Real fov ){ m_fov = fov; }

	Real GetFov( void ) const { return m_fov; }

protected:
	Math::Vec3 m_up; ///>�Ϸ�������
	Math::Vec3 m_t;	///>ˮƽ��������
	Math::Vec3 m_dir; ///>������������λ������
	Math::Vec3 m_pos; ///>�������λ��

	Real			 m_fov;	 ///>�ӿ��Ž�(���Ƕȼ�)
};

#endif