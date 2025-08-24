#include "file_access_gpc.h"
#include "core/os/os.h"
#include "core/os/time.h"
#include "core/config/project_settings.h"
#include "core/io/dir_access.h"
#include "core/io/marshalls.h"

Ref<FileAccess> FileAccessGPC::create_for_path(const String &p_path) {
    Ref<FileAccess> ret;
    if (p_path.begins_with("res://") || p_path.begins_with("uid://")) {
        ret = FileAccess::create(FileAccess::ACCESS_RESOURCES);
    } else if (p_path.begins_with("user://")) {
        ret = FileAccess::create(FileAccess::ACCESS_USERDATA);
    } else if (p_path.begins_with("pipe://")) {
        ret = FileAccess::create(FileAccess::ACCESS_PIPE);
    } else {
        ret = FileAccess::create(FileAccess::ACCESS_FILESYSTEM);
    }
    return ret;
}

Ref<FileAccess> FileAccessGPC::open_encrypted(const String &p_path, ModeFlags p_mode_flags, const Vector<uint8_t> &p_key, const Vector<uint8_t> &p_iv) {
    Ref<FileAccess> fa = FileAccess::open(p_path, p_mode_flags);
    if (fa.is_null()) {
        return fa;
    }

    Ref<FileAccessEncrypted> fae;
    fae.instantiate();

    FileAccessEncrypted::Mode mode = (p_mode_flags & FileAccess::WRITE) ?
                                     FileAccessEncrypted::MODE_WRITE_AES256 :
                                     FileAccessEncrypted::MODE_READ;

    Error err = fae->open_and_parse(fa, p_key, mode, true, p_iv);
    FileAccess::last_file_open_error = err;
    if (err != OK) {
        return Ref<FileAccess>();
    }
    return fae;
}

Ref<FileAccess> FileAccessGPC::open_encrypted_pass(const String &p_path, ModeFlags p_mode_flags, const String &p_pass) {
    Ref<FileAccess> fa = FileAccess::open(p_path, p_mode_flags);
    if (fa.is_null()) {
        return fa;
    }

    Ref<FileAccessEncrypted> fae;
    fae.instantiate();

    FileAccessEncrypted::Mode mode = (p_mode_flags & FileAccess::WRITE) ?
                                     FileAccessEncrypted::MODE_WRITE_AES256 :
                                     FileAccessEncrypted::MODE_READ;

    Error err = fae->open_and_parse_password(fa, p_pass, mode);
    FileAccess::last_file_open_error = err;
    if (err != OK) {
        return Ref<FileAccess>();
    }
    return fae;
}

Ref<FileAccess> FileAccessGPC::open_compressed(const String &p_path, ModeFlags p_mode_flags, CompressionMode p_compress_mode) {
    Ref<FileAccessCompressed> fac;
    fac.instantiate();
    fac->configure("GCPF", (Compression::Mode)p_compress_mode);
    Error err = fac->open_internal(p_path, p_mode_flags);
    FileAccess::last_file_open_error = err;
    if (err != OK) {
        return Ref<FileAccess>();
    }
    return fac;
}
