
#include <irrlicht.h>
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#include "i3DEngine.hpp"
#include "CBallPath.hpp"
#include "CGameControl.hpp"
#include "CPadController.hpp"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

IrrlichtDevice* irrDevice;

class CIrr3DObject: public i3DObject
{
	friend class CIrrCamera;
	friend class CIrrLight;
public:
    static i3DObject * createObject(_string name, _string meshName) 
    {
        ISceneManager* smgr = irrDevice->getSceneManager();
        IAnimatedMesh* mesh = smgr->getMesh(meshName);
        if (mesh == NULL)
        {
			printf("failed to load %s\n", meshName);
            return NULL;
        }

        return new CIrr3DObject(smgr, name, mesh);
    }
    
	CIrr3DObject(ISceneManager* smgr, _string name, IAnimatedMesh* mesh)
	{
		m_sceneNode = NULL;
		m_mesh = mesh;
		if (mesh)
		{
		    m_sceneNode = smgr->addAnimatedMeshSceneNode( m_mesh );
            m_sceneNode->setName(name);
		}

        m_sceneNode->setMaterialFlag(video::EMF_LIGHTING, true);

	//	m_sceneNode->setMaterialFlag(video::EMF_GOURAUD_SHADING, false);

        if (m_sceneNode->getMaterial(0).MaterialType == video::EMT_TRANSPARENT_ALPHA_CHANNEL)
		{
			m_sceneNode->getMaterial(0).BackfaceCulling = false;
		}
        
        m_sceneNode->setDebugDataVisible(scene::EDS_SKELETON);

        m_smgr = smgr;
        m_smgr->grab();
	}

    CIrr3DObject()
    {
    }


	~CIrr3DObject()
	{
		if (m_mesh)
		{
			m_mesh->drop();
		}

		if (m_sceneNode)
		{
			m_sceneNode->drop();
		}

        if (m_smgr)
        {
            m_smgr->drop();
        }
	}

    const char* getName()
    {
        return m_sceneNode->getName();
    }

    void setVisible(bool visible)
	{
		if (m_sceneNode)
		{
			m_sceneNode->setVisible(visible);
		}
	}

	void dumpInfo()
	{
		printf("Name:%s -- material type:%d\n", m_sceneNode->getName(), m_sceneNode->getMaterial(0).MaterialType);
	}

	void setPos(core::vector3df & pos)  
	{
		if (m_sceneNode)
		{
			m_sceneNode->setPosition(pos);
		}
	}

	void setPos(f32 x, f32 y, f32 z)  
	{
		if (m_sceneNode)
		{
			m_sceneNode->setPosition(core::vector3df(x, y, z));
		}
	}

    void getPos(core::vector3df &pos)
	{
		if (m_sceneNode)
		{
			pos = m_sceneNode->getPosition();
			 
		}
	}

	core::vector3df getPos() 
	{	
		return m_sceneNode->getPosition();
	}

	void setScale(core::vector3df &s)
	{
		if (m_sceneNode)
		{
			m_sceneNode->setScale(s);
		}
	}

	void setScale(f32 x, f32 y, f32 z)
	{
		if (m_sceneNode)
		{
			m_sceneNode->setScale(core::vector3df(x, y, z));
		}
	}

    void getRotation(core::vector3df &rotation)
    {
        rotation = m_sceneNode->getRotation();
    }

    const core::vector3df & getRotation()
    {
        return m_sceneNode->getRotation();
    }

	void setRotation(f32 x, f32 y, f32 z)
	{
		m_sceneNode->setRotation(core::vector3df(x, y, z));
	}

    void setRotation(core::vector3df &rotation)
	{
		m_sceneNode->setRotation(rotation);
	}

    
	void setRotation(core::quaternion & q)
	{
		core::vector3df euler;
		q.toEuler(euler);

		euler.X = core::radToDeg(euler.X);
		euler.Y = core::radToDeg(euler.Y);
		euler.Z = core::radToDeg(euler.Z);

		m_sceneNode->setRotation(euler);
	}

