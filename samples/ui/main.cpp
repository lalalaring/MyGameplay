
#include <iostream>
#include "gameplay.h"

using namespace gameplay;

class MainApp : public Game, Control::Listener {
    Form* form;
    Label* label;
    Button* button;

    void initialize() {

        Theme::setDefault(Theme::create("res/ui/default.theme"));

        form = Form::create("testForm", NULL);
        form->setSize(600, 600);

        label = Label::create("testLabel");
        label->setPosition(50, 50);
        label->setSize(200, 50);
        label->setText("Label");
        form->addControl(label);
        //label->release();

        button = Button::create("testButton");
        button->setPosition(45, 100);
        button->setSize(200, 100);
        button->setText("This is a button");
        button->addListener(this, Control::Listener::CLICK);
        form->addControl(button);
        //button->release();
    }

    void finalize() {
        SAFE_RELEASE(label);
        SAFE_RELEASE(form);
    }

    void update(float elapsedTime)
    {
        //_scene->update(elapsedTime);
    }

    void render(float elapsedTime)
    {
        //Rectangle viewport = getViewport();
        form->draw(NULL);
    }

    void controlEvent(Control* control, EventType evt)
    {
        if (evt == CLICK)
        {
            if (strcmp("testButton", control->getId()) == 0)
            {
                label->setText("clicked");
            }
        }
    }

};

MainApp instance;

//int main()
//{
//    std::cout << "Hello World!\n";
//}
