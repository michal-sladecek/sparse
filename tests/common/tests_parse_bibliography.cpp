#include <catch2/catch.hpp>
#include <iostream>
#include <sparse/common/utility.hpp>
#include <string>
#include <filesystem>
#include <sparse/common/parsers.hpp>


TEST_CASE("test-1107a_pdf")
{
    std::string whole_text = sparse::common::load_file_into_string("../1107a_pdf.txt");
    REQUIRE(whole_text.length() > 0);


    std::optional<sparse::common::bibliography_t> bib_optional = sparse::common::parse_bibliography(whole_text);

    REQUIRE(bib_optional.has_value());

    sparse::common::bibliography_t bib = bib_optional.value();

    REQUIRE(bib.contains("[1]"));
    REQUIRE(bib.contains("[18]"));
    REQUIRE(bib.contains("[47]"));

    REQUIRE(bib["[18]"] == "“UMSLC library for SLCx7 in 40nm”, v01.30.0564, 2019-06-19, Infineon Technologies AG");
    REQUIRE(bib["[47]"] == "“Site Technical Audit Report (STAR) UTAC Thai Limited, UTL1 / UTL.3”, Version 1, 2020-10-12, TÜV Informationstechnik GmbH");
}


TEST_CASE("Certification_Report_NSCIB-CC-200689-CR.txt")
{
    std::string whole_text = sparse::common::load_file_into_string("../Certification_Report_NSCIB-CC-200689-CR.txt");
    REQUIRE(whole_text.length() > 0);
    std::optional<sparse::common::bibliography_t> bib_optional = sparse::common::parse_bibliography(whole_text);

    REQUIRE(bib_optional.has_value());

    sparse::common::bibliography_t bib = bib_optional.value();

    REQUIRE(bib.contains("[CC]"));
    REQUIRE(bib.contains("[NSCIB]"));
    REQUIRE(bib.contains("[TR_03110]"));

    REQUIRE(bib["[CC]"] == "Common Criteria for Information Technology Security Evaluation, Parts I, II and III, Version 3.1 Revision 5, April 2017.");
    REQUIRE(bib["[NSCIB]"] == "Netherlands Scheme for Certification in the Area of IT Security, Version 2.5, 28 March 2019.");
    REQUIRE(bib["[TR_03110]"] == "Technical Guideline Advanced Security Mechanisms for Machine Readable Travel Documents – Part 1 – eMRTDs with BAC/PACEv2 and EACv1, Version 2.10, TR-03110, Bundesamt für Sicherheit in der Informationstechnik (BSI)");
}

TEST_CASE("NSCIB-CC-195714_3-STv3.1")
{
    std::string whole_text = sparse::common::load_file_into_string("../NSCIB-CC-195714_3-STv3.1.txt");
    REQUIRE(whole_text.length() > 0);
    std::optional<sparse::common::bibliography_t> bib_optional = sparse::common::parse_bibliography(whole_text);

    REQUIRE(bib_optional.has_value());
}