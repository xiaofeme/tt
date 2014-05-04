#ifndef DOT_SCENELOADER_H
#define DOT_SCENELOADER_H
 
// Includes
#include "rapidxml.hpp"

#include "platform_types.h"
#include "i3dObject.hpp"


struct _TRANSFORM
{
  _VECTOR3D position;
  _QUATERNION quaternion;
  _VECTOR3D scale;
};

struct _CAMERA_PARAM
{
    f32 fov;
    f32 aspectRatio;
    f32 nearDist;
    f32 farDist;
};

class CGameSceneManager
{
public:
    CGameSceneManager();
    ~CGameSceneManager();

    int load(_string &xml_file);
    i3DObject * getObject(_string & name);
    i3DCamera * getCamera(_string & name);
    i3DLight * getLight(_string & name);

    void changeCamera(int index = -1);
    virtual void toggleCameraFpsMode()
    {
    }

    void update(u32 time);
    void startAnimator(u32 time);
    void stopAnimator();

    virtual char * createFileContent(_string &file) = 0;
    virtual void freeFileContent(char * content) = 0;
    virtual void setAmbientLight(_color &c) = 0;
    virtual void setBackgroundColor(_color &c) = 0;

    virtual i3DObject * createObject(_string &name, _string &mesh) = 0;
    virtual i3DCamera * createCamera(_string&name, _TRANSFORM &t, _CAMERA_PARAM & param) = 0;
    virtual i3DLight * createLight(_string &name, _LIGHT_PARAM &param) = 0;

protected:
#ifdef _IRRLICHT
    _array<i3DCamera*> m_cameras;
    _array<i3DLight*> m_lights;
    _array<i3DObject*> m_all_objects;
#else
	Ogre::vector<i3DCamera*>::type m_cameras;
    Ogre::vector<i3DLight*>::type m_lights;
    Ogre::vector<i3DObject*>::type m_all_objects;
#endif
    void dumpInfo();

    void processScene(rapidxml::xml_node<>* XMLRoot);
    void processNodes(rapidxml::xml_node<>* XMLNode);

    void processLight(rapidxml::xml_node<>* XMLNode);
    void processCamera(rapidxml::xml_node<>* XMLNode, _TRANSFORM &t);
	void processEnvironment(rapidxml::xml_node<>* XMLNode);
    void processNode(rapidxml::xml_node<>* XMLNode);
    //void processLightRange(rapidxml::xml_node<>* XMLNode, Ogre::Light *pLight);
    void processLightAttenuation(rapidxml::xml_node<>* XMLNode, _ATTENUATION &att);

    const char* getAttrib(rapidxml::xml_node<>* XMLNode,  const char *parameter, const char * defaultValue = "");
    f32 getAttribReal(rapidxml::xml_node<>* XMLNode,  const char *parameter, f32 defaultValue = 0);
    _VECTOR3D parseVector3(rapidxml::xml_node<>* XMLNode);
    _QUATERNION parseQuaternion(rapidxml::xml_node<>* XMLNode);
    _color parseColour(rapidxml::xml_node<>* XMLNode);
};

extern CGameSceneManager * getGameSceneMgr();

#endif // DOT_SCENELOADER_H
