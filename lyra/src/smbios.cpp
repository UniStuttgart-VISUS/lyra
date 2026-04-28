// <copyright file="smbios.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2015 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/smbios.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <system_error>

#include "visus/lyra/version.h"

#include "file.h"
#include "property_set_impl.h"


/// <summary>
/// Holds the human-readable strings for the BIOS characteristics.
/// </summary>
static const char *bios_characteristics[] = {
    u8"Reserved.",
    u8"Reserved.",
    u8"Unknown.",
    u8"BIOS Characteristics are not supported."
    u8"ISA is supported.",
    u8"MCA is supported.",
    u8"EISA is supported.",
    u8"PCI is supported.",
    u8"PC card (PCMCIA) is supported.",
    u8"Plug and Play is supported.",
    u8"APM is supported.",
    u8"BIOS is upgradeable (Flash)."
    u8"BIOS shadowing is allowed.",
    u8"VL - VESA is supported.",
    u8"ESCD support is available.",
    u8"Boot from CD is supported.",
    u8"Selectable boot is supported.",
    u8"BIOS ROM is socketed.",
    u8"Boot from PC card (PCMCIA) is supported.",
    u8"EDD specification is supported.",
    u8"Int 13h - Japanese floppy for NEC 9800 1.2 MB (3.5\", 1K bytes/sector, 360 RPM) is supported.",
    u8"Int 13h - Japanese floppy for Toshiba 1.2 MB (3.5\", 360 RPM) is supported.",
    u8"Int 13h - 5.25\"/360 KB floppy services are supported.",
    u8"Int 13h - 5.25\"/1.2 MB floppy services are supported.",
    u8"Int 13h - 3.5\"/720 KB floppy services are supported.",
    u8"Int 13h - 3.5\"/2.88 MB floppy services are supported.",
    u8"Int 5h - print screen service is supported.",
    u8"Int 9h - 8042 keyboard services are supported.",
    u8"Int 14h - serial services are supported.",
    u8"Int 17h - printer services are supported.",
    u8"Int 10h - CGA/Mono video Services are supported.",
    u8"NEC PC - 98.",
    //Bits32:47 Reserved for BIOS vendor.
    //Bits 48 : 63 Reserved for system vendor.
};


/// <summary>
/// Holds the human-readable strings for the BIOS extension bytes 1.
/// </summary>
static const char *bios_extensions1[] = {
    /* 0x00 */ u8"ACPI is supported.",
    /* 0x01 */ u8"USB Legacy is supported.",
    /* 0x02 */ u8"AGP is supported.",
    /* 0x03 */ u8"I2O boot is supported.",
    /* 0x04 */ u8"LS-120 SuperDisk boot is supported.",
    /* 0x05 */ u8"ATAPI ZIP drive boot is supported.",
    /* 0x06 */ u8"1394 boot is supported.",
    /* 0x07 */ u8"Smart battery is supported."
};


/// <summary>
/// Holds the human-readable strings for the BIOS extension bytes 2.
/// </summary>
static const char *bios_extensions2[] = {
    /* 0x00 */ u8"BIOS Boot Specification is supported.",
    /* 0x01 */ u8"Function key-initiated network service boot is supported.",
    /* 0x02 */ u8"Enable targeted content distribution.",
    /* 0x03 */ u8"UEFI Specification is supported.",
    /* 0x04 */ u8"SMBIOS table describes a virtual machine.",
    /* 0x05 */ u8"Reserved 5",
    /* 0x06 */ u8"Reserved 6",
    /* 0x07 */ u8"Reserved 7",
};


/// <summary>
/// Holds the human-readable strings for the system chassis types.
/// </summary>
static const char *chassis_types[] = {
    /* 0x00 */ u8"",
    /* 0x01 */ u8"Other",
    /* 0x02 */ u8"Unknown",
    /* 0x03 */ u8"Desktop",
    /* 0x04 */ u8"Low Profile Desktop",
    /* 0x05 */ u8"Pizza Box",
    /* 0x06 */ u8"Mini Tower",
    /* 0x07 */ u8"Tower",
    /* 0x08 */ u8"Portable",
    /* 0x09 */ u8"Laptop",
    /* 0x0A */ u8"Notebook",
    /* 0x0B */ u8"Hand Held",
    /* 0x0C */ u8"Docking Station",
    /* 0x0D */ u8"All in One",
    /* 0x0E */ u8"Sub Notebook",
    /* 0x0F */ u8"Space-saving",
    /* 0x10 */ u8"Lunch Box",
    /* 0x11 */ u8"Main Server Chassis",
    /* 0x12 */ u8"Expansion Chassis",
    /* 0x13 */ u8"SubChassis",
    /* 0x14 */ u8"Bus Expansion Chassis",
    /* 0x15 */ u8"Peripheral Chassis",
    /* 0x16 */ u8"RAID Chassis",
    /* 0x17 */ u8"Rack Mount Chassis",
    /* 0x18 */ u8"Sealed-case PC",
    /* 0x19 */ u8"Multi-system chassis",
    /* 0x1A */ u8"Compact PCI",
    /* 0x1B */ u8"Advanced TCA",
    /* 0x1C */ u8"Blade,"
    /* 0x1D */ u8"Blade Enclosure,"
    /* 0x1E */ u8"Tablet",
    /* 0x1F */ u8"Convertible",
    /* 0x20 */ u8"Detachable"
};


