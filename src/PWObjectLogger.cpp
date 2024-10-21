#include <PWObjectLogger.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/FProperty.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <stdio.h>

namespace PW::ObjectLogger
{
    using namespace RC;
    using namespace Unreal;

    UFunction* pfnOnCompleteInitializeParameter = nullptr;
    UFunction* pfnRequestObtainLevelObject_ToServer = nullptr;

    inline bool IsValidUObject(UObject* Object) {
        return Object && !Object->IsUnreachable();
    }

    void Hook_RequestObtainLevelObject_ToServer() {
        // Just to be extra safe
        if (IsValidUObject(pfnRequestObtainLevelObject_ToServer))
        {
            pfnRequestObtainLevelObject_ToServer->UnregisterAllHooks();
            pfnRequestObtainLevelObject_ToServer = nullptr;
        }

        pfnRequestObtainLevelObject_ToServer = UObjectGlobals::StaticFindObject<UFunction*>
            (nullptr, nullptr, STR("/Script/Pal.PalNetworkPlayerComponent:RequestObtainLevelObject_ToServer"));

        pfnRequestObtainLevelObject_ToServer->RegisterPreHook(static_cast<UnrealScriptFunctionCallable>
        ([&](UnrealScriptFunctionCallableContext& Context, void* CustomData) {
            struct FLocalParams {
                AActor* LevelObject;
            };
            auto& params = Context.GetParams<FLocalParams>();

            auto prop = params.LevelObject->GetPropertyByNameInChain(STR("LevelObjectInstanceId"));
            if (prop != nullptr) {
                auto name = params.LevelObject->GetClassPrivate()->GetName();
                auto guid = prop->ContainerPtrToValuePtr<FGuid>(params.LevelObject);
                auto location = params.LevelObject->K2_GetActorLocation();
                wchar_t buffer[255]{};
                std::swprintf(buffer, sizeof(buffer), STR("%08X%08X%08X%08X"), guid->A & 0xffffffff, guid->B & 0xffffffff, guid->C & 0xffffffff, guid->D & 0xffffffff);
                Output::send<RC::LogLevel::Verbose>(STR("[PWObjectLogger]: Obtained `{}` - [GUID]: `{}`, [Location]: (X: {}, Y: {}, Z: {})\n"), name.c_str(), buffer, location.X(), location.Y(), location.Z());
            }
        }), nullptr);
    }

    void InstallHooks() {
        // Avoids potentially creating duplicate hooks (ie, during mod reloads)
        UninstallHooks();

        pfnOnCompleteInitializeParameter = UObjectGlobals::StaticFindObject<UFunction*>
            (nullptr, nullptr, STR("/Script/Pal.PalPlayerCharacter:OnCompleteInitializeParameter"));

        pfnOnCompleteInitializeParameter->RegisterPostHook(static_cast<UnrealScriptFunctionCallable>
        ([&](UnrealScriptFunctionCallableContext& Context, void* CustomData) {
            Output::send<RC::LogLevel::Verbose>(STR("[PWObjectLogger]: >>> OnCompleteInitializeParameter\n"));
            AActor* OwnerCharacter = nullptr;

            struct FLocalParams {
                AActor* InCharacter;
            };
            auto& params = Context.GetParams<FLocalParams>();
            if (params.InCharacter) {
                if (!params.InCharacter->IsUnreachable()) {
                    OwnerCharacter = params.InCharacter;
                    Output::send<RC::LogLevel::Verbose>(STR("[PWObjectLogger]: - [Character]: `{0}`\n"), params.InCharacter->GetFullName());
                    Hook_RequestObtainLevelObject_ToServer();
                }
                else
                    Output::send<RC::LogLevel::Error>(STR("[PWObjectLogger]: - [Character]: (UNREACHABLE)\n"));
            }
            else
                Output::send<RC::LogLevel::Error>(STR("[PWObjectLogger]: - [Character]: (INVALID)\n"));
        }), nullptr);
    }

    void UninstallHooks() {
        if(IsValidUObject(pfnRequestObtainLevelObject_ToServer)) {
            pfnRequestObtainLevelObject_ToServer->UnregisterAllHooks();
            pfnRequestObtainLevelObject_ToServer = nullptr;
        }
        if(IsValidUObject(pfnOnCompleteInitializeParameter)) {
            pfnOnCompleteInitializeParameter->UnregisterAllHooks();
            pfnOnCompleteInitializeParameter = nullptr;
        }
    }
}
