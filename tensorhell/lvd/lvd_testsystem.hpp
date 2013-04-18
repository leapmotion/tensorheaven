// ///////////////////////////////////////////////////////////////////////////
// lvd_testsystem.hpp by Victor Dods, created 2007/02/10
// Copyright Victor Dods, licensed for use by Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#if !defined(LVD_TESTSYSTEM_HPP_)
#define LVD_TESTSYSTEM_HPP_

// ///////////////////////////////////////////////////////////////////////////
// NOTE ! NOTE ! NOTE ! NOTE ! NOTE ! NOTE ! NOTE ! NOTE ! NOTE ! NOTE ! NOTE
// ///////////////////////////////////////////////////////////////////////////
// This file should be included after all includes because the 'assert' macro
// is redefined in this header file, which would be bad for (system) headers.
// ///////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <limits>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <complex>

#include "tenh/static_scalar_constants.hpp"

#define LVD_ADD_TEST_CASE_FUNCTION(directory, TestFunction, ...) \
    (directory)->AddTestCaseFunction(#TestFunction, TestFunction, __VA_ARGS__)
#define LVD_ADD_NAMED_TEST_CASE_FUNCTION(directory, test_case_name, TestFunction, ...) \
    (directory)->AddTestCaseFunction(test_case_name, TestFunction, __VA_ARGS__)

#define LVD_ADD_TEST_CASE_METHOD(DirectorySubclass, TestMethod, ...) \
    AddTestCaseMethod(#TestMethod, static_cast<Lvd::TestSystem::Directory::TestCaseMethod>(&DirectorySubclass::TestMethod), __VA_ARGS__)
#define LVD_ADD_NAMED_TEST_CASE_METHOD(DirectorySubclass, test_case_name, TestMethod, ...) \
    AddTestCaseMethod(test_case_name, static_cast<Lvd::TestSystem::Directory::TestCaseMethod>(&DirectorySubclass::TestMethod), __VA_ARGS__)

namespace Lvd {
namespace TestSystem {

enum Result
{
    RESULT_NO_ERROR = 0,
    RESULT_CAUGHT_SIGNAL,
    RESULT_UNCAUGHT_EXCEPTION,
    RESULT_UNSPECIFIED_FAILURE
};

enum Stage
{
    STAGE_FRAMEWORK_INTERNAL_PRE_INITIALIZE = 0,
    STAGE_INITIALIZE,
    STAGE_FRAMEWORK_INTERNAL_POST_INITIALIZE,
    STAGE_TEST_BODY,
    STAGE_FRAMEWORK_INTERNAL_POST_TEST_BODY,
    STAGE_SHUTDOWN,
    STAGE_COMPLETED
};

struct Directory;
struct Options;

// use this to set the executable filename (necessary for printing
// out the call stack when something goes wrong).
void SetExecutableFilename (char const *executable_filename);
// this is the method you call to start the test system running,
// which will run all scheduled test cases.  just pass in the argc,
// argv and envp arguments from main (along with the root directory).
int RunScheduled (int argc, char **argv, char **envp, Directory &root_directory);

// provides useful context info for each test case that runs.  also, the Data<>
// type provides a way to specify parameter data for a particular test case.
struct Context
{
    struct DataBaseclass
    {
        virtual ~DataBaseclass () = 0;
    }; // end of struct Context::DataBaseclass

    template <typename DataType>
    struct Data : public DataBaseclass
    {
        DataType m;

        operator DataType () { return m; }

        Data (DataType const &data) : m(data) { }
    }; // end of struct Context::Data<DataType>

    Context (std::string const &test_case_name, Directory const *directory, DataBaseclass const *data)
        :
        m_test_case_name(test_case_name),
        m_directory(directory),
        m_data(data)
    {
        assert(!m_test_case_name.empty());
        assert(m_directory != NULL);
    }

    std::string const &TestCaseName () const { return m_test_case_name; }
    Directory const *CurrentDirectory () const { return m_directory; }
    DataBaseclass const *Base () const { return m_data; }
    template <typename DataType>
    DataType const &DataAs () const
    {
        assert(m_data != NULL);
        assert(dynamic_cast<Data<DataType> const *>(m_data) != NULL);
        return dynamic_cast<Data<DataType> const *>(m_data)->m;
    }

private:

    std::string m_test_case_name;
    Directory const *m_directory;
    DataBaseclass const *m_data;
}; // end of struct Context

// organizes test cases hierarchically
struct Directory
{
    typedef void (*TestCaseFunction)(Context const &context);
    typedef void (Directory::*TestCaseMethod)(Context const &context);

    // default constructor for creating a root directory
    Directory ();
    // this constructor is for creating a non-root directory
    Directory (std::string const &name, Directory *parent);
    // deletes all subdirectories
    virtual ~Directory ();

    // this method will be called before running each test.  do not put test
    // code in this method -- only put state-preparing code here; code that is
    // meant to establish a consistent state for the test(s) to run within
    virtual void Initialize (Context const &context) { }
    // this method will be called after running each test, regardless of if
    // the test passed or failed.  do not put test code in this method -- only
    // put state-cleaning code here; code that is meant to clean up any mess
    // left behind by the test(s).
    virtual void Shutdown (Context const &context) { }

    Directory const *Parent () const { return m_parent_directory; }
    unsigned int SubordinateTestCaseCount (bool count_only_scheduled = false) const;
    bool TestPathIsValid (std::string const &test_path) const;
    void Print (std::ostream &out) const { Print(out, "/"); }

    // don't call these methods directly -- use the LVD_ADD_TEST_CASE_FUNCTION
    // or the LVD_ADD_NAMED_TEST_CASE_FUNCTION macros instead.
    void AddTestCaseFunction (
        std::string const &test_case_name,
        TestCaseFunction TestCaseFunction_,
        Result expected_result,
        Stage expected_stage = STAGE_COMPLETED,
        int expected_signum = 0);
    void AddTestCaseFunction (
        std::string const &test_case_name,
        TestCaseFunction TestCaseFunction_,
        Context::DataBaseclass const *data,
        Result expected_result,
        Stage expected_stage = STAGE_COMPLETED,
        int expected_signum = 0);
    // don't call these methods directly -- use the LVD_ADD_TEST_CASE_METHOD
    // or the LVD_ADD_NAMED_TEST_CASE_METHOD macros instead.
    void AddTestCaseMethod (
        std::string const &test_case_name,
        TestCaseMethod TestCaseMethod_,
        Result expected_result,
        Stage expected_stage = STAGE_COMPLETED,
        int expected_signum = 0);
    void AddTestCaseMethod (
        std::string const &test_case_name,
        TestCaseMethod TestCaseMethod_,
        Context::DataBaseclass const *data,
        Result expected_result,
        Stage expected_stage = STAGE_COMPLETED,
        int expected_signum = 0);

private:

    struct TestCase
    {
        bool m_is_scheduled;
        TestCaseMethod m_TestCaseMethod;
        TestCaseFunction m_TestCaseFunction;
        bool m_is_method;
        Context::DataBaseclass const *m_data;
        Result m_expected_result;
        Stage m_expected_stage;
        int m_expected_signum;

        TestCase () : m_data(NULL) { }
        TestCase (
            bool is_scheduled,
            TestCaseFunction TestCaseFunction_,
            Context::DataBaseclass const *data,
            Result expected_result,
            Stage expected_stage,
            int expected_signum)
            :
            m_is_scheduled(is_scheduled),
            m_TestCaseFunction(TestCaseFunction_),
            m_is_method(false),
            m_data(data),
            m_expected_result(expected_result),
            m_expected_stage(expected_stage),
            m_expected_signum(expected_signum)
        { }
        TestCase (
            bool is_scheduled,
            TestCaseMethod TestCaseMethod_,
            Context::DataBaseclass const *data,
            Result expected_result,
            Stage expected_stage,
            int expected_signum)
            :
            m_is_scheduled(is_scheduled),
            m_TestCaseMethod(TestCaseMethod_),
            m_is_method(true),
            m_data(data),
            m_expected_result(expected_result),
            m_expected_stage(expected_stage),
            m_expected_signum(expected_signum)
        { }
    }; // end of struct TestCase

    unsigned int SpawnScheduled (char *argv0, char **envp, std::string const &child_indicator_token);
    unsigned int RunScheduled ();
    void SetIsScheduled (std::string const &test_path, bool is_scheduled);
    void AddSubDirectory (std::string const &sub_directory_name, Directory &sub_directory);
    void Print (std::ostream &out, std::string const &path_prefix) const;
    void SetAllSubordinateIsScheduled (bool is_scheduled);
    void CheckTestCase (std::string const &test_case_name, Result expected_result, Stage expected_stage, int expected_signum);

    typedef std::map<std::string, Directory *> SubDirectoryMap;
    typedef std::map<std::string, TestCase> TestCaseMap;

    Directory *const m_parent_directory;
    SubDirectoryMap m_sub_directory_map;
    TestCaseMap m_test_case_map;
    std::string m_path;

    // this is the commandline parser class used by the test system
    friend struct Lvd::TestSystem::Options;
    // these are the helper functions involved in spawning child processes
    friend int Spawner (int argc, char **argv, char **envp, std::string const &child_indicator_token);
    friend int Runner (int argc, char **argv, char **envp);
}; // end of struct Directory


//TODO: think more about error bounds.
template <typename T>
bool about_equal(T lhs, T rhs)
{
    T bound = std::max(std::abs(lhs),std::abs(rhs))*8*std::numeric_limits<T>::epsilon();
    
    return std::abs(lhs - rhs) <= bound;
}

template <typename T> // oveload to use the epsilon for the underlying value type for a complex type
bool about_equal(std::complex<T> lhs, std::complex<T> rhs)
{
    T bound = std::max(std::abs(lhs),std::abs(rhs))*8*std::numeric_limits<T>::epsilon();
    
    return std::abs(lhs - rhs) <= bound;
}

// ///////////////////////////////////////////////////////////////////////////
// here's where we redefine the `assert` macro; we also define assert_eq
// ///////////////////////////////////////////////////////////////////////////

#if !defined(assert)
    #error("`assert` macro is missing; this is bad")
#else
    #undef assert
    #define assert(condition) Lvd::TestSystem::Assert(condition, #condition)
#endif // defined(assert)

// prefer these to assert(x == y), so that more descriptive assertion failure
// messages may be generated -- by ostream'ing the values.  if there is no
// ostream << operator defined for the types, use assert_XX_ instead.
#define assert_eq(left, right)  Lvd::TestSystem::AssertComparison(left == right, left, right, #left, "==", #right)
#define assert_about_eq(left, right)  Lvd::TestSystem::AssertComparison(about_equal(left, right), left, right, #left, " about equal ", #right)
#define assert_neq(left, right) Lvd::TestSystem::AssertComparison(left != right, left, right, #left, "!=", #right)
#define assert_lt(left, right)  Lvd::TestSystem::AssertComparison(left < right,  left, right, #left, "<", #right)
#define assert_leq(left, right) Lvd::TestSystem::AssertComparison(left <= right, left, right, #left, "<=", #right)
#define assert_gt(left, right)  Lvd::TestSystem::AssertComparison(left > right,  left, right, #left, ">", #right)
#define assert_geq(left, right) Lvd::TestSystem::AssertComparison(left >= right, left, right, #left, ">=", #right)

// same as above, but no ostream << streaming occurs.
#define assert_eq_(left, right)  Lvd::TestSystem::Assert(left == right, #left "==" #right)
#define assert_about_eq_(left, right)  Lvd::TestSystem::AssertComparison(about_equal(left, right), #left, " about equal ", #right)
#define assert_neq_(left, right) Lvd::TestSystem::Assert(left != right, #left "!=" #right)
#define assert_lt_(left, right)  Lvd::TestSystem::Assert(left < right,  #left "<" #right)
#define assert_leq_(left, right) Lvd::TestSystem::Assert(left <= right, #left "<=" #right)
#define assert_gt_(left, right)  Lvd::TestSystem::Assert(left > right,  #left ">" #right)
#define assert_geq_(left, right) Lvd::TestSystem::Assert(left >= right, #left ">=" #right)

// ///////////////////////////////////////////////////////////////////////////
// don't pay any attention to anything below; you shouldn't use them directly.
// ///////////////////////////////////////////////////////////////////////////

int FailAssert (std::string const &description);

inline int Assert (bool condition, std::string const &description)
{
    if (!condition)
        FailAssert(description);
    return 0;
}

template <typename LeftType, typename RightType>
int AssertComparison (bool condition, LeftType const &left, RightType const &right, std::string const &left_name, std::string const &comparison_name, std::string const &right_name)
{
    if (!condition)
    {
        std::ostringstream out;
        out << "(" << left_name << " = " << left << ") " << comparison_name << " (" << right_name << " = " << right << ")";
        FailAssert(out.str());
    }
    return 0;
}

} // end of namespace TestSystem
} // end of namespace Lvd

#endif // !defined(LVD_TESTSYSTEM_HPP_)
