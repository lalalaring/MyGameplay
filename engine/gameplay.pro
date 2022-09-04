#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------
QT -= core gui
TARGET = gameplay
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11
CONFIG -= qt
CONFIG(debug, debug|release): DEFINES += _DEBUG
DEFINES += GP_NO_LUA_BINDINGS


INCLUDEPATH += $$PWD/../gameplay/src
INCLUDEPATH += $$PWD/../external-deps/include
DEFINES += GP_USE_GAMEPAD

linux: SOURCES += src/platform/PlatformLinux.cpp
linux: SOURCES += src/platform/gameplay-main-linux.cpp
linux: QMAKE_CXXFLAGS += -lstdc++ -pthread -w
linux: DEFINES += __linux__
linux: INCLUDEPATH += /usr/include/gtk-2.0
linux: INCLUDEPATH += /usr/lib/x86_64-linux-gnu/gtk-2.0/include
linux: INCLUDEPATH += /usr/include/atk-1.0
linux: INCLUDEPATH += /usr/include/cairo
linux: INCLUDEPATH += /usr/include/gdk-pixbuf-2.0
linux: INCLUDEPATH += /usr/include/pango-1.0
linux: INCLUDEPATH += /usr/include/gio-unix-2.0
linux: INCLUDEPATH += /usr/include/freetype2
linux: INCLUDEPATH += /usr/include/glib-2.0
linux: INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
linux: INCLUDEPATH += /usr/include/pixman-1
linux: INCLUDEPATH += /usr/include/libpng12
linux: INCLUDEPATH += /usr/include/harfbuzz

macx: OBJECTIVE_SOURCES += src/platform/PlatformMacOSX.mm
macx: OBJECTIVE_SOURCES += src/platform/gameplay-main-macosx.mm
macx: QMAKE_CXXFLAGS += -x c++ -x objective-c++ -stdlib=libc++ -w -arch x86_64
macx: LIBS += -F/System/Library/Frameworks -framework GameKit
macx: LIBS += -F/System/Library/Frameworks -framework IOKit
macx: LIBS += -F/System/Library/Frameworks -framework QuartzCore
macx: LIBS += -F/System/Library/Frameworks -framework OpenAL
macx: LIBS += -F/System/Library/Frameworks -framework OpenGL
macx: LIBS += -F/System/Library/Frameworks -framework Cocoa
macx: LIBS += -F/System/Library/Frameworks -framework Foundation

win32: SOURCES += src/platform/PlatformWindows.cpp
win32: SOURCES += src/platform/gameplay-main-windows.cpp
win32: DEFINES += WIN32 _UNICODE UNICODE
win32: INCLUDEPATH += $$(DXSDK_DIR)/Include
win32: QMAKE_CXXFLAGS_WARN_ON -= -w34100
win32: QMAKE_CXXFLAGS_WARN_ON -= -w34189
win32: QMAKE_CXXFLAGS_WARN_ON -= -w4302

DISTFILES += \
    res/design/arial.ttf \
    res/design/default-theme.psd \
    res/design/icon_128.ico \
    res/design/icon_16.ico \
    res/design/icon_32.ico \
    res/design/icon_64.ico \
    res/design/logo.ai \
    res/icon.png \
    res/icon_tray.png \
    res/logo_black.png \
    res/logo_powered_black.png \
    res/logo_powered_white.png \
    res/logo_white.png \
    res/materials/terrain.material \
    res/shaders/colored.frag \
    res/shaders/colored.vert \
    res/shaders/font.frag \
    res/shaders/font.vert \
    res/shaders/lighting.frag \
    res/shaders/lighting.vert \
    res/shaders/skinning-none.vert \
    res/shaders/skinning.vert \
    res/shaders/sprite.frag \
    res/shaders/sprite.vert \
    res/shaders/terrain.frag \
    res/shaders/terrain.vert \
    res/shaders/textured.frag \
    res/shaders/textured.vert \
    res/ui/arial.ttf \
    res/ui/default-theme.png \
    res/ui/default.theme \
    src/gameplay.dox \
    src/org/gameplay3d/GamePlayNativeActivity.java