    void getRotation(core::quaternion &q)
    {
        core::vector3df euler;
		euler = m_sceneNode->getRotation();

		euler.X = core::degToRad(euler.X);
		euler.Y = core::degToRad(euler.Y);
		euler.Z = core::degToRad(euler.Z);

        q.set(euler);
    }
    
    void rotate(core::quaternion &q)
    {
        core::quaternion base;

        core::vector3df zero;

        base.rotationFromTo(zero, m_sceneNode->getRotation());

        core::quaternion result = base*q;
		core::vector3df euler;

        result.toEuler(euler);

		euler.X = core::radToDeg(euler.X);
		euler.Y = core::radToDeg(euler.Y);
		euler.Z = core::radToDeg(euler.Z);
		
		m_sceneNode->setRotation(euler);
    }

	void setParent(i3DObject * parent)
	{
		CIrr3DObject* object =(CIrr3DObject*) parent;
		if (m_sceneNode)
		{
			m_sceneNode->setParent(object->m_sceneNode);
		}
	}

    i3DObject * clone()
    {
        CIrr3DObject * clonedObj = new CIrr3DObject();
        clonedObj->m_mesh = m_mesh;
        clonedObj->m_mesh->grab();
        
        clonedObj->m_smgr = m_smgr;
        clonedObj->m_smgr->grab();
        
        clonedObj->m_sceneNode = m_smgr->addAnimatedMeshSceneNode( m_mesh );

        return clonedObj;
    }

private:

	IAnimatedMesh* m_mesh;
	IAnimatedMeshSceneNode* m_sceneNode;

    ISceneManager* m_smgr;
};


class CIrr3DCamera: public i3DCamera
{
public:

    CIrr3DCamera(_string &name, _TRANSFORM & t, _CAMERA_PARAM &param)
    {
        static bool firstCam = true;

		m_camNode = NULL;

        ISceneManager* smgr = irrDevice->getSceneManager();
        //m_camNode = smgr->addCameraSceneNodeFPS(0, 40.0f, 0.01f, -1, 0, 0, false, 
        //                                        0, false, firstCam);

        m_camNode = smgr->addCameraSceneNodeFPS(0,40.0f, 0.005);
        if (firstCam)
        {
            firstCam = false;
        }

        if (m_camNode == NULL)
        {
            printf("CIrr3DCamera, failed to create camera\n");
            return;
        }

     //   irrDevice->getCursorControl()->setVisible(false);
        
        m_camNode->setPosition(t.position);

        vector3df euler;
        t.quaternion.toEuler(euler);

        euler = euler*core::RADTODEG;

        euler.X += 89.8;
        m_camNode->setRotation(euler);

       // printf("rotate:(%f,%f,%f)\n",(euler.X), (euler.Y), (euler.Z));

     //   rotate(t.quaternion);

        setFov(param.fov);
        setNear(param.nearDist);
        setFar(param.farDist);
		m_camNode->setName(name);
    }

    ~CIrr3DCamera()
    {
        if (m_camNode)
            m_camNode->drop();
    }
    
    f32 getFov()
    {
        return m_camNode->getFOV();
    }
    
    void setFov(f32 fov)
    {
        m_camNode->setFOV(fov);
    }

    f32 getAspectRatio()
    {
        return m_camNode->getAspectRatio();
    }
    
    void setAspectRatio(f32 aspect)
    {
        m_camNode->setAspectRatio(aspect);
    }

    f32 getNear()
    {
        return m_camNode->getNearValue();
    }
    
    void setNear(f32 near)
    {
        m_camNode->setNearValue(near);
    }

    f32 getFar()
    {
        return m_camNode->getFarValue();
    }
    
    void setFar(f32 far)
    {
        m_camNode->setFarValue(far);
    }

