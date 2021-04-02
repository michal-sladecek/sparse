#include <catch2/catch.hpp>
#include <sparse/common/utility.hpp>
#include <sparse/parsers/parsers.hpp>

TEST_CASE("test-1107a_pdf")
{
    const auto whole_text = sparse::common::load_file_into_string("./1107a_pdf.txt");
    const auto versions   = sparse::parsers::parse_versions(whole_text);
    // REQUIRE() ToDo: eal
    REQUIRE(versions->global_platform.empty());
    REQUIRE(versions->java_card.empty());
    REQUIRE(versions->sha.empty());
    REQUIRE(versions->rsa.empty());
    REQUIRE(versions->ecc.empty());
    // REQUIRE(); ToDo: des
}


TEST_CASE("Certification_Report_NSCIB-CC-200689-CR.txt")
{
    const auto whole_text = sparse::common::load_file_into_string("./Certification_Report_NSCIB-CC-200689-CR.txt");
    const auto versions   = sparse::parsers::parse_versions(whole_text);

    // REQUIRE() ToDo: eal
    REQUIRE(versions->global_platform.empty());
    REQUIRE(versions->java_card.empty());
    REQUIRE(versions->sha.empty());
    REQUIRE(versions->rsa.empty());
    REQUIRE(versions->ecc.empty());
    REQUIRE(versions->des.empty());
}

TEST_CASE("NSCIB-CC-195714_3-STv3.1")
{
    const auto whole_text = sparse::common::load_file_into_string("./NSCIB-CC-195714_3-STv3.1.txt");
    const auto versions   = sparse::parsers::parse_versions(whole_text);

    // REQUIRE() ToDo: eal
    REQUIRE(versions->global_platform.empty());
    REQUIRE(versions->java_card.empty());
    // REQUIRE(versions->sha.empty()); ToDo: sha
    REQUIRE(versions->rsa.empty());
    REQUIRE(versions->ecc.empty());
    // REQUIRE(versions->des.empty()); ToDo: des
}