HEADERS += \
    src/ai/AIAgent.h \
    src/ai/AIController.h \
    src/ai/AIMessage.h \
    src/ai/AIState.h \
    src/ai/AIStateMachine.h \
    src/animation/Animation.h \
    src/animation/AnimationClip.h \
    src/animation/AnimationController.h \
    src/animation/AnimationTarget.h \
    src/animation/AnimationValue.h \
    src/audio/AudioBuffer.h \
    src/audio/AudioController.h \
    src/audio/AudioListener.h \
    src/audio/AudioSource.h \
    src/base/Activator.h \
    src/base/Base.h \
    src/base/Bundle.h \
    src/base/DebugNew.h \
    src/base/FileSystem.h \
    src/base/Logger.h \
    src/base/Properties.h \
    src/base/Ptr.h \
    src/base/Ref.h \
    src/base/Serializable.h \
    src/base/Serializer.h \
    src/base/SerializerBinary.h \
    src/base/SerializerJson.h \
    src/base/Stream.h \
    src/gameplay.h \
    src/material/VertexAttributeBinding.h \
    src/render/GLFrameBuffer.h \
    src/scene/AssetManager.h \
    src/scene/Component.h \
    src/ui/Font.h \
    src/material/Image.h \
    src/material/Image.inl \
    src/material/Effect.h \
    src/material/Material.h \
    src/material/MaterialParameter.h \
    src/material/RenderState.h \
    src/material/Texture.h \
    src/math/BoundingBox.h \
    src/math/BoundingBox.inl \
    src/math/BoundingSphere.h \
    src/math/BoundingSphere.inl \
    src/math/Curve.h \
    src/math/Frustum.h \
    src/math/MathUtil.h \
    src/math/MathUtil.inl \
    src/math/MathUtilNeon.inl \
    src/math/Matrix.h \
    src/math/Matrix.inl \
    src/math/Plane.h \
    src/math/Plane.inl \
    src/math/Quaternion.h \
    src/math/Quaternion.inl \
    src/math/Ray.h \
    src/math/Ray.inl \
    src/math/Rectangle.h \
    src/render/CompressedTexture.h \
    src/render/DepthStencilTarget.h \
    src/render/FrameBuffer.h \
    src/render/GLRenderer.h \
    src/render/RenderPipline.h \
    src/render/RenderTarget.h \
    src/render/Renderer.h \
    src/render/StateBlock.h \
    src/render/ogl.h \
    src/scene/Transform.h \
    src/math/Vector2.h \
    src/math/Vector2.inl \
    src/math/Vector3.h \
    src/math/Vector3.inl \
    src/math/Vector4.h \
    src/math/Vector4.inl \
    src/physics/PhysicsCharacter.h \
    src/physics/PhysicsCollisionObject.h \
    src/physics/PhysicsCollisionShape.h \
    src/physics/PhysicsConstraint.h \
    src/physics/PhysicsConstraint.inl \
    src/physics/PhysicsController.h \
    src/physics/PhysicsFixedConstraint.h \
    src/physics/PhysicsFixedConstraint.inl \
    src/physics/PhysicsGenericConstraint.h \
    src/physics/PhysicsGenericConstraint.inl \
    src/physics/PhysicsGhostObject.h \
    src/physics/PhysicsHingeConstraint.h \
    src/physics/PhysicsRigidBody.h \
    src/physics/PhysicsRigidBody.inl \
    src/physics/PhysicsSocketConstraint.h \
    src/physics/PhysicsSpringConstraint.h \
    src/physics/PhysicsSpringConstraint.inl \
    src/physics/PhysicsVehicle.h \
    src/physics/PhysicsVehicleWheel.h \
    src/platform/Game.h \
    src/platform/Game.inl \
    src/platform/Gamepad.h \
    src/platform/Gesture.h \
    src/platform/Keyboard.h \
    src/platform/Mouse.h \
    src/platform/Platform.h \
    src/platform/ScreenDisplayer.h \
    src/platform/TimeListener.h \
    src/platform/Touch.h \
    src/scene/Camera.h \
    src/scene/Drawable.h \
    src/scene/HeightField.h \
    src/scene/Joint.h \
    src/scene/Light.h \
    src/scene/Mesh.h \
    src/scene/MeshBatch.h \
    src/scene/MeshBatch.inl \
    src/scene/MeshPart.h \
    src/scene/MeshSkin.h \
    src/scene/Model.h \
    src/scene/Node.h \
    src/scene/ParticleEmitter.h \
    src/scene/Scene.h \
    src/scene/SceneLoader.h \
    src/scene/Sprite.h \
    src/scene/SpriteBatch.h \
    src/scene/Terrain.h \
    src/scene/TerrainPatch.h \
    src/scene/TileSet.h \
    src/scene/VertexFormat.h \
    src/script/Script.h \
    src/script/ScriptController.h \
    src/script/ScriptController.inl \
    src/script/ScriptTarget.h \
    src/ui/AbsoluteLayout.h \
    src/ui/Button.h \
    src/ui/CheckBox.h \
    src/ui/Container.h \
    src/ui/Control.h \
    src/ui/ControlFactory.h \
    src/ui/FlowLayout.h \
    src/ui/Form.h \
    src/ui/ImageControl.h \
    src/ui/JoystickControl.h \
    src/ui/Label.h \
    src/ui/Layout.h \
    src/ui/RadioButton.h \
    src/ui/Slider.h \
    src/ui/Text.h \
    src/ui/TextBox.h \
    src/ui/Theme.h \
    src/ui/ThemeStyle.h \
    src/ui/VerticalLayout.h

