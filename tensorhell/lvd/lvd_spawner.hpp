// ///////////////////////////////////////////////////////////////////////////
// lvd_spawner.hpp by Victor Dods, created 2007/02/09
// Copyright Victor Dods, licensed for use by Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#if !defined(LVD_SPAWNER_HPP_)
#define LVD_SPAWNER_HPP_

#include <string>

namespace Lvd {

typedef pid_t Pid;
typedef int (*ChildMainFunction)(int, char **, char **);
typedef int (*ParentMainFunction)(int, char **, char **, std::string const &);

int SpawnerMain (int argc, char **argv, char **envp, ParentMainFunction ParentMain, ChildMainFunction ChildMain, std::string const &child_indicator_token = "_child_");
Pid SpawnChild (int argc, char **argv, char **envp, std::string const &child_indicator_token);

} // end of namespace Lvd

#endif // !defined(LVD_SPAWNER_HPP_)
