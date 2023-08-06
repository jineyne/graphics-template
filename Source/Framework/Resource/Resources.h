#pragma once

#include "pch.h"

#include "Utility/Module.h"

namespace gt {
    class Resources : public Module<Resources> {
    protected:
        void onStartUp();
        void onShutDown();
    };
}