static const char *processor_families[] = {
    /* 0x0000 */ u8"",
    /* 0x0001 */ u8"Other",
    /* 0x0002 */ u8"Unknown",
    /* 0x0003 */ u8"8086",
    /* 0x0004 */ u8"80286",
    /* 0x0005 */ u8"Intel386™ processor",
    /* 0x0006 */ u8"Intel486™ processor",
    /* 0x0007 */ u8"8087",
    /* 0x0008 */ u8"80287",
    /* 0x0009 */ u8"80387",
    /* 0x000A */ u8"80487",
    /* 0x000B */ u8"Intel® Pentium® processor",
    /* 0x000C */ u8"Pentium® Pro processor",
    /* 0x000D */ u8"Pentium® II processor",
    /* 0x000E */ u8"Pentium® processor with MMX™ technology",
    /* 0x000F */ u8"Intel® Celeron® processor",
    /* 0x0010 */ u8"Pentium® II Xeon™ processor",
    /* 0x0011 */ u8"Pentium® III processor",
    /* 0x0012 */ u8"M1 Family",
    /* 0x0013 */ u8"M2 Family",
    /* 0x0014 */ u8"Intel® Celeron® M processor",
    /* 0x0015 */ u8"Intel® Pentium® 4 HT processor"
    /* 0x0016 */ u8"[Available for assignment]",
    /* 0x0017 */ u8"[Available for assignment]",
    /* 0x0018 */ u8"AMD Duron™ Processor Family",
    /* 0x0019 */ u8"K5 Family",
    /* 0x001A */ u8"K6 Family",
    /* 0x001B */ u8"K6-2",
    /* 0x001C */ u8"K6-3",
    /* 0x001D */ u8"AMD Athlon™ Processor Family",
    /* 0x001E */ u8"AMD29000 Family",
    /* 0x001F */ u8"K6-2+",
    /* 0x0020 */ u8"Power PC Family",
    /* 0x0021 */ u8"Power PC 601",
    /* 0x0022 */ u8"Power PC 603",
    /* 0x0023 */ u8"Power PC 603+",
    /* 0x0024 */ u8"Power PC 604",
    /* 0x0025 */ u8"Power PC 620",
    /* 0x0026 */ u8"Power PC x704",
    /* 0x0027 */ u8"Power PC 750",
    /* 0x0028 */ u8"Intel® Core™ Duo processor",
    /* 0x0029 */ u8"Intel® Core™ Duo mobile processor",
    /* 0x002A */ u8"Intel® Core™ Solo mobile processor",
    /* 0x002B */ u8"Intel® Atom™ processor",
    /* 0x002C */ u8"Intel® Core™ M processor",
    /* 0x002D */ u8"[Available for assignment]",
    /* 0x002E */ u8"[Available for assignment]",
    /* 0x002F */ u8"[Available for assignment]",
    /* 0x0030 */ u8"Alpha Family",
    /* 0x0031 */ u8"Alpha 21064",
    /* 0x0032 */ u8"Alpha 21066",
    /* 0x0033 */ u8"Alpha 21164",
    /* 0x0034 */ u8"Alpha 21164PC",
    /* 0x0035 */ u8"Alpha 21164a",
    /* 0x0036 */ u8"Alpha 21264",
    /* 0x0037 */ u8"Alpha 21364",
    /* 0x0038 */ u8"AMD Turion™ II Ultra Dual-Core Mobile M Processor Family",
    /* 0x0039 */ u8"AMD Turion™ II Dual-Core Mobile M Processor Family",
    /* 0x003A */ u8"AMD Athlon™ II Dual-Core M Processor Family",
    /* 0x003B */ u8"AMD Opteron™ 6100 Series Processor",
    /* 0x003C */ u8"AMD Opteron™ 4100 Series Processor",
    /* 0x003D */ u8"AMD Opteron™ 6200 Series Processor",
    /* 0x003E */ u8"AMD Opteron™ 4200 Series Processor",
    /* 0x003F */ u8"AMD FX™ Series Processor",
    /* 0x0040 */ u8"MIPS Family",
    /* 0x0041 */ u8"MIPS R4000",
    /* 0x0042 */ u8"MIPS R4200",
    /* 0x0043 */ u8"MIPS R4400",
    /* 0x0044 */ u8"MIPS R4600",
    /* 0x0045 */ u8"MIPS R10000",
    /* 0x0046 */ u8"AMD C-Series Processor",
    /* 0x0047 */ u8"AMD E-Series Processor",
    /* 0x0048 */ u8"AMD A-Series Processor",
    /* 0x0049 */ u8"AMD G-Series Processor",
    /* 0x004A */ u8"AMD Z-Series Processor",
    /* 0x004B */ u8"AMD R-Series Processor",
    /* 0x004C */ u8"AMD Opteron™ 4300 Series Processor",
    /* 0x004D */ u8"AMD Opteron™ 6300 Series Processor",
    /* 0x004E */ u8"AMD Opteron™ 3300 Series Processor,"
    /* 0x004F */ u8"AMD FirePro™ Series Processor",
    /* 0x0050 */ u8"SPARC Family",
    /* 0x0051 */ u8"SuperSPARC",
    /* 0x0052 */ u8"microSPARC II",
    /* 0x0053 */ u8"microSPARC IIep",
    /* 0x0054 */ u8"UltraSPARC",
    /* 0x0055 */ u8"UltraSPARC II",
    /* 0x0056 */ u8"UltraSPARC Iii",
    /* 0x0057 */ u8"UltraSPARC III",
    /* 0x0058 */ u8"UltraSPARC IIIi",
    /* 0x0059 */ u8"[Available for assignment]",
    /* 0x005A */ u8"[Available for assignment]",
    /* 0x005B */ u8"[Available for assignment]",
    /* 0x005C */ u8"[Available for assignment]",
    /* 0x005D */ u8"[Available for assignment]",
    /* 0x005E */ u8"[Available for assignment]",
    /* 0x005F */ u8"[Available for assignment]",
    /* 0x0060 */ u8"68040 Family",
    /* 0x0061 */ u8"68xxx",
    /* 0x0062 */ u8"68000",
    /* 0x0063 */ u8"68010",
    /* 0x0064 */ u8"68020",
    /* 0x0065 */ u8"68030",
    /* 0x0066 */ u8"AMD Athlon(TM) X4 Quad-Core Processor Family",
    /* 0x0067 */ u8"AMD Opteron(TM) X1000 Series Processor",
    /* 0x0068 */ u8"AMD Opteron(TM) X2000 Series APU",
    /* 0x0069 */ u8"[Available for assignment]",
    /* 0x006A */ u8"[Available for assignment]",
    /* 0x006B */ u8"[Available for assignment]",
    /* 0x006C */ u8"[Available for assignment]",
    /* 0x006D */ u8"[Available for assignment]",
    /* 0x006E */ u8"[Available for assignment]",
    /* 0x006F */ u8"[Available for assignment]",
    /* 0x0070 */ u8"Hobbit Family",
    /* 0x0071 */ u8"[Available for assignment]",
    /* 0x0072 */ u8"[Available for assignment]",
    /* 0x0073 */ u8"[Available for assignment]",
    /* 0x0074 */ u8"[Available for assignment]",
    /* 0x0075 */ u8"[Available for assignment]",
    /* 0x0076 */ u8"[Available for assignment]",
    /* 0x0077 */ u8"[Available for assignment]",
    /* 0x0078 */ u8"Crusoe™ TM5000 Family",
    /* 0x0079 */ u8"Crusoe™ TM3000 Family",
    /* 0x007A */ u8"Efficeon™ TM8000 Family",
    /* 0x007B */ u8"[Available for assignment]",
    /* 0x007C */ u8"[Available for assignment]",
    /* 0x007D */ u8"[Available for assignment]",
    /* 0x007E */ u8"[Available for assignment]",
    /* 0x007F */ u8"[Available for assignment]",
    /* 0x0080 */ u8"Weitek",
    /* 0x0081 */ u8"[Available for assignment]",
    /* 0x0082 */ u8"Itanium™ processor",
    /* 0x0083 */ u8"AMD Athlon™ 64 Processor Family",
    /* 0x0084 */ u8"AMD Opteron™ Processor Family",
    /* 0x0085 */ u8"AMD Sempron™ Processor Family",
    /* 0x0086 */ u8"AMD Turion™ 64 Mobile Technology",
    /* 0x0087 */ u8"Dual-Core AMD Opteron™ Processor Family",
    /* 0x0088 */ u8"AMD Athlon™ 64 X2 Dual-Core Processor Family",
    /* 0x0089 */ u8"AMD Turion™ 64 X2 Mobile Technology",
    /* 0x008A */ u8"Quad-Core AMD Opteron™ Processor Family",
    /* 0x008B */ u8"Third-Generation AMD Opteron™ Processor Family",
    /* 0x008C */ u8"AMD Phenom™ FX Quad-Core Processor Family",
    /* 0x008D */ u8"AMD Phenom™ X4 Quad-Core Processor Family",
    /* 0x008E */ u8"AMD Phenom™ X2 Dual-Core Processor Family",
    /* 0x008F */ u8"AMD Athlon™ X2 Dual-Core Processor Family",
    /* 0x0090 */ u8"PA-RISC Family",
    /* 0x0091 */ u8"PA-RISC 8500",
    /* 0x0092 */ u8"PA-RISC 8000",
    /* 0x0093 */ u8"PA-RISC 7300LC",
    /* 0x0094 */ u8"PA-RISC 7200",
    /* 0x0095 */ u8"PA-RISC 7100LC",
    /* 0x0096 */ u8"PA-RISC 7100",
    /* 0x0097 */ u8"[Available for assignment]",
    /* 0x0098 */ u8"[Available for assignment]",
    /* 0x0099 */ u8"[Available for assignment]",
    /* 0x009A */ u8"[Available for assignment]",
    /* 0x009B */ u8"[Available for assignment]",
    /* 0x009C */ u8"[Available for assignment]",
    /* 0x009D */ u8"[Available for assignment]",
    /* 0x009E */ u8"[Available for assignment]",
    /* 0x009F */ u8"[Available for assignment]",
    /* 0x00A0 */ u8"V30 Family",
    /* 0x00A1 */ u8"Quad-Core Intel® Xeon® processor 3200 Series",
    /* 0x00A2 */ u8"Dual-Core Intel® Xeon® processor 3000 Series",
    /* 0x00A3 */ u8"Quad-Core Intel® Xeon® processor 5300 Series",
    /* 0x00A4 */ u8"Dual-Core Intel® Xeon® processor 5100 Series",
    /* 0x00A5 */ u8"Dual-Core Intel® Xeon® processor 5000 Series",
    /* 0x00A6 */ u8"Dual-Core Intel® Xeon® processor LV",
    /* 0x00A7 */ u8"Dual-Core Intel® Xeon® processor ULV",
    /* 0x00A8 */ u8"Dual-Core Intel® Xeon® processor 7100 Series",
    /* 0x00A9 */ u8"Quad-Core Intel® Xeon® processor 5400 Series",
    /* 0x00AA */ u8"Quad-Core Intel® Xeon® processor",
    /* 0x00AB */ u8"Dual-Core Intel® Xeon® processor 5200 Series",
    /* 0x00AC */ u8"Dual-Core Intel® Xeon® processor 7200 Series",
    /* 0x00AD */ u8"Quad-Core Intel® Xeon® processor 7300 Series",
    /* 0x00AE */ u8"Quad-Core Intel® Xeon® processor 7400 Series",
    /* 0x00AF */ u8"Multi-Core Intel® Xeon® processor 7400 Series",
    /* 0x00B0 */ u8"Pentium® III Xeon™ processor",
    /* 0x00B1 */ u8"Pentium® III Processor with Intel® SpeedStep™ Technology",
    /* 0x00B2 */ u8"Pentium® 4 Processor",
    /* 0x00B3 */ u8"Intel® Xeon® processor",
    /* 0x00B4 */ u8"AS400 Family",
    /* 0x00B5 */ u8"Intel® Xeon™ processor MP",
    /* 0x00B6 */ u8"AMD Athlon™ XP Processor Family",
    /* 0x00B7 */ u8"AMD Athlon™ MP Processor Family",
    /* 0x00B8 */ u8"Intel® Itanium® 2 processor",
    /* 0x00B9 */ u8"Intel® Pentium® M processor",
    /* 0x00BA */ u8"Intel® Celeron® D processor",
    /* 0x00BB */ u8"Intel® Pentium® D processor",
    /* 0x00BC */ u8"Intel® Pentium® Processor Extreme Edition",
    /* 0x00BD */ u8"Intel® Core™ Solo Processor",
    /* 0x00BE */ u8"Reserved",
    /* 0x00BF */ u8"Intel® Core™ 2 Duo Processor",
    /* 0x00C0 */ u8"Intel® Core™ 2 Solo processor",
    /* 0x00C1 */ u8"Intel® Core™ 2 Extreme processor",
    /* 0x00C2 */ u8"Intel® Core™ 2 Quad processor",
    /* 0x00C3 */ u8"Intel® Core™ 2 Extreme mobile processor",
    /* 0x00C4 */ u8"Intel® Core™ 2 Duo mobile processor",
    /* 0x00C5 */ u8"Intel® Core™ 2 Solo mobile processor",
    /* 0x00C6 */ u8"Intel® Core™ i7 processor",
    /* 0x00C7 */ u8"Dual-Core Intel® Celeron® processor",
    /* 0x00C8 */ u8"IBM390 Family",
    /* 0x00C9 */ u8"G4",
    /* 0x00CA */ u8"G5",
    /* 0x00CB */ u8"ESA/390 G6",
    /* 0x00CC */ u8"z/Architecture base",
    /* 0x00CD */ u8"Intel® Core™ i5 processor",
    /* 0x00CE */ u8"Intel® Core™ i3 processor",
    /* 0x00CF */ u8"[Available for assignment]",
    /* 0x00D0 */ u8"[Available for assignment]",
    /* 0x00D1 */ u8"[Available for assignment]",
    /* 0x00D2 */ u8"VIA C7™-M Processor Family",
    /* 0x00D3 */ u8"VIA C7™-D Processor Family",
    /* 0x00D4 */ u8"VIA C7™ Processor Family",
    /* 0x00D5 */ u8"VIA Eden™ Processor Family",
    /* 0x00D6 */ u8"Multi-Core Intel® Xeon® processor",
    /* 0x00D7 */ u8"Dual-Core Intel® Xeon® processor 3xxx Series",
    /* 0x00D8 */ u8"Quad-Core Intel® Xeon® processor 3xxx Series",
    /* 0x00D9 */ u8"VIA Nano™ Processor Family",
    /* 0x00DA */ u8"Dual-Core Intel® Xeon® processor 5xxx Series",
    /* 0x00DB */ u8"Quad-Core Intel® Xeon® processor 5xxx Series",
    /* 0x00DC */ u8"[Available for assignment]",
    /* 0x00DD */ u8"Dual-Core Intel® Xeon® processor 7xxx Series",
    /* 0x00DE */ u8"Quad-Core Intel® Xeon® processor 7xxx Series",
    /* 0x00DF */ u8"Multi-Core Intel® Xeon® processor 7xxx Series",
    /* 0x00E0 */ u8"Multi-Core Intel® Xeon® processor 3400 Series"
    /* 0x00E1 */ u8"[Available for assignment]",
    /* 0x00E2 */ u8"[Available for assignment]",
    /* 0x00E3 */ u8"[Available for assignment]",
    /* 0x00E4 */ u8"AMD Opteron™ 3000 Series Processor",
    /* 0x00E5 */ u8"AMD Sempron™ II Processor",
    /* 0x00E6 */ u8"Embedded AMD Opteron™ Quad-Core Processor Family",
    /* 0x00E7 */ u8"AMD Phenom™ Triple-Core Processor Family",
    /* 0x00E8 */ u8"AMD Turion™ Ultra Dual-Core Mobile Processor Family",
    /* 0x00E9 */ u8"AMD Turion™ Dual-Core Mobile Processor Family",
    /* 0x00EA */ u8"AMD Athlon™ Dual-Core Processor Family",
    /* 0x00EB */ u8"AMD Sempron™ SI Processor Family",
    /* 0x00EC */ u8"AMD Phenom™ II Processor Family",
    /* 0x00ED */ u8"AMD Athlon™ II Processor Family",
    /* 0x00EE */ u8"Six-Core AMD Opteron™ Processor Family",
    /* 0x00EF */ u8"AMD Sempron™ M Processor Family",
    /* 0x00F0 */ u8"[Available for assignment]",
    /* 0x00F1 */ u8"[Available for assignment]",
    /* 0x00F2 */ u8"[Available for assignment]",
    /* 0x00F3 */ u8"[Available for assignment]",
    /* 0x00F4 */ u8"[Available for assignment]",
    /* 0x00F5 */ u8"[Available for assignment]",
    /* 0x00F6 */ u8"[Available for assignment]",
    /* 0x00F7 */ u8"[Available for assignment]",
    /* 0x00F8 */ u8"[Available for assignment]",
    /* 0x00F9 */ u8"[Available for assignment]",
    /* 0x00FA */ u8"i860",
    /* 0x00FB */ u8"i960",
    /* 0x00FC */ u8"[Available for assignment]",
    /* 0x00FD */ u8"[Available for assignment]",
    /* 0x00FE */ u8"Indicator to obtain the processor family from the Processor Family 2 field",
    /* 0x00FF */ u8"Reserved",
    /* 0x0100 */ u8"[Available for assignment]",
    /* 0x0101 */ u8"[Available for assignment]",
    /* 0x0102 */ u8"[Available for assignment]",
    /* 0x0103 */ u8"[Available for assignment]",
    /* 0x0104 */ u8"SH-3",
    /* 0x0105 */ u8"SH-4",
    /* 0x0106 */ u8"[Available for assignment]",
    /* 0x0107 */ u8"[Available for assignment]",
    /* 0x0108 */ u8"[Available for assignment]",
    /* 0x0109 */ u8"[Available for assignment]",
    /* 0x010A */ u8"[Available for assignment]",
    /* 0x010B */ u8"[Available for assignment]",
    /* 0x010C */ u8"[Available for assignment]",
    /* 0x010D */ u8"[Available for assignment]",
    /* 0x010E */ u8"[Available for assignment]",
    /* 0x010F */ u8"[Available for assignment]",
    /* 0x0110 */ u8"[Available for assignment]",
    /* 0x0111 */ u8"[Available for assignment]",
    /* 0x0112 */ u8"[Available for assignment]",
    /* 0x0113 */ u8"[Available for assignment]",
    /* 0x0114 */ u8"[Available for assignment]",
    /* 0x0115 */ u8"[Available for assignment]",
    /* 0x0116 */ u8"[Available for assignment]",
    /* 0x0117 */ u8"[Available for assignment]",
    /* 0x0118 */ u8"ARM",
    /* 0x0110 */ u8"StrongARM",
    /* 0x011A */ u8"[Available for assignment]",
    /* 0x011B */ u8"[Available for assignment]",
    /* 0x011C */ u8"[Available for assignment]",
    /* 0x011D */ u8"[Available for assignment]",
    /* 0x011E */ u8"[Available for assignment]",
    /* 0x011F */ u8"[Available for assignment]",
};


