#ifndef PACKED_SOURCE_GPC_H
#define PACKED_SOURCE_GPC_H

#include "core/io/file_access.h"
#include "core/io/file_access_pack.h"

class PackedSourceGPC : public PackedSource {
    GDCLASS(PackedSourceGPC, PackedSource);

public:
    Ref<FileAccess> try_open_pack(const String &p_path, bool skip_pack = false);
    Ref<FileAccess> get_file(const String &p_path, PackedData::PackedFile *pf = nullptr);
};

#endif // PACKED_SOURCE_GPC_H
