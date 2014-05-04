#ifndef __BALLPATH_H__
#define __BALLPATH_H__

#include "i3dObject.hpp"
#include "CBallFlightMgr.hpp"


#define MAX_BALLS 1000


class CBallFlightPath
{
    public:
    CBallFlightPath(CBallFlightMgr * flight)
    {		
		for (int i = 0; i < MAX_BALLS; i++)
        {
            m_PathBalls[i] = flight->getObject()->clone();
            m_PathBalls[i]->setVisible(false);
        }

		m_flightMgr = flight;
    }

    void display()
    {
        for (int i = 0; i < MAX_BALLS && i < m_flightMgr->getNodeNum(); i++)
        {
            m_PathBalls[i]->setVisible(true);
            m_PathBalls[i]->setPos(m_flightMgr->getNode(i).pos);
        }
    }

    void hide()
    {
        for (int i = 0; i < MAX_BALLS; i++)
        {
            m_PathBalls[i]->setVisible(false);
        }
    }

    private:
    
    CBallFlightMgr * m_flightMgr;
    i3DObject* m_PathBalls[MAX_BALLS];
};

#endif
