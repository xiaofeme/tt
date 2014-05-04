
#ifndef __CCOLLISION_H__
#define __CCOLLISION_H__

#include "platform_types.h"
#include "ICollision.hpp"
#include "IDimension.hpp"

class CCollsionSystem: public iClissionSystem
{
    private:
        iGameDimension * m_3dDimension; 
        unsigned int m_detectFlag;

        f32 m_vertPlanePos;
        bool m_faceUp;
        
        f32 m_horizPlanePos;
        bool m_faceNet;
        bool m_planeCollision;
        
        f32 m_tableFrictionCoeff;
        f32 m_floorFrictionCoeff;

        f32 m_tableRebounceCoeff;
        f32 m_floorRebounceCoeff;

    private:

        bool detectTable(_VECTOR3D & start, _VECTOR3D & end, _VECTOR3D &Q)
    	{
    		f32 ballRadius = m_3dDimension->getBallSize();
            f32 tableLong = m_3dDimension->getTableLong();
            f32 tableWidth = m_3dDimension->getTableWidth();

            if ((start._VECT_Y - ballRadius > 0 && end._VECT_Y - ballRadius > 0)
    			|| (start._VECT_Y + ballRadius < 0 && end._VECT_Y + ballRadius < 0)
    			
    			|| ((start._VECT_X - ballRadius > (0 + tableLong/2.0))
    			   && (end._VECT_X - ballRadius > (0 + 	tableLong/2.0)))
    			|| ((start._VECT_X + ballRadius < (0 - tableLong/2.0))
    			   && (end._VECT_X + ballRadius < (0 - 	tableLong/2.0)))
    			   
    			|| ((start._VECT_Z - ballRadius > (0 + tableWidth/2.0)) 
    			   && (end._VECT_Z - ballRadius > (0 + tableWidth/2.0)))
    			|| ((start._VECT_Z + ballRadius < (0 - tableWidth/2.0)) 
    			   && (end._VECT_Z + ballRadius < (0 - tableWidth/2.0)))
    			)
    		{
    			return false;
    		}
            
            _VECTOR3D dir;
            _VECTOR3D b1;
            _VECTOR3D b2;
            
            dir =(end-start);

            dir._VECT_Z = 0;

            if (dir._VECT_X > 0)
            {
				#ifdef _IRRLICHT
					dir.rotateXYBy(-90);
				#else
					rotateXYby(dir, -90);
				#endif
            }
            else
            {
				#ifdef _IRRLICHT
					dir.rotateXYBy(90);
				#else
					rotateXYby(dir, 90);
				#endif
            }

            dir._normalize();
            dir = dir * ballRadius;

            b1 = start;
            b2 = end;
            b1._VECT_Z = 0;
            b2._VECT_Z = 0;

            b1 += dir;
            b2 += dir;

            f32 k = (b2._VECT_Y - b1._VECT_Y)/(b2._VECT_X - b1._VECT_X);
    		f32 b = b1._VECT_Y - k*b1._VECT_X;

            f32 tableX = (0 - b)/k;

            if (tableX < 0 - tableLong/2.0
                || tableX > 0 + tableLong/2.0)
            {
                return false;
            }  

            ///// side table detection 

            dir =(end-start);

            dir._VECT_X = 0;

            if (dir._VECT_Z > 0)
            {
#ifdef _IRRLICHT
                dir.rotateYZBy(90);
#else
				rotateYZBy(dir, 90);
#endif
            }
            else
            {
#ifdef _IRRLICHT
                dir.rotateYZBy(-90);
#else
				rotateYZBy(dir, -90);
#endif
            }

            dir._normalize();

            dir = dir * ballRadius;

            b1 = start;
            b2 = end;
            b1._VECT_X = 0;
            b2._VECT_X = 0;

            b1 += dir;
            b2 += dir;

             k = (b2._VECT_Y - b1._VECT_Y)/(b2._VECT_Z - b1._VECT_Z);
    		 b = b1._VECT_Y - k*b1._VECT_Z;

            f32 tableZ = (0 - b)/k;

            if (tableZ < 0 - tableWidth/2.0
                || tableZ > 0 + tableWidth/2.0)
            {
                return false;
            }        
    		
    		
    		return true;
    	}

        
        bool detectNet(_VECTOR3D & start, _VECTOR3D & end, _VECTOR3D &Q)
    	{		
    		f32 ballRadius = m_3dDimension->getBallSize();
            f32 netWidth = m_3dDimension->getNetWidth();

            if ((start._VECT_X + ballRadius < 0 && end._VECT_X + ballRadius < 0)
    			|| (start._VECT_X - ballRadius > 0 && end._VECT_X - ballRadius > 0)
    			|| (start._VECT_Z + ballRadius < 0 -netWidth/2.0 && end._VECT_Z + ballRadius < -netWidth/2.0)
    			|| (start._VECT_Z - ballRadius > 0 +netWidth/2.0 && end._VECT_Z - ballRadius > netWidth/2.0))
    		{
    			return false;
    		}

#if 0
			if (end._VECT_X < 0.03)
			{
				printf("test");
			}
#endif
	
            _VECTOR3D dir =(end-start);

            dir._VECT_Z = 0;

            if (dir._VECT_X > 0)
            {
				#ifdef _IRRLICHT
					dir.rotateXYBy(-90);
				#else
					rotateXYby(dir, -90);
				#endif
            }
            else
            {
				#ifdef _IRRLICHT
					dir.rotateXYBy(90);
				#else
					rotateXYby(dir, 90);
				#endif
            }

            if (dir._VECT_Y > 0)
            {
                printf("warning: dir._VECT_Y > 0 !! \n");
            }

    		dir._normalize();

            dir = dir * ballRadius;

            _VECTOR3D b1 = start;
            _VECTOR3D b2 = end;
            b1._VECT_Z = 0;
            b2._VECT_Z = 0;

            b1 += dir;
            b2 += dir;

	        f32 netY = m_3dDimension->getNetHight();

#if 0
			if (b1._VECT_Y > netY && b2._VECT_Y  > netY)
			{
				return false;
			}

			if ((b1._VECT_X < 0 && b2._VECT_X < 0)
    			|| (b1._VECT_X > 0 && b2._VECT_X > 0))
			{
				return false;
			}
#endif
    	
    		f32 k = (b2._VECT_Y - b1._VECT_Y)/(b2._VECT_X - b1._VECT_X);
    		f32 b = b1._VECT_Y - k*b1._VECT_X;

    		if (b > netY)
    		{
    			return false;
    		}
    		else
    		{
    		    return true;
    		}

    	}

