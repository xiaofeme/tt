
#ifndef __I3DENGINE_H__
#define __I3DENGINE_H__

#include "CPadController.hpp"
#include "CGameControl.hpp"

class i3DEngine
{
    public:
        i3DEngine()
        {
        }
        
        void init()
        {
            init3DEngine();
            getGameSceneMgr()->load(_string("table.scene"));
            m_player.init();
            m_ball1.init();
			m_ball2.init();
        }

        // initialize underlying 3d engile
        virtual void init3DEngine() = 0;
		virtual bool render() = 0;
        virtual unsigned int getTime() = 0;

    protected:
        CPlayerController m_player;
        CBallControl m_ball1;
        CBallControl m_ball2;
};


extern i3DEngine * get3DEngine();

#endif