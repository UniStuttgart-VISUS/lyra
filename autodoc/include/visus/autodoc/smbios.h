// <copyright file="smbios.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_SMBIOS_H)
#define _LYRA_SMBIOS_H
#pragma once

#include <array>
#include <cassert>
#include <cinttypes>
#include <cstdlib>
#include <utility>

#include "visus/autodoc/collection_flags.h"
#include "visus/autodoc/property_set.h"
#include "visus/autodoc/result_type.h"
#include "visus/autodoc/trace.h"


LYRA_NAMESPACE_BEGIN

namespace smbios {

    /// <summary>
    /// Represents a byte in SMBIOS data.
    /// </summary>
    typedef std::uint8_t byte_type;

    /// <summary>
    /// Represents a DWORD in SMBIOS data.
    /// </summary>
    typedef std::uint32_t dword_type;

    /// <summary>
    /// Represents a handle in SMBIOS data.
    /// </summary>
    typedef std::uint16_t handle_type;

    /// <summary>
    /// Represents a QWORD in SMBIOS data.
    /// </summary>
    typedef std::uint64_t qword_type;

    /// <summary>
    /// Represents an (indexed) string in SMBIOS data.
    /// </summary>
    typedef std::uint8_t string_type;

    /// <summary>
    /// Represents a WORD in SMBIOS data.
    /// </summary>
    typedef std::uint16_t word_type;

    // See http://www.dmtf.org/sites/default/files/standards/documents/DSP0134_3.0.0.pdf
#pragma pack(push)
#pragma pack(1)

    /// <summary>
    /// The most basic DMI entry point structure we support.
    /// </summary>
    struct LYRA_API dmi_entry_point_type {
        byte_type anchor[5];
        byte_type checksum;
        word_type table_length;
        dword_type table_address;
        word_type table_num_structs;
        byte_type bcd_revision;
    };

    /// <summary>
    /// The SMBIOS 2.1 entry point structure.
    /// </summary>
    struct LYRA_API smbios_2_1_entry_point_type {
        byte_type anchor[4];
        byte_type checksum;
        byte_type struct_size;
        byte_type major_version;
        byte_type minor_version;
        word_type max_struct_size;
        byte_type revision;
        byte_type formatted_area[5];
        byte_type intermediate_anchor[5];
        byte_type intermediate_checksum;
        word_type table_length;
        dword_type table_address;
        word_type table_num_structs;
        byte_type bcd_revision;
        byte_type _padding;
    };

    /// <summary>
    /// The SMBIOS 3.0 entry point structure.
    /// </summary>
    struct LYRA_API smbios_3_0_entry_point_type {
        byte_type anchor[5];
        byte_type checksum;
        byte_type struct_size;
        byte_type major_version;
        byte_type minor_version;
        byte_type docrev;
        byte_type revision;
        byte_type _reserved;
        dword_type max_struct_size;
        qword_type table_address;
    };

    /// <summary>
    /// The SMBIOS structure header, which precedes all SMBIOS structures.
    /// </summary>
    struct LYRA_API header_type {
        byte_type type;
        byte_type length;
        handle_type handle;
    };

    /// <summary>
    /// The required BIOS information structure (type 0).
    /// </summary>
    struct LYRA_API bios_information_type {
        static constexpr smbios::byte_type id = 0;
        header_type header;
        string_type vendor;
        string_type version;
        word_type starting_address_segment;
        string_type release_date;
        byte_type rom_size;       // 64KB * (rom_size + 1) is actual size.
        /* SMBIOS 2.1+ */
        qword_type characteristics;
        /* SMBIOS 2.4+ */
        byte_type extension_bytes1;
        byte_type extension_bytes2;
        byte_type major_release;
        byte_type minor_release;
        byte_type firmware_major_release;
        byte_type firmware_minor_release;
    };

    /// <summary>
    /// The required system information structure (type 1).
    /// </summary>
    struct LYRA_API system_information_type {
        static constexpr smbios::byte_type id = 1;
        header_type header;
        string_type manufacturer;
        string_type product_name;
        string_type version;
        string_type serial_number;
        /* SMBIOS 2.1+ */
        byte_type uuid[16];
        byte_type wake_up_type;
        /* SMBIOS 2.4+ */
        byte_type sku_number;
        byte_type family;
    };

