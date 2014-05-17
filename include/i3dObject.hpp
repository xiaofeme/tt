
#ifndef __I3DOBJECT_H__
#define __I3DOBJECT_H__

#include "platform_types.h"

class i3DObject;

class IAnimatorObserver
{
    public:
        virtual void started(int id) = 0;
        virtual void running(int id) = 0;
        virtual void stopped(int id) = 0;
};


class CAnimator
{
    public:
        CAnimator()
        {
            m_idle = 0;
            m_started = false;
			m_observer = NULL;
        }
        
        void start(unsigned int timeMs)
        {
            m_startTime = timeMs;
            m_started = true;

            m_currTime = timeMs;

            if (m_observer) 
				m_observer->started(0);
        }

        void stop()
        {
            m_started = false;

			if (m_observer) 
				m_observer->started(0);
        }

        bool isMoving()
        {
            return m_started;
        }

        unsigned int getCurrentTime()
        {
            return m_currTime;
        }

        void update(unsigned int timeMs)
        {
            if (!m_started)
            {
                return;
            }

            u32 passed = (timeMs - m_startTime);

            if (m_idle > 0)
            {
                if (m_idle >= passed)
                {
                    //m_idle -= passed;
                    return;
                }
                else
                {
                    passed -= m_idle;
                    m_idle = 0;
                    m_startTime = timeMs- passed;
                }
            }
            m_currTime = timeMs;

            updateMovement(passed);

			if (m_observer) 
				m_observer->running(0);
        }

        void setObject(i3DObject * node)
        {
            m_node = node;
        }

        i3DObject * getObject()
        {
            return m_node;
        }
        

// should be implemented by inherited class
        virtual void updateMovement(unsigned int passedTime)=0;


        void insertIdle(u32 time)
        {
            m_idle = time;
        }

        virtual const char * getName()
        {
            return "unknown";
        }
        
private:
        u32 m_idle;
        u32 m_startTime;
        bool m_started;
        u32 m_currTime;
        IAnimatorObserver * m_observer;

protected:
        i3DObject * m_node;
};

#ifdef _IRRLICHT
	typedef _list<CAnimator*> AnimatorList;
#else
	typedef _list<CAnimator*>::type AnimatorList;
#endif
class i3DObject
{
public:
	virtual void setPos(f32 x, f32 y, f32 z)
    {
        
	}
    
	virtual void setRotation(f32 x, f32 y, f32 z)
	{
        
	}
    
    virtual void setPos(_VECTOR3D & pos)
    {
        
    }
    virtual void getPos(_VECTOR3D &pos) 
    {
    }
    
	virtual _VECTOR3D getPos() 
	{
        return _VECTOR3D();
	}

    virtual void setRotation(_QUATERNION &q)
    {
        
    }
    virtual void setRotation(_VECTOR3D &rotation)
    {
        
    }
    virtual void getRotation(_VECTOR3D &rotation)
    {
        
    }
    
    virtual const _VECTOR3D & getRotation()
    {
		static _VECTOR3D v;
        return v;
    }
    
    virtual void getRotation(_QUATERNION &q)
    {
        
    }

    virtual void setScale(_VECTOR3D & scale) 
    {
        
    }
    
    virtual void rotate(_QUATERNION &q) 
    {
    }

    virtual void setParent(i3DObject * parent)
    {
        
    }

    virtual void setVisible(bool visible)
    {
        
    }
    
    virtual i3DObject * clone() 
    {
        return NULL;
    }
    
    virtual const char* getName()
    {
        return "Empty";
    }

	virtual void dumpInfo()
	{
        
	}

    void addAnimator(CAnimator* animator)
	{
		if (animator)
		{
            animator->setObject(this);
			m_animators.push_back(animator);
		}

        printf("Animator %s is added to Object %s\n", 
                animator->getName(),  getName());
	}

    void removeAnimator(CAnimator* animator = NULL)
	{
		AnimatorList::Iterator it = m_animators.begin();
		for (; it != m_animators.end(); ++it)
		{
            if (animator == NULL)
            {
                animator->stop();
				m_animators.erase(it);
            }
            else if ((*it) == animator)
			{
                animator->stop();
				m_animators.erase(it);
                printf("Animator %s is removed from Object %s\n", 
                        animator->getName(),  getName());
				return;
			}
		}
	}

    void update(u32 time)
    {
		AnimatorList::Iterator it = m_animators.begin();
		for (; it != m_animators.end(); ++it)
		{
            CAnimator * anim = (*it);
            anim->update(time);
		}
	}

    void stopAnim()
    {
		AnimatorList::Iterator it = m_animators.begin();
		for (; it != m_animators.end(); ++it)
		{
            CAnimator * anim = (*it);
            printf("Object(%s)'s Animator (%s) is stopped\n", getName(), anim->getName());
            anim->stop();
		}
	}

    void startAnimate(u32 time)
    {
		AnimatorList::Iterator it = m_animators.begin();
		for (; it != m_animators.end(); ++it)
		{
            CAnimator * anim = (*it);
            printf("Object(%s)'s Animator (%s) is started\n", getName(), anim->getName());
            anim->start(time);
		}
	}

private:
     AnimatorList m_animators;
};

class i3DCamera: public i3DObject
{
public:
    virtual void setFov(f32 fov){}
    virtual void setAspectRatio(f32 ratio){}
    virtual void setNear(f32 near){}
    virtual void setFar(f32 far){}

    virtual void setTarget(_VECTOR3D & target){}

    virtual void getTarget(_VECTOR3D &target) {}

    virtual f32 getFov() {return 0;}
    virtual f32 getAspectRatio() {return 0;}
    virtual f32 getNear() {return 0;}
    virtual f32 getFar() {return 0;}

    virtual bool isActive() {return true;}
    virtual void setActive() {}
};


enum _LIGHT_TYPE
{
    _LIGHT_POINT,
    _LIGHT_DIECTIONAL,
    _LIGHT_SPORT
};

struct _ATTENUATION
{
    f32 range;
    f32 constant;
    f32 linear;
    f32 quadratic;
};

struct _LIGHT_PARAM
{
    _VECTOR3D position;
    _LIGHT_TYPE lightType;
    _color diffuse;
    _color specular;
    _VECTOR3D shadow;
    _VECTOR3D direction;

    _ATTENUATION attenuation;
};

class i3DLight: public i3DObject
{
    virtual int setParam(_LIGHT_PARAM &param) = 0;
};
#endif
