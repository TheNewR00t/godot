#ifndef GPC_PACKER_H
#define GPC_PACKER_H

#include "core/io/pck_packer.h"
#include "core/object/ref_counted.h"

extern bool gpc_enabled;

class GPCPacker : public RefCounted {
	GDCLASS(GPCPacker, RefCounted);

	Ref<PCKPacker> pck_packer;

protected:
	static void _bind_methods();

public:
	Error open(const String &p_path);
	Error add_file(const String &p_target_path, const String &p_source_path);
	Error flush();

	GPCPacker();
	~GPCPacker();
};

#endif // GPC_PACKER_H