    bool isActive() 
    {
        ISceneManager* smgr = irrDevice->getSceneManager();
        if (smgr->getActiveCamera() == m_camNode)
        {
            return true;
        }

        return false;
    }

    void setActive() 
    {
        scene::ICameraSceneNode * active = irrDevice->getSceneManager()->getActiveCamera();
        active->setInputReceiverEnabled(false);

        m_camNode->setInputReceiverEnabled(false);

        irrDevice->getSceneManager()->setActiveCamera(m_camNode);
    }

    const char* getName()
    {
		if (m_camNode)
			return m_camNode->getName();
		else return "";
    }

	void dumpInfo()
	{
		//printf("Name:%s -- material type:%d\n", m_sceneNode->getName(), m_sceneNode->getMaterial(0).MaterialType);
	}

	void setPos(core::vector3df & pos)  
	{
		if (m_camNode)
		{
			m_camNode->setPosition(pos);
		}
	}

	void setPos(f32 x, f32 y, f32 z)  
	{
		if (m_camNode)
		{
			m_camNode->setPosition(core::vector3df(x, y, z));
		}
	}

    void getPos(core::vector3df &pos)
	{
		if (m_camNode)
		{
			pos = m_camNode->getPosition();
			 
		}
	}

	core::vector3df getPos() 
	{	
		return m_camNode->getPosition();
	}

    void getRotation(core::vector3df &rotation)
    {
        rotation = m_camNode->getRotation();
    }

    const core::vector3df & getRotation()
    {
        return m_camNode->getRotation();
    }

	void setRotation(f32 x, f32 y, f32 z)
	{
		m_camNode->setRotation(core::vector3df(x, y, z));
	}

    void setRotation(core::vector3df &rotation)
	{
		m_camNode->setRotation(rotation);
	}
    
	void setRotation(core::quaternion & q)
	{
		core::vector3df euler;
		q.toEuler(euler);

		euler.X = core::radToDeg(euler.X);
		euler.Y = core::radToDeg(euler.Y);
		euler.Z = core::radToDeg(euler.Z);

		m_camNode->setRotation(euler);
	}

    void getRotation(core::quaternion &q)
    {
        core::vector3df euler;
		euler = m_camNode->getRotation();

		euler.X = core::degToRad(euler.X);
		euler.Y = core::degToRad(euler.Y);
		euler.Z = core::degToRad(euler.Z);

        q.set(euler);
    }
    
    void rotate(core::quaternion &q)
    {
        core::quaternion base;

        core::vector3df zero;

        base.rotationFromTo(zero, m_camNode->getRotation());

        core::quaternion result = base*q;
		core::vector3df euler;

        result.toEuler(euler);

		euler.X = core::radToDeg(euler.X);
		euler.Y = core::radToDeg(euler.Y);
		euler.Z = core::radToDeg(euler.Z);
		
		m_camNode->setRotation(euler);
    }

	void setParent(i3DObject * parent)
	{
	
	}

    void setTarget(_VECTOR3D & target)
    {
        m_camNode->setTarget(target);
    }

    void getTarget(_VECTOR3D &target)
    {
        target = m_camNode->getTarget();
    }

private:
        ICameraSceneNode * m_camNode;
};

class CIrr3DLight: public i3DLight
{
public:
    CIrr3DLight(_string & name, _LIGHT_PARAM &param)
    {
        m_lightNode = irrDevice->getSceneManager()->addLightSceneNode(0, param.position);
        setParam(param);
        m_lightNode->setName(name);
        m_lightNode->setVisible(true);
    }

    ~CIrr3DLight()
    {
        if (m_lightNode)
        {
            m_lightNode->drop();
        }
    }
    
    int setParam(_LIGHT_PARAM &param)
    {
        SLight light;
        light.Position = param.position;

        light.DiffuseColor = param.diffuse;
        light.SpecularColor= param.specular;
        light.Attenuation.X = param.attenuation.constant;
        light.Attenuation.Y = param.attenuation.linear;
        light.Attenuation.Z = param.attenuation.quadratic;

        light.Radius = 1/light.Attenuation.Y;
        m_lightNode->setLightData(light);

		return 0;
    }

