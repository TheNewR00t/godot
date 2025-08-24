#include "register_types.h"

#include "core/object/class_db.h"
#include "gpc_packer.h"

void initialize_gpc_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
    ClassDB::register_class<GPCPacker>();
    gpc_enabled = true;
}

void uninitialize_gpc_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
    gpc_enabled = false;
}
