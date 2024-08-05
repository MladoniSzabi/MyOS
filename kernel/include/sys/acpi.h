#pragma once

#include <stdint.h>
#include <stdbool.h>

struct XSDP_t
{
    char signature[8];
    uint8_t checksum;
    char OEMID[6];
    uint8_t revision;
    uint32_t rsdtAddr;

    uint32_t length;
    uint64_t xsdtAddr;
    uint8_t extChecksum;
    uint8_t reserved[3];
} __attribute__((packed));

bool validateXSDP(struct XSDP_t *xsdp);
struct RSDT *findRSDT();

struct ACPISDTHeader
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemID[6];
    char oemTableID[8];
    uint32_t oemRevision;
    uint32_t creatorID;
    uint32_t creatorRevision;
} __attribute__((packed));

bool doChecksum(struct ACPISDTHeader *tableHeader);

struct RSDT
{
    struct ACPISDTHeader h;
    uint32_t pointertootherSDT[];
} __attribute__((packed));

// struct XSDT
// {
//     struct ACPISDTHeader h;
//     uint64_t pointertootherSDT[];
// } __attribute__((packed));

void *findTable(const char *tableID);

struct GenericAddressStructure
{
    uint8_t addressSpace;
    uint8_t bitWidth;
    uint8_t bitOffset;
    uint8_t accessSize;
    uint64_t address;
} __attribute__((packed));

struct FADT
{
    struct ACPISDTHeader header;
    uint32_t firmwareControl;
    uint32_t dsdtAddress;

    // field used in ACPI 1.0; no longer in use, for compatibility only
    uint8_t reserved;

    uint8_t preferredPowerManagementProfile;
    uint16_t SCIInterrupt;
    uint32_t SMICommandPort;
    uint8_t acpiEnable;
    uint8_t acpiDisable;
    uint8_t S4BIOS_REQ;
    uint8_t PSTATE_Control;
    uint32_t PM1aEventBlock;
    uint32_t PM1bEventBlock;
    uint32_t PM1aControlBlock;
    uint32_t PM1bControlBlock;
    uint32_t PM2ControlBlock;
    uint32_t PMTimerBlock;
    uint32_t GPE0Block;
    uint32_t GPE1Block;
    uint8_t PM1EventLength;
    uint8_t PM1ControlLength;
    uint8_t PM2ControlLength;
    uint8_t PMTimerLength;
    uint8_t GPE0Length;
    uint8_t GPE1Length;
    uint8_t GPE1Base;
    uint8_t CStateControl;
    uint16_t worstC2Latency;
    uint16_t worstC3Latency;
    uint16_t flushSize;
    uint16_t flushStride;
    uint8_t dutyOffset;
    uint8_t dutyWidth;
    uint8_t dayAlarm;
    uint8_t monthAlarm;
    uint8_t century;

    // reserved in ACPI 1.0; used since ACPI 2.0+
    uint16_t bootArchitectureFlags;

    uint8_t reserved2;
    uint32_t flags;

    // 12 byte structure; see below for details
    struct GenericAddressStructure resetReg;

    uint8_t resetValue;
    uint8_t reserved3[3];

    // 64bit pointers - Available on ACPI 2.0+
    uint64_t X_FirmwareControl;
    uint64_t X_Dsdt;

    struct GenericAddressStructure X_PM1aEventBlock;
    struct GenericAddressStructure X_PM1bEventBlock;
    struct GenericAddressStructure X_PM1aControlBlock;
    struct GenericAddressStructure X_PM1bControlBlock;
    struct GenericAddressStructure X_PM2ControlBlock;
    struct GenericAddressStructure X_PMTimerBlock;
    struct GenericAddressStructure X_GPE0Block;
    struct GenericAddressStructure X_GPE1Block;
} __attribute__((packed));