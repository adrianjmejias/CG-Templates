#pragma once
#include "PressF/pch.h"


namespace PF
{
    struct EngineConfig
    {
        static Owns<EngineConfig> instance;
        static EngineConfig* GetInstance();
        Bool showFPS{ true };
        Bool useStereoscopic{ false };
        Bool useSSAO{ true };
        Bool useBloom{ true }; 
        Bool useDeferredRendering{true};
        void ImGui();
        Float IOD;
        Float zDistance;
    protected:
        
        EngineConfig() = default;
    };


}