    /// <summary>
    /// Information about the mainboard (type 2).
    /// </summary>
    struct LYRA_API baseboard_information_type {
        static constexpr smbios::byte_type id = 2;
        header_type header;
        string_type manufacturer;
        string_type product_name;
        string_type version;
        string_type serial_number;
        string_type asset_tag;
        byte_type feature_flags;
        string_type location_in_chassis;
        handle_type chassis_handle;
        byte_type board_type;
        byte_type number_of_contained_object_handles;
        handle_type contained_object_handles[255];
    };

    /// <summary>
    /// Information about the chassis (type 3).
    /// </summary>
    struct LYRA_API chassis_information_type {
        static constexpr smbios::byte_type id = 3;
        header_type header;
        string_type manufacturer;
        string_type type;
        string_type version;
        string_type serial_number;
        string_type asset_tag;
        /* SMBIOS 2.1+ */
        byte_type bootup_state;
        byte_type power_supply_state;
        byte_type thermal_state;
        byte_type security_status;
        /* SMBIOS 2.3+ */
        dword_type oem_defined;
        byte_type height;
        byte_type number_of_power_cords;
        byte_type contained_element_count;
        byte_type contained_element_record_length;
        byte_type contained_elements[255 * 255];
    };

    /// <summary>
    /// The required processor information structure (type 4).
    /// </summary>
    struct LYRA_API processor_information_type {
        static const smbios::byte_type id = 4;
        header_type header;
        string_type socket_designation;
        byte_type type;
        byte_type family;
        string_type manufacturer;
        qword_type cpuid;
        string_type version;
        byte_type voltage;
        word_type external_clock;
        word_type max_speed;
        word_type current_speed;
        byte_type status;
        byte_type upgrade;
        /* SMBIOS 2.1+ */
        handle_type l1_cache_handle;
        handle_type l2_cache_handle;
        handle_type l3_cache_handle;
        /* SMBIOS 2.3+ */
        string_type serial_number;
        string_type asset_tag;
        string_type part_number;
        /* SMBIOS 2.5+ */
        byte_type core_count;
        byte_type core_enabled;
        byte_type thread_count;
        word_type characteristics;
        /* SMBIOS 2.6+ */
        word_type family2;
        /* SMBIOS 3.0+ */
        word_type core_count2;
        word_type core_enabled2;
        word_type thread_count2;
    };

    struct LYRA_API memory_controller_information_type {
        static const smbios::byte_type id = 5;
        header_type header;
        byte_type error_detecting_method;
        byte_type error_correction_capability;
        byte_type supported_interleave;
        byte_type current_interleave;
        byte_type maximum_memory_module_size; // 2^n is the maximum size in MB.
        word_type supported_speeds;
        word_type supported_memory_types;
        byte_type memory_module_voltage;
        byte_type number_of_associated_memory_slots;          // x
        handle_type memory_module_configuration_handles[1]; // (2x -1)
        /* SMBIOS 2.1+ */
        //byte_type enabled_error_correcting_capabilities;
    };

    /// <summary>
    /// Information about a memory module (type 6).
    /// </summary>
    struct LYRA_API memory_module_information_type {
        static const smbios::byte_type id = 6;
        header_type header;
        string_type socket_designation;
        byte_type bank_connections;
        byte_type current_speed;
        word_type type;
        byte_type installed_size;
        byte_type enabled_size;
        byte_type error_status;
    };

    /// <summary>
    /// The required cache information structure (type 7).
    /// </summary>
    struct LYRA_API cache_information_type {
        static const smbios::byte_type id = 7;
        header_type header;
        string_type socket_designation;
        word_type configuration;
        word_type maximum_size;         // Bit 15 == 0: 1K; == 1: 64K
        word_type installed_size;       // Bit 15 == 0: 1K; == 1: 64K
        word_type supported_sram_type;
        word_type current_sram_type;
        byte_type speed;
        byte_type error_correction_type;
        byte_type system_cache_type;
        byte_type associativity;
    };

    struct LYRA_API physical_memory_array_type {
        static const smbios::byte_type id = 16;
        header_type header;
        /* SMBIOS 2.1+ */
        byte_type location;
        byte_type use;
        byte_type memory_error_correction;
        dword_type maximum_capacity;
        handle_type memory_error_information_handle;
        word_type number_of_memory_devices;
        /* SMBIOS 2.7+ */
        qword_type extended_maximum_capacity;
    };