        void netRebounce(RK_STEP_NODE & before, RK_STEP_NODE & after)
    	{
    	
            after.pos = before.pos;
            after.w = before.w * 0.75f;

    		after.vel._VECT_X = before.vel._VECT_X*(-0.2f);
    		after.vel._VECT_Y = before.vel._VECT_Y*(0.8f);
    		after.vel._VECT_Z = before.vel._VECT_Z*(0.9f);

    	}

        void tableRebounce(RK_STEP_NODE & before, RK_STEP_NODE & after, 
                           f32 e, f32 mu) 
        {
            f32 r = m_3dDimension->getBallSize();
            f32 m = rk_getBallMass();
    		f32 Jc = 2.0f*m*r*r/3.0f;

			

            _VECTOR3D v1, w1;
            _VECTOR3D v2, w2;
            
            _VECTOR3D R(0, -r, 0);
            
            _VECTOR3D Vxy = before.vel;
            Vxy._VECT_Y = 0;
            _VECTOR3D Wxy = before.w;
            Wxy._VECT_Y = 0;
            
            _VECTOR3D f = before.vel + before.w.crossProduct(R);
            f._VECT_Y = 0;
            f._normalize();

            // slide friction results
            v1 = f*mu*(1+e)*before.vel._VECT_Y;
            w1 = R.crossProduct(f)*m*mu*(1+e)*before.vel._VECT_Y/Jc;


            //roll friction results
            v2 = (R.crossProduct(Wxy) - Vxy)*0.4f;
            w2 = (Wxy + R.crossProduct(Vxy)/(r*r))*(-0.6f);
#if 0
            printf("v1 = ");
            v1.print();
            printf("w1 = ");
            w1.print();

            printf("\n");
            printf("v2 = ");
            v2.print();
            printf("w2 = ");
            w2.print();

            printf("\n");
#endif
			if (v1._getVectLength() < v2._getVectLength())
            {
			//	printf("SLIDE \n");
				after.vel = before.vel + v1;
				after.w = before.w + w1;
            }
            else
            {
				//printf("ROLL \n");
                after.vel = before.vel + v2;
				after.w = before.w + w2;
            }

            after.pos = before.pos;
            after.rotation = before.rotation;

            after.vel._VECT_Y = -e*before.vel._VECT_Y;
            after.w._VECT_Y = before.w._VECT_Y;
#if 0

            printf("before ");
			before.print();
			printf("after ");
			after.print();
            printf("\n");
#endif            
        }

        void tableRebounce_old2(RK_STEP_NODE & before, RK_STEP_NODE & after, 
                           f32 e, f32 mu) 
        {
            f32 r = m_3dDimension->getBallSize();
            f32 m = rk_getBallMass();
    		f32 Jc = 2.0f*m*r*r/3.0f;
            
            _VECTOR3D R(0, -r, 0);
            _VECTOR3D f = before.vel + before.w.crossProduct(R);
            f._normalize();
            f._VECT_Y = 0;

            after.vel = before.vel + f*mu*(1+e)*before.vel._VECT_Y;
            after.vel._VECT_Y = -e*before.vel._VECT_Y;

            after.w = before.w + R.crossProduct(f)*m*mu*(1+e)*before.vel._VECT_Y;
            after.w._VECT_Y = before.w._VECT_Y;

            after.pos = before.pos;
            after.rotation = before.rotation;
        }


