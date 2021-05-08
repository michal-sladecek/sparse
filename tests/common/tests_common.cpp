#include <catch2/catch.hpp>
#include <sparse/common/utility.hpp>

TEST_CASE("load_file_into_string")
{
    const auto whole_text = sparse::common::load_file_into_string("../parsers/1107a_pdf.txt");
    REQUIRE(whole_text.length() > 0);

    REQUIRE_THROWS_WITH(sparse::common::load_file_into_string("nonexisting.txt"), Catch::Contains("File does not exist"));
}
