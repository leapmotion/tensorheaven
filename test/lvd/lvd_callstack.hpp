// ///////////////////////////////////////////////////////////////////////////
// lvd_callstack.hpp by Victor Dods, created 2007/02/14
// Copyright Victor Dods, licensed for use by Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#if !defined(LVD_CALLSTACK_HPP_)
#define LVD_CALLSTACK_HPP_

#include <ostream>

namespace Lvd {

enum PathFormat { FULL_PATH = 0, CHOMP_DOT_DOT, CHOMP_PATH };

void PrintCallstack (
    std::ostream &out,
    char const *executable_filename,
    PathFormat path_format = CHOMP_DOT_DOT,
    unsigned int frame_skip_count = 0);

} // end of namespace Lvd

#endif // !defined(LVD_CALLSTACK_HPP_)