static const char *processor_upgrades[] = {
    u8"",
    u8"Other",
    u8"Unknown",
    u8"Daughter Board",
    u8"ZIF Socket",
    u8"Replaceable Piggy Back",
    u8"None",
    u8"LIF Socket",
    u8"Slot 1",
    u8"Slot 2",
    u8"370-pin socket",
    u8"Slot A",
    u8"Slot M",
    u8"Socket 423",
    u8"Socket A (Socket 462)",
    u8"Socket 478",
    u8"Socket 754",
    u8"Socket 940",
    u8"Socket 939",
    u8"Socket mPGA604",
    u8"Socket LGA771",
    u8"Socket LGA775",
    u8"Socket S1",
    u8"Socket AM2",
    u8"Socket F (1207)",
    u8"Socket LGA1366",
    u8"Socket G34",
    u8"Socket AM3",
    u8"Socket C32",
    u8"Socket LGA1156",
    u8"Socket LGA1567",
    u8"Socket PGA988A",
    u8"Socket BGA1288",
    u8"Socket rPGA988B",
    u8"Socket BGA1023",
    u8"Socket BGA1224",
    u8"Socket LGA1155",
    u8"Socket LGA1356",
    u8"Socket LGA2011",
    u8"Socket FS1",
    u8"Socket FS2",
    u8"Socket FM1",
    u8"Socket FM2",
    u8"Socket LGA2011-3",
    u8"Socket LGA1356-3",
    u8"Socket LGA1150",
    u8"Socket BGA1168",
    u8"Socket BGA1234",
    u8"Socket BGA1364"
};