        void tableRebounce_old(RK_STEP_NODE & before, RK_STEP_NODE & after, f32 
        rebounce, f32 friction) 
    	{
    		int i;

    		f32 y0[6];
    		f32 w0[3];
    		f32 y[6];
    		f32 w[3];

    		f32 r = m_3dDimension->getBallSize();
    		f32 e = rebounce;
    		f32 m = rk_getBallMass();
    		f32 mu = friction;
    		f32 Jc = 2.0f*m*r*r/3.0f;
    		f32 pi = 3.1415926f;

    		y0[0] = before.pos._VECT_X;
    		y0[1] = before.pos._VECT_Y;
    		y0[2] = before.pos._VECT_Z;

    		y0[3] = before.vel._VECT_X;
    		y0[4] = before.vel._VECT_Y;
    		y0[5] = before.vel._VECT_Z;

    		w0[0] = before.w._VECT_X;
    		w0[1] = before.w._VECT_Y;
    		w0[2] = before.w._VECT_Z;
    	    

    		f32 v0[3],v[3];
    		

    		f32 ve[3];
    		ve[0]=y0[3];
    		ve[1]=y0[4];
    		ve[2]=y0[5];
    		v0[0]=ve[0]+r*w0[2];
    		v0[1]=ve[1];
    		v0[2]=ve[2]-r*w0[0];
    		f32 flag[2],alpha;
    		for(i=0;i<2;i++)
    		{
    			if(v0[2*i]>0.0)
    				flag[i]=-1.0;
    			else if(v0[2*i]<0.0)
    				flag[i]=1.0;
    			else
    				flag[i]=0.0;
    		}
    		if(fabs(v0[3])>1e-6)
    			alpha=atan(v0[0]/v0[2]);
    		else
    			alpha=pi/2;
    		v[1]=-e*v0[1];
    		f32 sf0,sx0,sz0;
    		f32 w1[3];
    		sf0=-mu*m*(e+1)*v0[1];

    		f32 tmp1 = sin(alpha);
    		f32 tmp2 = cos(alpha);
    		sx0=flag[0]*sf0*fabs(sin(alpha));
    		sz0=flag[1]*sf0*fabs(cos(alpha));
    		v[0]=ve[0]+sx0/m;
    		v[2]=ve[2]+sz0/m;
    		w1[0]=w0[0]-r*sz0/Jc;
    		w1[1]=w0[1];
    		w1[2]=w0[2]+r*sx0/Jc;

    		f32 vg[3],wg[3];
    		vg[0]=-0.4f*w0[2]*r+0.6f*ve[0];
    		vg[1]=-e*v0[1];
    		vg[2]=0.4f*w0[0]*r+0.6f*ve[2];
    		wg[0]=vg[2]/r;
    		wg[1]=w0[1];
    		wg[2]=-vg[0]/r;

    		for(i=0;i<3;i=i+2)
    		{
    			if(fabs(ve[i]-vg[i])>=fabs(ve[i]-v[i]))
    			{
                  //  if (i == 0)
    				//  printf("huadong\n");
    				y[3+i]=v[i];
    				w[2-i]=w1[2-i];
    			}
    			else
    			{
    				//if (i ==0 )printf("gundong\n");
    				y[3+i]=vg[i];
    				w[2-i]=wg[2-i];
    			}
    		}
    		w[1]=w1[1];
    		y[4]=v[1];


    		y[0] = y0[0];
    		y[1] = y0[1];
    		y[2] = y0[2];

    		after.pos._VECT_X = y[0]; 
    		after.pos._VECT_Y = y[1]; 
    		after.pos._VECT_Z = y[2];

    		after.vel._VECT_X = y[3];
    		after.vel._VECT_Y = y[4];
    		after.vel._VECT_Z = y[5];

    		after.w._VECT_X = w[0];
    		after.w._VECT_Y = w[1];
    		after.w._VECT_Z = w[2];

#if 0
    		printf("Collide with table result:\n");
    		printf("before: ");
    		before.print();
    		printf("after:");
    		after.print();
    		printf("\n");
#endif
    	}
    public:

        CCollsionSystem()
        {
            m_detectFlag = 0;
            m_3dDimension = getGameDimension();

            m_tableFrictionCoeff = 0.15f;
            m_floorFrictionCoeff = 0.4f;

            m_tableRebounceCoeff = 0.96f;
            m_floorRebounceCoeff = 0.8f;

            m_planeCollision = false;
        }

        ~CCollsionSystem()
        {
            
        }

