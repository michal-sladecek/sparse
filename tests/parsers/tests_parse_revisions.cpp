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

TEST_CASE("0939V3b_pdf")
{
    std::vector<std::string> versions{"4.1", "4.2", "4.3", "4.4"};
    std::vector<std::string> dates{"2015-11-23", "2018-05-29", "2018-06-21", "2018-10-29"};
    std::vector<std::string> description{"Derived from P60D024/016/012yVB(Y/Z/A)/yVF Security Target",
                                         "Revise wording for access control policy SFRs to be consistent with datasheet MMU errata update",
                                         "This ST now claims compliance to CC v3.1 Rev. 5 Improve consistency in SFR wording Clarify read/write access "
                                         "permission FCS.COP.1[HW_DES] now claims compliance to NIST SP800-67",
                                         "Update access control SFRs to reflect three access permissions enforeced by MMU"};

    const auto whole_text = sparse::common::load_file_into_string("./0939V3b_pdf.txt");
    const auto res        = parse_revisions(whole_text);

    REQUIRE(versions.size() == res.size());
    for (auto i = 0ul; i < res.size(); ++i)
    {
        REQUIRE(res[i].version == versions[i]);
        REQUIRE(res[i].date == dates[i]);
        REQUIRE(res[i].description == description[i]);
    }
}
