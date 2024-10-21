#include <Mod/CppUserModBase.hpp>
#include <UE4SSProgram.hpp>
#include <PWObjectLogger.hpp>

class PWObjectLoggerMod : public RC::CppUserModBase
{
public:
    PWObjectLoggerMod() : CppUserModBase()
    {
        ModName = STR("PWObjectLogger");
        ModVersion = STR("1.0.0");
        ModDescription = STR("Logs the GUIDs and locations of any Lifmunk Effigy or Journal Entry upon collection");
        ModAuthors = STR("@spectre.cular");
    }

    auto on_unreal_init() -> void
    {
        PW::ObjectLogger::InstallHooks();
    }

    ~PWObjectLoggerMod() override = default;
};

#define PW_OBJECT_LOGGER_MOD_API __declspec(dllexport)
extern "C"
{
    PW_OBJECT_LOGGER_MOD_API RC::CppUserModBase* start_mod()
    {
        return new PWObjectLoggerMod();
    }

    PW_OBJECT_LOGGER_MOD_API void uninstall_mod(RC::CppUserModBase* mod)
    {
        delete mod;
    }
}