static const char *processor_characteristics[] = {
    u8"Reserved",
    u8"Unknown",
    u8"64-bit Capable",
    u8"Multi-Core",
    u8"Hardware Thread",
    u8"Execute Protection",
    u8"Enhanced Virtualization",
    u8"Power/Performance Control",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved"
};


static const char *memory_controller_error_detection_methods[] = {
    u8"",
    u8"Other",
    u8"Unknown",
    u8"None",
    u8"8-bit Parity",
    u8"32-bit ECC",
    u8"64-bit ECC",
    u8"128-bit ECC",
    u8"CRC"
};


static const char *memory_controller_error_correction_capabilities[] = {
    u8"Other",
    u8"Unknown",
    u8"None",
    u8"Single-Bit Error Correcting",
    u8"Double-Bit Error Correcting",
    u8"Error Scrubbing"
};


static const char *memory_controller_interleaves[] = {
    u8"",
    u8"Other",
    u8"Unknown",
    u8"One-Way Interleave",
    u8"Two-Way Interleave",
    u8"Four-Way Interleave",
    u8"Eight-Way Interleave",
    u8"Sixteen-Way Interleave",
};


static const char *memory_controller_speeds[] = {
    u8"Other",
    u8"Unknown",
    u8"70ns",
    u8"60ns",
    u8"50ns",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved"
};


