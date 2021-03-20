#pragma once

#include <string>
#include <tuple>
#include <unordered_map>
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
 * @brief Parsed table of content of the certificate
 */
struct table_of_content
{
    using data_t = std::vector<std::tuple<std::string, std::string, int>>;

    data_t data; ///< the table of content
};

/**
 * @brief Parsed revisions of the certificate
 */
struct revisions
{
    using data_t = std::vector<std::unordered_map<std::string, std::string>>;

    data_t data; ///< revisions
};

/**
 * @brief Parsed bibliography of the certificate
 */
struct bibliography
{
    using data_t = std::unordered_map<std::string, std::string>;

    data_t data; ///< the bibliography
};

/**
 * @brief Parsed section other of the certificate
 */
struct other
{
    using data_t = std::unordered_map<std::string, std::string>;

    data_t data; ///< other
};
} // namespace sparse::common
