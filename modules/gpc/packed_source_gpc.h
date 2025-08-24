#ifndef PACKED_SOURCE_GPC_H
#define PACKED_SOURCE_GPC_H

#include "core/io/file_access.h"
#include "core/io/file_access_pack.h"

class PackedSourceGPC : public PackSource {
public:
    virtual bool try_open_pack(const String &p_path, bool p_replace_files, uint64_t p_offset) override;
    virtual Ref<FileAccess> get_file(const String &p_path, PackedData::PackedFile *p_file) override;
};

#endif // PACKED_SOURCE_GPC_H
