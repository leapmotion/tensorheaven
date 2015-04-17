// ///////////////////////////////////////////////////////////////////////////
// lvd_testsystem.cpp by Victor Dods, created 2007/02/10
// Copyright Victor Dods, licensed for unrestricted use by Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include <csignal>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <sstream>
#if !defined(_WIN32)
#include <sys/wait.h>
#include <unistd.h>

#include "lvd_callstack.hpp"
#endif
#include "lvd_commandlineparser.hpp"
#include "lvd_spawner.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

#define THROW_STRING(x) { ostringstream out; out << x; throw out.str(); }
#define PRINT_DEBUG_MESSAGE(x) do { if (g_print_debug_messages) { cerr << x; } } while (false)

using namespace std;

namespace Lvd {
namespace TestSystem {

// ///////////////////////////////////////////////////////////////////////////
// Lvd::TestSystem::Options
// ///////////////////////////////////////////////////////////////////////////

struct Options : public CommandLineParser
{
    Options (
        Directory &root_directory,
        string const &executable_filename,
        string const &program_description)
        :
        CommandLineParser(
            &Options::NonOptionArgumentHandlerMethod,
            ms_option,
            ms_option_count,
            executable_filename,
            program_description,
            "[options]"),
        m_root_directory(root_directory),
        m_is_print_all_requested(false),
        m_is_print_debug_messages_requested(false),
        m_is_help_requested(false)
    { }

    bool IsHelpRequested () const { return m_is_help_requested; }
    bool IsPrintDebugMessagesRequested () const { return m_is_print_debug_messages_requested; }

    virtual void Parse (int argc, char const *const *argv, std::ostream &out = std::cerr)
    {
        CommandLineParser::Parse(argc, argv);

        if (m_is_help_requested)
        {
            PrintHelpMessage(out);
            Status(EXIT_WITH_SUCCESS);
            return;
        }

        if (m_is_print_all_requested)
        {
            m_root_directory.Print(out);
            Status(EXIT_WITH_SUCCESS);
            return;
        }
    }

private:

    void NonOptionArgumentHandlerMethod (string const &arg)
    {
        THROW_STRING("superfluous argument \"" << arg << "\"");
    }
    void PrintAll ()
    {
        m_is_print_all_requested = true;
    }
    void Add (string const &arg)
    {
        if (!m_root_directory.TestPathIsValid(arg))
            THROW_STRING("invalid path \"" << arg << "\"");
        m_root_directory.SetIsScheduled(arg, true);
    }
    void Remove (string const &arg)
    {
        if (!m_root_directory.TestPathIsValid(arg))
            THROW_STRING("invalid path \"" << arg << "\"");
        m_root_directory.SetIsScheduled(arg, false);
    }
    void Debug ()
    {
        m_is_print_debug_messages_requested = true;
    }
    void RequestHelp ()
    {
        m_is_help_requested = true;
    }

    Directory &m_root_directory;
    bool m_is_print_all_requested;
    bool m_is_print_debug_messages_requested;
    bool m_is_help_requested;

