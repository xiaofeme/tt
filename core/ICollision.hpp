
#ifndef __ICOLLISION_DETECT_H__
#define __ICOLLISION_DETECT_H__

#include "rk.h"


enum COLLISION_OBJECT
{
    COLLISION_OBJECT_NONE = 0x0,
    COLLISION_OBJECT_TABLE = 0x01,
    COLLISION_OBJECT_NET = 0x02,
    COLLISION_OBJECT_FLOOR = 0x04,
    COLLISION_OBJECT_VERT_PLANE = 0x08,
    COLLISION_OBJECT_HORIZ_PLANE = 0x10,
    COLLISION_OBJECT_ALL = 0xFF
};


class iClissionSystem
{
    public:

    	virtual COLLISION_OBJECT detectBall(RK_STEP_NODE & start, 
    	                                  RK_STEP_NODE  & end,  
    	                                 RK_STEP_NODE & rebounce ) = 0;

        virtual void setDetectObjects(unsigned int flag) = 0;

        virtual void setVerticalPlane(f32 pos, bool faceNet, bool collision = false) = 0;    	 

        virtual void setHorizPlane(f32 pos, bool faceUp, bool collision = false) = 0;   

        virtual void setTableFrictionCoeff(f32 coeff) = 0;
        virtual void setTableRebounceCoeff(f32 coeff) = 0;

        virtual f32 getTableFrictionCoeff() = 0;
        virtual f32 getTableRebounceCoeff() = 0;

        virtual void setFloorFrictionCoeff(f32 coeff) = 0;
        virtual void setFloorRebounceCoeff(f32 coeff) = 0;
};


extern iClissionSystem * getClissionSystem();

#endif