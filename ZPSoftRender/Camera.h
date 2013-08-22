#ifndef ZP_CAMERA
#define ZP_CAMERA

#include "ZPDependency.h"
#include "ZPMathDependency.h"

class Camera
{
public:
	Camera(void);
	virtual ~Camera(void);

	void RotateWithUp( const Real theta );

	void RotateWithT( const Real theta );

	void RotateWithDir( const Real theta );

	Math::Vec3 GetPos(void) const { return m_pos; }

	void SetPos( const Math::Vec3& pos ){ m_pos = pos; }

	void Reset( void );

	Math::Matrix4 GetCameraMatrix( void );

protected:
	Math::Vec3 m_up; ///>上方向向量
	Math::Vec3 m_t;	///>水平方向向量
	Math::Vec3 m_dir; ///>摄像机所面向的位置向量
	Math::Vec3 m_pos; ///>摄像机的位置
};

#endif