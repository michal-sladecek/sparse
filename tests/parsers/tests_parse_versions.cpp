#include <catch2/catch.hpp>
#include <sparse/common/utility.hpp>
#include <sparse/parsers/parsers.hpp>
#include <string>
#include <vector>

bool contains_all(const std::vector<std::string>& vec, std::vector<std::string> should_contain) noexcept
{
    for (const auto& element : should_contain)
    {
        if (std::find(vec.begin(), vec.end(), element) == vec.end())
        {
            return false;
        }
    }
    return true;
}

TEST_CASE("test file NSCIB-CC-0075446-CRv2.txt")
{
    const auto whole_text = sparse::common::load_file_into_string("./NSCIB-CC-0075446-CRv2.txt");
    REQUIRE_FALSE(whole_text.empty());

    const auto versions = sparse::parsers::parse_versions(whole_text);
    REQUIRE(versions.global_platform.empty());
    REQUIRE(contains_all(versions.eal, {"EAL6", "EAL6+", "EAL4", "EAL2+", "EAL 6"}));
    REQUIRE(contains_all(versions.java_card, {"Java Card 3"}));
    REQUIRE(contains_all(versions.sha, {"SHA-1", "SHA-224", "SHA-256", "SHA-384", "SHA-512"}));
    REQUIRE(contains_all(versions.des, {"3DES"}));
}

TEST_CASE("test file 0782V5b_pdf.txt")
{
    const auto whole_text = sparse::common::load_file_into_string("./0782V5b_pdf.txt");
    REQUIRE_FALSE(whole_text.empty());

    const auto versions = sparse::parsers::parse_versions(whole_text);
    REQUIRE(versions.global_platform.empty());
    REQUIRE(versions.java_card.empty());
    REQUIRE(contains_all(versions.eal, {"EAL4+", "EAL6", "EAL6+", "EAL 6"}));
    REQUIRE(contains_all(versions.sha, {"SHA-2", "SHA-256", "SHA-512", "SHA1", "SHA256", "SHA 512", "SHA 256"}));
    REQUIRE(contains_all(versions.rsa, {"RSA2048", "RSA4096"}));
    REQUIRE(contains_all(versions.des, {"TDES", "Triple DES", "triple-DES"}));
}
