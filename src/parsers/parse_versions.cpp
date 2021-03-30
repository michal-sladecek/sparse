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

std::vector<std::string> parse_via_regex(std::string text, const std::regex& regex)
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
    common::versions_t parsed_versions;
    return {};
}

} // namespace sparse::parsers
