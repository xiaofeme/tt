
#ifndef __CBALLFLIGHTMGR_H__
#define __CBALLFLIGHTMGR_H__

#include "platform_types.h"
#include "rk.h"
#include <vector>
#include "IDimension.hpp"
#include "ICollision.hpp"
#include "i3dobject.hpp"
using namespace std;

enum TableSide
{
    TableSide_A,
    TableSide_B,
    TableSide_AUTO
};

struct SPosOnTable
{
    bool sideB; 
    bool rightHand;
    f32 high;
    f32 tableEnd;
    f32 tableSide;
};


struct SFlightParam
{
    SFlightParam():speed(0.0), spin(0.0), upAngle(0.0), sideAngle(0.0), sideSpin(0.0), vertSpin(0.0)
    {
        netAngle = false;
    }

    SFlightParam(f32 _speed, f32 _spin, f32 _upAngle = 0, 
                    f32 _sideAngle = 0, f32 _sideSpin = 0, f32 _vertSpin = 0)
    {
        speed = _speed;
		spin = _spin;
		upAngle = _upAngle;
		sideAngle = _sideAngle;
		sideSpin = _sideSpin;
		vertSpin =  _vertSpin;
        netAngle = false;
    }
    
    f32 speed;
    f32 spin;
    f32 upAngle;
    bool netAngle;
    
    f32 sideAngle;
    f32 sideSpin;
    f32 vertSpin;
};


struct SSegmentData
{

    SSegmentData()
    {
        startIndex = -1;
        endIndex = -1;
        next = -1;
        topIndex = -1;
        sequence = -1;
    }
    
    int startIndex;
    int endIndex;


    COLLISION_OBJECT clissionObject;

    RK_STEP_NODE rebounce;

    int next;

    int sequence;

    int topIndex;
};


class CFlightSegments
{
public:
    
    #define MAX_FLIGHT_DATA 64
    SSegmentData m_segmengData[MAX_FLIGHT_DATA];
    int m_freeHeader;
        
public:
    CFlightSegments()
    {
        m_freeHeader = 0;
        for (int i = 0; i < MAX_FLIGHT_DATA - 1; i++)
        {
            m_segmengData[i].next = i+1;
        }

        m_segmengData[MAX_FLIGHT_DATA - 1].next = -1;
    }

    ~CFlightSegments()
    {
    }

    int alloc()
    {
        if (m_freeHeader < 0)
        {
            return -1;
        }

        int result = m_freeHeader;

        m_freeHeader = m_segmengData[m_freeHeader].next;
        m_segmengData[result].next = -1;

        return result;
    }

    void free(int header, int tail)
    {
        if (header < 0 || tail < 0)
        {
            return;
        }
        
        m_segmengData[tail].next = m_freeHeader;
        m_freeHeader = header;
    }

    SSegmentData * getData(int index)
    {
        if (index < 0)
        {
            return NULL;
        }
        return &m_segmengData[index];
    }
};



class CBallFlightMgr : public CAnimator
{
private:
    std::vector<RK_STEP_NODE> m_vecFlightNode;
    unsigned long m_lastNode;

    CFlightSegments m_segments;
    int m_startSegment;
    int m_lastSegment;

    SFlightParam m_param;
	_VECTOR3D m_startPos;

	SPosOnTable m_startTablePos;
    
    int m_side;

    //bool m_started;
    //unsigned long m_startTime;

	RK_STEP_NODE updatedNode;
	
public:
        CBallFlightMgr()
        {
 	        rk_init();    

            m_side = TableSide_AUTO;

            m_vecFlightNode.reserve(1000);
		    m_vecFlightNode.resize(1000);

			m_startSegment = -1;
            m_lastSegment = -1;

            reset();
        }

        ~CBallFlightMgr()
        {
         
        }

        const char * getName()
        {
            return "BallFlight";
        }

		void setTablePos(SPosOnTable & pos)
		{
			m_startTablePos = pos;
			m_startPos._X = getGameDimension()->getTableLong()/2 - pos.tableEnd;
			m_startPos._Y = pos.high;
			m_startPos._Z = -getGameDimension()->getTableWidth()/2 + pos.tableSide;

			if (pos.sideB)
			{
				m_startPos._X = - m_startPos._X;
				if (!pos.rightHand)
				{
					m_startPos._Z = - m_startPos._Z;
				}
			}
			else
			{
				if (pos.rightHand)
				{
					m_startPos._Z = - m_startPos._Z;
				}
			}

            m_node->setPos(m_startPos);
		}

		SPosOnTable & getTablePos()
		{
			return m_startTablePos;
		}

		void getTablePos(SPosOnTable & pos)
		{
			pos = m_startTablePos;
		}

