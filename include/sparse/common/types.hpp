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

    data_t data;
};

/**
 * @brief Parsed versions of the certificate
 */
struct versions
{
    using data_t = std::vector<std::string>;

    data_t eal;
    data_t global;
    data_t java_card;
    data_t sha;
    data_t rsa;
    data_t ecc;
    data_t des;
};

/**
 * @brief Parsed table of content of the certificate
 */
struct table_of_content
{
    using data_t = std::vector<std::tuple<std::string, std::string, int>>;

    data_t data;
};

/**
 * @brief Parsed revisions of the certificate
 */
struct revisions
{
    using data_t = std::vector<std::unordered_map<std::string, std::string>>;

    data_t data;
};

/**
 * @brief Parsed bibliography of the certificate
 */
struct bibliography
{
    using data_t = std::unordered_map<std::string, std::string>;

    data_t data;
};

/**
 * @brief Parsed section other of the certificate
 */
struct other
{
    using data_t = std::unordered_map<std::string, std::string>;

    data_t data;
};
} // namespace sparse::common