SOURCES += \
    src/ai/AIAgent.cpp \
    src/ai/AIController.cpp \
    src/ai/AIMessage.cpp \
    src/ai/AIState.cpp \
    src/ai/AIStateMachine.cpp \
    src/animation/Animation.cpp \
    src/animation/AnimationClip.cpp \
    src/animation/AnimationController.cpp \
    src/animation/AnimationTarget.cpp \
    src/animation/AnimationValue.cpp \
    src/audio/AudioBuffer.cpp \
    src/audio/AudioController.cpp \
    src/audio/AudioListener.cpp \
    src/audio/AudioSource.cpp \
    src/base/Activator.cpp \
    src/base/Bundle.cpp \
    src/base/DebugNew.cpp \
    src/base/FileSystem.cpp \
    src/base/Logger.cpp \
    src/base/Properties.cpp \
    src/base/Ptr.cpp \
    src/base/Ref.cpp \
    src/base/Serializer.cpp \
    src/base/SerializerBinary.cpp \
    src/base/SerializerJson.cpp \
    src/base/Stream.cpp \
    src/material/VertexAttributeBinding.cpp \
    src/render/GLFrameBuffer.cpp \
    src/scene/AssetManager.cpp \
    src/scene/Component.cpp \
    src/ui/Font.cpp \
    src/material/Image.cpp \
    src/material/Effect.cpp \
    src/material/Material.cpp \
    src/material/MaterialParameter.cpp \
    src/material/RenderState.cpp \
    src/material/Texture.cpp \
    src/math/BoundingBox.cpp \
    src/math/BoundingSphere.cpp \
    src/math/Curve.cpp \
    src/math/Frustum.cpp \
    src/math/MathUtil.cpp \
    src/math/Matrix.cpp \
    src/math/Plane.cpp \
    src/math/Quaternion.cpp \
    src/math/Ray.cpp \
    src/math/Rectangle.cpp \
    src/render/CompressedTexture.cpp \
    src/render/DepthStencilTarget.cpp \
    src/render/FrameBuffer.cpp \
    src/render/GLRenderer.cpp \
    src/render/RenderPipline.cpp \
    src/render/RenderTarget.cpp \
    src/render/Renderer.cpp \
    src/render/StateBlock.cpp \
    src/scene/Transform.cpp \
    src/math/Vector2.cpp \
    src/math/Vector3.cpp \
    src/math/Vector4.cpp \
    src/physics/PhysicsCharacter.cpp \
    src/physics/PhysicsCollisionObject.cpp \
    src/physics/PhysicsCollisionShape.cpp \
    src/physics/PhysicsConstraint.cpp \
    src/physics/PhysicsController.cpp \
    src/physics/PhysicsFixedConstraint.cpp \
    src/physics/PhysicsGenericConstraint.cpp \
    src/physics/PhysicsGhostObject.cpp \
    src/physics/PhysicsHingeConstraint.cpp \
    src/physics/PhysicsRigidBody.cpp \
    src/physics/PhysicsSocketConstraint.cpp \
    src/physics/PhysicsSpringConstraint.cpp \
    src/physics/PhysicsVehicle.cpp \
    src/physics/PhysicsVehicleWheel.cpp \
    src/platform/Game.cpp \
    src/platform/Gamepad.cpp \
    src/platform/Platform.cpp \
    src/platform/ScreenDisplayer.cpp \
    src/scene/Camera.cpp \
    src/scene/Drawable.cpp \
    src/scene/HeightField.cpp \
    src/scene/Joint.cpp \
    src/scene/Light.cpp \
    src/scene/Mesh.cpp \
    src/scene/MeshBatch.cpp \
    src/scene/MeshPart.cpp \
    src/scene/MeshSkin.cpp \
    src/scene/Model.cpp \
    src/scene/Node.cpp \
    src/scene/ParticleEmitter.cpp \
    src/scene/Scene.cpp \
    src/scene/SceneLoader.cpp \
    src/scene/Sprite.cpp \
    src/scene/SpriteBatch.cpp \
    src/scene/Terrain.cpp \
    src/scene/TerrainPatch.cpp \
    src/scene/TileSet.cpp \
    src/scene/VertexFormat.cpp \
    src/script/Script.cpp \
    src/script/ScriptController.cpp \
    src/script/ScriptTarget.cpp \
    src/ui/AbsoluteLayout.cpp \
    src/ui/Button.cpp \
    src/ui/CheckBox.cpp \
    src/ui/Container.cpp \
    src/ui/Control.cpp \
    src/ui/ControlFactory.cpp \
    src/ui/FlowLayout.cpp \
    src/ui/Form.cpp \
    src/ui/ImageControl.cpp \
    src/ui/JoystickControl.cpp \
    src/ui/Label.cpp \
    src/ui/Layout.cpp \
    src/ui/RadioButton.cpp \
    src/ui/Slider.cpp \
    src/ui/Text.cpp \
    src/ui/TextBox.cpp \
    src/ui/Theme.cpp \
    src/ui/ThemeStyle.cpp \
    src/ui/VerticalLayout.cpp
