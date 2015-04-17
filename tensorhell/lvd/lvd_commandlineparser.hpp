// ///////////////////////////////////////////////////////////////////////////
// lvd_commandlineparser.hpp by Victor Dods, created 2006/02/18
// Copyright Victor Dods, licensed for unrestricted use by Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#if !defined(LVD_COMMANDLINEPARSER_HPP_)
#define LVD_COMMANDLINEPARSER_HPP_

#include <cassert>
#include <iostream>
#include <string>

namespace Lvd {

struct CommandLineOption;

struct CommandLineParser
{
    enum StatusCode { CONTINUE = 0, EXIT_WITH_SUCCESS, EXIT_WITH_ERROR };

    typedef void (CommandLineParser::*HandlerMethodWithArgument)(std::string const &);
    typedef void (CommandLineParser::*HandlerMethodWithOptionalArgument)(bool, std::string const &);
    typedef void (CommandLineParser::*HandlerMethodWithoutArgument)();

    template <typename CommandLineParserSubclass_>
    CommandLineParser (
        void (CommandLineParserSubclass_::*NonOptionArgumentHandlerMethod)(std::string const &),
        CommandLineOption const *option,
        unsigned int option_count,
        std::string const &executable_filename,
        std::string const &program_description,
        std::string const &argument_text)
        :
        m_NonOptionArgumentHandlerMethod(static_cast<HandlerMethodWithArgument>(NonOptionArgumentHandlerMethod)),
        m_option(option),
        m_option_count(option_count),
        m_executable_name(executable_filename),
        m_program_description(program_description),
        m_argument_text(argument_text),
        m_status(CONTINUE)
    {
        assert(m_NonOptionArgumentHandlerMethod != NULL && "you must specify a non-option argument handler");
        assert(m_option != NULL && "you must specify an option table of nonzero length");
        assert(m_option_count > 0 && "you must specify an option table of nonzero length");
        assert(!m_executable_name.empty() && "you must specify an executable name for the help message");
        CheckForOptionNameCollisions();
    }
    virtual ~CommandLineParser () { }

    StatusCode Status () const { return m_status; }

    virtual void Parse (int argc, char const *const *argv, std::ostream &out = std::cerr);
    void PrintHelpMessage (std::ostream &out) const;

protected:

    void Status (StatusCode status) { m_status = status; }

private:

    // the return value indicates if next_arg was eaten up and should be skipped
    bool HandleOption (bool is_long_name_option, char const *token, char const *next_token);
    CommandLineOption const *FindOptionByName (bool is_long_name_option, char const *name) const;
    void CheckForOptionNameCollisions () const;

    HandlerMethodWithArgument const m_NonOptionArgumentHandlerMethod;
    CommandLineOption const *const m_option;
    unsigned int const m_option_count;
    std::string const m_executable_name;
    std::string const m_program_description;
    std::string const m_argument_text;
    StatusCode m_status;
}; // end of struct CommandLineParser

struct CommandLineOption
{
    char const m_short_name;
    std::string const m_long_name;
    CommandLineParser::HandlerMethodWithArgument const m_handler_method_with_argument;
    CommandLineParser::HandlerMethodWithOptionalArgument const m_handler_method_with_optional_argument;
    CommandLineParser::HandlerMethodWithoutArgument const m_handler_method_without_argument;
    std::string const m_description;

    bool IsAControlOption () const
    {
        return m_handler_method_with_argument == NULL &&
               m_handler_method_with_optional_argument == NULL &&
               m_handler_method_without_argument == NULL;
    }