    /// <summary>
    /// The required memory device structure (type 17).
    /// </summary>
    struct LYRA_API memory_device_type {
        static const smbios::byte_type id = 17;
        header_type header;
        /* SMBIOS 2.1+ */
        handle_type physical_memory_array_handle;
        handle_type memory_error_information_handle;
        word_type total_width;
        word_type data_width;
        word_type size;     // 0: not installed,  7FFFh: use extended_size
        byte_type form_factor;
        byte_type device_set;
        string_type device_locator;
        string_type bank_locator;
        byte_type type;
        word_type type_detail;
        /* SMBIOS 2.3+ */
        word_type speed;
        string_type manufacturer;
        string_type serial_number;
        string_type asset_tag;
        string_type part_number;
        /* SMBIOS 2.6+ */
        byte_type attributes;
        /* SMBIOS 2.7+ */
        dword_type extended_size;
        word_type configured_memory_clock_speed;
        /* SMBIOS 2.8+ */
        word_type minimum_voltage;
        word_type maximum_voltage;
        word_type configured_voltage;
    };

    struct LYRA_API memory_error_information_type {
        static const smbios::byte_type id = 18;
        header_type header;
        /* SMBIOS 2.1+ */
        byte_type type;
        byte_type granularity;
        byte_type operation;
        dword_type vendor_syndrome;
        dword_type memory_array_error_address;
        dword_type device_error_address;
        dword_type resolution;
    };
#pragma pack(pop)


    /// <summary>
    /// A container for the raw SMBIOS data, which also provides rudimentary
    /// parsing capabilities.
    /// </summary>
    class LYRA_API data final {

    public:

        /// <summary>
        /// The type to represent the SMBIOS version.
        /// </summary>
        typedef std::pair<byte_type, byte_type> version_type;

        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        inline data(void) noexcept
            : _begin(0),
            _data(nullptr),
            _end(0),
            _flags(0),
            _size(0),
            _version(0, 0) { }

        /// <summary>
        /// Initialisation from move.
        /// </summary>
        /// <param name="other">The object ot be moved.</param>
        inline data(_Inout_ data&& other) noexcept
                : _begin(other._begin),
                _data(other._data),
                _end(other._end),
                _flags(other._flags),
                _size(other._size),
                _version(other._version) {
            other._data = nullptr;
        }

        /// <summary>
        /// Finalises the instance.
        /// </summary>
        ~data(void) noexcept;

        /// <summary>
        /// Invokes <paramref name="callback" /> for each entry in the SMBIOS
        /// data until the callback returns <see langword="false "/>.
        /// </summary>
        /// <typeparam name="TCallback"></typeparam>
        /// <param name="callback"></param>
        /// <returns></returns>
        template<class TCallback>
        std::size_t each_entry(_In_ TCallback callback) const;

        /// <summary>
        /// Retrieves all SMBIOS entries fulfilling the given predicate.
        /// </summary>
        /// <typeparam name="TType">The type of the entry, which should be
        /// <see cref="header_type" /> if the predicate does not limit the
        /// result set to a more specific type that can be safely casted. Note
        /// that the conversion of the output data is done after the predicate
        /// was evaluated.</typeparam>
        /// <typeparam name="TIterator">The output iterator type for pointers to
        /// <typeparamref name="TType" />.</typeparam>
        /// <typeparam name="TPredicate">The type of the predicate checking the
        /// entries, which on <typeparamref name="header_type" /> references.
        /// </typeparam>
        /// <param name="oit">An output iterator receiving pointers to the
        /// entries.</param>
        /// <param name="pred">The predicate deciding based on the header
        /// whether an item should be returned.</param>
        /// <returns>The number of entries returned.</returns>
        template<class TType, class TIterator, class TPredicate>
        std::size_t entries(_In_ TIterator oit, _In_ TPredicate pred) const;

