#include <catch2/catch.hpp>
#include <sparse/common/utility.hpp>
#include <sparse/parsers/parsers.hpp>

#include <iostream>

using namespace sparse::parsers;

TEST_CASE("Certification_Report_NSCIB-CC-200689-CR")
{
    const auto whole_text = sparse::common::load_file_into_string("./Certification_Report_NSCIB-CC-200689-CR.txt");
    const auto res        = parse_revisions(whole_text);
    REQUIRE(res.empty());
}
