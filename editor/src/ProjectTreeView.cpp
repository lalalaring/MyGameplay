#include "ProjectTreeView.h"
#include "Project.h"
#include "ProjectFileView.h"
#include "ui_ProjectFileView.h"
#include <QtWidgets>


ProjectTreeView::ProjectTreeView(QWidget* parent) : QTreeView(parent), 
    _projectView(static_cast<ProjectFileView*>(parent))
{
    setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);
    setSortingEnabled(true);
}

void ProjectTreeView::onItemDoubleClicked(const QModelIndex& index)
{
    openSelectedItem();
}

void ProjectTreeView::onOpenFile()
{
    openSelectedItem();
}

void ProjectTreeView::openSelectedItem()
{
    gameplay::Ptr<Project> project = _projectView->getProject();
    ProjectSortFilterProxyModel* sortFilter = _projectView->sortFilter();
    const QModelIndex index = sortFilter->mapToSource(this->selectionModel()->currentIndex());

    QString sceneFilePath = project->filePath(index);
    if (sceneFilePath.endsWith(".scene"))
    {
        _projectView->openScene(sceneFilePath);
    }
}

void ProjectTreeView::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    Ui::ProjectFileView* ui = _projectView->ui();
    menu.addAction(ui->actionOpenFile);
    menu.addSeparator();
    menu.addAction(ui->actionNewFile);
    menu.addAction(ui->actionNewFolder);
    menu.addSeparator();
    menu.addAction(ui->actionImportFile);
    menu.exec(event->globalPos());
}


