#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

namespace sparse::parsers::revisions
{
namespace states
{
// clang-format off
struct init {};
struct end {};
struct header_found_version {};
struct header_not_found {};
struct version_destription {};
struct version_date_description {};
struct date_version_change {};
struct version_date_author_version {};
struct description_not_found {};
struct version_description_found {};
struct revision_date_description_found {};
struct date_version_change_found {};
struct version_date_author_changes_found {};
// clang-format on
} // namespace states
} // namespace sparse::parsers::revisions
#endif // STATE_MACHINE_H
