// ///////////////////////////////////////////////////////////////////////////
// lvd_commandlineparser.cpp by Victor Dods, created 2006/02/18
// Copyright Victor Dods, licensed for use by Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "lvd_commandlineparser.hpp"

using namespace std;

namespace Lvd {

void CommandLineParser::Parse (int argc, char const *const *argv, std::ostream &out)
{
    assert(argc >= 1);
    assert(argv != NULL);

    try
    {
        while (++argv, --argc > 0) // iterate through each commandline token
        {
            assert(*argv != NULL);
            char const *token = *argv;

            // if the token begins with a hyphen and is not "-" or "--",
            // handle using the corresponding short or long named option.
            if (*token == '-' && token != string("-") && token != string("--"))
            {
                ++token; // skip the hyphen.

                char const *next_token = (argc == 1) ? NULL : *(argv + 1);

                // if there is a second hyphen before the option name,
                // this is a long option -- so skip the second hyphen.
                bool is_long_name_option = *token == '-';
                if (is_long_name_option)
                    ++token;
                // handle the option whose name is given by token.  if
                // next_token was used up during option handling (indicated
                // by a return value of true), skip next_token.
                if (HandleOption(is_long_name_option, token, next_token))
                    ++argv, --argc;
            }
            else
            {
                (this->*m_NonOptionArgumentHandlerMethod)(token);
            }
        }
    }
    catch (string const &exception)
    {
        // output any caught error messages.
        out << "error: " << exception << endl;
        m_status = EXIT_WITH_ERROR;
    }
}

void CommandLineParser::PrintHelpMessage (ostream &out) const
{
    if (!m_program_description.empty())
        out << m_program_description << "\n" << endl;

    out << "usage: " << m_executable_name << " " << m_argument_text << endl << endl;

    bool just_printed_a_section_header = true;
    for (CommandLineOption const *option = m_option,
                                 *option_end = m_option + m_option_count;
         option != option_end;
         ++option)
    {
        if (option->IsAControlOption())
        {
            if (option->m_description.empty())
                out << endl;
            else
                out << (just_printed_a_section_header ? "" : "\n") << option->m_description << "\n" << endl;
            just_printed_a_section_header = true;
        }
        else
        {
            assert(option->m_short_name != '\0' || !option->m_long_name.empty());

            if (option->m_short_name != '\0')
                out << "-" << option->m_short_name;

            if (option->m_short_name != '\0' && !option->m_long_name.empty())
                out << ",";

            if (!option->m_long_name.empty())
                out << "--" << option->m_long_name;

            if (option->m_handler_method_with_argument != NULL)
                out << " <argument>";
            else if (option->m_handler_method_with_optional_argument != NULL)
                out << " [argument]";

            if (!option->m_description.empty())
                out << "\n" << option->m_description;

            out << endl;

            just_printed_a_section_header = false;
        }
    }
}

bool CommandLineParser::HandleOption (bool is_long_name_option, char const *token, char const *const next_token)
{
    assert(token != NULL);
    assert(*token != '\0');
    // next_token may be NULL

    // a short-name option must be solitary (e.g. "-a" and not "-ab")
    if (!is_long_name_option && *(token+1) != '\0')
        throw string("invalid short option: -") + token;

    CommandLineOption const *option = FindOptionByName(is_long_name_option, token);
    if (option == NULL)
    {
        if (is_long_name_option)
            throw string("no such long option: --") + token;
        else
            throw string("no such short option: -") + token;
    }

    if (option->m_handler_method_with_argument != NULL) // argument required
    {
        if (next_token == NULL)
        {
            if (is_long_name_option)
                throw string("option --") + option->m_long_name + " requires an argument";
            else
                throw string("option -") + option->m_short_name + " requires an argument";
        }

        (this->*(option->m_handler_method_with_argument))(string(next_token));
        return true; // next_token was used up.
    }
    else if (option->m_handler_method_with_optional_argument != NULL) // optional argument
    {
        // if the next token is not present, or it's an option ("-" and "--"
        // are not considered options), call the handler without the argument.
        if (next_token == NULL || (*next_token == '-' && next_token != string("-") && next_token != string("--")))
        {
            (this->*(option->m_handler_method_with_optional_argument))(false, string());
            return false; // next_token was NOT used up.
        }
        else
        {
            (this->*(option->m_handler_method_with_optional_argument))(true, string(next_token));
            return true; // next_token was used up.
        }
    }
    else // no argument
    {
        assert(option->m_handler_method_without_argument != NULL);
        (this->*(option->m_handler_method_without_argument))();
        return false; // next_token was NOT used up.
    }
}

CommandLineOption const *CommandLineParser::FindOptionByName (bool is_long_name_option, char const *name) const
{
    assert(name != NULL);
    assert(*name != '\0');
    // a short-name option must be a single non-hyphen character.
    if (!is_long_name_option)
    {
        assert(*name != '-');
        assert(*(name+1) == '\0');
    }
    // check each option for a match
    for (CommandLineOption const *option = m_option,
                                 *option_end = m_option + m_option_count;
         option != option_end;
         ++option)
    {
        // if the appropriate name matched, return the option.
        if ((is_long_name_option && !option->m_long_name.empty() && name == option->m_long_name)
            ||
            (!is_long_name_option && option->m_short_name != '\0' && *name == option->m_short_name))
        {
            return option;
        }
    }
    // if no option matched, return NULL
    return NULL;
}

void CommandLineParser::CheckForOptionNameCollisions () const
{
    bool is_a_short_name_collision, is_a_long_name_collision;
    // make sure that there are no short/long option name collisions.
    for (CommandLineOption const *option_0 = m_option,
                                 *option_end_0 = m_option + m_option_count;
         option_0 != option_end_0;
         ++option_0)
    {
        // loop over all options after this one in the array, so that
        // every option is compared with every other option.
        for (CommandLineOption const *option_1 = option_0 + 1;
             option_1 != option_end_0;
             ++option_1)
        {
            is_a_short_name_collision =
                !option_0->IsAControlOption() && // control options can't collide
                !option_1->IsAControlOption() && // control options can't collide
                option_0->m_short_name != '\0' && // a non-existent name can't collide
                option_0->m_short_name == option_1->m_short_name; // only return true if they're equal.
            is_a_long_name_collision =
                !option_0->IsAControlOption() && // control options can't collide
                !option_1->IsAControlOption() && // control options can't collide
                !option_0->m_long_name.empty() && // a non-existent name can't collide
                option_0->m_long_name == option_1->m_long_name; // only return true if they're equal.
            assert(!is_a_short_name_collision && "option short-name collision");
            assert(!is_a_long_name_collision && "option long-name collision");
        }
    }
}

} // end of namespace Lvd
