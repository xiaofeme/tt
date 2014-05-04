#include "GameSceneManager.hpp"

#pragma warning(disable:4390)
#pragma warning(disable:4305)


CGameSceneManager::CGameSceneManager() 
{
}
 
CGameSceneManager::~CGameSceneManager()
{

}

void  CGameSceneManager::changeCamera(int index)
{
    if (index >= 0 && index < m_cameras.size())
    {
        m_cameras[index]->setActive();
        return;
    }
    
    for (u32 i=0; i< m_cameras.size(); ++i)
    {
        if (m_cameras[i]->isActive())
        {
            u32 active = (i+1)%m_cameras.size();
            m_cameras[active]->setActive();
            return;
        }
    }

    if (m_cameras.size() > 0)
    {
        m_cameras[0]->setActive();
    }
}


void CGameSceneManager::update(u32 time)
{
    for (u32 i=0; i< m_all_objects.size(); ++i)
	{
        m_all_objects[i]->update(time);
	}
}

void CGameSceneManager::stopAnimator()
{
    for (u32 i=0; i< m_all_objects.size(); ++i)
	{
        m_all_objects[i]->stopAnim();
	}
}

void CGameSceneManager::startAnimator(u32 time)
{
	for (u32 i=0; i< m_all_objects.size(); ++i)
	{
        m_all_objects[i]->startAnimate(time);
	}
}

i3DObject * CGameSceneManager::getObject(_string & name) 
{
	for (u32 i=0; i< m_all_objects.size(); ++i)
	{
        if (name == m_all_objects[i]->getName())
		{
			return m_all_objects[i];
		}
	}

	return NULL;
}

i3DCamera * CGameSceneManager::getCamera(_string & name) 
{
    return (i3DCamera*)getObject(name);
}

i3DLight * CGameSceneManager::getLight(_string & name) 
{
    return (i3DLight*)getObject(name);
}

void CGameSceneManager::dumpInfo()
{
	u32 i;
    for (i=0; i< m_cameras.size(); ++i)
    {
        if (m_cameras[i]->isActive())
        {
            printf("* cam %d -- %s\n", i+1, m_cameras[i]->getName());
        }
        else
        {
            
            printf("  cam %d -- %s\n", i+1, m_cameras[i]->getName());
        }
    }

	if (i > 0)
        printf("\n\n");

    for (i=0; i< m_lights.size(); ++i)
    {
        printf("  light %d -- %s\n", i+1, m_lights[i]->getName());
    }

    if (i > 0)
            printf("\n\n");

    for (i=0; i< m_all_objects.size(); ++i)
    {
        printf("  object %d -- %s\n", i+1, m_all_objects[i]->getName());
    }
}


int CGameSceneManager::load(_string &xml_file)
{
    // set up shared object values
 
    rapidxml::xml_document<> XMLDoc;    // character type defaults to char
 
    rapidxml::xml_node<>* XMLRoot;
 
    char* xml = createFileContent(xml_file);
    if (xml == NULL)
    {
        return -1;
    }
    
    XMLDoc.parse<0>(xml);
 
    // Grab the scene node
    XMLRoot = XMLDoc.first_node("scene");
 
    // Validate the File
    if( getAttrib(XMLRoot, "formatVersion", "") == "")
    {
        freeFileContent(xml);
        return -1;
    }

    // Process the scene
    processScene(XMLRoot);
 
    freeFileContent(xml);

    changeCamera(0);

    dumpInfo();

	return 0;
}
 
void CGameSceneManager::processScene(rapidxml::xml_node<>* XMLRoot)
{
    // Process the scene parameters
    rapidxml::xml_node<>* pElement;
 
    // Process environment (?)
    pElement = XMLRoot->first_node("environment");
    if(pElement)
        processEnvironment(pElement);
 
    // Process nodes (?)
    pElement = XMLRoot->first_node("nodes");
    if(pElement)
        processNodes(pElement);
}
 
void CGameSceneManager::processNodes(rapidxml::xml_node<>* XMLNode)
{
    rapidxml::xml_node<>* pElement;
 
    // Process node (*)
    pElement = XMLNode->first_node("node");
    while(pElement)
    {
        processNode(pElement);
        pElement = pElement->next_sibling("node");
    }
}
 
void CGameSceneManager::processEnvironment(rapidxml::xml_node<>* XMLNode)
{
    rapidxml::xml_node<>* pElement;

    // Process colourAmbient (?)
    pElement = XMLNode->first_node("colourAmbient");
    if(pElement)
        setAmbientLight(parseColour(pElement));
 
    // Process colourBackground (?)
    pElement = XMLNode->first_node("colourBackground");
    if(pElement)
        setBackgroundColor(parseColour(pElement));
}
 
