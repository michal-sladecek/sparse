#include <catch2/catch.hpp>
#include <iostream>
#include <sparse/parsers/title_parser.hpp>

using namespace sparse::parsers;

TEST_CASE("With document ID", "[title]")
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

TEST_CASE("With security target before", "[title]")
{
    constexpr std::string_view ss = "       SECURITY TARGET LITE\n"
                                    " IDEAL PASS v2.3-n JC WITH PRIVACY\n"
                                    "PROTECTION (SAC/EAC/POLYMORPHIC\n"
                                    "      EMRTD CONFIGURATION)\n"
                                    "\n"
                                    "\n"
                                    "         Reference: 2018_2000036361\n"
                                    "    Security Target Lite\n"
                                    "Ref.:\n"
                                    " IDeal Pass v2.3-n JC with Privacy\n"
                                    "            2018_2000036361";
    const auto res = parse_title(ss);
    REQUIRE(res);
    REQUIRE(*res == "IDEAL PASS v2.3-n JC WITH PRIVACY PROTECTION (SAC/EAC/POLYMORPHIC EMRTD CONFIGURATION)");
}

TEST_CASE("With security target after", "[title]")
{
    SECTION("Single line")
    {
        constexpr std::string_view ss = "  JCOP 4.7 SE051\n"
                                        "  Security Target Lite\n"
                                        "  Rev. 1.3 — 30 June 2020                                         Evaluation document\n"
                                        "  NSCIB-CC-0095534                                                             PUBLIC\n";
        const auto res = parse_title(ss);
        REQUIRE(res);
        REQUIRE(*res == "JCOP 4.7 SE051");
    }
    SECTION("Multiline")
    {
        constexpr std::string_view ss = "       MF2DL(H)x0, MF2ID(H)10, NT4H2x21Gf\n"
                                        "       and NT4H2x21Tf\n"
                                        "       Security Target Lite\n"
                                        "            Rev. 1.0 – 2018-12-31                                  Evaluation documentation";
        const auto res = parse_title(ss);
        REQUIRE(res);
        REQUIRE(*res == "MF2DL(H)x0, MF2ID(H)10, NT4H2x21Gf and NT4H2x21Tf");
    }
}

TEST_CASE("With version", "[title]")
{
    constexpr std::string_view ss = "                                                 TÜV Rheinland Nederland B.V.\n"
                                    "  \n"
                                    "  \n"
                                    "  \n"
                                    "  \n"
                                    "                                                                      Certification Report\n"
                                    "      Version 2020-2\n"
                                    "  \n"
                                    "  \n"
                                    "  \n"
                                    "  \n"
                                    "  Voyager ePassport v2.20\n"
                                    "  \n"
                                    "  \n"
                                    "  \n"
                                    "  \n"
                                    "  Sponsor and developer:           Infineon Technologies AG\n"
                                    "              Am Campeon 1-15\n"
                                    "          85579 Neubiberg\n"
                                    "              Germany\n";
    const auto res = parse_title(ss);
    REQUIRE(res);
    REQUIRE(*res == "Voyager ePassport v2.20");
}
