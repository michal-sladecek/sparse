#pragma once

#include <chrono>
#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace sparse::common
{
/**
 * @brief Parsed title of the certificate
 */
struct title
{
    using data_t = std::string;

    data_t data; ///< the title
};

/**
 * @brief Parsed versions of the certificate
 */
struct versions
{
    using data_t = std::vector<std::string>;

    data_t eal;             ///< versions of eal
    data_t global_platform; ///< versions of global platform
    data_t java_card;       ///< versions of java card
    data_t sha;             ///< versions of sha
    data_t rsa;             ///< versions of rsa
    data_t ecc;             ///< versions of ecc
    data_t des;             ///< versions of des
};

/**
 * @brief Parsed section of the certificate
 */
struct section
{
    std::string id;       ///< id of the section
    std::string name;     ///< name of the section
    uint32_t page_number; ///< page number of the beginning of the section
};

/**
 * @brief Parsed table of contents of the certificate
 */
struct table_of_contents
{
    using data_t = std::vector<section>;

    data_t data; ///< the table of contents
};


/**
 * @brief Parsed revision of the certificate
 */
struct revision
{
    std::string version;              ///< version
    std::chrono::year_month_day date; ///< date
    std::string description;          ///< description
};

/**
 * @brief Parsed revisions of the certificate
 */
struct revisions
{
    using data_t = std::vector<revision>;

    data_t data; ///< revisions
};

/**
 * @brief Parsed bibliography of the certificate
 */
struct bibliography
{
    using data_t = std::map<std::string, std::string>;

    data_t data; ///< the bibliography
};

/**
 * @brief Parsed section other of the certificate
 */
struct other
{
    using data_t = std::map<std::string, std::string>;

    data_t data; ///< other
};
} // namespace sparse::common
