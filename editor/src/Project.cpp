#include "Project.h"
#include <QCoreApplication>

#define PROJECT_FILE "game.project"
#define PROJECT_CONFIG_FILE "game.config"
#define PROJECT_ASSETS_DIR "assets"
#define PROJECT_SCENE_EXT ".scene"
#define PROJECT_SCENE_NAME "main"
#define PROJECT_CAMERA_NAME "mainCamera"
#define PROJECT_LIGHT_NAME "directionalLight"

bool Project::_serializerActivated = false;

using namespace gameplay;


Project::Project(QObject* parent) : QFileSystemModel(parent)
{
}

Project::~Project()
{
}

Ptr<Project> Project::create(const QString& path, const QString& name, QObject* parent)
{
    auto project = new Project(parent);

    project->_name = name.toStdString();
    project->_path = path.toStdString();
    project->_scenePath = std::string(PROJECT_ASSETS_DIR) + std::string("/") + std::string(PROJECT_SCENE_NAME) + std::string(PROJECT_SCENE_EXT);

    // Create a assets directory
    QDir projectDir(path);
    if (!projectDir.mkdir(PROJECT_ASSETS_DIR))
    {
        //project->reset();
        delete project;
        return Ptr<Project>(nullptr);
    }
    // Create a project file
    if (!_serializerActivated)
    {
        gameplay::SerializerManager::getActivator()->registerType("gameplay::Project", &Project::createObject);
        _serializerActivated = true;
    }
    QString projectFilePath(path + QString("/") + QString(QLatin1String(PROJECT_FILE)));
    QByteArray projectFilePathByteArray = projectFilePath.toLatin1();
    const char* projectFilePathStr = projectFilePathByteArray.data();
    auto projectWriter = gameplay::SerializerJson::createWriter(projectFilePathStr);
    projectWriter->writeObject(nullptr, project);
    projectWriter->close();

    // Create a config file
    QString configFilePath(path + QString("/") + QString(QLatin1String(PROJECT_CONFIG_FILE)));
    QByteArray configFilePathByteArray = configFilePath.toLatin1();
    const char* configFilePathStr = configFilePathByteArray.data();
    auto configWriter = gameplay::SerializerJson::createWriter(configFilePathStr);
    auto config = new gameplay::Game::Config();
    configWriter->writeObject(nullptr, config);
    configWriter->close();

    // Create an empty scene.
    auto scene = new gameplay::Scene();
    scene->setId(PROJECT_SCENE_NAME);

    // Add and object with a camera component
    auto camera = gameplay::Camera::createPerspective(60, 1, 0.1, 1000);
    //camera->setMode(gameplay::Camera::Mode::ePerspective);
    auto cameraObject = gameplay::Node::create(PROJECT_CAMERA_NAME);
    //cameraObject->setName(PROJECT_CAMERA_NAME);
    cameraObject->addComponent(camera);
    scene->addNode(cameraObject);

    // Add an object with a directional light component
    auto light = gameplay::Light::createDirectional(1, 1, 1);
    auto lightObject = gameplay::Node::create(PROJECT_LIGHT_NAME);
    //lightObject->setName(PROJECT_LIGHT_NAME);
    lightObject->addComponent(light);
    //lightObject->setPosition(gameplay::Vector3(0.0f, 0.0f, 10.0f));
    scene->addNode(lightObject);

    // Create a scene file
    QString sceneFilePath(path + QDir::separator() + QString(QLatin1String(PROJECT_ASSETS_DIR)) +
                                 QDir::separator() + QString(QLatin1String(PROJECT_SCENE_NAME)) + QString(QLatin1String(PROJECT_SCENE_EXT)));

    QByteArray sceneFilePathByteArray = sceneFilePath.toLatin1();
    const char* sceneFilePathStr = sceneFilePathByteArray.data();
    auto sceneWriter = gameplay::SerializerJson::createWriter(sceneFilePathStr);
    sceneWriter->writeObject(nullptr, scene);
    sceneWriter->close();

    return Ptr<Project>(project);
}

Ptr<Project> Project::open(const QString& path, QObject* parent)
{
    if (!_serializerActivated)
    {
        gameplay::SerializerManager::getActivator()->registerType("gameplay::Project", &Project::createObject);
        _serializerActivated = true;
    }

    QString projectPath(path + QString("/") + QString(QLatin1String(PROJECT_FILE)));
    if (!QFile(projectPath).exists())
        return Ptr<Project>(nullptr);

    QByteArray ba = projectPath.toLatin1();
    const char* str = ba.data();
    auto projectReader = gameplay::Serializer::createReader(str);
    auto project = dynamic_cast<Project*>(projectReader->readObject(nullptr));
    projectReader->close();

    return Ptr<Project>(project);
}

QString Project::getPath()
{
    return QString::fromStdString(_path);
}

QString Project::getName()
{
    return QString::fromStdString(_name);
}

QString Project::getScenePath()
{
    return QString::fromStdString(_scenePath);
}

void Project::setScenePath(const QString& scenePath)
{
    _scenePath = scenePath.toStdString();
}

QVariant Project::data(const QModelIndex& index, int role) const
{
    switch(role)
    {
        case Qt::TextAlignmentRole:
        {
            return Qt::AlignBottom;
        }
        case Qt::DecorationRole:
        {
            if (index.column() == 0)
            {
                QFileInfo info = Project::fileInfo(index);
                if(info.isFile())
                {
                    if(info.suffix() == "config")
                        return QPixmap(":/res/images/project-config.png");
                    else if(info.suffix() == "scene")
                        return QPixmap(":/res/images/project-scene.png");
                }
            }
            break;
        }
    }
    return QFileSystemModel::data(index, role);
}

Serializable *Project::createObject()
{
    return new Project();
}

std::string Project::getClassName()
{
    return "gameplay::Project";
}

void Project::onSerialize(gameplay::Serializer* serializer)
{
    serializer->writeString("name", _name.c_str(), "");
    serializer->writeString("scene", _scenePath.c_str(), "");
}

void Project::onDeserialize(gameplay::Serializer* serializer)
{
    serializer->readString("name", _name, "" );
    serializer->readString("scene", _scenePath, "");
    _path = serializer->getPath();
}

