#include <catch2/catch.hpp>
#include <iostream>
#include <sparse/parsers/title_parser.hpp>

using namespace sparse::parsers;

TEST_CASE("title type 1")
{
    std::cout << "testing title" << std::endl;
    std::string_view ss = "            BSI-DSZ-CC-1051-2019\n\n"
                          "                     for\n\n"
                          "NXP Smart Card Controller P61N1M3VD/VD-1/PVE-1\n"
                          "           with IC Dedicated Software\n\n"
                          "                     from\n\n"
                          "      NXP Semiconductors Germany GmbH\n"
                          "                     BSI - Bundesamt für Sicherheit in der Informationstechnik, Postfach 20 03 63, D-53133 Bonn\n"
                          "                      Phone +49 (0)228 99 9582-0, Fax +49 (0)228 9582-5477, Infoline +49 (0)228 99 9582-111\n";

    const auto res = parse_title(ss);
    REQUIRE(res);
    REQUIRE(*res == "NXP Smart Card Controller P61N1M3VD/VD-1/PVE-1 with IC Dedicated Software");
}
