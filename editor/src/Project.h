#pragma once

#include <QFileSystemModel>
#include "gameplay.h"

/**
 * Defines the file system model of the project for tree view.
 */
class Project : public QFileSystemModel, public gameplay::Serializable, public gameplay::Ref
{
    Q_OBJECT

    //friend class gameplay::Activator;

public:

    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit Project(QObject* parent = nullptr);

    /**
     * Destructor.
     */
    ~Project();

public:

    /**
     * Creates a new project at the specified path.
     *
     * @param name The name of the project to be created.
     * @param path The path to create the project at.
     */
    static gameplay::Ptr<Project> create(const QString& path, const QString& name, QObject* parent = nullptr);

    /**
     * Opens a project from the specified path.
     *
     * @param projectPath The path to open the project from.
     */
    static gameplay::Ptr<Project> open(const QString& path, QObject* parent = 0);

    /**
     * Gets the project name.
     *
     * @return The project name.
     */
    QString getName();

    /**
     * Gets the project path.
     *
     * @return The project path.
     */
    QString getPath();

    /**
     * Gets the last scene opened in the project.
     *
     * @return The last scene opened in the project.
     */
    QString getScenePath();

    /**
     * Sets the path of the scene currently open in the project.
     *
     * @param scene The path of the scene currently open in the project.
     */
    void setScenePath(const QString& scene);

    /**
     * @see QAbstractItemModel::data
     */
    QVariant data(const QModelIndex& index, int role) const;

    /**
     * @see Activator::CreateObjectCallback::createObject
     */
    static gameplay::Serializable *createObject();

protected:

    /**
     * @see Serializable::getClassName
     */
    std::string getClassName();

    /**
     * @see Serializable::onSerialize
     */
    void onSerialize(gameplay::Serializer* serializer);

    /**
     * @see Serializable::onDeserialize
     */
    void onDeserialize(gameplay::Serializer* serializer);

private:

    static bool _serializerActivated;
    std::string _path;
    std::string _name;
    std::string _scenePath;
};

