#pragma once

namespace PhysiXal {

#ifdef PX_PLATFORM_LINUX

    class OpenGLRenderer
    {
    public:
        static void Init();
        static void Shutdown();
        
        static void BeginFrame();
    private:
    };
#endif
}

