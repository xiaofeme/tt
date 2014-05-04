#ifndef __MY_RT_HPP__
#define __MY_RT_HPP__


#include "platform_types.h"

class RK_STEP_NODE
{
public:
    
    _VECTOR3D pos;
    _VECTOR3D vel;
    _VECTOR3D w;

    _VECTOR3D rotation;

    unsigned int index;

	RK_STEP_NODE(){}
	RK_STEP_NODE(_VECTOR3D &p, _VECTOR3D &v, _VECTOR3D &_w){pos = p; vel = v; w = _w;}

	RK_STEP_NODE operator *(f32 d)
	{
	   RK_STEP_NODE node;
       node.pos = pos*d;
       node.vel = vel*d;
	   node.w = w*d;
       node.rotation = rotation*d;
	   return node;
	}


	RK_STEP_NODE operator /(f32 d)
	{
	   RK_STEP_NODE node;
       node.pos = pos/d;
       node.vel = vel/d;
	   node.w = w/d;
       node.rotation= rotation/d;
	   return node;
	}

	RK_STEP_NODE operator +(RK_STEP_NODE & d)
	{
	   RK_STEP_NODE node;
       node.pos = pos + d.pos;
       node.vel = vel + d.vel;
	   node.w = w + d.w;
       node.rotation= rotation + d.rotation;
	   return node;
	}

	RK_STEP_NODE operator -(RK_STEP_NODE & d)
	{
	   RK_STEP_NODE node;
       node.pos = pos - d.pos;
       node.vel = vel - d.vel;
	   node.w = w - d.w;
       node.rotation = rotation - d.rotation;
	   return node;
	}

	void print()
	{
		printf("pos:");
		printf("x:%f,y:%f,z:%f\n", pos._VECT_X, pos._VECT_Y, pos._VECT_Z);

		printf("vel:");
		printf("x:%f,y:%f,z:%f\n", vel._VECT_X, vel._VECT_Y, vel._VECT_Z);

		printf("w:");
		printf("x:%f,y:%f,z:%f\n", w._VECT_X, w._VECT_Y, w._VECT_Z);
		//w.print(1.0/(2*PI));

		printf("\n");
	}
};



int rk_init();

f32 rk_getStep();

void rk_setStep(f32 step);

f32 rk_getBallMass();
void rk_setBallMass(f32 mass);

f32 rk_getLiftCoeff();
void rk_setLiftCoeff(f32 lift);

f32 rk_getAirDensity();
void rk_setAirDensity(f32 density);


f32 rk_getDragCoeff();
void rk_setDragCoeff(f32 density);

void rk_setSpinDrag(f32 drag);
f32 rk_getSpinDrag();


f32 rk_getBallDiameter();
void rk_setBallDiameter(f32 diameter);


int rk_next(RK_STEP_NODE & curr, RK_STEP_NODE & next);

#endif
