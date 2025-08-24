#ifndef GPC_EXPORT_PLUGIN_H
#define GPC_EXPORT_PLUGIN_H

#include "editor/export/editor_export_plugin.h"

class GPCExportPlugin : public EditorExportPlugin {
    GDCLASS(GPCExportPlugin, EditorExportPlugin);

public:
    virtual void _export_file(const String &p_path, const String &p_type, const HashSet<String> &p_features) override;
};

#endif // GPC_EXPORT_PLUGIN_H
