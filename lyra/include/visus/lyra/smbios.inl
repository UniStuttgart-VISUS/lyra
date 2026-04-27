// <copyright file="smbios.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * LYRA_NAMESPACE::smbios::data::each_entry
 */
template<class TCallback>
std::size_t LYRA_NAMESPACE::smbios::data::each_entry(
        _In_ TCallback callback) const {
    const auto begin = this->_data + this->_begin;
    const auto end = this->_data + this->_end;
    assert(begin <= end);

    std::size_t retval = 0;

    for (auto ptr = begin; ptr < end;) {
        auto header = reinterpret_cast<const header_type *>(ptr);
        if (header->length < 4) {
            LYRA_TRACE(_T("SMBIOS structure is broken: a structure has a ")
                _T("length of only %d bytes, which is less than the required ")
                _T("minimum of four bytes."), static_cast<int>(header->length));
            break;
        }

        // Evaluate the predicate and return the pointer f
        ++retval;
        if (!callback(header)) {
            return retval;
        }

        // Search next handle.
        // From http://git.savannah.gnu.org/cgit/dmidecode.git/tree/dmidecode.c
        ptr += header->length;
        while ((ptr < end) && ((ptr[0] != 0) || (ptr[1] != 0))) {
            ++ptr;
        }
        ptr += 2;

        // From http://git.savannah.gnu.org/cgit/dmidecode.git/tree/dmidecode.c:
        // SMBIOS v3 requires stopping at this marker.
        if ((header->type == 127) && (this->_flags & flag_stop_at_eot)) {
            break;
        }
    }

    return retval;
}


/*
 * LYRA_NAMESPACE::smbios::data::entries
 */
template<class TType, class TIterator, class TPredicate>
std::size_t  LYRA_NAMESPACE::smbios::data::entries(
        _In_ TIterator oit,
        _In_ TPredicate pred) const {
    std::size_t retval = 0;

    this->each_entry([&](const header_type *header) {
        assert(header != nullptr);
        if (pred(*header)) {
            *oit++ = reinterpret_cast<const TType *>(header);
            ++retval;
        }
        return true;
    });

    return retval;
}


/*
 * LYRA_NAMESPACE::smbios::data::entries_of_type
 */
template<class TType, class TIterator>
std::size_t LYRA_NAMESPACE::smbios::data::entries_of_type(
        _In_ TIterator&& oit) const {
    return this->entries<TType>(std::forward<TIterator>(oit),
        [](const header_type& h) { return (h.type == TType::id); });
}


/*
 * LYRA_NAMESPACE::smbios::get_string
 */
template<class TType>
_Ret_maybenull_z_ const char *LYRA_NAMESPACE::smbios::get_string(
        _In_ const TType *structure, _In_ const string_type id) {
    assert(structure != nullptr);
    if (structure == nullptr) {
        return nullptr;
    }
    if (id <= 0) {
        return nullptr;
    }

    auto header = &(structure->header);
    auto retval = reinterpret_cast<const char *>(header) + header->length;
    for (string_type s = 1; s < id; ++s) {
        while (*retval != 0) {
            ++retval;
        }
        ++retval;
    }

    // TODO: Handle illegal indices
    return retval;
}
