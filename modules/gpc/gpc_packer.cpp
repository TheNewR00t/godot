#include "gpc_packer.h"

bool gpc_enabled = false;

void GPCPacker::_bind_methods() {
	ClassDB::bind_method(D_METHOD("open", "path"), &GPCPacker::open);
	ClassDB::bind_method(D_METHOD("add_file", "target_path", "source_path"), &GPCPacker::add_file);
	ClassDB::bind_method(D_METHOD("flush"), &GPCPacker::flush);
}

Error GPCPacker::open(const String &p_path) {
	pck_packer = memnew(PCKPacker);

	// Obfuscated key
	Vector<uint8_t> obfuscated_key;
	obfuscated_key.resize(32);
	// This is a dummy key, a real implementation should use a more secure key
	obfuscated_key.write[0] = 0x12;
	obfuscated_key.write[1] = 0x34;
	obfuscated_key.write[2] = 0x56;
	obfuscated_key.write[3] = 0x78;
	obfuscated_key.write[4] = 0x90;
	obfuscated_key.write[5] = 0xab;
	obfuscated_key.write[6] = 0xcd;
	obfuscated_key.write[7] = 0xef;
	obfuscated_key.write[8] = 0x12;
	obfuscated_key.write[9] = 0x34;
	obfuscated_key.write[10] = 0x56;
	obfuscated_key.write[11] = 0x78;
	obfuscated_key.write[12] = 0x90;
	obfuscated_key.write[13] = 0xab;
	obfuscated_key.write[14] = 0xcd;
	obfuscated_key.write[15] = 0xef;
	obfuscated_key.write[16] = 0x12;
	obfuscated_key.write[17] = 0x34;
	obfuscated_key.write[18] = 0x56;
	obfuscated_key.write[19] = 0x78;
	obfuscated_key.write[20] = 0x90;
	obfuscated_key.write[21] = 0xab;
	obfuscated_key.write[22] = 0xcd;
	obfuscated_key.write[23] = 0xef;
	obfuscated_key.write[24] = 0x12;
	obfuscated_key.write[25] = 0x34;
	obfuscated_key.write[26] = 0x56;
	obfuscated_key.write[27] = 0x78;
	obfuscated_key.write[28] = 0x90;
	obfuscated_key.write[29] = 0xab;
	obfuscated_key.write[30] = 0xcd;
	obfuscated_key.write[31] = 0xef;

	// XOR key
	Vector<uint8_t> xor_key;
	xor_key.resize(32);
	// This is a dummy key, a real implementation should use a more secure key
	xor_key.write[0] = 0xef;
	xor_key.write[1] = 0xcd;
	xor_key.write[2] = 0xab;
	xor_key.write[3] = 0x90;
	xor_key.write[4] = 0x78;
	xor_key.write[5] = 0x56;
	xor_key.write[6] = 0x34;
	xor_key.write[7] = 0x12;
	xor_key.write[8] = 0xef;
	xor_key.write[9] = 0xcd;
	xor_key.write[10] = 0xab;
	xor_key.write[11] = 0x90;
	xor_key.write[12] = 0x78;
	xor_key.write[13] = 0x56;
	xor_key.write[14] = 0x34;
	xor_key.write[15] = 0x12;
	xor_key.write[16] = 0xef;
	xor_key.write[17] = 0xcd;
	xor_key.write[18] = 0xab;
	xor_key.write[19] = 0x90;
	xor_key.write[20] = 0x78;
	xor_key.write[21] = 0x56;
	xor_key.write[22] = 0x34;
	xor_key.write[23] = 0x12;
	xor_key.write[24] = 0xef;
	xor_key.write[25] = 0xcd;
	xor_key.write[26] = 0xab;
	xor_key.write[27] = 0x90;
	xor_key.write[28] = 0x78;
	xor_key.write[29] = 0x56;
	xor_key.write[30] = 0x34;
	xor_key.write[31] = 0x12;

	Vector<uint8_t> key;
	key.resize(32);
	for (int i = 0; i < 32; i++) {
		key.write[i] = obfuscated_key[i] ^ xor_key[i];
	}

	String key_hex;
	for (int i = 0; i < key.size(); i++) {
		key_hex += String::num_int64(key[i], 16).pad_zeros(2);
	}

	return pck_packer->pck_start(p_path, 32, key_hex, true);
}

Error GPCPacker::add_file(const String &p_target_path, const String &p_source_path) {
	return pck_packer->add_file(p_target_path, p_source_path, true);
}

Error GPCPacker::flush() {
	return pck_packer->flush(true);
}

GPCPacker::GPCPacker() {
	pck_packer = nullptr;
}

GPCPacker::~GPCPacker() {
	if (pck_packer) {
		memdelete(pck_packer);
	}
}
