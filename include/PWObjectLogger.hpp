#pragma once

#include <Unreal/UFunction.hpp>

namespace PW::ObjectLogger
{
    void InstallHooks();
    void UninstallHooks();
    extern RC::Unreal::UFunction* pfnOnCompleteInitializeParameter;
    extern RC::Unreal::UFunction* pfnRequestObtainLevelObject_ToServer;
}
