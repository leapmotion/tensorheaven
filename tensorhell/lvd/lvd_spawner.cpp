// ///////////////////////////////////////////////////////////////////////////
// lvd_spawner.cpp by Victor Dods, created 2007/02/09
// Copyright Victor Dods, licensed for use by Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#if !defined(_WIN32)
#include <sys/types.h>
#include <unistd.h>
#endif
#include <cerrno>

#include "lvd_spawner.hpp"

#if !defined(_WIN32)

using namespace std;

namespace Lvd {

enum { STDIN = 0, STDOUT = 1, STDERR = 2 }; // standard file descriptors
enum { PIPE_READ = 0, PIPE_WRITE = 1 }; // ends of the pipe

int SpawnerMain (int argc,
                 char **argv,
                 char **envp,
                 ParentMainFunction ParentMain,
                 ChildMainFunction ChildMain,
                 string const &child_indicator_token)
{
    // if we got the value of child_indicator_token as the first arg,
    if (argc >= 2 && argv[1] == child_indicator_token)
    {
        // remove the child_indicator_token argument
        --argc;
        for (int i = 1; i < argc; ++i)
            argv[i] = argv[i+1];
        argv[argc] = NULL;
        // call ChildMain
        return ChildMain(argc, argv, envp);
    }
    // otherwise just call ParentMain
    else
    {
        // call ParentMain
        return ParentMain(argc, argv, envp, child_indicator_token);
    }
}

Pid SpawnChild (int argc,
                char **argv,
                char **envp,
                int &child_to_parent_pipe__read_fd,
                int &parent_to_child_pipe__write_fd,
                string const &child_indicator_token)
{
    // create pipes to share between parent and child process
    int child_to_parent_pipe[2];
    int parent_to_child_pipe[2];
    try
    {
        if (pipe(child_to_parent_pipe) != 0)
        {
            throw runtime_error(string("child-to-parent pipe creation failed (before fork): ") + strerror(errno));
        }
        if (pipe(parent_to_child_pipe) != 0)
        {
            throw runtime_error(string("parent-to-child pipe creation failed (before fork): ") + strerror(errno));
        }
    }
    catch (exception &e)
    {
        cerr << "caught exception: " << e.what() << '\n';
        // TODO: probably shouldn't exit, but rather let the exception pass through
        // -- will also need to close the first pipe if it succeeded but the second failed.
        exit(-1);
    }
    // fork the process
    Pid pid = fork();
    if (pid == 0) // child process
    {
        try
        {
            // connect up the pipes.
            if (dup2(parent_to_child_pipe[PIPE_READ], STDIN) < 0)
                throw runtime_error(string("parent-to-child pipe connection for STDIN failed (in child process): ") + strerror(errno));
            if (dup2(child_to_parent_pipe[PIPE_WRITE], STDOUT) < 0)
                throw runtime_error(string("child-to-parent pipe connection for STDOUT failed (in child process): ") + strerror(errno));
        }
        catch (exception &e)
        {
            cerr << "caught exception: " << e.what() << '\n';
            exit(-1);
        }
        // close the unused end of each pipe (this MUST be done, otherwise
        // there will be dangling file descriptors choking up the junk).
        close(parent_to_child_pipe[PIPE_WRITE]);
        close(child_to_parent_pipe[PIPE_READ]);
        // construct the argc, argv, and envp for the child process
        int child_argc = argc+1;
        char **child_argv = new char *[child_argc];
        char **child_envp = envp;
        child_argv[0] = argv[0];
        child_argv[1] = strdup(child_indicator_token.c_str());
        for (int i = 2; i < child_argc; ++i)
            child_argv[i] = argv[i-1];
        // execute the child process
        execve(child_argv[0], child_argv, child_envp);
        // this should only happen if execve() fails
        exit(-1);
    }
    else // parent process
    {
        try
        {
            // connect up the pipes.
            if (dup2(child_to_parent_pipe[PIPE_READ], STDIN) < 0)
                throw runtime_error(string("child-to-parent pipe connection for STDIN failed (in parent process): ") + strerror(errno));
            if (dup2(parent_to_child_pipe[PIPE_WRITE], STDOUT) < 0)
                throw runtime_error(string("parent-to-child pipe connection for STDOUT failed (in parent process): ") + strerror(errno));
        }
        catch (exception &e)
        {
            cerr << "caught exception: " << e.what() << '\n';
            exit(-1);
        }
        // set the return value pipe file descriptors for the parent to close later
        child_to_parent_pipe__read_fd = child_to_parent_pipe[PIPE_READ];
        parent_to_child_pipe__write_fd = parent_to_child_pipe[PIPE_WRITE];
        // close the unused end of each pipe (this MUST be done, otherwise
        // there will be dangling file descriptors choking up the junk).
        close(parent_to_child_pipe[PIPE_READ]);
        close(child_to_parent_pipe[PIPE_WRITE]);
        // return the child process pid
        return pid;
    }
}

} // end of namespace Lvd

#endif
