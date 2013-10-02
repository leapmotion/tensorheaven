// ///////////////////////////////////////////////////////////////////////////
// lvd_spawner.hpp by Victor Dods, created 2007/02/09
// Copyright Victor Dods, licensed for use by Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#if !defined(LVD_SPAWNER_HPP_)
#define LVD_SPAWNER_HPP_

#if !defined(_WIN32)

#include <string>

namespace Lvd {

typedef pid_t Pid;
typedef int (*ChildMainFunction)(int, char **, char **);
typedef int (*ParentMainFunction)(int, char **, char **, std::string const &);

int SpawnerMain (int argc,
                 char **argv,
                 char **envp,
                 ParentMainFunction ParentMain,
                 ChildMainFunction ChildMain,
                 std::string const &child_indicator_token = "_child_");

// the parent process is responsible for close()ing the file descriptors returned in
// child_to_parent_pipe__read_fd and parent_to_child_pipe__write_fd.  Otherwise
// repeated calls to SpawnChild will use up all the file descriptors and cause
// calls to pipe(), etc. to fail.
Pid SpawnChild (int argc,
                char **argv,
                char **envp,
                int &child_to_parent_pipe__read_fd,
                int &parent_to_child_pipe__write_fd,
                std::string const &child_indicator_token);

} // end of namespace Lvd

#endif

#endif // !defined(LVD_SPAWNER_HPP_)
