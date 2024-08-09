#include "sys/ps2.h"

#include "sys/acpi.h"

bool is_ps2_present()
{
    struct FADT *fadt = (struct FADT *)findTable("FACP");
    if (fadt == 0)
        return true;

    if (fadt->bootArchitectureFlags & 1)
        return true;

    return false;
}