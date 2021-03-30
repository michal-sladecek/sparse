#include <regex>
#include <sparse/parsers/parsers.hpp>

namespace sparse::parsers
{
namespace detail
{
const static std::regex eal_regex("\\sEAL[ ]?\\d[+]?\\s");
const static std::regex global_platform_regex("\\sGlobal Platform (\\d.\\d|\\d.\\d.\\d)\\s");
const static std::regex java_card_regex("\\sJava Card (\\d|\\d.\\d.\\d)\\s");
const static std::regex sha_regex("\\sSHA[- ]?(1|2|3|224|256|384|512)\\s");
const static std::regex rsa_regex("\\sRSA([ ]?(2048|4096|1024)|-CRT|SignaturePKCS1|SSA-PSS)\\s");
const static std::regex ecc_regex("\\s(ECC 224|ECC]?)\\s");
const static std::regex des_regex("\\s(3[ ]?DES|DES[ ]?3|(Triple|triple)[- ]?DES)\\s");

std::vector<std::string> parse_via_regex(const std::string& text, const std::regex& regex)
{
    std::vector<std::string> parsed;

    auto words = std::sregex_iterator(text.begin(), text.end(), regex);
    for (auto iter = words; iter != std::sregex_iterator(); ++iter)
    {
        parsed.emplace_back(iter->str());
    }

    return parsed;
}
} // namespace detail


std::optional<common::versions_t> parse_versions(const std::string& certificate) noexcept
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
