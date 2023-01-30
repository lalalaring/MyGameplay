#include "Toolkit.h"

using namespace gameplay;


Toolkit* Toolkit::g_instance;

Toolkit* Toolkit::cur() { return g_instance; }