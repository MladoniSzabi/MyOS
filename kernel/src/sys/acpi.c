#include "sys/acpi.h"

#include "string.h"

#include "sys/debug.h"
#include "sys/tty.h"

struct RSDT *rsdtPointer = NULL;

bool validateXSDP(struct XSDP_t *xsdp)
{
    uint8_t sum = 0;
    uint8_t *bytes = (uint8_t *)xsdp;
    unsigned int i = 0;
    for (; i < 20; i++)
    {
        sum += bytes[i];
    }

    if (sum != 0)
        return false;

    if (xsdp->revision == 0)
        return true;

    for (; i < sizeof(struct XSDP_t); i++)
    {
        sum += bytes[i];
    }

    if (xsdp->revision == 2)
        return true;

    return false;
}

struct XSDP_t *findRSDP()
{
    struct XSDP_t *potential_pointer = (struct XSDP_t *)0x0000040E;
    if (strncmp(potential_pointer->signature, "RSD PTR ", 8) == 0)
    {
        if (validateXSDP(potential_pointer))
        {
            return potential_pointer;
        }
    }

    uint32_t addr = 0x000E0000;
    while (addr < 0x000FFFFF)
    {
        potential_pointer = (struct XSDP_t *)addr;
        if (strncmp(potential_pointer->signature, "RSD PTR ", 8) == 0)
        {
            if (validateXSDP(potential_pointer))
            {
                return potential_pointer;
            }
        }

        addr += 16;
    }

    return NULL;
}

struct RSDT *findRSDT()
{
    struct XSDP_t *xsdp = findRSDP();
    if (xsdp == NULL)
        return NULL;

    rsdtPointer = (struct RSDT *)xsdp->rsdtAddr;
    return rsdtPointer;
}

bool doChecksum(struct ACPISDTHeader *tableHeader)
{
    uint8_t sum = 0;
    for (unsigned int i = 0; i < tableHeader->length; i++)
    {
        sum += ((uint8_t *)tableHeader)[i];
    }

    return sum == 0;
}
void *findTable(const char *tableID)
{
    if (rsdtPointer == 0)
        return NULL;
    unsigned int entryCount = (rsdtPointer->h.length - sizeof(struct ACPISDTHeader)) / 4;
    for (unsigned int i = 0; i < entryCount; i++)
    {
        struct ACPISDTHeader *header = (struct ACPISDTHeader *)rsdtPointer->pointertootherSDT[i];
        for (int i = 0; i < 4; i++)
        {
            terminal_putchar(header->signature[i]);
        }
        printspc();
        if (strncmp(tableID, header->signature, 4) == 0)
        {
            return (void *)header;
        }
    }

    return NULL;
}