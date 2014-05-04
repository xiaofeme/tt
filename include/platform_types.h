#ifndef __PLATFORM_TYPES_H__
#define __PLATFORM_TYPES_H__

#ifdef _IRRLICHT
    #include "irrList.h"
	#include "irrArray.h"
	#include "irrMap.h"
    #include "vector3d.h"
    #include "quaternion.h"
	#include "SColor.h"
    using namespace irr;
    using namespace core;
	using namespace video;

    typedef core::vector3df _VECTOR3D;
    typedef core::quaternion _QUATERNION;
    #define _VECT_X X
    #define _VECT_Y Y
    #define _VECT_Z Z

	#define _X X
    #define _Y Y
    #define _Z Z
	#define _W W 

    #define _setVectLength setLength
    #define _getVectLength getLength
	#define _normalize normalize
    #define _PI core::PI
	#define _degToRad core::degToRad
	#define  _list core::list
	#define _array core::array
	#define _string core::stringc
	#define _map core::map
	#define _color video::SColorf
#else
// TODO include ogre header files
	#include <Ogre.h>
	using namespace Ogre;

    typedef Vector3 _VECTOR3D;
    typedef Quaternion _QUATERNION;

    #define _VECT_X x
    #define _VECT_Y y
    #define _VECT_Z z
	#define _X x
    #define _Y y
    #define _Z z
	#define _W w
    #define _setVectLength setLength
    #define _getVectLength length
    typedef float f32;
	typedef unsigned int u32;
	#define  _list Ogre::list
	#define _PI Math::PI
	#define Iterator iterator
    typedef Ogre::ColourValue _color;
	#define _normalize normalise
	#define _degToRad Math::DegreesToRadians
	#define _string Ogre::String
	#define _array Ogre::vector

	#define fromAngleAxis(angle, axis) FromAngleAxis(Radian(angle), axis)

	inline void rotateXYby(_VECTOR3D & vec, f32 degrees, _VECTOR3D & center=_VECTOR3D())
	{
			degrees = Math::DegreesToRadians(degrees);
			f32 cs = Math::Cos(degrees);
			f32 sn = Math::Sin(degrees);
			vec.x -= center.x;
			vec.y -= center.y;

			vec.x = vec.x*cs - vec.y*sn;
			vec.y = vec.x*sn + vec.y*cs;

			vec.x += center.x;
			vec.y += center.y;
	}

	inline void rotateXZBy(_VECTOR3D & vec, f32 degrees, _VECTOR3D & center=_VECTOR3D())
	{
		degrees = Math::DegreesToRadians(degrees);
		f32 cs = Math::Cos(degrees);
		f32 sn = Math::Sin(degrees);
		vec.x -= center.x;
		vec.z -= center.z;
		
		vec.x = vec.x*cs - vec.z*sn;
		vec.z = vec.x*sn + vec.z*cs;
		
		vec.x += center.x;
		vec.z += center.z;
	}

	inline void rotateYZBy(_VECTOR3D & vec, f32 degrees, _VECTOR3D & center=_VECTOR3D())
	{
		degrees = Math::DegreesToRadians(degrees);
		f32 cs = Math::Cos(degrees);
		f32 sn = Math::Sin(degrees);
		vec.z -= center.z;
		vec.y -= center.y;

		vec.y = vec.y*cs - vec.z*sn;
		vec.z = vec.y*sn + vec.z*cs;
		
		vec.z += center.z;
		vec.y += center.y;
	}
#endif

	

#endif