        _VECTOR3D getStartPos()
		{
			return m_startPos;
		}

        void setParam(SFlightParam & param)
        {
            m_param = param;
        }
        
        SFlightParam & getParam()
        {
            return m_param;
        }

        void removeAfter(int seg)
        {
            SSegmentData * segData = m_segments.getData(seg);
            if (segData == NULL)
            {
                return;
            }
            
            m_segments.free(segData->next, m_lastSegment);
            segData->next = -1;
            m_lastSegment = seg;

            m_lastNode = segData->endIndex + 1;
        }

        RK_STEP_NODE * getCollisionNode(int seg)
		{
			SSegmentData * segData = m_segments.getData(seg);
            return &segData->rebounce;
		}

        COLLISION_OBJECT getClissionType(int seg)
		{
            SSegmentData * segData = m_segments.getData(seg);
			return segData->clissionObject;
		}

        f32 getSegDuration(int seg)
        {
            SSegmentData * segData = m_segments.getData(seg);
            
            return  (segData->endIndex - segData->startIndex + 1)* rk_getStep();
        }

        f32 getDuration()
        {
            return m_lastNode* rk_getStep(); 
        }
        
        RK_STEP_NODE * getSegmengNode(int seg, int index)
        {
            SSegmentData * segData = m_segments.getData(seg);
            if (index > segData->endIndex - segData->startIndex)
            {
                return NULL;
            }

            if (index < 0)
            {
                return &m_vecFlightNode[segData->endIndex];
            }
            else
            {
                return &m_vecFlightNode[segData->startIndex + index];
            }
        }

        void cutSegment(int seg, int nodeIndex)
        {
            removeAfter(seg);

            SSegmentData * data = m_segments.getData(seg);
            data->endIndex = nodeIndex;
            m_lastNode = nodeIndex + 1;
        }

        void makeSegment(SSegmentData * seg, RK_STEP_NODE & init)
        {
            
            
            iClissionSystem * detect = getClissionSystem();

            seg->startIndex = m_lastNode;
            
            if (m_lastNode >= m_vecFlightNode.size())
			{
				m_vecFlightNode.reserve(m_vecFlightNode.size()*2);
				m_vecFlightNode.resize(m_vecFlightNode.size()*2);
			}

            if (init.index == 1)
            {
			    m_vecFlightNode[m_lastNode++] = init;
            }
            else
            {
                rk_next(init, m_vecFlightNode[m_lastNode]);
                seg->clissionObject = detect->detectBall(init,m_vecFlightNode[m_lastNode], seg->rebounce);

                if (seg->clissionObject == COLLISION_OBJECT_NONE)
                {
                    m_lastNode++;
                }
                else
    			{
                    seg->startIndex = -1;
                    seg->endIndex = -1;
                    return;
    			}
            }

            f32 top = m_vecFlightNode[m_lastNode-1].pos._Y;
            int topIndex = m_lastNode-1;
            
            while (true)
            {
				if (m_lastNode >= m_vecFlightNode.size())
    			{
    				m_vecFlightNode.reserve(m_vecFlightNode.size()*2);
    				m_vecFlightNode.resize(m_vecFlightNode.size()*2);
    			}

                rk_next(m_vecFlightNode[m_lastNode-1], m_vecFlightNode[m_lastNode]);

                

                seg->clissionObject = detect->detectBall(m_vecFlightNode[m_lastNode-1],m_vecFlightNode[m_lastNode], seg->rebounce);

                if (seg->clissionObject == COLLISION_OBJECT_NONE)
                {
                    if (m_vecFlightNode[m_lastNode].pos._VECT_Y > top)
                    {
                        topIndex = m_lastNode;
                        top = m_vecFlightNode[m_lastNode].pos._VECT_Y;
                    }
                }
                else
    			{
                    seg->rebounce.index = m_vecFlightNode[m_lastNode-1].index;
    				break;
    			}

                m_lastNode++;
				
            }        

            seg->endIndex = m_lastNode -1;
        }

        int rebound(int seg)
        {
            int newSeg = m_segments.alloc();
            if (newSeg == -1)
            {
                return -1;
            }

            m_segments.getData(seg)->next = newSeg;

            m_segments.getData(newSeg)->sequence = m_segments.getData(seg)->sequence + 1;
            
            makeSegment(m_segments.getData(newSeg), m_segments.getData(seg)->rebounce);

            m_lastSegment = newSeg;
            return newSeg;
        }

        int getSegSequence(int seg) 
        {
           return m_segments.getData(seg)->sequence;
        }
        