    static CommandLineOption const ms_option[];
    static unsigned int const ms_option_count;
};

CommandLineOption const Options::ms_option[] =
{
    CommandLineOption("Test case/directory discovery options"),
    CommandLineOption(
        'p',
        "print-all",
        &Options::PrintAll,
        "    Print the set of tests scheduled to run, then exit.  This will be done\n"
        "    after processing all other commandline options."),
    CommandLineOption(
        "Test scheduling options -- these options perform addition/removal operations\n"
        "on the set of tests scheduled to run.  The operations are performed in the\n"
        "order they were specified on the commandline, from left to right.  All tests\n"
        "are scheduled to run by default."),
    CommandLineOption(
        'a',
        "add",
        &Options::Add,
        "    Adds the given test subhierarchy or test case to the set of those scheduled\n"
        "    to run.  The argument must be of the form \"/path/to/element\".  Specifying\n"
        "    \"/\" will cause all tests to be added.  See also option -r."),
    CommandLineOption(
        'r',
        "remove",
        &Options::Remove,
        "    Removes the given test subhierarchy or test case from the set of those\n"
        "    scheduled to run.  The argument must be of the form \"/path/to/element\".\n"
        "    Specifying \"/\" will cause all tests to be removed.  See also option -a."),
    CommandLineOption(""),
    CommandLineOption(
        'd',
        "debug",
        &Options::Debug,
        "    Enable debug messages (really only useful for lvd development)."),
    CommandLineOption(""),
    CommandLineOption(
        'h',
        "help",
        &Options::RequestHelp,
        "    Prints this help message.")
};
unsigned int const Options::ms_option_count = sizeof(Options::ms_option) / sizeof(CommandLineOption);

// ///////////////////////////////////////////////////////////////////////////
// Lvd::TestSystem::
// ///////////////////////////////////////////////////////////////////////////

namespace { // for preventing anyone from getting to these via extern

bool g_print_debug_messages = false;
Stage g_test_stage = STAGE_FRAMEWORK_INTERNAL_PRE_INITIALIZE;
string g_executable_filename;
Directory *g_root_directory = NULL;
string g_premade_caught_signal_message;
static string::size_type const gs_max_message_length = 0x8000; // arbitrary

string const &ResultString (Result result)
{
    static string const s_result_string[] =
    {
        "RESULT_NO_ERROR",
        "RESULT_CAUGHT_SIGNAL",
        "RESULT_UNCAUGHT_EXCEPTION",
        "RESULT_UNSPECIFIED_FAILURE"
    };
    static int const s_result_string_count = sizeof(s_result_string) / sizeof(*s_result_string);
    static string const s_invalid("*INVALID*RESULT*");
    if (result >= 0 && result < s_result_string_count)
        return s_result_string[result];
    else
        return s_invalid;
}

string const &StageString (Stage stage)
{
    static string const s_stage_string[] =
    {
        "STAGE_FRAMEWORK_INTERNAL_PRE_INITIALIZE",
        "STAGE_INITIALIZE",
        "STAGE_FRAMEWORK_INTERNAL_POST_INITIALIZE",
        "STAGE_TEST_BODY",
        "STAGE_FRAMEWORK_INTERNAL_POST_TEST_BODY",
        "STAGE_SHUTDOWN",
        "STAGE_COMPLETED"
    };
    static int const s_stage_string_count = sizeof(s_stage_string) / sizeof(*s_stage_string);
    static string const s_invalid("*INVALID*STAGE*");
    if (stage >= 0 && stage < s_stage_string_count)
        return s_stage_string[stage];
    else
        return s_invalid;
}

#if !defined(_WIN32)
enum { LOWEST_CAUGHT_SIGNAL = SIGHUP, HIGHEST_CAUGHT_SIGNAL = SIGTERM };
#endif

string const &SignalString (int signum)
{
#if defined(_WIN32)
	static string const s_retval("unspecified-signal (message not implemented in WIN32)");
	return s_retval;
#else
    static string const s_signal_string[HIGHEST_CAUGHT_SIGNAL+1] =
    {
        "UNKNOWN", // no signal for index 0
        "SIGHUP",  // = 1
        "SIGINT",  // = 2
        "SIGQUIT", // = 3
        "SIGILL",  // = 4 (not caught)
        "SIGTRAP", // = 5 (not caught)
        "SIGABRT", // = 6
        "SIGBUS",  // = 7 (not caught)
        "SIGFPE",  // = 8
        "SIGKILL", // = 9 (can't be handled)
        "SIGUSR1", // = 10 (not caught)
        "SIGSEGV", // = 11
        "SIGUSR2", // = 12 (not caught)
        "SIGPIPE", // = 13 (not caught)
        "SIGALRM", // = 14 (not caught)
        "SIGTERM"  // = 15
    };
    if (signum >= LOWEST_CAUGHT_SIGNAL && signum <= HIGHEST_CAUGHT_SIGNAL)
        return s_signal_string[signum];
    else
        return s_signal_string[0];
#endif
}

void EmitStatusMessage (Result result, Stage stage, int signum, string const &message)
{
    PRINT_DEBUG_MESSAGE("*** RUNNER DEBUG MESSAGE *** : emitting status: result = " << ResultString(result)
                                                                   << ", stage = " << StageString(stage)
                                                                   << ", signal = " << SignalString(signum) << '\n');
    cout << result << '\n' << stage << '\n' << signum << '\n';
    if (result != RESULT_NO_ERROR)
    {
        if (message.length() > gs_max_message_length)
        {
            cerr << "outgoing message exceeded max length of " << gs_max_message_length << "; truncating" << endl;
            cout << gs_max_message_length << endl;
            cout.write(message.data(), gs_max_message_length);
        }
        else
        {
            cout << message.length() << endl;
            cout << message;
        }
    }
    // make sure the buffer is flushed, so that the listening parent
    // process can read all the bytes we just sent to it.
    cout.flush();
    // also flush stderr, why not.
    cerr.flush();
}

#if !defined(_WIN32)
void SignalHandler (int signum)
{
    PRINT_DEBUG_MESSAGE("*** RUNNER DEBUG MESSAGE *** : caught signal " << SignalString(signum) << '\n');

    ostringstream out;

    // if there was a premade message (e.g. from FailAssert), use that
    // instead of printing out a call stack).
    if (!g_premade_caught_signal_message.empty())
        out << g_premade_caught_signal_message;
    else
        // skipping the top stack frame (this function), because printing
        // it in the report is unnecessary and confusing.
        PrintCallstack(out, g_executable_filename.c_str(), CHOMP_DOT_DOT, 1);

    // this is the thing that communicates the result to the parent process
    EmitStatusMessage(RESULT_CAUGHT_SIGNAL, g_test_stage, signum, out.str());

    // re-raise the signal, so the program dies (or whatever) normally.  at
    // this point, the signal handler has been reset to default, since the
    // SA_RESETHAND flag was specified when we called sigaction originally.
    raise(signum);
}
#endif
} // end of unnamed namespace

int Spawner (int argc, char **argv, char **envp, std::string const &child_indicator_token)
{
    assert(!g_executable_filename.empty() && "you must call SetExecutableFilename() during setup");
    assert(g_root_directory != NULL);

    // parse the options
    Options options(*g_root_directory, argv[0], "TODO: program description");
    options.Parse(argc, argv);
    if (options.Status() != Options::CONTINUE)
        return options.Status() == Options::EXIT_WITH_SUCCESS ? 0 : 1;

    // set the print-debug-messages flag
    g_print_debug_messages = options.IsPrintDebugMessagesRequested();

    // spawn the scheduled tests (we are the parent process)
    unsigned int failure_count = g_root_directory->SpawnScheduled(argv[0], envp, child_indicator_token);

    // print a summary report
    {
        unsigned int scheduled_test_case_count = g_root_directory->SubordinateTestCaseCount(true);
        assert(scheduled_test_case_count >= failure_count);
        unsigned int pass_count = scheduled_test_case_count - failure_count;

        cerr.precision(2);
        cerr.setf(ios_base::fixed);
        if (scheduled_test_case_count == 0)
            cerr << "summary: 0 tests were run" << endl;
        else
        {
            cerr << "summary: " << 100.0f * pass_count / scheduled_test_case_count << "% passed (";
            if (failure_count > 0)
                cerr << failure_count << " failed out of ";
            cerr << scheduled_test_case_count << " total)" << endl;
        }
    }

    return int(failure_count);
}

int Runner (int argc, char **argv, char **envp)
{
    assert(!g_executable_filename.empty() && "you must call SetExecutableFilename() during setup");
    assert(g_root_directory != NULL);

#if !defined(_WIN32)
    // set up the signal handler for various signals
    struct sigaction sa;
    sa.sa_handler = SignalHandler;
    memset(&sa.sa_mask, 0, sizeof(sa.sa_mask));
    sa.sa_flags = SA_RESETHAND; // restore each signal handler to the default one
                                // when the signal handler has been called once.
    sigaction(SIGHUP,  &sa, NULL); // terminal hung up
    sigaction(SIGINT,  &sa, NULL); // process interrupted (e.g. ctrl+c)
    sigaction(SIGQUIT, &sa, NULL); // user has requested a core dump (e.g. ctrl+\)
    sigaction(SIGABRT, &sa, NULL); // abort() has been called (e.g. a failed assert)
    sigaction(SIGFPE,  &sa, NULL); // floating point exception (e.g. divide by zero)
    sigaction(SIGSEGV, &sa, NULL); // segmentation fault (naughty programmer!)
    sigaction(SIGTERM, &sa, NULL); // someone kindly asks this process to terminate.
#endif

    // parse the options
    Options options(*g_root_directory, argv[0], "TODO: program description");
    options.Parse(argc, argv);
    if (options.Status() != Options::CONTINUE)
        return options.Status() == Options::EXIT_WITH_SUCCESS ? 0 : 1;

    // set the print-debug-messages flag
    g_print_debug_messages = options.IsPrintDebugMessagesRequested();

    // run the scheduled test(s) (we are the child process)
    unsigned int failure_count = g_root_directory->RunScheduled();

    return (failure_count > 0) ? 1 : 0;
}

void SetExecutableFilename (char const *executable_filename)
{
    g_executable_filename = executable_filename;
    assert(!g_executable_filename.empty() && "executable_filename must be non-empty");
}

int RunScheduled (int argc, char **argv, char **envp, Directory &root_directory)
{
    SetExecutableFilename(argv[0]);
    g_root_directory = &root_directory;

#if defined(_WIN32)
    // TEMP HACK: just call ChildMain
    return Runner(argc, argv, envp);
#else // !defined(_WIN32)
    return SpawnerMain(argc, argv, envp, Spawner, Runner);
#endif
}

int FailAssert (std::string const &description)
{
    ostringstream out;
    out << "assertion `" << description << "` failed" << endl;
#if !defined(_WIN32)
    // skip the top 2 (framework) stack frames, as they are unnecessary.
    PrintCallstack(out, g_executable_filename.c_str(), CHOMP_DOT_DOT, 2);
#endif
    g_premade_caught_signal_message = out.str();
    // raise the SIGABRT signal
    abort();
    return 0;
}

// ///////////////////////////////////////////////////////////////////////////
// Lvd::TestSystem::Context::DataBaseclass
// ///////////////////////////////////////////////////////////////////////////

Context::DataBaseclass::~DataBaseclass () { }

// ///////////////////////////////////////////////////////////////////////////
// Lvd::TestSystem::Directory
// ///////////////////////////////////////////////////////////////////////////

Directory::Directory ()
    :
    m_parent_directory(NULL),
    m_path("/")
{ }

Directory::~Directory ()
{
    // delete all subdirectories
    for (SubDirectoryMap::iterator it = m_sub_directory_map.begin(),
                                   it_end = m_sub_directory_map.end();
         it != it_end;
         ++it)
    {
        Directory *sub_directory = it->second;
        assert(sub_directory != NULL);
        delete sub_directory;
    }
    m_sub_directory_map.clear();

    // need to go through and delete the data elements of each TestCase
    for (TestCaseMap::iterator it = m_test_case_map.begin(),
                               it_end = m_test_case_map.end();
         it != it_end;
         ++it)
    {
        TestCase &test_case = it->second;
        delete test_case.m_data;
    }
    m_test_case_map.clear();
}

unsigned int Directory::SubordinateTestCaseCount (bool count_only_scheduled) const
{
    unsigned int count = 0;

    for (TestCaseMap::const_iterator it = m_test_case_map.begin(),
                                     it_end = m_test_case_map.end();
         it != it_end;
         ++it)
    {
        TestCase const &test_case = it->second;

        // if we're counting all tests, or, if we're counting only
        // scheduled tests and this one is scheduled, increment the count.
        if (!count_only_scheduled || test_case.m_is_scheduled)
            ++count;
    }

    for (SubDirectoryMap::const_iterator it = m_sub_directory_map.begin(),
                                         it_end = m_sub_directory_map.end();
         it != it_end;
         ++it)
    {
        Directory const &sub_directory = *it->second;
        count += sub_directory.SubordinateTestCaseCount(count_only_scheduled);
    }

    return count;
}

bool Directory::TestPathIsValid (string const &test_path) const
{
    if ((test_path.empty() && m_parent_directory != NULL) || test_path == "/")
        return true;
    if (test_path[0] != '/')
        return false;

    string::size_type end_of_path_fragment =
        min(test_path.length(),
            test_path.find_first_of("/", 1));
    string path_fragment = test_path.substr(1, end_of_path_fragment-1);

    SubDirectoryMap::const_iterator it;
    return (m_test_case_map.find(path_fragment) != m_test_case_map.end() &&
            path_fragment.length() == test_path.length()-1)
           ||
           ((it = m_sub_directory_map.find(path_fragment)) != m_sub_directory_map.end() &&
            it->second->TestPathIsValid(test_path.substr(end_of_path_fragment)));
}

Directory &Directory::GetSubDirectory (
    std::string const &sub_directory_name,
    bool create_if_necessary)
{
    if (sub_directory_name.empty())
        THROW_STRING("TEST CODE PROGRAMMER ERROR in getting subdirectory (name = \"" << sub_directory_name <<
                     "\"): every (non-root) Directory name must be non-empty");
    if (sub_directory_name.find_first_of("/") != string::npos)
        THROW_STRING("TEST CODE PROGRAMMER ERROR in getting subdirectory (name = \"" << sub_directory_name <<
                     "\"): every Directory name must not contain any forwardslashes");
    if (m_test_case_map.find(sub_directory_name) != m_test_case_map.end())
        THROW_STRING("TEST CODE PROGRAMMER ERROR in getting subdirectory (name = \"" << sub_directory_name <<
                     "\"): subdirectory name collision with existing test case");

    SubDirectoryMap::iterator it = m_sub_directory_map.find(sub_directory_name);
    if (it != m_sub_directory_map.end())
        return *it->second;

    if (!create_if_necessary)
        THROW_STRING("TEST CODE PROGRAMMER ERROR in getting subdirectory (name = \"" << sub_directory_name <<
                     "\"): subdirectory doesn't exist, and CREATE_IF_NECESSARY wasn't specified");

    Directory *sub_directory = new Directory(sub_directory_name, *this);
    m_sub_directory_map[sub_directory_name] = sub_directory;
    return *sub_directory;
}

void Directory::AddTestCaseFunction (
    string const &test_case_name,
    Directory::TestCaseFunction TestCaseFunction_,
    Result expected_result,
    Stage expected_stage,
    int expected_signum)
{
    CheckTestCase(test_case_name, expected_result, expected_stage, expected_signum);
    m_test_case_map[test_case_name] = TestCase(true, TestCaseFunction_, NULL, expected_result, expected_stage, expected_signum);
}

void Directory::AddTestCaseFunction (
    string const &test_case_name,
    Directory::TestCaseFunction TestCaseFunction_,
    Context::DataBaseclass const *data,
    Result expected_result,
    Stage expected_stage,
    int expected_signum)
{
    if (data == NULL)
        THROW_STRING("if you don't want data, use the other AddTestCaseFunction");
    CheckTestCase(test_case_name, expected_result, expected_stage, expected_signum);
    m_test_case_map[test_case_name] = TestCase(true, TestCaseFunction_, data, expected_result, expected_stage, expected_signum);
}

void Directory::AddTestCaseMethod (
    string const &test_case_name,
    Directory::TestCaseMethod TestCaseMethod_,
    Result expected_result,
    Stage expected_stage,
    int expected_signum)
{
    CheckTestCase(test_case_name, expected_result, expected_stage, expected_signum);
    m_test_case_map[test_case_name] = TestCase(true, TestCaseMethod_, NULL, expected_result, expected_stage, expected_signum);
}

void Directory::AddTestCaseMethod (
    string const &test_case_name,
    Directory::TestCaseMethod TestCaseMethod_,
    Context::DataBaseclass const *data,
    Result expected_result,
    Stage expected_stage,
    int expected_signum)
{
    if (data == NULL)
        THROW_STRING("if you don't want data, use the other AddTestCaseFunction");
    CheckTestCase(test_case_name, expected_result, expected_stage, expected_signum);
    m_test_case_map[test_case_name] = TestCase(true, TestCaseMethod_, data, expected_result, expected_stage, expected_signum);
}

Directory::Directory (string const &name, Directory &parent)
    :
    m_parent_directory(&parent),
    m_path(parent.m_path + name + "/")
{
    if (name.empty())
        THROW_STRING("only the root directory may have an empty name");
    if (m_parent_directory == NULL)
        THROW_STRING("a non-root directory must have a parent");
    if (name.find_first_of("/") != string::npos)
        THROW_STRING("name must not contain any forwardslashes");
}

unsigned int Directory::SpawnScheduled (char *argv0, char **envp, string const &child_indicator_token)
{
#if defined(_WIN32)
	assert(false && "not implemented in WIN32");
	return 0;
#else // !defined(_WIN32)
    unsigned int failure_count = 0;

    // spawn each scheduled test, one by one, waiting for each result
    // before continuing the loop.
    for (TestCaseMap::iterator it = m_test_case_map.begin(),
                               it_end = m_test_case_map.end();
         it != it_end;
         ++it)
    {
        string const &test_case_name = it->first;
        TestCase const &test_case = it->second;

        if (!test_case.m_is_scheduled)
            continue;

        // spawn one child process per scheduled test case
        string test_path = m_path + test_case_name;
        static int const argc = g_print_debug_messages ? 7 : 6;
        char *argv[7]; // exe -r / -a /path/to/test_case
        argv[0] = argv0;
        argv[1] = const_cast<char *>("-r");
        argv[2] = const_cast<char *>("/");
        argv[3] = const_cast<char *>("-a");
        argv[4] = const_cast<char *>(test_path.c_str());
        argv[5] = const_cast<char *>(g_print_debug_messages ? "-d" : NULL);
        argv[6] = NULL;

        // set the child process running on the current test
        PRINT_DEBUG_MESSAGE("*** SPAWNER DEBUG MESSAGE *** : spawning child process\n");
        int child_to_parent_pipe__read_fd;
        int parent_to_child_pipe__write_fd;
        Pid child_pid = SpawnChild(argc, argv, envp, child_to_parent_pipe__read_fd, parent_to_child_pipe__write_fd, child_indicator_token);

        // the child has printed to its stdout (which goes to our stdin)
        // the result, stage, signum, message length, and message.
        int temp;
        cin >> temp;
        Result actual_result = Result(temp);
        cin >> temp;
        Stage actual_stage = Stage(temp);
        cin >> temp;
        int actual_signum = temp;
        PRINT_DEBUG_MESSAGE("*** SPAWNER DEBUG MESSAGE *** : child process output result = " << ResultString(actual_result)
                                                                            << ", stage = " << StageString(actual_stage)
                                                                            << ", signal = " << SignalString(actual_signum) << '\n');
        string message;
        // there's only a message to read if we didn't get RESULT_NO_ERROR
        if (actual_result != RESULT_NO_ERROR)
        {
            string::size_type message_length;
            cin >> message_length; // "promised" length of message
            cin.get(); // throw away the newline after failure message length
            if (message_length > gs_max_message_length)
            {
                cerr << "incoming message exceeded max length of " << gs_max_message_length << "; only reading first " << gs_max_message_length << " bytes" << endl;
                message_length = gs_max_message_length;
            }
            while (message_length-- > 0)
                message += cin.get();
        }

        // at this point, we're done with the child.
        waitpid(child_pid, NULL, 0);
        // close the pipe fds returned from SpawnChild
        close(child_to_parent_pipe__read_fd);
        close(parent_to_child_pipe__write_fd);

        // record if it was a failure
        if (actual_result != test_case.m_expected_result)
            ++failure_count;

        // generate the report
        if (actual_result == test_case.m_expected_result)
        {
            // cerr << test_path << " passed" << endl;
        }
        else
        {
            cerr << test_path << " FAILED! expected " << ResultString(test_case.m_expected_result);
            switch (test_case.m_expected_result)
            {
                case RESULT_NO_ERROR:
                    break;
                case RESULT_CAUGHT_SIGNAL:
                    cerr << ' ' << SignalString(test_case.m_expected_signum) << " during " << StageString(test_case.m_expected_stage);
                    break;
                case RESULT_UNCAUGHT_EXCEPTION:
                    cerr << " during " << StageString(test_case.m_expected_stage);
                    break;
                default: // this should never happen
                    cerr << " *INVALID*EXPECTED*RESULT* (this indicates an internal Lvd::TestSystem error)";
                    break;
            }
            cerr << " but got " << ResultString(actual_result);
            switch (actual_result)
            {
                case RESULT_NO_ERROR:
                    break;
                case RESULT_CAUGHT_SIGNAL:
                    cerr << " " << SignalString(actual_signum) << " during " << StageString(actual_stage);
                    break;
                case RESULT_UNCAUGHT_EXCEPTION:
                case RESULT_UNSPECIFIED_FAILURE:
                    cerr << " during " << StageString(actual_stage);
                    break;
                default: // this should never happen
                    cerr << " *INVALID*ACTUAL*RESULT* (this indicates an internal Lvd::TestSystem error)";
                    break;
            }
            cerr << endl;

            // print the message to stderr
            if (!message.empty())
            {
                cerr << endl << message;
                if (*message.rbegin() != '\n')
                    cerr << endl;
                cerr << endl;
            }
        }
    }

    for (SubDirectoryMap::iterator it = m_sub_directory_map.begin(),
                                   it_end = m_sub_directory_map.end();
         it != it_end;
         ++it)
    {
        Directory &sub_directory = *it->second;
        failure_count += sub_directory.SpawnScheduled(argv0, envp, child_indicator_token);
    }

    return failure_count;
#endif
}

unsigned int Directory::RunScheduled ()
{
    unsigned int failure_count = 0;

    for (TestCaseMap::iterator it = m_test_case_map.begin(),
                               it_end = m_test_case_map.end();
         it != it_end;
         ++it)
    {
        string const &test_case_name = it->first;
        TestCase const &test_case = it->second;

        if (!test_case.m_is_scheduled)
            continue;

        Result result = RESULT_NO_ERROR;
        g_test_stage = STAGE_FRAMEWORK_INTERNAL_PRE_INITIALIZE;
        string message;

        g_premade_caught_signal_message.clear();

        // set up a Context object for each test case
        Context context(test_case_name, *this, test_case.m_data);

        PRINT_DEBUG_MESSAGE("*** RUNNER DEBUG MESSAGE *** : RUNNING TEST CASE \"" << Path() << test_case_name << "\"\n");

        // if the test case is a method, then we have to run Initialize and
        // Shutdown before and after the test body, respectively.
        if (test_case.m_is_method)
        {
            // run initialize
            g_test_stage = STAGE_INITIALIZE;
            PRINT_DEBUG_MESSAGE("*** RUNNER DEBUG MESSAGE *** : STAGE_INITIALIZE... ");
            try
            {
                Initialize(context);
                g_test_stage = STAGE_FRAMEWORK_INTERNAL_POST_INITIALIZE;
                PRINT_DEBUG_MESSAGE("succeeded\n");
            }
            catch (exception &e)
            {
                result = RESULT_UNCAUGHT_EXCEPTION;
                g_test_stage = STAGE_INITIALIZE;
                message = e.what();
                PRINT_DEBUG_MESSAGE("uncaught exception: " << e.what() << '\n');
            }
            catch (...)
            {
                result = RESULT_UNCAUGHT_EXCEPTION;
                g_test_stage = STAGE_INITIALIZE;
                PRINT_DEBUG_MESSAGE("uncaught exception (of unknown type)\n");
            }

            // if initialize didn't fail, run the test case
            if (result == RESULT_NO_ERROR)
            {
                g_test_stage = STAGE_TEST_BODY;
                PRINT_DEBUG_MESSAGE("*** RUNNER DEBUG MESSAGE *** : STAGE_TEST_BODY... ");
                try
                {
                    (this->*test_case.m_TestCaseMethod)(context);
                    g_test_stage = STAGE_FRAMEWORK_INTERNAL_POST_TEST_BODY;
                    PRINT_DEBUG_MESSAGE("succeeded\n");
                }
                catch (exception &e)
                {
                    result = RESULT_UNCAUGHT_EXCEPTION;
                    g_test_stage = STAGE_TEST_BODY;
                    message = e.what();
                    PRINT_DEBUG_MESSAGE("uncaught exception: " << e.what() << '\n');
                }
                catch (...)
                {
                    result = RESULT_UNCAUGHT_EXCEPTION;
                    g_test_stage = STAGE_TEST_BODY;
                    PRINT_DEBUG_MESSAGE("uncaught exception (of unknown type)\n");
                }
            }

            // run shutdown, only recording a failure if nothing failed before
            g_test_stage = STAGE_SHUTDOWN;
            PRINT_DEBUG_MESSAGE("*** RUNNER DEBUG MESSAGE *** : STAGE_SHUTDOWN... ");
            try
            {
                Shutdown(context);
                g_test_stage = STAGE_COMPLETED;
                PRINT_DEBUG_MESSAGE("succeeded\n");
            }
            catch (exception &e)
            {
                if (result == RESULT_NO_ERROR)
                {
                    result = RESULT_UNCAUGHT_EXCEPTION;
                    g_test_stage = STAGE_SHUTDOWN;
                    message = e.what();
                    PRINT_DEBUG_MESSAGE("uncaught exception: " << e.what() << '\n');
                }
            }
            catch (...)
            {
                if (result == RESULT_NO_ERROR)
                {
                    result = RESULT_UNCAUGHT_EXCEPTION;
                    g_test_stage = STAGE_SHUTDOWN;
                    PRINT_DEBUG_MESSAGE("uncaught exception (of unknown type)\n");
                }
            }
        }
        // a function test case has no Initialize or Shutdown, only test body.
        else
        {
            g_test_stage = STAGE_TEST_BODY;
            PRINT_DEBUG_MESSAGE("*** RUNNER DEBUG MESSAGE *** : STAGE_TEST_BODY... ");
            try
            {
                (test_case.m_TestCaseFunction)(context);
                g_test_stage = STAGE_COMPLETED;
                PRINT_DEBUG_MESSAGE("succeeded\n");
            }
            catch (exception &e)
            {
                result = RESULT_UNCAUGHT_EXCEPTION;
                g_test_stage = STAGE_TEST_BODY;
                message = e.what();
                PRINT_DEBUG_MESSAGE("uncaught exception: " << e.what() << '\n');
            }
            catch (...)
            {
                result = RESULT_UNCAUGHT_EXCEPTION;
                g_test_stage = STAGE_TEST_BODY;
                PRINT_DEBUG_MESSAGE("uncaught exception (of unknown type)\n");
            }
        }

        // this will only happen for non-fatal failures (i.e. uncaught exceptions).
        // signals (e.g. SIGABRT, SIGSEGV) will be handled separately, because their
        // error messages must be printed immediately, as they cause the process to
        // terminate.
        if (result != RESULT_NO_ERROR)
        {
            ++failure_count;
            PRINT_DEBUG_MESSAGE("*** RUNNER DEBUG MESSAGE *** : recorded failure (failure count = " << failure_count << ")\n");
        }

        EmitStatusMessage(result, g_test_stage, 0, message);
    }

    for (SubDirectoryMap::iterator it = m_sub_directory_map.begin(),
                                   it_end = m_sub_directory_map.end();
         it != it_end;
         ++it)
    {
        Directory &sub_directory = *it->second;
        failure_count += sub_directory.RunScheduled();
    }

    return failure_count;
}

void Directory::SetIsScheduled (std::string const &test_path, bool is_scheduled)
{
    assert(TestPathIsValid(test_path));

    if (test_path.empty() || test_path == "/")
    {
        SetAllSubordinateIsScheduled(is_scheduled);
        return;
    }

    string::size_type end_of_path_fragment =
        min(test_path.length(),
            test_path.find_first_of("/", 1));
    string path_fragment = test_path.substr(1, end_of_path_fragment-1);

    SubDirectoryMap::iterator sub_directory_it = m_sub_directory_map.find(path_fragment);
    if (sub_directory_it != m_sub_directory_map.end())
    {
        Directory &sub_directory = *sub_directory_it->second;
        sub_directory.SetIsScheduled(test_path.substr(end_of_path_fragment), is_scheduled);
        return;
    }

    TestCaseMap::iterator test_case_it = m_test_case_map.find(path_fragment);
    assert(test_case_it != m_test_case_map.end());
    test_case_it->second.m_is_scheduled = is_scheduled;
}

void Directory::Print (ostream &out, string const &path_prefix) const
{
    if (m_test_case_map.empty() && m_sub_directory_map.empty())
        out << m_path << endl;

    for (TestCaseMap::const_iterator it = m_test_case_map.begin(),
                                     it_end = m_test_case_map.end();
         it != it_end;
         ++it)
    {
        string const &test_case_name = it->first;
        TestCase const &test_case = it->second;
        if (test_case.m_is_scheduled)
            out << path_prefix << test_case_name << endl;
    }

    for (SubDirectoryMap::const_iterator it = m_sub_directory_map.begin(),
                                         it_end = m_sub_directory_map.end();
         it != it_end;
         ++it)
    {
        string const &sub_directory_name = it->first;
        Directory &sub_directory = *it->second;
        sub_directory.Print(out, path_prefix + sub_directory_name + "/");
    }
}

void Directory::SetAllSubordinateIsScheduled (bool is_scheduled)
{
    for (TestCaseMap::iterator it = m_test_case_map.begin(),
                               it_end = m_test_case_map.end();
         it != it_end;
         ++it)
    {
        TestCase &test_case = it->second;
        test_case.m_is_scheduled = is_scheduled;
    }

    for (SubDirectoryMap::iterator it = m_sub_directory_map.begin(),
                                   it_end = m_sub_directory_map.end();
         it != it_end;
         ++it)
    {
        Directory &sub_directory = *it->second;
        sub_directory.SetAllSubordinateIsScheduled(is_scheduled);
    }
}

void Directory::CheckTestCase (
    string const &test_case_name,
    Result expected_result,
    Stage expected_stage,
    int expected_signum)
{
    bool error_encountered = false;
    if (test_case_name.empty())
    {
        cerr << "TEST CODE PROGRAMMER ERROR in adding a test case: every test case name must be non-empty\n";
        error_encountered = true;
    }
    if (test_case_name.find_first_of("/") != string::npos)
    {
        cerr << "TEST CODE PROGRAMMER ERROR in adding a test case: every test case name must not contain any forwardslashes\n";
        error_encountered = true;
    }
    if (m_sub_directory_map.find(test_case_name) != m_sub_directory_map.end())
    {
        cerr << "TEST CODE PROGRAMMER ERROR in adding a test case: test case name collision with existing subdirectory \""
             << test_case_name << "\"\n";
        error_encountered = true;
    }
    if (m_test_case_map.find(test_case_name) != m_test_case_map.end())
    {
        cerr << "TEST CODE PROGRAMMER ERROR in adding a test case: test case name collision with existing test case \""
             << test_case_name << "\"\n";
        error_encountered = true;
    }
    if (expected_result == RESULT_UNSPECIFIED_FAILURE)
    {
        cerr << "TEST CODE PROGRAMMER ERROR in adding a test case: RESULT_UNSPECIFIED_FAILURE is not a valid expected_result\n";
        error_encountered = true;
    }
    if (expected_result == RESULT_NO_ERROR)
    {
        if (expected_stage != STAGE_COMPLETED)
        {
            cerr << "TEST CODE PROGRAMMER ERROR in adding a test case: if expected_result is RESULT_NO_ERROR, then expected_stage must be STAGE_COMPLETED\n";
            error_encountered = true;
        }
        if (expected_signum != 0)
        {
            cerr << "TEST CODE PROGRAMMER ERROR in adding a test case: if expected_result is RESULT_NO_ERROR, then expected_signum must be 0\n";
            error_encountered = true;
        }
    }
    else if (expected_result == RESULT_CAUGHT_SIGNAL)
    {
        if (!(expected_stage == STAGE_INITIALIZE || expected_stage == STAGE_TEST_BODY || expected_stage == STAGE_SHUTDOWN))
        {
            cerr << "TEST CODE PROGRAMMER ERROR in adding a test case: if expected_result is RESULT_CAUGHT_SIGNAL, then expected_stage must be one of STAGE_INITIALIZE, STAGE_TEST_BODY, or STAGE_SHUTDOWN\n";
            error_encountered = true;
        }
        if (expected_signum == 0)
        {
            cerr << "TEST CODE PROGRAMMER ERROR in adding a test case: if expected_result is RESULT_CAUGHT_SIGNAL, then expected_signum must not be 0\n";
            error_encountered = true;
        }
    }
    else if (expected_result == RESULT_UNCAUGHT_EXCEPTION)
    {
        if (!(expected_stage == STAGE_INITIALIZE || expected_stage == STAGE_TEST_BODY || expected_stage == STAGE_SHUTDOWN))
        {
            cerr << "TEST CODE PROGRAMMER ERROR in adding a test case: if expected_result is RESULT_UNCAUGHT_EXCEPTION, then expected_stage must be one of STAGE_INITIALIZE, STAGE_TEST_BODY, or STAGE_SHUTDOWN\n";
            error_encountered = true;
        }
        if (expected_signum != 0)
        {
            cerr << "TEST CODE PROGRAMMER ERROR in adding a test case: if expected_result is RESULT_UNCAUGHT_EXCEPTION, then expected_signum must be 0\n";
            error_encountered = true;
        }
    }
    assert(!error_encountered && "the test case check failed (see stderr output for messages)");
}

} // end of namespace TestSystem
} // end of namespace Lvd
