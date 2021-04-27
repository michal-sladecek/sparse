#include <catch2/catch.hpp>
#include <sparse/parsers/parsers.hpp>

#include <iostream>

using namespace sparse::parsers;


TEST_CASE("test1")
{
    std::string s = "Revision History\n"
                    "Some text\n"
                    "Version       Description of change\n"
                    "0.2           Initial draft version\n"
                    "3.5           Final version\n";
    const auto res = parse_revisions(s);
}


TEST_CASE("test2")
{
    std::string s = "Revision history"
                    "Rev         Date                 Description\n"
                    "number\n"
                    "Rev. 4.1    23-November-2015     Derived from P60D024/016/012yVB(Y/Z/A)/yVF Security Target\n"
                    "Rev. 4.2    29-May-2018          Revise wording for access control policy SFRs to be consistent with datasheet MMU\n"
                    "errata update\n"
                    "Rev. 4.3    21-June-2018         This ST now claims compliance to CC v3.1 Rev. 5\n"
                    "Improve consistency in SFR wording\n"
                    "Clarify read/write access permission\n"
                    "FCS.COP.1[HW_DES] now claims compliance to NIST SP800-67\n"
                    "Rev. 4.4    29-October-2018      Update access control SFRs to reflect three access permissions enforeced by MMU\n";
    const auto res = parse_revisions(s);
    for (const auto& r : res)
    {
        std::cout << ">>" << r.description << std::endl;
    }
}