        // return fight id
        virtual int makeFlight(int parent = -1)
        {
            
            RK_STEP_NODE  init;     

            bool firstNode = true;

            if (parent != -1)
            {
                firstNode = false;
                int endIndex = m_segments.getData(parent)->endIndex;
                init.pos = m_vecFlightNode[endIndex].pos;
                init.rotation =  m_vecFlightNode[endIndex].rotation;
                init.index = m_vecFlightNode[endIndex].index;
            }
            else
            {
                init.index = 1;

				init.pos = m_startPos; 
				//init.rotation = _VECTOR3D(); 
            }

            int side = m_side;

            if (m_side = TableSide_AUTO)
            {
                if (init.pos._VECT_X < 0)
                {
                    side = TableSide_B;
                }
                else
                {
                    side = TableSide_A;
                }
            }

            _VECTOR3D dir;
            _VECTOR3D target;

            if (m_param.netAngle)
            {
                target._Y = getGameDimension()->getNetHight();
            }
            else
            {
                target._VECT_Y = init.pos._VECT_Y;
            }

            dir = target - init.pos;
            dir._VECT_Z = 0;

            if (side == TableSide_B)
            {
				#ifdef _IRRLICHT
					dir.rotateXYBy(m_param.upAngle);
				#else
					rotateXYby(dir, m_param.upAngle);
				#endif
            }
            else
            {
				#ifdef _IRRLICHT
					dir.rotateXYBy(-m_param.upAngle);
				#else
					rotateXYby(dir, -m_param.upAngle);
				#endif
            }

			dir._normalize();

			#ifdef _IRRLICHT
            dir.rotateXZBy(m_param.sideAngle);
			#else
			rotateXZBy(dir, m_param.sideAngle);
			#endif

            init.vel = dir*m_param.speed;    


            f32 spinVal = m_param.spin;
            
            if (side == TableSide_B)
            {
                spinVal = - spinVal;
            }
            
            _VECTOR3D spin(0, 0, spinVal*2*(_PI));

            f32 sideAngle = m_param.sideSpin * 90;
            f32 vertAngle = m_param.vertSpin * 90;  

#ifdef _IRRLICHT
            spin.rotateXZBy(m_param.sideAngle);
            spin.rotateXZBy(sideAngle);
            spin.rotateYZBy(vertAngle);
#else
			rotateXZBy(spin, m_param.sideAngle);
            rotateXZBy(spin, sideAngle);
            rotateYZBy(spin, vertAngle);
#endif
            init.w = spin;
            
            if (side == TableSide_B)
            {
               vertAngle = -vertAngle;
            }

            int seg = m_segments.alloc();

            if (seg < 0)
            {
                return -1;
            }

            if (parent < 0)
            {
                reset();
                m_startSegment = seg;
            }
            else
            {
                removeAfter(parent);
                m_segments.getData(parent)->next = seg;
            }

            m_lastSegment = seg;

            m_segments.getData(seg)->sequence = 1;

            makeSegment(m_segments.getData(seg), init);
            return seg;
        }
        
        virtual void reset()
        {
            m_lastNode = 0;
        
			if (m_startSegment >= 0)
			{
				m_segments.free(m_startSegment, m_lastSegment);
                m_startSegment = -1;
                m_lastSegment = -1;
			}

			m_param = SFlightParam();

        }

        void dumpFlightNodes()
        {
            for (unsigned long i = 0; i < m_lastNode; i++)
            {
                printf("%d: ", i+1);
                m_vecFlightNode[i].print();
            }
        }

        void updateMovement(unsigned int passedTime)
        {
            if (m_lastNode <= 0)
            {
                stop();
                return;
            }

            f32 oldStep = rk_getStep();

            unsigned int flightTime = (u32)m_lastNode* oldStep*1000; 

            if (passedTime >= flightTime)
            {
				stop();
                return;
            }

            f32 timeIndex = passedTime/(oldStep*1000);
            unsigned long index = (unsigned long)floor(timeIndex);

            if (index >= m_lastNode)
            {
                stop();
                return;
            }

            f32 microStep = (timeIndex - index)*oldStep;

          //  printf("passedTime = %d, timeIndex = %3.3f, index=%d, microStep = %3.3f\n", passedTime, timeIndex, index, microStep);

            RK_STEP_NODE result;
            if (microStep > oldStep/10)
            {
                RK_STEP_NODE target;
                rk_setStep(microStep);
                rk_next(m_vecFlightNode[(int)index], result);

                rk_setStep(oldStep);
            }
            else
            {
				result = m_vecFlightNode[(int)index];
            }

            m_node->setPos(result.pos);

			return;
        }

		int getNodeNum()
		{
			return m_lastNode;
		}

		RK_STEP_NODE & getNode(int index)
		{
			return m_vecFlightNode[index];
		}
};

#endif
