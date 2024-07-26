#pragma once

#include <stdint.h>
#include <sys/types.h>

enum MEMORY_FLAGS
{
	MEMORY_BLOCK_USED = 1,
	MEMORY_RESERVED = 2,
	MEMORY_FREE = 4,
	MEMORY_READABLE = 8,
	MEMORY_WRITEABLE = 16,
	MEMORY_EXECUTABLE = 32,
	MEMORY_RECLAIMABLE = 64,
};

struct MemoryMapEntry
{
	uint64_t start;
	uint64_t size;
	uint64_t flags;
	pid_t owner;
	struct MemoryMapEntry *next;
};

struct BIOSMemoryMapEntry
{
	uint32_t start_lower;
	uint32_t start_higher;
	uint32_t size_lower;
	uint32_t size_higher;
	uint32_t region_type;
	uint32_t acpi3_data;
};

int BIOSMemoryMapEntry_cmp(struct BIOSMemoryMapEntry first, struct BIOSMemoryMapEntry second);

void initialise_memory_manager();
void *allocate_memory();
struct MemoryMapEntry *allocate_memory_map_entry();
void debug_check_memory_map();
