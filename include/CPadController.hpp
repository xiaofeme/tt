
#ifndef __CPADCONTROLLER_HPP__
#define __CPADCONTROLLER_HPP__

#include "platform_types.h"
#include "i3dObject.hpp"

#include "CMovements.hpp"
#include "GameSceneManager.hpp"

class CPlayerController: public IAnimatorObserver, public CAnimator
{
public:

    void makeForhandPull(f32 time, f32 degree, f32 dir)
    {
        
    }

    CPlayerController()
    {
      
    }

    void init()
    {
		
        static int pad_num = 0;
        pad_num++;
		char name[16];
		sprintf(name, "player%d", pad_num);
        m_pad = getGameSceneMgr()->createObject(_string(name), _string("m1_male1591.mesh"));
        if (m_pad == NULL)
        {
            return;
        }

        m_pad->setPos(-2.3f, -0.76, -1);

        m_pad->addAnimator(this);

#if 0
        CGameSceneMgr::getInstance()->addAnimatedNode(m_arm);
        CGameSceneMgr::getInstance()->addAnimatedNode(m_body);

       //
       CAnimator * bodyRot = new CRotation(90, 1500, 0.8f);
       m_body->addAnimator(bodyRot);

        CAnimator * armRot = new CRotation(50, 500, 0.8f);
        m_arm->addAnimator(armRot);
        armRot->insertIdle(1500);

        m_body->setPos(-2.0f, -0.2f, 0);

        m_arm->setParent(m_body);
        m_arm->setPos(0.2f, 0.3f, -0.3f);
       // m_pad->setParent(m_arm);
        _QUATERNION q;
        q.rotationFromTo(_VECTOR3D(0, 1, 0), _VECTOR3D(1, 0, 0));
        m_pad->setRotation(q);        
#endif
    }

    void updateMovement(unsigned int passedTime)
    {
        
    }

    void started(int id)
    {
    }

    void stopped(int id)
    {
    }

    void running(int id)
    {
    }
    
    
private:
    i3DObject * m_pad;
};
#endif
