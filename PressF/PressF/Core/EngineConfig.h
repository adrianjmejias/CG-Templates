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

        Float stepSize;
        Vec2 convSize;
        Vec2 convPivot;
        void ImGui();
        Float IOD;
        Float zDistance;
    protected:
        
        EngineConfig() = default;
    };


}
