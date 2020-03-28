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
        Int showTex{ 6 };

        Float HDRGamma{ 2.2f };
        Float HDRExposure{5};
        Int boolBlurAmmount{5};
        Float stepSize;
        Float bloomThreshold{0.56f};
        Vec2 convSize;
        Vec2 convPivot;
        void ImGui();
        Float IOD;
        Float zDistance;
    protected:
        
        EngineConfig() = default;
    };


}
