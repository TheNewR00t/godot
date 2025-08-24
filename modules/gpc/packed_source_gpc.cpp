#include "packed_source_gpc.h"

#include "core/io/file_access_memory.h"
#include "core/io/file_access_pack.h"
#include "aes/aes.h"
#include "key_obf.h"

// PKCS7 unpadding
static Vector<uint8_t> pkcs7_unpad(const Vector<uint8_t> &data) {
    if (data.is_empty()) {
        return data;
    }
    int padding = data[data.size() - 1];
    if (padding > data.size()) {
        return data; // Invalid padding
    }
    return data.slice(0, data.size() - padding);
}

bool PackedSourceGPC::try_open_pack(const String &p_path, bool p_replace_files, uint64_t p_offset) {
    return false; // We are not a pack format, so we will never open a file.
}

Ref<FileAccess> PackedSourceGPC::get_file(const String &p_path, PackedData::PackedFile *p_file) {
    if (!p_file->encrypted) {
        // Not encrypted, so use default PCK handler.
        PackedSourcePCK pck_source;
        return pck_source.get_file(p_path, p_file);
    }

    Ref<FileAccess> f = FileAccess::open(p_file->pack, FileAccess::READ);
    if (f.is_null()) {
        return Ref<FileAccess>();
    }

    f->seek(p_file->offset);
    Vector<uint8_t> encrypted_data = f->get_buffer(p_file->size);
    f->close();

    if (encrypted_data.size() < AES_BLOCKLEN) {
        return Ref<FileAccess>(); // Not enough data for IV
    }

    uint8_t key[32];
    get_deobfuscated_key(key);

    uint8_t iv[AES_BLOCKLEN];
    for (int i = 0; i < AES_BLOCKLEN; i++) {
        iv[i] = encrypted_data[i];
    }

    Vector<uint8_t> data_to_decrypt = encrypted_data.slice(AES_BLOCKLEN, encrypted_data.size());

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_decrypt_buffer(&ctx, data_to_decrypt.ptrw(), data_to_decrypt.size());

    Vector<uint8_t> decrypted_data = pkcs7_unpad(data_to_decrypt);

    Ref<FileAccessMemory> fam;
    fam.instantiate();
    fam->open_custom(decrypted_data.ptr(), decrypted_data.size());

    return fam;
}
