#include <catch2/catch.hpp>
#include <iostream>
#include <sparse/parsers/title_parser.hpp>

using namespace sparse::parsers;

TEST_CASE("title type 1")
{
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

TEST_CASE("title type 2")
{
    constexpr std::string_view ss = "  JCOP 4.7 SE051\n"
                                    "  Security Target Lite\n"
                                    "  Rev. 1.3 — 30 June 2020                                         Evaluation document\n"
                                    "  NSCIB-CC-0095534                                                             PUBLIC\n";

    constexpr std::string_view ss2 = "       MF2DL(H)x0, MF2ID(H)10, NT4H2x21Gf\n"
                                     "       and NT4H2x21Tf\n"
                                     "       Security Target Lite\n"
                                     "            Rev. 1.0 – 2018-12-31                                  Evaluation documentation";

    const auto res1 = parse_title(ss);
    REQUIRE(res1);
    REQUIRE(*res1 == "JCOP 4.7 SE051");

    const auto res2 = parse_title(ss2);
    REQUIRE(res2);
    REQUIRE(*res2 == "MF2DL(H)x0, MF2ID(H)10, NT4H2x21Gf and NT4H2x21Tf");
}