    const char* getName()
    {
        return m_lightNode->getName();
    }

	void dumpInfo()
	{
		//printf("Name:%s -- material type:%d\n", m_lightNode->getName(), m_lightNode->getMaterial(0).MaterialType);
	}

	void setPos(core::vector3df & pos)  
	{
		if (m_lightNode)
		{
			m_lightNode->setPosition(pos);
		}
	}

	void setPos(f32 x, f32 y, f32 z)  
	{
		if (m_lightNode)
		{
			m_lightNode->setPosition(core::vector3df(x, y, z));
		}
	}

    void getPos(core::vector3df &pos)
	{
		if (m_lightNode)
		{
			pos = m_lightNode->getPosition();
			 
		}
	}

	core::vector3df getPos() 
	{	
		return m_lightNode->getPosition();
	}

    void getRotation(core::vector3df &rotation)
    {
        rotation = m_lightNode->getRotation();
    }

    const core::vector3df & getRotation()
    {
        return m_lightNode->getRotation();
    }

	void setRotation(f32 x, f32 y, f32 z)
	{
		m_lightNode->setRotation(core::vector3df(x, y, z));
	}

    void setRotation(core::vector3df &rotation)
	{
		m_lightNode->setRotation(rotation);
	}

    
	void setRotation(core::quaternion & q)
	{
		core::vector3df euler;
		q.toEuler(euler);

		euler.X = core::radToDeg(euler.X);
		euler.Y = core::radToDeg(euler.Y);
		euler.Z = core::radToDeg(euler.Z);

		m_lightNode->setRotation(euler);
	}
    
    void getRotation(core::quaternion &q)
    {
        core::vector3df euler;
		euler = m_lightNode->getRotation();

		euler.X = core::degToRad(euler.X);
		euler.Y = core::degToRad(euler.Y);
		euler.Z = core::degToRad(euler.Z);

        q.set(euler);
    }
    
    void rotate(core::quaternion &q)
    {
        core::quaternion base;

        core::vector3df zero;

        base.rotationFromTo(zero, m_lightNode->getRotation());

        core::quaternion result = base*q;
		core::vector3df euler;

        result.toEuler(euler);

		euler.X = core::radToDeg(euler.X);
		euler.Y = core::radToDeg(euler.Y);
		euler.Z = core::radToDeg(euler.Z);
		
		m_lightNode->setRotation(euler);
    }

	void setParent(i3DObject * parent)
	{
	}

private:
    ILightSceneNode *m_lightNode;
};

class CIrrGameSceneMgr: public CGameSceneManager
{
public:
    char * createFileContent(_string & file_name)
    {
        io::IFileSystem * filesys = irrDevice->getFileSystem();
        if (!filesys->existFile(file_name))
        {
            return NULL;
        }

        io::IReadFile *file = filesys->createAndOpenFile(file_name);
        if (file == NULL)
        {
            return NULL;
        }

        long size = file->getSize();
        char * buf = new char[size+1];
        if (buf == NULL)
        {
            file->drop();
            return NULL;
        }
        
        if (file->read(buf,size) != size)
        {
            printf("read error");
            file->drop();
            return NULL;

        }

        buf[size] = '\0';

        file->drop();

        return buf;
        
    }
    
    void freeFileContent(char * content)
    {
        if (content)
            delete []content;
    }
  
    void setAmbientLight(_color&  c)
    {
        
    }

    void setBackgroundColor(_color&  c)
    {
        
    }
 
    i3DObject * createObject(_string &name, _string &mesh_file)
    {
        i3DObject * obj = CIrr3DObject::createObject(name, mesh_file);
        if (obj == NULL)
        {
            return NULL;
        }

        m_all_objects.push_back(obj);
        return obj;
    }
    
