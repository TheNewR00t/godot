#include "register_types.h"
#include "file_access_encrypted.h"
#include "packed_source_gpc.h"

void register_gpc_types() {
    ClassDB::register_class<FileAccessEncrypted>();
    ClassDB::register_class<PackedSourceGPC>();
}

void unregister_gpc_types() {
    // Nada necesario por ahora
} 