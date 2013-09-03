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
	* @brief 相机根据自身的up轴旋转theta度
	* @param theta 旋转的度数（弧度值）
	*/
	void RotateWithUp( const Real theta );

	/**
	* @brief 相机根据自身的t轴旋转theta度
	* @param theta 旋转的度数（弧度值）
	*/
	void RotateWithT( const Real theta );

	/**
	* @brief 相机根据自身的朝向轴旋转theta度
	* @param theta 旋转的度数（弧度值）
	*/
	void RotateWithDir( const Real theta );

	/**
	* @brief 相机向自身朝向方位移动len个单位
	* @param len 要移动的长度
	*/
	void MoveAlongDir( const Real len );

	/**
	* @brief 相机向自身up轴指向方位移动len个单位
	* @param len 要移动的长度
	*/
	void MoveAlongUp( const Real len );

	/**
	* @brief 相机向自身t轴指向方位移动len个单位
	* @param len 要移动的长度
	*/
	void MoveAlongT( const Real len );

	/**
	* @brief  获取相机位置向量
	*/
	inline Math::Vec3 GetPos(void) const { return m_pos; }

	/**
	* @brief  设置相机的位置
	* @param pos 相机位置
	*/
	inline void SetPos( const Math::Vec3& pos ){ m_pos = pos; }

	/**
	* @brief 重置相机的方位
	*/
	void Reset( void );

	/**
	* @brief 获得相机的OpenGL风格矩阵
	*/
	Math::Matrix4 GetGLCameraMatrix( void );

	/**
	* @brief 获得相机矩阵
	*/
	Math::Matrix4 GetCameraMatrix( void );
	
	/**
	* @brief 设置视椎体张角
	* @param fov 视椎体张角（角度值）
	*/
	void SetFov( const Real fov ){ m_fov = fov; }

	/**
	* @brief 获得视椎体张角
	*/
	Real GetFov( void ) const { return m_fov; }

protected:
	Math::Vec3 m_up;    ///>上方向向量
	Math::Vec3 m_t;	   ///>水平方向向量
	Math::Vec3 m_dir;   ///>摄像机所面向的位置向量
	Math::Vec3 m_pos; ///>摄像机的位置
	Real			 m_fov;	  ///>视口张角(按角度计)
};

#endif