    i3DCamera * createCamera(_string&name, _TRANSFORM & t, _CAMERA_PARAM &param)
    {
        return new CIrr3DCamera(name, t, param);
    }
    
    i3DLight * createLight(_string &name, _LIGHT_PARAM &param)
    {
        return new CIrr3DLight(name, param);
    }

    void toggleCameraFpsMode()
    {
        ICameraSceneNode * cam = irrDevice->getSceneManager()->getActiveCamera();
        if (cam)
        {
            cam->setInputReceiverEnabled( !cam->isInputReceiverEnabled());
        }
    }
};

CGameSceneManager * getGameSceneMgr()
{
    static CIrrGameSceneMgr gameMgr;

    return &gameMgr;
}

class CIrr3DEngine: public i3DEngine, public IEventReceiver
{
	private:
		IVideoDriver* driver;
		ISceneManager* smgr;
		IGUIEnvironment* guienv;

		CBallFlightPath * m_ballPath1;
		CBallFlightPath * m_ballPath2;

        CBallControl * m_ballCtrl;

        bool  m_showBall1;
        bool  m_showBall2;

        u32 windowWidth;
		u32 windowHight;

		CIrrGameSceneMgr m_gameSceneMgr;

    public:

		CIrr3DEngine()
		{
            m_showBall1 = false;
            m_showBall2 = false;
            
            windowWidth = 1024;
            windowHight = 968;
            m_ballCtrl = NULL;
		}

		~CIrr3DEngine()
		{
		}


        bool OnEvent(const SEvent & event);
        void initTestPannel();
        void initPhysicsPannel(IGUIElement* dlg);
        void setPhysics();
        void setMics();

        virtual void init3DEngine()
        {
            initDevice();            
            initGui();
        }

		int initDevice()
		{
			irr::SIrrlichtCreationParameters params;
			params.DriverType=EDT_OPENGL;
			params.WindowSize=core::dimension2d<u32>(windowWidth, windowHight);
            params.Fullscreen = false;
			//params.AntiAlias = 8;
			//params.Vsync = true;
		//	params.Stencilbuffer = true;
		//	params.HandleSRGB = true;

            params.EventReceiver = this;
			irrDevice = createDeviceEx(params);

			if (!irrDevice)
				return -1;

			driver = irrDevice->getVideoDriver();
			smgr = irrDevice->getSceneManager();
			guienv = irrDevice->getGUIEnvironment();

			irrDevice->setWindowCaption(L"Table Tennis Game");
	
			irrDevice->getFileSystem()->addFileArchive("../../models/table");
			irrDevice->getFileSystem()->addFileArchive("../../models/ball");
            irrDevice->getFileSystem()->addFileArchive("../../models/test");
			m_ballCtrl = &m_ball1;

			return 0;

		}

        void initGui();

		bool render() 
		{
			static s32 lastFPS = -1;

			if (irrDevice == NULL)
			{
				return false;
			}

			if (!irrDevice->run())
			{
				irrDevice->drop();
				irrDevice = NULL;
				return false;
			}

            getGameSceneMgr()->update(getTime());
       
			driver->beginScene(true, true, video::SColor(255,113,113,133));

			smgr->drawAll();
			guienv->drawAll();

			driver->endScene();

			const s32 fps = driver->getFPS();

			if (lastFPS != fps)
			{
                const char * camName = irrDevice->getSceneManager()->getActiveCamera()->getName();
				core::stringw str = L"stat: cam - ";
				str += camName;
				str += " FPS:";
				str += fps;

				irrDevice->setWindowCaption(str.c_str());
				lastFPS = fps;
			}

			return true;
		}

		u32 getTime()
		{
			return irrDevice->getTimer()->getTime();
		}

        bool hasModalDialog();
};

 i3DEngine * get3DEngine()
 {
	 static CIrr3DEngine * engine = NULL;

	 if (engine == NULL)
	 {
		engine = new CIrr3DEngine();
	 }

	 return engine;
 }
