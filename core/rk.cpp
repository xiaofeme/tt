#include "rk.h"


struct RK_PARAM
{
	f32 m;
	f32 d;

	f32 p;
	f32 rw;

	f32 cl;
	f32 cd;
	f32 g;
};

static RK_PARAM g_rk_param;
static f32 g_time_step;

void rk_function(RK_PARAM & param, f32 t,RK_STEP_NODE & in, RK_STEP_NODE & out)
{
    f32 cl;
    f32 cd;

    _VECTOR3D G(0, -param.m*param.g, 0); // 重力
    _VECTOR3D FF = _VECTOR3D(0, 1, 0)*param.p*param.g*_PI*param.d*param.d*param.d/6.0f; // 空气浮力
    _VECTOR3D R;
    _VECTOR3D FM;

	if (in.w._getVectLength() > 0.1)
    {
        f32 v_w = in.vel._getVectLength()/in.w._getVectLength();

        cl = (1.0f/(2.02f+0.981f*v_w));
        cd = 0.508f + powf(1.0f/(22.053f+4.196f*powf(v_w, 2.5f)), 0.4f);
        FM = in.w.crossProduct(in.vel)*cl*param.p*param.d*param.d*param.d/(2*_PI); // 马格努斯力
    }
    else
    {
        cl = 0;
        cd = 0.508f;
    }    
    
    R = in.vel*(-0.125f)*_PI*cd*param.p*param.d*param.d*in.vel._getVectLength(); // 空气阻力
	
    out.pos = in.vel;
	out.vel = (G + R + FM + FF)/param.m; // a =F/m
	out.w = in.w*(-0.2f)*_PI*param.cd*param.p*param.d*param.d*in.w._getVectLength(); //_VECTOR3D(0,0,0);

    out.rotation = in.w;
}

void rk_setDefaultParam(RK_PARAM & param)
{
    param.m=0.0027f;
	param.d=0.04f;
	param.rw=0;
	//param.p=1.293;
    param.p=1.3f;
	param.cl=1.23f;
	param.cd=0.5f;
	param.g=9.8f;
}

f32 rk_getStep()
{
    return g_time_step;
}

void rk_setStep(f32 step)
{
    g_time_step = step;
}



f32 rk_getBallMass()
{
    return g_rk_param.m;
}

void rk_setBallMass(f32 mass)
{
    
}

f32 rk_getLiftCoeff()
{
    return g_rk_param.cl;
}

void rk_setLiftCoeff(f32 lift)
{
    g_rk_param.cl = lift;
}

f32 rk_getAirDensity()
{
    return g_rk_param.p;
}

void rk_setAirDensity(f32 density)
{
    g_rk_param.p = density;
}

f32 rk_getDragCoeff()
{
    return g_rk_param.cd;
}

f32 rk_getSpinDrag()
{
    return g_rk_param.rw;
}

void rk_setSpinDrag(f32 drag)
{
    g_rk_param.rw = drag;
}


void rk_setDragCoeff(f32 drag)
{
    g_rk_param.cd = drag;
}


f32 rk_getBallDiameter()
{
    return g_rk_param.d;
}

void rk_setBallDiameter(f32 diameter)
{
    g_rk_param.d = diameter;
}

int rk_init()
{
	g_time_step = 0.005f;

	rk_setDefaultParam(g_rk_param);
	return 0;
}

int rk_next(RK_STEP_NODE & curr, RK_STEP_NODE & next)
{
	RK_STEP_NODE K1,K2,K3,K4;
    
	rk_function(g_rk_param, 0, curr,K1);
	rk_function(g_rk_param, 0+g_time_step/2, curr+K1*(g_time_step*0.5f), K2);
	rk_function(g_rk_param, 0+g_time_step/2,curr+K2*(g_time_step*0.5f), K3);
	rk_function(g_rk_param, 0+g_time_step,curr+K3*g_time_step, K4);
        
	next = curr + (K1 + K2*2 + K3*2 + K4)*g_time_step/6; 

    next.index = curr.index + 1;
	return 0;
}


#if 0
#define MAX_NODES 200
void test()
{
    int len;
    RK_STEP_NODE node1[MAX_NODES];
    RK_STEP_NODE node2[MAX_NODES];
    RK_STEP_NODE init(_VECTOR3D(1,2,3), _VECTOR3D(1,2,3), _VECTOR3D(10, 20, 30));

    rk_init(init, node1, MAX_NODES, 0.001, len, NULL);
    rk_calc2(init, node2, MAX_NODES, 0.001, len, NULL);

    for (int i = 0; i < MAX_NODES; i++)
    {
        node1[i].print();
        node2[i].print();
        printf("\n");
    }
}


void test()
{
	_VECTOR3D  start(-0.026, 0.8488, 0);
	_VECTOR3D  end(-0.0325, 0.8447, 0);

	checkNetCollision(start, end);
}

#endif