static const char *memory_module_types[] = {
    u8"Other",
    u8"Unknown",
    u8"Standard",
    u8"Fast Page Mode",
    u8"EDO",
    u8"Parity",
    u8"ECC",
    u8"SIMM",
    u8"DIMM",
    u8"Burst EDO",
    u8"SDRAM",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved"
};


static const char *cache_sram_types[] = {
    u8"Other",
    u8"Unknown",
    u8"Non-Burst",
    u8"Burst",
    u8"Pipeline Burst",
    u8"Synchronous",
    u8"Asynchronous",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved"
};


static const char *cache_error_correction_types[] = {
    u8"",
    u8"Other",
    u8"Unknown",
    u8"None",
    u8"Parity",
    u8"Single-bit ECC",
    u8"Multi-bit ECC"
};


static const char *cache_types[] = {
    u8"",
    u8"Other",
    u8"Unknown",
    u8"Instruction",
    u8"Data",
    u8"Unified"
};


static const char *cache_associativities[] = {
    u8"",
    u8"Other",
    u8"Unknown",
    u8"Direct Mapped",
    u8"2-way Set-Associative",
    u8"4-way Set-Associative",
    u8"Fully Associative",
    u8"8-way Set-Associative",
    u8"16-way Set-Associative",
    u8"12-way Set-Associative",
    u8"24-way Set-Associative",
    u8"32-way Set-Associative",
    u8"48-way Set-Associative",
    u8"64-way Set-Associative",
    u8"20-way Set-Associative"
};


//static const char *MEMORY_ARRAY_LOCATIONS[] = {
//    "",
//1    "Other",
//2    "Unknown",
//3    "System board or motherboard",
//4    "ISA add - on card",
//5    "EISA add - on card",
//6    "PCI add - on card",
//7    "MCA add - on card",
//8    "PCMCIA add - on card",
//9    "Proprietary add - on card",
//A    "NuBus",
//a0    "PC - 98 / C20 add - on card",
//a1   "PC - 98 / C24 add - on card",
//a2    "PC - 98 / E add - on card",
// a3   "PC - 98 / Local bus add - on card"
//};


static const char *memory_array_uses[] = {
    u8"",
    u8"Other",
    u8"Unknown",
    u8"System memory",
    u8"Video memory",
    u8"Flash memory",
    u8"Non-volatile RAM",
    u8"Cache memory"
};


static const char *memory_array_error_correction_types[] = {
    u8"",
    u8"Other",
    u8"Unknown",
    u8"None",
    u8"Parity",
    u8"Single-bit ECC",
    u8"Multi-bit ECC",
    u8"CRC"
};


static const char *memory_device_form_factors[] = {
    u8"",
    u8"Other",
    u8"Unknown",
    u8"SIMM",
    u8"SIP",
    u8"Chip",
    u8"DIP",
    u8"ZIP",
    u8"Proprietary Card",
    u8"DIMM",
    u8"TSOP",
    u8"Row of chips",
    u8"RIMM",
    u8"SODIMM",
    u8"SRIMM",
    u8"FB-DIMM"
};