void CGameSceneManager::processLight(rapidxml::xml_node<>* XMLNode)
{
    _LIGHT_PARAM param;
    // Process attributes
    _string name = getAttrib(XMLNode, "name");
 
    // Create the light

    _string sValue = getAttrib(XMLNode, "type");
    if(sValue == "point")
        param.lightType = _LIGHT_POINT;
    else if(sValue == "directional")
        param.lightType = _LIGHT_DIECTIONAL;
    else if(sValue == "spot")
        param.lightType = _LIGHT_SPORT;
    else if(sValue == "radPoint")
        param.lightType = _LIGHT_SPORT;
 
    rapidxml::xml_node<>* pElement;

    pElement = XMLNode->first_node("position");
    if(pElement)
    {
        param.position = parseVector3(pElement);
    }
 
    pElement = XMLNode->first_node("directionVector");
    if(pElement)
    {
        param.direction = parseVector3(pElement);
    }
 
    // Process colourDiffuse (?)
    pElement = XMLNode->first_node("colourDiffuse");
    if(pElement)
        param.diffuse = parseColour(pElement);
 
    // Process colourSpecular (?)
    pElement = XMLNode->first_node("colourSpecular");
    if(pElement)
        param.specular= parseColour(pElement);
 
    if(sValue != "directional")
    {
        // Process lightAttenuation (?)
        pElement = XMLNode->first_node("lightAttenuation");
        if(pElement)
            processLightAttenuation(pElement, param.attenuation);
    }

    i3DLight * light = createLight(name, param);
    if (light)
    {
        //m_all_objects.push_back(light);
        m_lights.push_back(light);
    }
}
 
void CGameSceneManager::processCamera(rapidxml::xml_node<>* XMLNode, _TRANSFORM &t)
{
    _CAMERA_PARAM param;
    // Process attributes
    _string name = getAttrib(XMLNode, "name");
    param.fov = getAttribReal(XMLNode, "fov", 45);
    param.aspectRatio = getAttribReal(XMLNode, "aspectRatio", 1.3333);
    rapidxml::xml_node<>* pElement;
 
    // Process clipping (?)
    pElement = XMLNode->first_node("clipping");
    if(pElement)
    {
        param.nearDist = getAttribReal(pElement, "near");
        param.farDist =  getAttribReal(pElement, "far");
    }

    i3DCamera * cam = createCamera(name, t, param);
    if (cam)
    {
        //m_all_objects.push_back(cam);
        m_cameras.push_back(cam);
    }
 }
 
void CGameSceneManager::processNode(rapidxml::xml_node<>* XMLNode)
{
    _TRANSFORM t;
    
    // Construct the node's name
    _string name = getAttrib(XMLNode, "name");

    rapidxml::xml_node<>* pElement;
 
    // Process position (?)
    pElement = XMLNode->first_node("position");
    if(pElement)
    {
        t.position = parseVector3(pElement);
    }
 
    // Process rotation (?)
    pElement = XMLNode->first_node("rotation");
    if(pElement)
    {
        t.quaternion = parseQuaternion(pElement);
    }
 
    // Process scale (?)
    pElement = XMLNode->first_node("scale");
    if(pElement)
    {
        t.scale = parseVector3(pElement);
    }
 
    // Process entity (*)
    pElement = XMLNode->first_node("entity");
    if (pElement)
    {
        _string meshFile = getAttrib(pElement, "meshFile");
        i3DObject * obj = createObject(name, meshFile);
        if (obj)
        {
            obj->setPos(t.position);
            obj->setScale(t.scale);
            obj->setRotation(t.quaternion);

            m_all_objects.push_back(obj);
        }
        return;
    }
 
    // Process light (*)
    pElement = XMLNode->first_node("light");
    if (pElement)
    {
        processLight(pElement);
        return;
    }
 
    // Process camera (*)
    pElement = XMLNode->first_node("camera");
    if (pElement)
    {
        processCamera(pElement, t);
    }
}
 
void CGameSceneManager::processLightAttenuation(rapidxml::xml_node<>* XMLNode, _ATTENUATION &att)
{
    // Process attributes
    att.range = getAttribReal(XMLNode, "range");
    att.constant = getAttribReal(XMLNode, "constant");
    att.linear = getAttribReal(XMLNode, "linear");
    att.quadratic = getAttribReal(XMLNode, "quadratic");
}

 const char* CGameSceneManager::getAttrib(rapidxml::xml_node<>* XMLNode, const char* attrib, const char * defaultValue)
{
    if(XMLNode->first_attribute(attrib))
        return XMLNode->first_attribute(attrib)->value();
    else
        return defaultValue;
}
 
f32 CGameSceneManager::getAttribReal(rapidxml::xml_node<>* XMLNode, const char* attrib, f32 defaultValue)
{
    if(XMLNode->first_attribute(attrib))
        return (f32)atof(XMLNode->first_attribute(attrib)->value());
    else
        return defaultValue;
}
 
_VECTOR3D CGameSceneManager::parseVector3(rapidxml::xml_node<>* XMLNode)
{
    return _VECTOR3D(
        (f32)atof(XMLNode->first_attribute("x")->value()),
        (f32)atof(XMLNode->first_attribute("y")->value()),
        (f32)atof(XMLNode->first_attribute("z")->value())
    );
}
 
_QUATERNION CGameSceneManager::parseQuaternion(rapidxml::xml_node<>* XMLNode)
{
    _QUATERNION orientation;

    orientation._W = (f32)atof(XMLNode->first_attribute("qw")->value());
    orientation._X = (f32)atof(XMLNode->first_attribute("qx")->value());
    orientation._Y = (f32)atof(XMLNode->first_attribute("qy")->value());
    orientation._Z = (f32)atof(XMLNode->first_attribute("qz")->value());
   
    return orientation;
}
 
_color CGameSceneManager::parseColour(rapidxml::xml_node<>* XMLNode)
{
    _color c(
        (f32)atof(XMLNode->first_attribute("r")->value()),
        (f32)atof(XMLNode->first_attribute("g")->value()),
        (f32)atof(XMLNode->first_attribute("b")->value()),
        XMLNode->first_attribute("a") != NULL ? (f32)atof(XMLNode->first_attribute("a")->value()) : 1
    );

    return c;
}
 
