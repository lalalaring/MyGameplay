#pragma once

#include "EditorWindow.h"
#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QSize>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "gameplay.h"


/**
 * The main game view to render scenes into viewport(s).
 */
class GameView : public QOpenGLWidget, public gameplay::Game, protected QOpenGLFunctions
{
    Q_OBJECT
public:

    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit GameView(QWidget* parent = nullptr);

    /**
     * Destructor.
     */
    ~GameView();

    /**
     * Sets the editor window this game view uses.
     *
     * @param editor The editor window this game view uses.
     */
    void setEditor(EditorWindow* editor);

public slots:
    /**
     * Handler when the scene changes.
     */
    void onSceneChanged();

protected:
     void initializeGL();
     void paintGL();
     void resizeGL(int width, int height);

protected:
     void update(float elapsedTime);
     void render(float elapsedTime);

protected:

    /**
     * @see QWidget::mousePressEvent
     */
    void mousePressEvent(QMouseEvent* evt);

    /**
     * @see QWidget::mouseReleaseEvent
     */
    void mouseReleaseEvent(QMouseEvent* evt);

    /**
     * @see QWidget::mouseMoveEvent
     */
    void mouseMoveEvent(QMouseEvent* evt);

    /**
     * @see QWidget::mouseWheelEvent
     */
    void mouseWheelEvent(QWheelEvent* evt);

    /**
     * @see QWidget::keyPressEvent
     */
    void keyPressEvent(QKeyEvent* evt);

    /**
     * @see QWidget::keyReleaseEvent
     */
    void keyReleaseEvent(QKeyEvent* evt);

    /**
     * @see QWidget::closeEvent
     */
    void closeEvent(QCloseEvent* evt);

private:

    EditorWindow* _editor;
    gameplay::Ptr<gameplay::Scene> _scene;
    bool _wireframe;
    QSize _size;
    QPoint _mousePosition;
    double _mouseScroll;
};