static const char *memory_device_types[] = {
    u8"",
    u8"Other",
    u8"Unknown",
    u8"DRAM",
    u8"EDRAM",
    u8"VRAM",
    u8"SRAM",
    u8"RAM",
    u8"ROM",
    u8"FLASH",
    u8"EEPROM",
    u8"FEPROM",
    u8"EPROM",
    u8"CDRAM",
    u8"3DRAM",
    u8"SDRAM",
    u8"SGRAM",
    u8"RDRAM",
    u8"DDR",
    u8"DDR2",
    u8"DDR2 FB-DIMM",
    u8"Reserved",
    u8"Reserved",
    u8"Reserved",
    u8"DDR3",
    u8"FBD2",
    u8"DDR4",
    u8"LPDDR",
    u8"LPDDR2",
    u8"LPDDR3",
    u8"LPDDR4"
};


static const char *memory_device_type_details[] = {
    u8"Reserved",
    u8"Other",
    u8"Unknown",
    u8"Fast-paged",
    u8"Static column",
    u8"Pseudo-static",
    u8"RAMBUS",
    u8"Synchronous",
    u8"CMOS",
    u8"EDO",
    u8"Window DRAM",
    u8"Cache DRAM",
    u8"Non-volatile",
    u8"Registered (Buffered)",
    u8"Unbuffered (Unregistered)",
    u8"LRDIMM"
};


static const char *memory_error_types[] = {
    u8"",
    u8"Other",
    u8"Unknown",
    u8"OK",
    u8"Bad read",
    u8"Parity error",
    u8"Single-bit error",
    u8"Double-bit error",
    u8"Multi-bit error",
    u8"Nibble error",
    u8"Checksum error",
    u8"CRC error",
    u8"Corrected single-bit error",
    u8"Corrected error",
    u8"Uncorrectable error"
};


static const char *memory_error_granularities[] = {
    u8"",
    u8"Other",
    u8"Unknown",
    u8"Device level",
    u8"Memory partition level"
};


static const char *memory_error_operations[] = {
    u8"",
    u8"Other",
    u8"Read",
    u8"Write",
    u8"Partial Write"
};


/// <summary>
/// Lookup a string value from one of the static tables above.
/// </summary>
template<class TChar, std::size_t N>
static std::string lookup_string(
        _In_ const std::size_t index,
        _In_reads_(N) const TChar(&values)[N]) {
    if (index < N) {
        if (*values[index] == 0) {
            return std::to_string(index);
        }

        std::string retval(values[index]);
        retval += " (";
        retval += std::to_string(index);
        retval += ")";
        return retval;

    } else {
        return std::to_string(index);
    }
}


/// <summary>
/// Validates the SMBIOS checksum of a buffer.
/// </summary>
static bool validate_checksum(_In_reads_bytes_(cnt) const std::uint8_t *buffer,
        _In_ const std::size_t cnt) noexcept {
    assert(buffer != nullptr);
    std::uint8_t sum = 0;

    for (std::size_t i = 0; i < cnt; ++i) {
        sum += buffer[i];
    }

    return (sum == 0);
}

/// <summary>
/// Check whether <paramref name="version" /> is at least
/// <paramref name="major"/>.<paramref name="minor"/>.
/// </summary>
static bool version_at_least(
        _In_ const LYRA_NAMESPACE::smbios::data::version_type version,
        _In_ const LYRA_NAMESPACE::smbios::byte_type major,
        _In_ const LYRA_NAMESPACE::smbios::byte_type minor) {
    if (version.first > major) {
        return true;
    }

    if (version.first == major) {
        return (version.second >= minor);
    }

    return false;
}


/// <summary>
/// Adds a string property from an SMBIOS structure named <c>info</c> to a
/// property set named <c>ps</c>.
/// </summary>
#define _LYRA_ADD_STRING_PROP(n, f)\
    {\
        auto v = get_string(info, info->f);\
        if (v != nullptr) {\
            ps.add(n::name, v);\
        }\
    }\


