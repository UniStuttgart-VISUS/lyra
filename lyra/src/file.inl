// <copyright file="file.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * LYRA_DETAIL_NAMESPACE::file_hash
 */
template<class TChar>
std::string LYRA_DETAIL_NAMESPACE::file_hash(_In_z_ const TChar *path) {
    return file_hash(open_read(path));
}
