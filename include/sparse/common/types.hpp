#pragma once

#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace sparse::common
{
/**
 * @brief Parsed title of the certificate.
 */
using title_t = std::string;

/**
 * @brief Parsed versions of the certificate.
 */
struct versions_t
{
    std::vector<std::string> eal;             ///< versions of eal
    std::vector<std::string> global_platform; ///< versions of global platform
    std::vector<std::string> java_card;       ///< versions of java card
    std::vector<std::string> sha;             ///< versions of sha
    std::vector<std::string> rsa;             ///< versions of rsa
    std::vector<std::string> ecc;             ///< versions of ecc
    std::vector<std::string> des;             ///< versions of des
};

/**
 * @brief Parsed section of the certificate.
 */
struct section_t
{
    std::string id;       ///< id
    std::string name;     ///< name
    uint32_t page_number; ///< page number of the beginning of the section
};

/**
 * @brief Parsed table of contents of the certificate
 */
using table_of_contents_t = std::vector<section_t>;

/**
 * @brief Parsed revision of the certificate.
 */
struct revision_t
{
    std::string version;     ///< version
    std::string date;        ///< date
    std::string description; ///< description
};

/**
 * @brief Parsed revisions of the certificate.
 */
using revisions_t = std::vector<revision_t>;

/**
 * @brief Parsed bibliography of the certificate.
 */
using bibliography = std::vector<std::string, std::string>;


/**
 * @brief Parsed section other of the certificate.
 */
using other_t = std::map<std::string, std::string>;
} // namespace sparse::common