/// <summary>
/// Constructs a set of SMBIOS information for the specified entry type from a
/// local <c>data</c> variable.
/// </summary>
#define _LYRA_ADD_SMBIOS_PROP(n, e)\
    {\
        std::vector<property_set> sets;\
        std::vector<const e##_type *> values;\
        data.entries_of_type<e##_type>(std::back_inserter(values));\
        for (auto v : values) {\
            sets.push_back(get_##e(v, data.version()));\
        }\
        ps.add(n::name, std::move(sets));\
    }


/*
 * LYRA_NAMESPACE::smbios::data::~data
 */
LYRA_NAMESPACE::smbios::data::~data(void) noexcept {
    delete[] this->_data;
}


/*
 * LYRA_NAMESPACE::smbios::data::operator =
 */
LYRA_NAMESPACE::smbios::data& LYRA_NAMESPACE::smbios::data::operator =(
        _Inout_ data&& rhs) noexcept {
    if (this != std::addressof(rhs)) {
        this->_begin = rhs._begin;
        delete[] this->_data;
        this->_data = rhs._data;
        rhs._data = nullptr;
        this->_end = rhs._end;
        this->_flags = rhs._flags;
        this->_size = rhs._size;
        this->_version = rhs._version;
    }

    return *this;
}


/*
 * LYRA_NAMESPACE::smbios::read
 */
LYRA_NAMESPACE::smbios::data LYRA_NAMESPACE::smbios::read(void) {
    data retval;
    assert(retval._flags == 0);

#if defined(_WIN32)
#pragma pack(push)
#pragma pack(1)
    // See https://msdn.microsoft.com/en-us/library/ms724379(VS.85).aspx
    struct RawSMBIOSData {
        BYTE Used20CallingMethod;
        BYTE SMBIOSMajorVersion;
        BYTE SMBIOSMinorVersion;
        BYTE DmiRevision;
        DWORD Length;
        //BYTE SMBIOSTableData[];
    };
#pragma pack(pop)

    // Determine required buffer size.
    {
        auto status = ::GetSystemFirmwareTable('RSMB', 0, nullptr, 0);
        if (status == 0) {
            const auto error = ::GetLastError();
            LYRA_TRACE(_T("GetSystemFirmwareTable failed with error code %d."),
                error);
            throw std::system_error(error, std::system_category());
        }
        assert(retval._data== nullptr);
        retval._size = status;
        retval._data = new std::uint8_t[retval._size];
    }

    // Read the data.
    {
        auto status = ::GetSystemFirmwareTable('RSMB', 0, retval._data,
            static_cast<DWORD>(retval._size));
        if (status != retval._size) {
            const auto error = ::GetLastError();
            LYRA_TRACE(_T("GetSystemFirmwareTable failed with error code %d."),
                error);
            throw std::system_error(error, std::system_category());
        }
    }

    // Determine the bounds of the structures.
    retval._begin = sizeof(RawSMBIOSData);
    retval._end = retval._size;

    // Process the header.
    {
        auto header = reinterpret_cast<RawSMBIOSData *>(retval._data);
        retval._version = std::make_pair(header->SMBIOSMajorVersion,
            header->SMBIOSMinorVersion);

        if (header->SMBIOSMajorVersion >= 3) {
            retval._flags |= data::flag_stop_at_eot;
        }
    }

#else /* defined(_WIN32) */
    // https://github.com/lyonel/lshw/blob/master/src/core/dmi.cc
    // Cf. http://git.savannah.gnu.org/cgit/dmidecode.git/tree/dmidecode.c
    std::uint16_t version = 0;

    /* Determine how to handle the data by parsing the entry point. */
    auto ef = detail::read_all_bytes("/sys/firmware/dmi/tables/"
        "smbios_entry_point");

    if ((ef.size() >= 24) && (::memcmp(ef.data(), "_SM3_", 5) == 0)) {
        // SMBIOS 3
        if (!validate_checksum(ef.data(), ef[0x06])) {
            throw std::runtime_error("Checksum error in SMBIOS 3 entry point.");
        }
        retval._version = std::make_pair(ef[0x07], ef[0x08]);
        version = (ef[0x07] << 8) + ef[0x08];
        retval._begin = *reinterpret_cast<uint64_t *>(ef.data() + 0x10);
        retval._end = retval._begin + ef[0x0C];
        retval._flags |= data::flag_stop_at_eot;

    } else if ((ef.size() >= 31) && (::memcmp(ef.data(), "_SM_", 4) == 0)) {
        if (!validate_checksum(ef.data(), ef[0x5])
                || (::memcmp(ef.data() + 0x10, "_DMI_", 5) != 0)
                || !validate_checksum(ef.data() + 0x10, 0x0F)) {
            throw std::runtime_error("Checksum error in SMBIOS entry point.");
        }
        retval._version = std::make_pair(ef[0x06], ef[0x07]);
        version = (ef[0x06] << 8) + ef[0x07];

        // Fix version like GNU dmidecode:
        switch (version) {
            case 0x021F:
            case 0x0221:
                version = 0x0203;
                break;

            case 0x0233:
                version = 0x0206;
        }

        retval._begin = ef[0x18];
        retval._end = retval._begin + ef[0x16];
        auto num = ef[0x1c];

    } else if ((ef.size() >= 15) && (::memcmp(ef.data(), "_DMI_", 5) == 0)) {
        retval._version = std::make_pair(ef[0x0e] & 0xf0, ef[0x0e] & 0x0f);
        version = ((ef[0x0e] & 0xf0) << 4) + (ef[0x0e] & 0x0f);
        retval._begin = ef[0x08];
        retval._end = retval._begin + ef[0x06];
        auto num = ef[0x0c];

    } else {
        throw std::runtime_error("SMBIOS entry point is defective.");
    }

    // Read the table.
    auto table = detail::read_all_bytes("/sys/firmware/dmi/tables/DMI");
    retval._size = table.size();
    retval._data = new std::uint8_t[retval._size];
    std::copy(table.data(), table.data() + table.size(), retval._data);
#endif /* defined(_WIN32) */

    return retval;
}


/*
 * LYRA_NAMESPACE::smbios::get_baseboard_information
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::smbios::get_baseboard_information(
        _In_ const baseboard_information_type *info,
        _In_ const data::version_type smbios_version) {
    detail::property_set_impl ps;
    property_set retval;

    if (info != nullptr) {
        _LYRA_ADD_STRING_PROP(manufacturer, manufacturer);
        _LYRA_ADD_STRING_PROP(product, product_name);
        _LYRA_ADD_STRING_PROP(revision, version);
        _LYRA_ADD_STRING_PROP(serial_number, serial_number);
        _LYRA_ADD_STRING_PROP(asset_tag, asset_tag);
        //byte_type feature_flags;
        _LYRA_ADD_STRING_PROP(location, location_in_chassis);
        //handle_type chassis_handle;
        //byte_type board_type;
        //byte_type number_of_contained_object_handles;
        //handle_type contained_object_handles[255];

    }

    realise(retval, std::move(ps));
    return retval;
}


/*
 * LYRA_NAMESPACE::smbios::get_bios_information
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::smbios::get_bios_information(
        _In_ const bios_information_type *info,
        _In_ const data::version_type smbios_version) {
    typedef LYRA_NAMESPACE::version::version structured_version;
    detail::property_set_impl ps;
    property_set retval;

    if (info != nullptr) {
        _LYRA_ADD_STRING_PROP(vendor, vendor);
        _LYRA_ADD_STRING_PROP(bios_version, version);
        //word_type starting_address_segment;
        _LYRA_ADD_STRING_PROP(release_date, release_date);
        ps.add<rom_size>(64 * 1024 * (info->rom_size + 1));

        if (version_at_least(smbios_version, 2, 1)) {
            //ps.add<characteristics>(info->characteristics);
        }

        if (version_at_least(smbios_version, 2, 4)) {
            //ps.add<extension_bytes1>(info->extension_bytes1);
            //ps.add<extension_bytes2>(info->extension_bytes2);
            ps.add<structured_version>(LYRA_NAMESPACE::version::make(
                info->major_release, info->minor_release));
            ps.add<firmware_version>(LYRA_NAMESPACE::version::make(
                info->firmware_major_release, info->firmware_minor_release));
        }
    }

    realise(retval, std::move(ps));
    return retval;
}


/*
 * LYRA_NAMESPACE::smbios::get_memory_device
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::smbios::get_memory_device(
        _In_ const memory_device_type *info,
        _In_ const data::version_type smbios_version) {
    detail::property_set_impl ps;
    property_set retval;

    if (info != nullptr) {
        if (version_at_least(smbios_version, 2, 1)) {
            //handle_type physical_memory_array_handle;
            //handle_type memory_error_information_handle;
            //word_type total_width;
            //word_type data_width;
            if ((info->size != 0x7fff)
                    && !version_at_least(smbios_version, 2, 7)) {
                ps.add<installed_size>(info->size);
            }
            ps.add(form_factor::name, lookup_string(
                static_cast<std::size_t>(info->type),
                memory_device_form_factors));
            //byte_type device_set;
            _LYRA_ADD_STRING_PROP(socket, device_locator);
            _LYRA_ADD_STRING_PROP(bank, bank_locator);
            ps.add(component_type::name, lookup_string(
                static_cast<std::size_t>(info->type),
                memory_device_types));
            //word_type type_detail;
        }

        if (version_at_least(smbios_version, 2, 3)) {
            //word_type speed;
            _LYRA_ADD_STRING_PROP(manufacturer, manufacturer);
            _LYRA_ADD_STRING_PROP(serial_number, serial_number);
            _LYRA_ADD_STRING_PROP(asset_tag, asset_tag);
            _LYRA_ADD_STRING_PROP(part_number, part_number);
        }

        if (version_at_least(smbios_version, 2, 6)) {
            //word_type attributes;
        }

        if (version_at_least(smbios_version, 2, 7)) {
            ps.add<installed_size>(info->extended_size);
            ps.add<current_speed>(info->configured_memory_clock_speed);
        }

        if (version_at_least(smbios_version, 2, 8)) {
            ps.add<minimum_voltage>(info->minimum_voltage);
            ps.add<maximum_voltage>(info->maximum_voltage);
            ps.add<voltage>(info->configured_voltage);
        }
    }

    realise(retval, std::move(ps));
    return retval;
}


/*
 * LYRA_NAMESPACE::smbios::get_memory_module_information
 */
LYRA_NAMESPACE::property_set
LYRA_NAMESPACE::smbios::get_memory_module_information(
        _In_ const memory_module_information_type *info,
        _In_ const data::version_type smbios_version) {
    detail::property_set_impl ps;
    property_set retval;

    if (info != nullptr) {
        _LYRA_ADD_STRING_PROP(socket, socket_designation);
        //byte_type bank_connections;
        ps.add<current_speed>(info->current_speed);
        //word_type type;
        ps.add<installed_size>(info->installed_size);
        ps.add<enabled_size>(info->enabled_size);
        //byte_type error_status;
    }

    realise(retval, std::move(ps));
    return retval;
}


/*
 * LYRA_NAMESPACE::smbios::get_processor_information
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::smbios::get_processor_information(
        _In_ const processor_information_type *info,
        _In_ const data::version_type smbios_version) {
    detail::property_set_impl ps;
    property_set retval;

    if (info != nullptr) {
        _LYRA_ADD_STRING_PROP(socket, socket_designation);
        //byte_type type;
        //byte_type family;
        _LYRA_ADD_STRING_PROP(manufacturer, manufacturer);
        ps.add<cpuid>(info->cpuid);
        _LYRA_ADD_STRING_PROP(cpu_version, version);
        ps.add<voltage>(info->voltage);
        ps.add<external_clock>(info->external_clock);
        ps.add<maximum_speed>(info->max_speed);
        ps.add<current_speed>(info->current_speed);
        //byte_type status;
        //byte_type upgrade;

        if (version_at_least(smbios_version, 2, 1)) {
            //ps.add<l1_cache_handle>(info->l1_cache_handle);
            //ps.add<l2_cache_handle>(info->l2_cache_handle);
            //ps.add<l3_cache_handle>(info->l3_cache_handle);
        }

        if (version_at_least(smbios_version, 2, 3)) {
            _LYRA_ADD_STRING_PROP(serial_number, serial_number);
            _LYRA_ADD_STRING_PROP(asset_tag, asset_tag);
            _LYRA_ADD_STRING_PROP(part_number, part_number);
        }

        if (version_at_least(smbios_version, 2, 5)
                && !version_at_least(smbios_version, 3, 0)) {
            // If possible, use the better fields from SMBIOS 3.
            ps.add<installed_cores>(info->core_count);
            ps.add<enabled_cores>(info->core_enabled);
            ps.add<installed_threads>(info->thread_count);
        }

        if (version_at_least(smbios_version, 2, 5)) {
            //ps.add<characteristics>(info->characteristics);
        }

        if (version_at_least(smbios_version, 2, 6)) {
            //ps.add<family2>(info->family2);
        }

        if (version_at_least(smbios_version, 3, 0)) {
            ps.add<installed_cores>(info->core_count2);
            ps.add<enabled_cores>(info->core_enabled2);
            ps.add<installed_threads>(info->thread_count2);
        }
    }

    realise(retval, std::move(ps));
    return retval;
}


/*
 * LYRA_NAMESPACE::smbios::get_smbios
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::smbios::get_smbios(void) {
    detail::property_set_impl ps;
    property_set retval;

    try {
        const auto data = read();

        // Add the SMBIOS version.
        ps.add<LYRA_NAMESPACE::version::version>(LYRA_NAMESPACE::version::make(
            data.version().first, data.version().second));

        //case bios_information_type::id:
        //case system_information_type::id:
        //case baseboard_information_type::id:
        //case chassis_information_type::id:
        //case processor_information_type::id:
        //case memory_controller_information_type::id:
        //case memory_module_information_type::id:
        //case cache_information_type::id:
        //case physical_memory_array_type::id:
        //case memory_device_type::id:
        //case memory_error_information_type::id:

        _LYRA_ADD_SMBIOS_PROP(bios, bios_information);
        _LYRA_ADD_SMBIOS_PROP(baseboard, baseboard_information);
        _LYRA_ADD_SMBIOS_PROP(cpu, processor_information);
        _LYRA_ADD_SMBIOS_PROP(memory_device, memory_device);
        _LYRA_ADD_SMBIOS_PROP(memory_module, memory_module_information);

    } catch (... ) {
        LYRA_TRACE(_T("An error occurred while retrieving SMBIOS ")
            _T("information."));
    }

    realise(retval, std::move(ps));
    return retval;
}