    // constructor for help message section header
    CommandLineOption (std::string const &header_text)
        :
        m_short_name('\0'),
        m_long_name(),
        m_handler_method_with_argument(NULL),
        m_handler_method_with_optional_argument(NULL),
        m_handler_method_without_argument(NULL),
        m_description(header_text)
    { }
    // constructors for options which require a single argument
    template <typename CommandLineParserSubclass_>
    CommandLineOption (
        char short_name,
        void (CommandLineParserSubclass_::*handler_method_with_argument)(std::string const &),
        std::string const &description)
        :
        m_short_name(short_name),
        m_long_name(),
        m_handler_method_with_argument(static_cast<CommandLineParser::HandlerMethodWithArgument>(handler_method_with_argument)),
        m_handler_method_with_optional_argument(NULL),
        m_handler_method_without_argument(NULL),
        m_description(description)
    {
        AssertIsValidShortName();
        assert(m_handler_method_with_argument != NULL && "the argument handler method must not be NULL");
    }
    template <typename CommandLineParserSubclass_>
    CommandLineOption (
        std::string const &long_name,
        void (CommandLineParserSubclass_::*handler_method_with_argument)(std::string const &),
        std::string const &description)
        :
        m_short_name('\0'),
        m_long_name(long_name),
        m_handler_method_with_argument(static_cast<CommandLineParser::HandlerMethodWithArgument>(handler_method_with_argument)),
        m_handler_method_with_optional_argument(NULL),
        m_handler_method_without_argument(NULL),
        m_description(description)
    {
        AssertIsValidLongName();
        assert(m_handler_method_with_argument != NULL && "the argument handler method must not be NULL");
    }
    template <typename CommandLineParserSubclass_>
    CommandLineOption (
        char short_name,
        std::string const &long_name,
        void (CommandLineParserSubclass_::*handler_method_with_argument)(std::string const &),
        std::string const &description)
        :
        m_short_name(short_name),
        m_long_name(long_name),
        m_handler_method_with_argument(static_cast<CommandLineParser::HandlerMethodWithArgument>(handler_method_with_argument)),
        m_handler_method_with_optional_argument(NULL),
        m_handler_method_without_argument(NULL),
        m_description(description)
    {
        AssertIsValidShortName();
        AssertIsValidLongName();
        assert(m_handler_method_with_argument != NULL && "the argument handler method must not be NULL");
    }
    // constructors for options which can take an optional argument
    template <typename CommandLineParserSubclass_>
    CommandLineOption (
        char short_name,
        void (CommandLineParserSubclass_::*handler_method_with_optional_argument)(bool, std::string const &),
        std::string const &description)
        :
        m_short_name(short_name),
        m_long_name(),
        m_handler_method_with_argument(NULL),
        m_handler_method_with_optional_argument(static_cast<CommandLineParser::HandlerMethodWithOptionalArgument>(handler_method_with_optional_argument)),
        m_handler_method_without_argument(NULL),
        m_description(description)
    {
        AssertIsValidShortName();
        assert(m_handler_method_with_optional_argument != NULL && "the argument handler method must not be NULL");
    }
    template <typename CommandLineParserSubclass_>
    CommandLineOption (
        std::string const &long_name,
        void (CommandLineParserSubclass_::*handler_method_with_optional_argument)(bool, std::string const &),
        std::string const &description)
        :
        m_short_name('\0'),
        m_long_name(long_name),
        m_handler_method_with_argument(NULL),
        m_handler_method_with_optional_argument(static_cast<CommandLineParser::HandlerMethodWithOptionalArgument>(handler_method_with_optional_argument)),
        m_handler_method_without_argument(NULL),
        m_description(description)
    {
        AssertIsValidLongName();
        assert(m_handler_method_with_optional_argument != NULL && "the argument handler method must not be NULL");
    }
    template <typename CommandLineParserSubclass_>
    CommandLineOption (
        char short_name,
        std::string const &long_name,
        void (CommandLineParserSubclass_::*handler_method_with_optional_argument)(bool, std::string const &),
        std::string const &description)
        :
        m_short_name(short_name),
        m_long_name(long_name),
        m_handler_method_with_argument(NULL),
        m_handler_method_with_optional_argument(static_cast<CommandLineParser::HandlerMethodWithOptionalArgument>(handler_method_with_optional_argument)),
        m_handler_method_without_argument(NULL),
        m_description(description)
    {
        AssertIsValidShortName();
        AssertIsValidLongName();
        assert(m_handler_method_with_optional_argument != NULL && "the argument handler method must not be NULL");
    }
    // constructors for options which take no arguments
    template <typename CommandLineParserSubclass_>
    CommandLineOption (
        char short_name,
        void (CommandLineParserSubclass_::*handler_method_without_argument)(),
        std::string const &description)
        :
        m_short_name(short_name),
        m_long_name(),
        m_handler_method_with_argument(NULL),
        m_handler_method_with_optional_argument(NULL),
        m_handler_method_without_argument(static_cast<CommandLineParser::HandlerMethodWithoutArgument>(handler_method_without_argument)),
        m_description(description)
    {
        AssertIsValidShortName();
        assert(m_handler_method_without_argument != NULL && "the argument handler method must not be NULL");
    }
    template <typename CommandLineParserSubclass_>
    CommandLineOption (
        std::string const &long_name,
        void (CommandLineParserSubclass_::*handler_method_without_argument)(),
        std::string const &description)
        :
        m_short_name('\0'),
        m_long_name(long_name),
        m_handler_method_with_argument(NULL),
        m_handler_method_with_optional_argument(NULL),
        m_handler_method_without_argument(static_cast<CommandLineParser::HandlerMethodWithoutArgument>(handler_method_without_argument)),
        m_description(description)
    {
        AssertIsValidLongName();
        assert(m_handler_method_without_argument != NULL && "the argument handler method must not be NULL");
    }
    template <typename CommandLineParserSubclass_>
    CommandLineOption (
        char short_name,
        std::string const &long_name,
        void (CommandLineParserSubclass_::*handler_method_without_argument)(),
        std::string const &description)
        :
        m_short_name(short_name),
        m_long_name(long_name),
        m_handler_method_with_argument(NULL),
        m_handler_method_with_optional_argument(NULL),
        m_handler_method_without_argument(static_cast<CommandLineParser::HandlerMethodWithoutArgument>(handler_method_without_argument)),
        m_description(description)
    {
        AssertIsValidShortName();
        AssertIsValidLongName();
        assert(m_handler_method_without_argument != NULL && "the argument handler method must not be NULL");
    }

private:

    void AssertIsValidShortName ()
    {
        assert(m_short_name > ' ' && m_short_name <= '~' && "must only use printable characters in option short names");
        assert(m_short_name != '-' && "must not use '-' (hyphen) in option short names");
    }
    void AssertIsValidLongName ()
    {
        for (std::string::const_iterator it = m_long_name.begin(), it_end = m_long_name.end(); it != it_end; ++it)
            assert(*it > ' ' && *it <= '~' && "must only use printable characters in option long names");
    }
}; // end of struct CommandLineOption

} // end of namespace Lvd

#endif // !defined(LVD_COMMANDLINEPARSER_HPP_)
