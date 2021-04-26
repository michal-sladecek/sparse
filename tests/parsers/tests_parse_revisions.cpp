#include <catch2/catch.hpp>
#include <sparse/parsers/parsers.hpp>

#include <iostream>

using namespace sparse::parsers;


TEST_CASE("test1")
{
    std::string s = "asfhbajfbhfb,hdbsn alnfkjlsf"
                    "afjnajhfsjfbshjdnfsfnhdsbjhsdjnjk jsflks.jf History"
                    "Revision History\n"
                    "Version       Description of change\n"
                    "0.2           Initial draft version\n"
                    "3.5           Final version\n";
    const auto res = parse_revisions(s);
    for (const auto& r : *res)
    {
        std::cout << r.version << std::endl;
    }
}
