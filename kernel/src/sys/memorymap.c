#include <sys/memorymap.h>
#include <sys/debug.h>
#include <sys/tty.h>
#include <string.h>

/// We know we have free space at 0x800 since we had our bootloader there
/// Should have enough space to put together a basic memory map
struct MemoryMapEntry *memorymap_pos;
unsigned int total_memory_map_entries;

int BIOSMemoryMapEntry_cmp(struct BIOSMemoryMapEntry first, struct BIOSMemoryMapEntry second)
{
	if (first.start_higher < second.start_higher)
		return -1;

	if (first.start_higher > second.start_higher)
		return 1;

	if (first.start_lower < second.start_lower)
		return -1;

	if (first.start_lower > second.start_lower)
		return 1;
	return 0;
}

struct MemoryMapEntry *get_free_memory_block()
{
	struct MemoryMapEntry *block = memorymap_pos;
	for (int i = 0; i < total_memory_map_entries; i++)
	{
		if (block->flags == 0)
			return block;
	}
	return 0;
}

// TODO: Stop entries from overlapping
void reserve_kernel_memory(uint64_t start_pos, uint64_t size, uint64_t flags)
{
	struct MemoryMapEntry *curr = memorymap_pos;
	while (curr->next != 0)
	{
		if (curr->start <= start_pos && start_pos < (curr->start + curr->size))
			break;
		curr = curr->next;
	}

	// we are inside an existing block so modify it
	if ((curr->start + curr->size) < start_pos)
	{
		kernel_panic("Trying to allocate memory that is not available");
	}
	struct MemoryMapEntry *new_entry = get_free_memory_block();
	if (curr->start == start_pos)
	{
		new_entry->start = start_pos + size;
		new_entry->size = curr->size - size;
		new_entry->owner = curr->owner;
		new_entry->flags = curr->flags;
		new_entry->next = curr->next;
		curr->size = size;
		curr->owner = 0;
		curr->flags = flags | MEMORY_BLOCK_USED;
		curr->next = new_entry;
	}
	else
	{
		new_entry->start = start_pos;
		new_entry->size = size;
		new_entry->owner = 0;
		new_entry->flags = flags | MEMORY_BLOCK_USED;
		new_entry->next = curr->next;
		if ((new_entry->start + new_entry->size) < (curr->start + curr->size))
		{
			struct MemoryMapEntry *after_entry = get_free_memory_block();
			after_entry->start = start_pos + size;
			after_entry->size = (curr->start + curr->size) - start_pos;
			after_entry->owner = curr->owner;
			after_entry->flags = curr->flags;
			after_entry->next = curr->next;
			new_entry->next = after_entry;
		}
		curr->size = start_pos - curr->start;
		curr->next = new_entry;
	}
}

void print_bios_memory_map(struct BIOSMemoryMapEntry *entry, unsigned int entry_count)
{
	for (unsigned int c = 0; c < entry_count; c++)
	{
		printhex(entry[c].start_higher);
		printspc();
		printhex(entry[c].start_lower);
		printspc();
		printhex(entry[c].size_higher);
		printspc();
		printhex(entry[c].size_lower);
		printspc();
		printhex(entry[c].region_type);
		println();
	}
	println();
}

void debug_check_memory_map()
{
	struct MemoryMapEntry *current = memorymap_pos;
	while (current != 0)
	{
		printhex_digits((uint32_t)current, 16);
		printspc();
		printhex_digits(current->start, 64);
		printspc();
		printhex_digits(current->size, 48);
		printspc();
		printhex_digits(current->owner, 16);
		printspc();
		printhex_digits(current->flags, 16);
		printspc();
		printhex_digits((uint32_t)current->next, 32);
		println();
		current = current->next;
	}
}

