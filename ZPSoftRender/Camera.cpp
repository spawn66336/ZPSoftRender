#include "Camera.h"


Camera::Camera(void)
{
	m_up = Math::Vec3::UNIT_Y;
	m_t = Math::Vec3::UNIT_X;
	m_dir = Math::Vec3::UNIT_Z; 
}


Camera::~Camera(void)
{
}

void Camera::RotateWithUp( const Real theta )
{
	Math::Matrix3 rotateMatrix = Math::Matrix3::MakeRotateWithAxisMatrix( m_up , theta );
	m_dir = m_dir * rotateMatrix;
	m_t = m_t * rotateMatrix;

	m_dir.Normalize();
	m_t.Normalize();
}

void Camera::RotateWithT( const Real theta )
{
	Math::Matrix3 rotateMatrix = Math::Matrix3::MakeRotateWithAxisMatrix( m_t , theta );
	m_up = m_up * rotateMatrix;
	m_dir = m_dir * rotateMatrix;

	m_up.Normalize();
	m_dir.Normalize();
}

void Camera::RotateWithDir( const Real theta )
{
	Math::Matrix3 rotateMatrix = Math::Matrix3::MakeRotateWithAxisMatrix( m_dir , theta );
	m_up = m_up * rotateMatrix;
	m_t = m_t * rotateMatrix;

	m_up.Normalize();
	m_t.Normalize();
}

void Camera::Reset( void )
{
	m_up = Math::Vec3::UNIT_Y;
	m_t = Math::Vec3::UNIT_X;
	m_dir = Math::Vec3::UNIT_Z; 
}

Math::Matrix4 Camera::GetCameraMatrix( void )
{
	return Math::Matrix4(
			m_t.x , m_up.x , m_dir.x , 0.0f ,
			m_t.y , m_up.y , m_dir.y , 0.0f ,
			m_t.z , m_up.z , m_dir.z  , 0.0f ,
			m_t.DotProduct( -m_pos ) , m_up.DotProduct( -m_pos ) , m_dir.DotProduct( -m_pos) , 1.0f
		);
}
