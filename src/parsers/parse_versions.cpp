#include <regex>
#include <sparse/parsers/parsers.hpp>

namespace sparse::parsers
{
namespace detail
{
const std::regex eal_regex("\\sEAL[ ]?\\d[+]?\\s");
const std::regex sha_regex("\\sSHA[- ]?(1|2|3|224|256|384|512)\\s");
const std::regex rsa_regex("\\sRSA([ ]?(2048|4096|1024)|-CRT|SignaturePKCS1|SSA-PSS)\\s");
const std::regex des_regex("\\s(3[ ]?DES|DES[ ]?3|(Triple|triple)[- ]?DES)\\s");
const std::regex ecc_regex("\\s(ECC 224|ECC]?)\\s");
const std::regex java_card_regex("\\sJava Card (\\d|\\d.\\d.\\d)\\s");
const std::regex global_platform_regex("\\sGlobal Platform (\\d.\\d|\\d.\\d.\\d)\\s");
} // namespace detail

std::optional<common::versions_t> parse_versions(const std::string& certificate) noexcept
{
    common::versions_t parsed_versions;
    return {};
}

} // namespace sparse::parsers
