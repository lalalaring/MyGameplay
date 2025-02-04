﻿
#include <iostream>
#include "gameplay.h"

using namespace gameplay;

class MainApp : public Game, Control::Listener {
    AudioSource * _audioEngine;

    void initialize() {

        _audioEngine = AudioSource::create("res/audio/engine_loop.ogg", false);
        _audioEngine->setLooped(true);
        _audioEngine->play();
    }

    void finalize() {
        SAFE_RELEASE(_audioEngine);
    }

    void update(float elapsedTime)
    {
        //_scene->update(elapsedTime);
    }

    void render(float elapsedTime)
    {
    }

    void controlEvent(Control* control, EventType evt)
    {
    }
};

MainApp instance;

int main() {
    return Platform::run();
}