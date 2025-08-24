#include "gpc_export_plugin.h"
#include "editor/export/editor_export.h"
#include "core/io/file_access.h"
#include "aes/aes.h"
#include "core/os/os.h"
#include "core/math/random_pcg.h"

// PKCS7 padding
static Vector<uint8_t> pkcs7_pad(const Vector<uint8_t> &data, int block_size) {
    int padding = block_size - (data.size() % block_size);
    Vector<uint8_t> padded_data = data;
    for (int i = 0; i < padding; i++) {
        padded_data.push_back(padding);
    }
    return padded_data;
}

void GPCExportPlugin::_export_file(const String &p_path, const String &p_type, const HashSet<String> &p_features) {
    Ref<EditorExportPreset> preset = get_export_preset();
    if (preset.is_null() || !preset->get_enc_pck()) {
        skip();
        return;
    }

    Vector<String> enc_in_filters = preset->get_enc_in_filter().split(",");
    Vector<String> enc_ex_filters = preset->get_enc_ex_filter().split(",");

    bool encrypt = false;
    for (int i = 0; i < enc_in_filters.size(); ++i) {
        if (!enc_in_filters[i].is_empty() && (p_path.matchn(enc_in_filters[i].strip_edges()) || p_path.trim_prefix("res://").matchn(enc_in_filters[i].strip_edges()))) {
            encrypt = true;
            break;
        }
    }

    for (int i = 0; i < enc_ex_filters.size(); ++i) {
        if (!enc_ex_filters[i].is_empty() && (p_path.matchn(enc_ex_filters[i].strip_edges()) || p_path.trim_prefix("res://").matchn(enc_ex_filters[i].strip_edges()))) {
            encrypt = false;
            break;
        }
    }

    if (!encrypt) {
        skip();
        return;
    }

    Vector<uint8_t> key;
    String script_key = preset->get_script_encryption_key();
    key.resize(32);
    if (script_key.length() == 64) {
        for (int i = 0; i < 32; i++) {
            int v = 0;
            if (i * 2 < script_key.length()) {
                char32_t ct = script_key[i * 2];
                if (is_digit(ct)) {
                    ct = ct - '0';
                } else if (ct >= 'a' && ct <= 'f') {
                    ct = 10 + ct - 'a';
                }
                v |= ct << 4;
            }

            if (i * 2 + 1 < script_key.length()) {
                char32_t ct = script_key[i * 2 + 1];
                if (is_digit(ct)) {
                    ct = ct - '0';
                } else if (ct >= 'a' && ct <= 'f') {
                    ct = 10 + ct - 'a';
                }
                v |= ct;
            }
            key.write[i] = v;
        }
    } else {
        // Invalid key, skip encryption
        skip();
        return;
    }

    Vector<uint8_t> file_data = FileAccess::get_file_as_bytes(p_path);
    if (file_data.is_empty()) {
        add_file(p_path, file_data, false);
        return;
    }

    Vector<uint8_t> padded_data = pkcs7_pad(file_data, AES_BLOCKLEN);

    uint8_t iv[AES_BLOCKLEN];
    uint64_t seed = preset->get_seed();
    if (seed == 0) {
        seed = OS::get_singleton()->get_unix_time();
    }
    RandomPCG rng = RandomPCG(seed);
    for (int i = 0; i < AES_BLOCKLEN; i++) {
        iv[i] = rng.rand() % 256;
    }

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key.ptr(), iv);
    AES_CBC_encrypt_buffer(&ctx, padded_data.ptrw(), padded_data.size());

    Vector<uint8_t> encrypted_data;
    for (int i = 0; i < AES_BLOCKLEN; i++) {
        encrypted_data.push_back(iv[i]);
    }
    encrypted_data.append_array(padded_data);

    add_file(p_path, encrypted_data, true);
}