void initialise_memory_manager()
{
	memorymap_pos = (struct MemoryMapEntry *)0x800;
	total_memory_map_entries = 200;

	for (int i = 0; i < total_memory_map_entries; i++)
	{
		memorymap_pos[i].flags = 0;
	}

	unsigned int entry_count = *(int *)(0x505);
	struct BIOSMemoryMapEntry *entry = (struct BIOSMemoryMapEntry *)0x50d;

	/// put entries in order (use bubble sort, since can't assign memory yet)
	struct BIOSMemoryMapEntry swap;
	for (unsigned int c = entry_count - 1; c > 0; c--)
	{
		for (unsigned int d = 0; d < c; d++)
			if (BIOSMemoryMapEntry_cmp(entry[d], entry[d + 1]) == 1)
			{
				swap = entry[d];
				entry[d] = entry[d + 1];
				entry[d + 1] = swap;
			}
	}

	for (unsigned int c = 1; c < entry_count; c++)
	{
		uint32_t end_prev_higher = entry[c - 1].start_higher + entry[c - 1].size_higher;
		if (entry[c - 1].size_lower > UINT32_MAX - entry[c - 1].start_lower)
		{
			// overflow happened, carry over
			end_prev_higher += 1;
		}
		uint32_t end_prev_lower = entry[c - 1].start_lower + entry[c - 1].size_lower;

		uint32_t end_curr_higher = (entry[c].start_higher + entry[c].size_higher);
		if (entry[c].size_lower > UINT32_MAX - entry[c].start_lower)
		{
			end_curr_higher += 1;
		}
		uint32_t end_curr_lower = (entry[c].start_lower + entry[c].size_lower);

		/// Fill in gaps between memory entries
		if (
			entry[c].start_higher > end_prev_higher ||
			(entry[c].start_higher == end_prev_higher && entry[c].start_lower > end_prev_lower))
		{
			if (entry[c - 1].region_type == 2)
			{

				entry[c - 1].size_lower = entry[c].start_lower - entry[c - 1].start_lower;
				if (entry[c - 1].size_lower > entry[c].start_lower)
				{
					entry[c - 1].size_higher -= 1;
				}

				entry[c - 1].size_higher = entry[c].start_higher - entry[c - 1].start_higher;
			}
			else if (entry[c].region_type == 2)
			{
				entry[c].size_lower = end_curr_lower - end_prev_lower;
				if (entry[c].size_lower > end_curr_lower)
				{
					entry[c].size_higher -= 1;
				}
				entry[c].size_higher = end_curr_higher - end_curr_higher;
				entry[c].start_lower = end_prev_lower;
				entry[c].start_higher = end_prev_higher;
			}
		}
	}

	/// write memory entries in format used by the kernel
	for (unsigned int c = 0; c < entry_count; c++)
	{
		memorymap_pos[c].start = entry[c].start_higher;
		memorymap_pos[c].start = (memorymap_pos[c].start << 32) | entry[c].start_lower;
		memorymap_pos[c].size = entry[c].start_higher;
		memorymap_pos[c].size = (memorymap_pos[c].size << 32) | entry[c].size_lower;
		memorymap_pos[c].owner = 0;
		memorymap_pos[c].next = 0;
		if (entry[c].region_type == 1)
		{
			memorymap_pos[c].flags = MEMORY_FREE | MEMORY_BLOCK_USED;
		}
		else if (entry[c].region_type == 2)
		{
			memorymap_pos[c].flags = MEMORY_RESERVED | MEMORY_BLOCK_USED;
		}
		else if (entry[c].region_type == 3)
		{
			memorymap_pos[c].flags = MEMORY_RECLAIMABLE | MEMORY_BLOCK_USED;
		}
		else
		{
			memorymap_pos[c].flags = MEMORY_RESERVED | MEMORY_BLOCK_USED;
		}

		if (c != 0)
		{
			memorymap_pos[c - 1].next = memorymap_pos + c;
		}
	}

	// TODO: Place memory map in a free block and make sure enough there is enough space for 100 entries

	/// Spaces not to use:
	///  0x0 -> 0x500 BIOS stuff i think?
	///  0x100000 -> size of kernel, because it is reserved by our kernel
	///  0x800 -> initial size of memory map, because it is used by the memory map

	unsigned int size_of_kernel = *((unsigned int *)0x500);

	reserve_kernel_memory(0, 0x500, MEMORY_RESERVED);
	reserve_kernel_memory(0x800, sizeof(struct MemoryMapEntry) * total_memory_map_entries, MEMORY_WRITEABLE | MEMORY_READABLE);
	reserve_kernel_memory(0x100000, size_of_kernel, MEMORY_EXECUTABLE);

	debug_check_memory_map();
	println();
}

void deallocate_memory_map_entry(struct MemoryMapEntry *entry)
{
}

struct MemoryMapEntry *allocate_memory_map_entry()
{
	struct MemoryMapEntry *block = get_free_memory_block();
	if (block == NULL)
	{
		uint32_t new_count = total_memory_map_entries * 1.5;
		struct MemoryMapEntry *current = memorymap_pos;
		while (current != 0)
		{
			if (current->flags | MEMORY_FREE != 0 && current->size >= (new_count * sizeof(struct MemoryMapEntry)))
			{
				memcpy(current->start, memorymap_pos, total_memory_map_entries * sizeof(struct MemoryMapEntry));
				struct MemoryMapEntry *entry = current->start;
				for (int i = total_memory_map_entries; i < new_count; i++)
				{
					entry[i].flags = 0;
				}
				deallocate_memory_map_entry(memorymap_pos);
				total_memory_map_entries = new_count;
				reserve_kernel_memory(current->start, new_count * sizeof(struct MemoryMapEntry), MEMORY_WRITEABLE | MEMORY_READABLE);
				memorymap_pos = current->start;
			}
		}
		if (current == 0)
		{
			kernel_panic("No memory left on device");
		}
		struct MemoryMapEntry *block = get_free_memory_block();
		if (block == NULL)
		{
			kernel_panic("Could not allocate memory map entry.");
		}
	}

	return block;
}

void *allocate_memory(unsigned int size, pid_t owner, enum MEMORY_FLAGS flags)
{
	/// allocate memorymap entry first
	struct MemoryMapEntry *mm_entry = allocate_memory_map_entry();
	struct MemoryMapEntry *current = memorymap_pos;
	while (current != 0)
	{
		if (current->flags == MEMORY_FREE && current->size > size)
			break;
		current = current->next;
	}
	if (current == 0)
		return NULL;

	mm_entry->start = current->start;
	mm_entry->size = size;
	mm_entry->flags = flags | MEMORY_BLOCK_USED;
	mm_entry->owner = owner;
	mm_entry->next = current;
	current->start += size;
	current->size -= size;
	return mm_entry->start;
}

void deallocate_memory(void *mem)
{
	struct MemoryMapEntry *current = memorymap_pos;
	while (current != 0)
	{
		if (current->start == mem)
			break;
		current = current->next;
	}
	if (current == 0)
	{
		kernel_panic("Could not find entry for reserved memory.");
	}

	deallocate_memory_map_entry(current);
}