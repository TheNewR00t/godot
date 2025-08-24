#include "register_types.h"

#include "core/config/engine.h"
#include "core/io/file_access_pack.h"
#include "packed_source_gpc.h"

#ifdef TOOLS_ENABLED
#include "editor/editor_node.h"
#include "editor/export/editor_export.h"
#include "gpc_export_plugin.h"
#endif

static PackedSourceGPC *gpc_source = nullptr;

#ifdef TOOLS_ENABLED
static Ref<GPCExportPlugin> gpc_export_plugin;

static void editor_init_callback() {
	gpc_export_plugin.instantiate();
	EditorExport::get_singleton()->add_export_plugin(gpc_export_plugin);
}
#endif

void initialize_gpc_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_CORE) {
		gpc_source = memnew(PackedSourceGPC);
		PackedData::get_singleton()->add_pack_source(gpc_source);
	}
#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		ClassDB::register_class<GPCExportPlugin>();
		EditorNode::add_init_callback(editor_init_callback);
	}
#endif
}

void uninitialize_gpc_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_CORE) {
		return;
	}
	if (gpc_source) {
		// PackedData::get_singleton()->remove_pack_source(gpc_source); // This method doesn't exist
		memdelete(gpc_source);
		gpc_source = nullptr;
	}
}