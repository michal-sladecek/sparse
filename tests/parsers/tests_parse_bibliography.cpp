#include <catch2/catch.hpp>
#include <filesystem>
#include <iostream>
#include <sparse/common/utility.hpp>
#include <sparse/parsers/parsers.hpp>
#include <string>


TEST_CASE("test-1107a_pdf")
{
    const std::string whole_text = sparse::common::load_file_into_string("./1107a_pdf.txt");
    REQUIRE(!whole_text.empty());


    const std::optional<sparse::common::bibliography_t> bib_optional = sparse::parsers::parse_bibliography(whole_text);

    REQUIRE(bib_optional.has_value());

    REQUIRE(bib_optional->at("[18]") == "“UMSLC library for SLCx7 in 40nm”, v01.30.0564, 2019-06-19, Infineon Technologies AG");
    REQUIRE(bib_optional->at("[47]") == "“Site Technical Audit Report (STAR) UTAC Thai Limited, UTL1 / UTL.3”, Version 1, 2020-10-12, TÜV Informationstechnik GmbH");
}


TEST_CASE("Certification_Report_NSCIB-CC-200689-CR.txt")
{
    const std::string whole_text = sparse::common::load_file_into_string("./Certification_Report_NSCIB-CC-200689-CR.txt");
    REQUIRE(!whole_text.empty());
    const std::optional<sparse::common::bibliography_t> bib_optional = sparse::parsers::parse_bibliography(whole_text);

    REQUIRE(bib_optional.has_value());

    REQUIRE(bib_optional->at("[CC]") == "Common Criteria for Information Technology Security Evaluation, Parts I, II and III, Version 3.1 Revision 5, April 2017.");
    REQUIRE(bib_optional->at("[NSCIB]") == "Netherlands Scheme for Certification in the Area of IT Security, Version 2.5, 28 March 2019.");
    REQUIRE(bib_optional->at("[TR_03110]") == "Technical Guideline Advanced Security Mechanisms for Machine Readable Travel Documents – Part 1 – eMRTDs with BAC/PACEv2 and "
                                 "EACv1, Version 2.10, TR-03110, Bundesamt für Sicherheit in der Informationstechnik (BSI)");
}

TEST_CASE("NSCIB-CC-195714_3-STv3.1")
{
    const std::string whole_text = sparse::common::load_file_into_string("./NSCIB-CC-195714_3-STv3.1.txt");
    REQUIRE(!whole_text.empty());
    const std::optional<sparse::common::bibliography_t> bib_optional = sparse::parsers::parse_bibliography(whole_text);

    REQUIRE(bib_optional.has_value());
}
