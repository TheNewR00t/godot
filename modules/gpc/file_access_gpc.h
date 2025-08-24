#ifndef FILE_ACCESS_GPC_H
#define FILE_ACCESS_GPC_H

#include "core/io/file_access.h"
#include "core/io/file_access_encrypted.h"
#include "core/io/file_access_pack.h"
#include "core/io/file_access_memory.h"
#include "core/crypto/crypto_core.h"

class FileAccessGPC : public FileAccess {
    GDCLASS(FileAccessGPC, FileAccess);

public:
    static Ref<FileAccess> open_encrypted(const String &p_path, ModeFlags p_mode_flags, const Vector<uint8_t> &p_key, const Vector<uint8_t> &p_iv = Vector<uint8_t>());
    static Ref<FileAccess> open_encrypted_pass(const String &p_path, ModeFlags p_mode_flags, const String &p_pass);
    static Ref<FileAccess> open_compressed(const String &p_path, ModeFlags p_mode_flags, CompressionMode p_compress_mode = COMPRESSION_FASTLZ);

    static Ref<FileAccess> create_for_path(const String &p_path);
};

#endif // FILE_ACCESS_GPC_H