        /// <summary>
        /// Retrieves all SMBIOS entries.
        /// </summary>
        /// <typeparam name="TIterator">The output iterator for pointers to
        /// <see cref="header_type" />.</typeparam>
        /// <param name="oit">The output iterator receiving the entries.</param>
        /// <returns>The number of entries returned.</returns>
        template<class TIterator>
        inline std::size_t entries(_In_ TIterator&& oit) const {
            return this->entries<header_type>(
                std::forward<TIterator>(oit),
                [](const header_type&) { return true; });
        }

        /// <summary>
        /// Retrieves all SMBIOS entries of type<typeparamref name="TType" />.
        /// </summary>
        /// <typeparam name="TType">The type of the entries to be retrieved.
        /// </typeparam>
        /// <tparam name="TIterator">The output iterator type for pointers
        /// to <typeparamref name="TType" />.</tparam>
        /// <param name="oit">An output iterator for
        /// <c>const <tparamref name="TType" /> *</c>.</param>
        /// <returns>The number of entries returned.</returns>
        template<class TType, class TIterator>
        std::size_t entries_of_type(_In_ TIterator&& oit) const;

        /// <summary>
        /// Gts teh first SMBIOS entry of type <typeparamref name="TType" />.
        /// </summary>
        /// <typeparam name="TType">The type of the entry to be retrieved.
        /// </typeparam>
        /// <returns>A pointer to the entry or <see langword="nullptr" /> if no
        /// such entry was found.</returns>
        template<class TType> _Ret_maybenull_ const TType *get(void) const {
            std::array<const TType *, 1> retval { nullptr };
            this->entries<TType>(
                retval.begin(),
                [&retval](const header_type&) {
                    return (retval.front() == nullptr);
                });
            return retval.front();
        }

        /// <summary>
        /// Answer the version of the SMBIOS data enclosed in this object.
        /// </summary>
        /// <returns>A pair of the major and minor version.</returns>
        inline version_type version(void) const noexcept {
            return this->_version;
        }

        /// <summary>
        /// Move assignment.
        /// </summary>
        /// <param name="rhs">The right-hand-side operand.</param>
        /// <returns><c>*<see langword="this" /></c>.</returns>
        data& operator =(_Inout_ data&& rhs) noexcept;

    private:

        static constexpr std::uint32_t flag_no_file_offset = 0x0001;
        static constexpr std::uint32_t flag_stop_at_eot = 0x0002;

        std::size_t _begin;
        std::uint8_t *_data;
        std::size_t _end;
        std::uint32_t _flags;
        std::size_t _size;
        version_type _version;

        friend LYRA_API result_type read(data& data);
    };

    /// <summary>
    /// Reads the SMBIOS information of the local system and returns the
    /// whole block in a <see cref="data" /> structure.
    /// </summary>
    /// <param name="data">An empty <see cref="data" /> structure to receive the
    /// SMBIOS data..</param>
    /// <returns>Zero in case of success, a system error code otherwise.
    /// </returns>
    LYRA_API result_type read(_Out_ data& data);

    /// <summary>
    /// Identifies a memory bank.
    /// </summary>
    struct bank final {
        typedef const char *type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Bank";
    };

    /// <summary>
    /// Identifies all mainboard-related information from SMBIOS. This property
    /// can be retrieved from the SMBIOS root property set obtained from
    /// <see cref="get_smbios" />.
    /// </summary>
    struct baseboard final {
        typedef property_set type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Baseboard";
    };

    /// <summary>
    /// Identifies an asset tag assigned to the mainboard.
    /// </summary>
    struct baseboard_asset_tag final {
        typedef const char *type;
        static constexpr auto dmi_id = u8"board_asset_tag";
        static constexpr auto is_immutable = true;
        static constexpr auto is_sensitive = true;
        static constexpr auto name = u8"AssetTag";
    };

    /// <summary>
    /// Identifies where the baseboard is located in the chassis.
    /// </summary>
    struct baseboard_location final {
        typedef const char *type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Location";
    };

    /// <summary>
    /// Identifies the name of the baseboard.
    /// </summary>
    struct baseboard_product final {
        typedef const char *type;
        static constexpr auto dmi_id = u8"board_name";
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Product";
    };

    /// <summary>
    /// Identifies the serial number of the baseboard.
    /// </summary>
    struct baseboard_serial final {
        typedef const char *type;
        static constexpr auto dmi_id = u8"board_serial";
        static constexpr auto is_immutable = true;
        static constexpr auto is_sensitive = true;
        static constexpr auto name = u8"Serial Number";
    };

