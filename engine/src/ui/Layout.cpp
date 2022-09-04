#include "base/Base.h"
#include "Layout.h"
#include "Control.h"
#include "Container.h"
#include "platform/Game.h"

namespace gameplay
{

bool Layout::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    return false;
}

}