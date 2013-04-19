// ///////////////////////////////////////////////////////////////////////////
// lvd_callstack.cpp by Victor Dods, created 2007/02/14
// Copyright Victor Dods, licensed for use by Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <cerrno>
#include <execinfo.h>
#include <sstream>
#include <string>
#include <unistd.h>

#include <iostream> // TEMP HIPPO

#include "lvd_callstack.hpp"

using namespace std;

namespace Lvd {

enum { STDIN = 0, STDOUT = 1, STDERR = 2 }; // standard file descriptors
enum { PIPE_READ = 0, PIPE_WRITE = 1 }; // ends of the pipe

struct Addr2line
{
    Addr2line (char const *executable_filename)
    {
        assert(executable_filename != NULL && *executable_filename != '\0');
        if (pipe(m_child_to_parent_pipe) != 0)
            throw runtime_error(string("child-to-parent pipe creation failed: ") + strerror(errno));
        if (pipe(m_parent_to_child_pipe) != 0)
            throw runtime_error(string("parent-to-child pipe creation failed: ") + strerror(errno));
        if ((m_child_pid = fork()) == 0)
        {
            // connect up the pipes
            dup2(m_parent_to_child_pipe[PIPE_READ], STDIN);
            dup2(m_child_to_parent_pipe[PIPE_WRITE], STDOUT);
            // create the commandline
// #if defined(__MACH__) // Mac builds
//             char *argv[4];
//             argv[0] = const_cast<char *>("atos");
//             argv[1] = const_cast<char *>("-o");
//             argv[2] = const_cast<char *>(executable_filename);
//             argv[3] = NULL;
// #else // Linux builds
            char *argv[7];
            argv[0] = const_cast<char *>("addr2line");
            argv[1] = const_cast<char *>("--exe");
            argv[2] = const_cast<char *>(executable_filename);
            argv[3] = const_cast<char *>("--demangle");
            argv[4] = const_cast<char *>("--functions");
            argv[5] = const_cast<char *>("--inlines");
            argv[6] = NULL;
// #endif
            // execute the child process
            execvp(argv[0], argv);
            // this should only happen if execv() fails
            std::cerr << "execvp failed" << std::endl;
            exit(-1);
        }
        // TODO: have the parent actually check that the child execvp succeeded
    }
    ~Addr2line ()
    {
        // close the pipes and terminate the child (if it's not already dead)
        close(m_child_to_parent_pipe[PIPE_READ]);
        close(m_parent_to_child_pipe[PIPE_WRITE]);
        // DIE!!!!!!!!1
        kill(m_child_pid, SIGKILL);
    }

    string StringifyAddress (void const *address, PathFormat path_format)
    {
// #if defined(__MACH__)
//         return "TEMP DUMMY STRINGIFIED ADDRESS";
// #endif

        // send the address to the child process
        ostringstream out;
        out << address << endl;
        if (write(m_parent_to_child_pipe[PIPE_WRITE], out.str().c_str(), out.str().length()) == -1)
            throw runtime_error(string("address write failed: ") + strerror(errno));

        // read the response from the child process
        enum { RESPONSE_BUFFER_SIZE = 0x400 };
        char buffer[RESPONSE_BUFFER_SIZE];
        size_t bytes_actually_read = read(m_child_to_parent_pipe[PIPE_READ], buffer, RESPONSE_BUFFER_SIZE);
        if (bytes_actually_read == size_t(-1))
            throw runtime_error(string("read response from child failed: ") + strerror(errno));
        assert(bytes_actually_read > 0);
        assert(bytes_actually_read <= RESPONSE_BUFFER_SIZE);
        string response(buffer, bytes_actually_read);

//         // parse the response into function name and file location
//         string::size_type first_newline = response.find_first_of("\n");
//         assert(first_newline != string::npos);
//         string function_name(response.substr(0, first_newline));
//         string file_location(response.substr(first_newline+1));
// //         std::cerr << "\nHIPPO: file_location = \"" << file_location << "\" END HIPPO\n\n";
//         assert(file_location.find_first_of("\n") == file_location.length()-1);
//         file_location.resize(file_location.length()-1);
//
//         // perform PathFormat formatting
//         if (path_format == CHOMP_DOT_DOT)
//         {
//             string::size_type pos = 0;
//             string::size_type new_pos;
//             while ((new_pos = file_location.find("../", pos)) != string::npos)
//                 pos = new_pos + 3;
//             file_location.erase(0, pos);
//         }
//         else if (path_format == CHOMP_PATH)
//         {
//             string::size_type pos = file_location.find_last_of("/");
//             if (pos != string::npos)
//                 file_location.erase(0, pos+1);
//         }
//
//         // format the return string and return it
//         response.clear();
//         response += file_location;
//         response += " - ";
//         response += function_name;
//         return response;
        // TODO: if -O0 is specified, it seems like the above formatting assumptions hold.
        // if there is optimization and therefore inlining of function calls, then the formatting
        // may include multiple lines.  so for now, just use the full response, with a check
        // that it ends with a newline.
        if (response[response.length()-1] != '\n')
            response += '\n';
        // tag on a little arrow to delimit each item in the callstack
        return "--> " + response;
    }

private:

    int m_child_to_parent_pipe[2];
    int m_parent_to_child_pipe[2];
    pid_t m_child_pid;
};

void PrintCallstack (
    std::ostream &out,
    char const *executable_filename,
    PathFormat path_format,
    unsigned int frame_skip_count)
{
    enum
    {
        MAX_CALLSTACK_DEPTH = 0x100,
        BUFFER_SIZE = sizeof(void *) * MAX_CALLSTACK_DEPTH,
    };

    void *callstack_pointers[MAX_CALLSTACK_DEPTH];
    size_t callstack_depth;
    // note that many compiler optimizations (e.g. tail calls, inlining,
    // frame pointer elimination) will prevent accurate backtraces.
    callstack_depth = backtrace(callstack_pointers, MAX_CALLSTACK_DEPTH);

    // method 1: spawn a child process of addr2line and use that to
    // turn addresses into human-readable file locations.  this
    // seems to sometimes result in unleashing a lot of zombies.
    {

        out << "*** BEGIN CALLSTACK ***\n";
        Addr2line addr2line(executable_filename);
        for (size_t i = 1+frame_skip_count; i < callstack_depth; ++i)
            out << "  " << addr2line.StringifyAddress(callstack_pointers[i], path_format) << '\n';
        out << "*** END CALLSTACK ***" << endl;
    }
}

} // end of namespace Lvd