    /// <summary>
    /// Identifies the vendor of the baseboard.
    /// </summary>
    struct baseboard_vendor final {
        typedef const char *type;
        static constexpr auto dmi_id = u8"board_vendor";
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Vendor";
    };

    /// <summary>
    /// Identifies the version of the baseboard.
    /// </summary>
    struct baseboard_version final {
        typedef const char *type;
        static constexpr auto dmi_id = u8"board_version";
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Version";
    };

    /// <summary>
    /// Identifies all BIOS-related information from SMBIOS. This property can
    /// be retrieved from the SMBIOS root property set obtained from
    /// <see cref="get_smbios" />.
    /// </summary>
    struct bios final {
        typedef property_set type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"BIOS";
    };

    /// <summary>
    /// Identifies the release date of the BIOS.
    /// </summary>
    struct bios_date final {
        typedef const char *type;
        static constexpr auto dmi_id = u8"bios_date";
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Release Date";
    };

    /// <summary>
    /// Identifies a firmware version property.
    /// </summary>
    struct bios_firmware_version final {
        typedef property_set type;
        static constexpr auto name = u8"Firmware Version";
    };

    /// <summary>
    /// Identifies the version of the BIOS.
    /// </summary>
    struct bios_release final {
        typedef const char *type;
        static constexpr auto dmi_id = u8"bios_release";
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Release";
    };

