#pragma once

#include "EditorWindow.h"
#include "ProjectSortFilterProxyModel.h"
#include <QWidget>
#include "Project.h"

namespace Ui
{
class ProjectFileView;
}
//class Project;


/**
 * Defines a project view showing files visible in the project directory.
 */
class ProjectFileView : public QWidget
{
    Q_OBJECT
    friend class ProjectTreeView;

public:

    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit ProjectFileView(QWidget* parent = nullptr);

    /**
     * Destructor.
     */
    ~ProjectFileView();

    /**
     * Opens a project at the specified path.
     *
     * @param path The project path.
     */
    void openProject(const QString& path);

    /**
     * Closes the currently open project.
     */
    void closeProject();

    /**
     * Gets the project that is open.
     *
     * @return The open project.
     */
    gameplay::Ptr<Project> getProject() const;

    /**
     * Gets the sort filter for the project model.
     *
     * @return The sort filter for the project model.
     */
    ProjectSortFilterProxyModel* sortFilter() const;

public slots:

    /**
     * Handler for when the search filter text changes.
     *
     * @param text The changed text.
     */
    void onSearchTextChanged(const QString& text);

signals:

    /**
     * Signal emmited with a new scene has been opened.
     *
     * @param path The path of the scene being opened.
     */
    void sceneOpened(const QString& path);

protected:

    /**
     * Gets the ui associated with this view.
     *
     * @return The view ui.
     */
    Ui::ProjectFileView* ui();

    /**
     * Opens a scene at the specified path.
     *
     * @param path The path of the scene to be opened.
     */
    void openScene(const QString& path);

private:

    Ui::ProjectFileView* _ui;
    gameplay::Ptr<Project> _project;
    ProjectSortFilterProxyModel* _sortFilter;
};
