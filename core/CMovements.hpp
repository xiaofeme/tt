
#ifndef  __CMOVEMENTS_HPP__
#define __CMOVEMENTS_HPP__

#include "platform_types.h"
#include "i3dobject.hpp"

class CRotation: public CAnimator
{
    public:
        CRotation()
        {
        }
        
        CRotation(f32 angle, f32 time, f32 aComp)
        {
            setParam(angle, time, aComp);
        }

        
        void init()
    	{
            //_QUATERNION q;
    		//q.rotationFromTo(_VECTOR3D(0, 1, 0), m_padDir);

    		//m_pad->setRotation(q);
    	}

        void setParam(f32 angle, f32 time, f32 aCompoment)
        {
            m_angle = angle;
            m_time = time;
            m_aTime = m_time*aCompoment;
            m_dTime = m_time - m_aTime;

            f32 h = 2.0f*m_angle/m_time;

            m_a1 = h/m_aTime;
            m_a2 = h/m_dTime;

            m_axis._VECT_X = 0;
            m_axis._VECT_Y = 1;
            m_axis._VECT_Z = 0;

            m_axis._normalize();
        }

        void updateMovement(unsigned int passedTime)
        {
            _QUATERNION q;
            f32 currAngle;
            
            if (passedTime <= m_aTime)
            {
                currAngle = 0.5f*m_a1*passedTime*passedTime;

            }
            else if (passedTime > m_aTime && passedTime <= m_time)
            {
                currAngle = m_angle - 0.5f*m_a2*(m_time-passedTime)*(m_time-passedTime);
            }
            else
            {
                currAngle = m_angle;

                stop();
            }
            
            q.fromAngleAxis(_degToRad(currAngle), m_axis);
            m_node->setRotation(startRotation*q);
        }

        const char * getName()
        {
            return "Rotation";
        }
        
    private:
        f32 m_time; // total time
        f32 m_aTime;  // accelarate time
        f32 m_dTime;  // decelatate time

        _QUATERNION startRotation;

        f32 m_angle;
	    _VECTOR3D m_axis;

        f32 m_a1;
        f32 m_a2;
};

#endif