        virtual void setTableFrictionCoeff(f32 coeff)
        {
            m_tableFrictionCoeff = coeff;
        }

        virtual void setTableRebounceCoeff(f32 coeff)
        {
            m_tableRebounceCoeff = coeff;
        }

        virtual f32 getTableFrictionCoeff()
        {
            return m_tableFrictionCoeff;
        }

        virtual f32 getTableRebounceCoeff()
        {
            return m_tableRebounceCoeff;
        }

        virtual void setFloorFrictionCoeff(f32 coeff)
        {
            m_floorFrictionCoeff = coeff;
        }
        
        virtual void setFloorRebounceCoeff(f32 coeff)
        {
            m_floorRebounceCoeff = coeff;
        }
        
        virtual COLLISION_OBJECT detectBall(RK_STEP_NODE & start, 
    	                                        RK_STEP_NODE  & end,  
    	                                        RK_STEP_NODE & rebounce)
        {
            if (m_detectFlag & COLLISION_OBJECT_FLOOR)
            {
                if (end.pos._VECT_Y - m_3dDimension->getBallSize() < (0-m_3dDimension->getTableHight())) 
        		{
                    tableRebounce(start, rebounce, m_floorRebounceCoeff, m_floorFrictionCoeff);
        			return COLLISION_OBJECT_FLOOR;
        		}
            }
            
            if (m_detectFlag & COLLISION_OBJECT_NET && detectNet(start.pos, end.pos, rebounce.pos))
    		{
                netRebounce(start, rebounce);
                return COLLISION_OBJECT_NET;
    		}
            
    		if (m_detectFlag & COLLISION_OBJECT_TABLE && detectTable(start.pos, end.pos, rebounce.pos))
    		{
                tableRebounce(start, rebounce, m_tableRebounceCoeff, m_tableFrictionCoeff);
                return COLLISION_OBJECT_TABLE;
    		}
            
            if (m_detectFlag & COLLISION_OBJECT_HORIZ_PLANE)
    		{
                if (m_faceUp && end.vel._VECT_Y <= 0.0)
                {
                    if (end.pos._VECT_Y - m_3dDimension->getBallSize() < m_horizPlanePos) 
            		{
                        if (m_planeCollision)
                        {
                            tableRebounce(start, rebounce, m_tableRebounceCoeff, m_tableFrictionCoeff);
                        }
            			return COLLISION_OBJECT_HORIZ_PLANE;
            		}
                }
                else if (!m_faceUp && end.vel._VECT_Y >= 0.0)
                {
                    if (end.pos._VECT_Y + m_3dDimension->getBallSize() > m_horizPlanePos) 
            		{
                        if (m_planeCollision)
                        {
                            tableRebounce(start, rebounce, m_tableRebounceCoeff, m_tableFrictionCoeff);
                        }
            			return COLLISION_OBJECT_HORIZ_PLANE;
            		}
                }
    		}
            
            if (m_detectFlag & COLLISION_OBJECT_VERT_PLANE)
    		{
                if (m_faceNet && (end.pos._VECT_X-start.pos._VECT_X)*m_vertPlanePos > 0.0)
                {
                    if (m_horizPlanePos > 0) 
            		{
                        if (end.pos._VECT_X + m_3dDimension->getBallSize() > m_vertPlanePos)
                        {
            			    return COLLISION_OBJECT_VERT_PLANE;
                        }
            		}
                    else
                    {
                        if (end.pos._VECT_X - m_3dDimension->getBallSize() < m_vertPlanePos)
                        {
            			    return COLLISION_OBJECT_VERT_PLANE;
                        }
                    }
                }
                else if (!m_faceNet && (end.pos._VECT_X-start.pos._VECT_X)*m_vertPlanePos < 0.0)
                {
                    if (m_horizPlanePos > 0) 
            		{
                        if (end.pos._VECT_X - m_3dDimension->getBallSize() < m_vertPlanePos)
                        {
            			    return COLLISION_OBJECT_VERT_PLANE;
                        }
            		}
                    else
                    {
                        if (end.pos._VECT_X + m_3dDimension->getBallSize() > m_vertPlanePos)
                        {
            			    return COLLISION_OBJECT_VERT_PLANE;
                        }
                    }
                }
    		}

			return COLLISION_OBJECT_NONE;
        }

        virtual void setDetectObjects(unsigned int flag)
        {
            m_detectFlag = flag;
        }

        virtual void setVerticalPlane(f32 pos, bool faceNet, bool collision)
        {
            m_vertPlanePos = pos;
            m_faceNet= faceNet;

            m_planeCollision = collision;
        }

        virtual void setHorizPlane(f32 pos, bool faceUp, bool collision)
        {
            m_horizPlanePos = pos;
            m_faceUp = faceUp;

            m_planeCollision = collision;
            
        }      
};


#endif
