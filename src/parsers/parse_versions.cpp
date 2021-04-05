#include <regex>
#include <sparse/common/utility.hpp>
#include <sparse/parsers/parsers.hpp>
#include <unordered_set>

namespace sparse::parsers
{
namespace detail
{
const static std::regex eal_regex("EAL[ ]?\\d[+]?");
const static std::regex global_platform_regex("Global[ ]?Platform (\\d\\.\\d\\.\\d|\\d\\.\\d)");
const static std::regex java_card_regex("Java Card (\\d\\.\\d\\.\\d|\\d)");
const static std::regex sha_regex("SHA[- _]?[\n]?(224|256|384|512|1|2|3)");
const static std::regex rsa_regex("RSA([ _]?(2048|4096|1024)|-CRT|SignaturePKCS1|SSA-PSS)");
const static std::regex ecc_regex("(ECC[ ]?(224|256)|ECC)");
const static std::regex des_regex("(3[ ]?DES|DES[ ]?3|(Triple|triple|T)[- ]?DES)");

std::string remove_newlines(std::string str)
{
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    return str;
}

std::vector<std::string> parse_via_regex(const std::string& text, const std::regex& regex)
{
    std::unordered_set<std::string> parsed;

    auto words = std::sregex_iterator(text.begin(), text.end(), regex);
    for (auto iter = words; iter != std::sregex_iterator(); ++iter)
    {
        parsed.emplace(remove_newlines(iter->str()));
    }

    return {parsed.begin(), parsed.end()};
}
} // namespace detail


common::versions_t parse_versions(const std::string& certificate) noexcept
{
    using namespace detail;

    common::versions_t parsed_versions;
    parsed_versions.eal             = parse_via_regex(certificate, eal_regex);
    parsed_versions.global_platform = parse_via_regex(certificate, global_platform_regex);
    parsed_versions.java_card       = parse_via_regex(certificate, java_card_regex);
    parsed_versions.sha             = parse_via_regex(certificate, sha_regex);
    parsed_versions.rsa             = parse_via_regex(certificate, rsa_regex);
    parsed_versions.ecc             = parse_via_regex(certificate, ecc_regex);
    parsed_versions.des             = parse_via_regex(certificate, des_regex);
    return parsed_versions;
}
} // namespace sparse::parsers
