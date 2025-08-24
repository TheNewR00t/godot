#include "packed_source_gpc.h"

Ref<FileAccess> PackedSourceGPC::try_open_pack(const String &p_path, bool skip_pack) {
    if (!skip_pack) {
        Ref<FileAccess> fa = PackedData::get_singleton()->try_open_path(p_path);
        if (fa.is_valid()) {
            return fa;
        }
    }
    return FileAccessGPC::create_for_path(p_path);
}

Ref<FileAccess> PackedSourceGPC::get_file(const String &p_path, PackedData::PackedFile *pf) {
    if (!pf) {
        PackedData::PackedFile file;
        pf = &file;
    }
    return PackedData::get_singleton()->get_file(p_path, pf);
}
