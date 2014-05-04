
#ifndef __CGAMECONTROL_H__
#define __CGAMECONTROL_H__

#include "CBallFlightMgr.hpp"

class CBallControl: public CBallFlightMgr
{
    private:
        SFlightParam m_throwParam;
		SFlightParam m_hitParam;
			
        f32 m_serveHight;

    public:
		CBallControl()
		{
            m_serveHight = 0.25f;
		}

        void init()
        {
            static int ball_num = 0;
            ball_num++;

			char name[16];
			sprintf(name, "ball%d", ball_num);
            m_node = getGameSceneMgr()->createObject(_string(name), _string("ttball.obj"));
            if (m_node == NULL)
            {
                return;
            }
            
            m_node->addAnimator(this);
            
            SPosOnTable pos;
			pos.high = 0.2f;
			pos.rightHand = false;
			pos.sideB = false;
			pos.tableEnd = -0.3f;
			pos.tableSide = 0.3f;
			
			setTablePos(pos);
        }

		SFlightParam & getThrowParam()
		{
			return m_throwParam;
		}

		SFlightParam & getHitParam()
		{
			return m_hitParam;
		}

		f32 & getServeHight()
		{

			return m_serveHight;
		}
	
        void doHit()
        {
            int flight;
            f32 flightTime = 0;
            
            reset();

			setParam(m_hitParam);
            
            getClissionSystem()->setDetectObjects(COLLISION_OBJECT_TABLE|COLLISION_OBJECT_NET|COLLISION_OBJECT_FLOOR);
           // getClissionSystem()->setDetectObjects(COLLISION_OBJECT_TABLE|COLLISION_OBJECT_FLOOR);

            flight = makeFlight();

		    while (true)
            {
                flight = rebound(flight);
                if (flight < 0)
				{
					break;
				}

                f32 time =  getSegDuration(flight);
				if (time < 0.01 && getSegSequence(flight) > 6)
				{
					break;
				}
            }   
        }

		void doServe()
		{
			int flight, flight1, flight2;

			reset();

			setParam(m_throwParam);
	
			getClissionSystem()->setHorizPlane(m_serveHight, true);
			getClissionSystem()->setDetectObjects(COLLISION_OBJECT_HORIZ_PLANE);

			flight = makeFlight();

			if (getClissionType(flight) != COLLISION_OBJECT_HORIZ_PLANE)
			{
			    // exception
			}

			findServeUpAngle (flight);
			//      
			//m_detector->setDetecPlane(0, true);
			getClissionSystem()->setDetectObjects(COLLISION_OBJECT_TABLE|COLLISION_OBJECT_NET|COLLISION_OBJECT_FLOOR);

			flight1 = makeFlight(flight);
			if (getClissionType(flight1) != COLLISION_OBJECT_TABLE)
			{
			    printf("flight1->getClissionType() != COLLISION_OBJECT_TABLE %d\n", getClissionType(flight1));
			 //   return -1;
			}

			flight2 = rebound(flight1);

			if (getClissionType(flight2) != COLLISION_OBJECT_TABLE)
			{
			    printf("flight2->getClissionType() != COLLISION_OBJECT_TABLE\n");
			    //return -1;
			}

			while (flight2 >= 0 )
			{
			    flight2 = rebound(flight2);
				if (flight2 < 0)
				{
					break;
				}
				f32 time =  getSegDuration(flight2);
				if (time < 0.01 && getSegSequence(flight) > 6)
				{
					break;
				}
			}   
        }

    private:            
        f32 findServeUpAngle(int throwFlight)
        {
            int flight1, flight2;
            
            f32 upTableAngle = 30;
            f32 downTableAngle = -60;
            
            f32 tableAngle = (upTableAngle + downTableAngle)/2;

            int maxTry = 5;

            setParam(m_hitParam);

            getClissionSystem()->setHorizPlane(0, true, true);
            getClissionSystem()->setDetectObjects(COLLISION_OBJECT_HORIZ_PLANE);

            while (maxTry > 0)
            {
                maxTry--;

                getParam().upAngle = tableAngle;
                
                flight1 = makeFlight(throwFlight);

                flight2 = rebound(flight1);

                _VECTOR3D & left = getSegmengNode(flight2, 0)->pos;
                _VECTOR3D & right = getSegmengNode(flight2, -1)->pos;

                _VECTOR3D center  = (left + right)/2;
                _VECTOR3D distance = (right - left);

                // center accuracy;
                if (fabs(center._X/distance._X) < 0.2f)
                {
                    break;
                }
                else if (getStartPos()._X * center._X > 0)
                {
                    downTableAngle = tableAngle;
                }
                else
                {
                    upTableAngle = tableAngle;
                }

                tableAngle = (upTableAngle + downTableAngle)/2;
            }

            removeAfter(throwFlight);

            return tableAngle;
        }

};
#endif 
