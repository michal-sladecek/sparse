#include <catch2/catch.hpp>
#include <sparse/common/utility.hpp>


TEST_CASE("test")
{
    REQUIRE(true);
}


TEST_CASE("load_file_into_string")
{
    std::string whole_text = sparse::common::load_file_into_string("../1107a_pdf.txt");
    REQUIRE(whole_text.length() > 0);

    std::string file_does_not_exist = sparse::common::load_file_into_string("../1107a_pf.txt");
    REQUIRE(file_does_not_exist.length() == 0);
}