    /// <summary>
    /// Identifies the size of the BIOS ROM in bytes.
    /// </summary>
    struct bios_rom_size final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"ROM Size";
    };

    /// <summary>
    /// Identifies the vendor of the BIOS.
    /// </summary>
    struct bios_vendor final {
        typedef const char *type;
        static constexpr auto dmi_id = u8"bios_vendor";
        static constexpr auto is_immutable = true;
        static constexpr auto name = baseboard_vendor::name;
    };

    /// <summary>
    /// Identifies the version of the BIOS.
    /// </summary>
    struct bios_version final {
        typedef const char *type;
        static constexpr auto dmi_id = u8"bios_version";
        static constexpr auto is_immutable = true;
        static constexpr auto name = baseboard_version::name;
    };

    /// <summary>
    /// Groups all information about the chassis.
    /// </summary>
    struct chassis final {
        typedef property_set type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Chassis";
    };

    /// <summary>
    /// Identifies an asset tag assigned to the chassis.
    /// </summary>
    struct chassis_asset_tag final {
        typedef const char *type;
        static constexpr auto dmi_id = u8"board_asset_tag";
        static constexpr auto is_immutable = true;
        static constexpr auto is_sensitive = true;
        static constexpr auto name = baseboard_asset_tag::name;
    };

    /// <summary>
    /// Identifies the serial number of the chassis.
    /// </summary>
    struct chassis_serial final {
        typedef const char *type;
        static constexpr auto dmi_id = u8"chassis_serial";
        static constexpr auto is_immutable = true;
        static constexpr auto is_sensitive = true;
        static constexpr auto name = baseboard_serial::name;
    };

    /// <summary>
    /// Identifies the type of the chassis.
    /// </summary>
    struct chassis_type final {
        typedef const char *type;
        static constexpr auto dmi_id = u8"chassis_type";
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Type";
    };

    /// <summary>
    /// Identifies the vendor of the chassis.
    /// </summary>
    struct chassis_vendor final {
        typedef const char *type;
        static constexpr auto dmi_id = u8"chassis_vendor";
        static constexpr auto is_immutable = true;
        static constexpr auto name = baseboard_vendor::name;
    };

    /// <summary>
    /// Identifies the version of the chassis.
    /// </summary>
    struct chassis_version final {
        typedef const char *type;
        static constexpr auto dmi_id = u8"chassis_version";
        static constexpr auto is_immutable = true;
        static constexpr auto name = baseboard_version::name;
    };

    /// <summary>
    /// Identifies all CPU-related information from SMBIOS. This property can
    /// be retrieved from the SMBIOS root property set obtained from
    /// <see cref="get_smbios" />.
    /// </summary>
    struct cpu final {
        typedef property_set type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"CPU";
    };

    /// <summary>
    /// Identifies an asset tag assigned to the CPU.
    /// </summary>
    struct cpu_asset_tag final {
        typedef const char *type;
        static constexpr auto is_immutable = true;
        static constexpr auto is_sensitive = true;
        static constexpr auto name = baseboard_asset_tag::name;
    };

    /// <summary>
    /// Identifies the CPU characteristics.
    /// </summary>
    struct cpu_characteristics final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Characteristics";
    };

    /// <summary>
    /// Identifies the number of available cores of the CPU.
    /// </summary>
    struct cpu_cores final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Cores";
    };

    /// <summary>
    /// Identifies the number of enables cores of the CPU.
    /// </summary>
    struct cpu_enabled_cores final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Enabled Cores";
    };

    /// <summary>
    /// The external clock of the CPU.
    /// </summary>
    struct cpu_external_clock final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"External Clock";
    };

    /// <summary>
    /// Identifies the unique ID of the CPU.
    /// </summary>
    struct cpu_id final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"ID";
    };

    /// <summary>
    /// The maximum speed of the CPU.
    /// </summary>
    struct cpu_maximum_speed final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Maximum Speed";
    };

    /// <summary>
    /// Identifies the part number of the CPU.
    /// </summary>
    struct cpu_part_number final {
        typedef const char *type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Part Number";
    };

    /// <summary>
    /// Identifies the serial number of the CPU.
    /// </summary>
    struct cpu_serial final {
        typedef const char *type;
        static constexpr auto is_sensitive = true;
        static constexpr auto name = baseboard_serial::name;
    };

    /// <summary>
    /// Identifies the socket designation of the CPU.
    /// </summary>
    struct cpu_socket final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Socket";
    };

    /// <summary>
    /// The current speed of the CPU.
    /// </summary>
    struct cpu_speed final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Current Speed";
    };

    /// <summary>
    /// Identifies the number of available threads, which might be more than
    /// <see cref="cpu_cores" /> if the CPU supports hyper-threading.
    /// </summary>
    struct cpu_threads final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Threads";
    };

    /// <summary>
    /// Identifies a CPU upgrade description.
    /// </summary>
    struct cpu_upgrade final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Upgrade";
    };

    /// <summary>
    /// The vendor of the CPU.
    /// </summary>
    struct cpu_vendor final {
        typedef const char *type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = baseboard_vendor::name;
    };

    /// <summary>
    /// The CPU version, which is basically its name.
    /// </summary>
    struct cpu_version final {
        typedef const char *type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = baseboard_version::name;
    };

    /// <summary>
    /// Identifies a CPU's voltage.
    /// </summary>
    struct cpu_voltage final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Voltage";
    };

    /// <summary>
    /// Identifies an asset tag assigned to a memory module.
    /// </summary>
    struct memory_asset_tag final {
        typedef const char *type;
        static constexpr auto is_immutable = true;
        static constexpr auto is_sensitive = true;
        static constexpr auto name = baseboard_asset_tag::name;
    };

    /// <summary>
    /// Identifies the bank locator of a memory module.
    /// </summary>
    struct memory_bank_locator final {
        typedef const char *type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Bank Locator";
    };

    /// <summary>
    /// Groups all information about a single memory module.
    /// </summary>
    struct memory_device final {
        typedef property_set type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Memory Device";
    };

    /// <summary>
    /// Identifies the device locator of a memory module.
    /// </summary>
    struct memory_device_locator final {
        typedef const char *type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Device Locator";
    };

    /// <summary>
    /// Identifies the usable size of a memory module.
    /// </summary>
    struct memory_enabled_size final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Enabled Size";
    };

    /// <summary>
    /// Identifies the form factor of a component, for instance the type of a
    /// memory module.
    /// </summary>
    struct memory_form_factor final {
        typedef const char *type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Form Factor";
    };

    /// <summary>
    /// The maximum allowed voltage, typically for a memory module.
    /// </summary>
    struct memory_maximum_voltage final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Maximum Voltage";
    };

    /// <summary>
    /// The minimum allowed voltage, typically for a memory module.
    /// </summary>
    struct memory_minimum_voltage final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Minimum Voltage";
    };

    /// <summary>
    /// Groups all information about a single memory module.
    /// </summary>
    struct memory_module final {
        typedef property_set type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Memory Module";
    };

    /// <summary>
    /// Identifies the part number of a memory module.
    /// </summary>
    struct memory_part_number final {
        typedef const char *type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = cpu_part_number::name;
    };

    /// <summary>
    /// Identifies the serial number of the CPU.
    /// </summary>
    struct memory_serial final {
        typedef const char *type;
        static constexpr auto is_immutable = true;
        static constexpr auto is_sensitive = true;
        static constexpr auto name = baseboard_serial::name;
    };

    /// <summary>
    /// Identifies the size of an installed memory module.
    /// </summary>
    struct memory_size final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Size";
    };

    /// <summary>
    /// Identifies the socket designation of a memory module.
    /// </summary>
    struct memory_socket final {
        typedef const char *type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Socket";
    };

    /// <summary>
    /// Identifies the current speed of a memory module.
    /// </summary>
    struct memory_speed final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Speed";
    };

    /// <summary>
    /// Identifies the type of a memory module.
    /// </summary>
    struct memory_type final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Type";
    };

    /// <summary>
    /// Identifies the vendor of a memory module.
    /// </summary>
    struct memory_vendor final {
        typedef const char *type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Vendor";
    };

    /// <summary>
    /// Identifies the memory voltage.
    /// </summary>
    struct memory_voltage final {
        typedef std::uint32_t type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = cpu_voltage::name;
    };

    /// <summary>
    /// Retrieves all SMBIOS information and fills a property set with the
    /// available data.
    /// </summary>
    /// <param name="flags">Allows for in-depth control of how the information
    /// is collected.</param>
    /// <returns>A property set containing all available SMBIOS
    /// information.</returns>
    LYRA_API property_set get(_In_ const collection_flags flags
        = collection_flags::none);

    /// <summary>
    /// Constructs a property set for the given mainboard information.
    /// </summary>
    LYRA_API property_set get_baseboard_information(
        _In_ const baseboard_information_type *info,
        _In_ const data::version_type smbios_version,
        _In_ const collection_flags flags = collection_flags::none);

    /// <summary>
    /// Constructs a property set for the given BIOS information.
    /// </summary>
    LYRA_API property_set get_bios_information(
        _In_ const bios_information_type *info,
        _In_ const data::version_type smbios_version,
        _In_ const collection_flags flags = collection_flags::none);

    /// <summary>
    /// Constructs a property set for the given chassis information.
    /// </summary>
    LYRA_API property_set get_chassis_information(
        _In_ const chassis_information_type *info,
        _In_ const data::version_type smbios_version,
        _In_ const collection_flags flags = collection_flags::none);

    /// <summary>
    /// Constructs a property set for the given memory-device information.
    /// </summary>
    LYRA_API property_set get_memory_device(
        _In_ const memory_device_type *info,
        _In_ const data::version_type smbios_version,
        _In_ const collection_flags flags = collection_flags::none);

    /// <summary>
    /// Constructs a property set for the given memory-module information.
    /// </summary>
    LYRA_API property_set get_memory_module_information(
        _In_ const memory_module_information_type *info,
        _In_ const data::version_type smbios_version,
        _In_ const collection_flags flags = collection_flags::none);

    /// <summary>
    /// Constructs a property set for the given processor information.
    /// </summary>
    LYRA_API property_set get_processor_information(
        _In_ const processor_information_type *info,
        _In_ const data::version_type smbios_version,
        _In_ const collection_flags flags = collection_flags::none);

    /// <summary>
    /// Gets a string resource from an SMBIOS structure.
    /// </summary>
    /// <typeparam name="TType">The type of the structure, which must be one of
    /// the SMBIOS structures defined above.</typeparam>
    /// <param name="structure">A pointer to the SMBIOS structure from
    /// <see cref="data" />.</param>
    /// <param name="id">The <see cref="string_type" /> from the 
    /// <paramref name="structure" /> to be retrieved.</param>
    /// <returns>The string referenced by the specified <paramref name="id" />.
    /// </returns>
    template<class TType> _Ret_maybenull_z_ const char *get_string(
        _In_ const TType *structure, _In_ const string_type id);

} /* namespace smbios */

LYRA_NAMESPACE_END

#include "visus/autodoc/smbios.inl"

#endif /* !defined(_LYRA_SMBIOS_H) */
