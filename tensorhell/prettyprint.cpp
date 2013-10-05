#include <iostream>
#include <sstream>
#include <string>

#include "tenh/meta/typestringof.hpp"

using namespace std;
using namespace Tenh;

int main (int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "usage: " << argv[0] << " <shortify-depth> [indent-size]\n"
                "    Pretty-prints each line of input from stdin, meaning that template typenames\n"
                "    are printed as an indented hierarchy.\n"
                "\n"
                "    The required shortify-depth argument indicates the depth to which the hierarchy\n"
                "    will be collapsed (a value of 0 causes no collapsing).\n"
                "\n"
                "    The optional indent-size argument controls the number of spaces in each indent.\n"
                "    The default value is 4 spaces.\n"
                "\n"
                "    Note that the bash command for redirecting stderr to stdout (combining them)\n"
                "    is `program 2>&1`, from which stdout can be piped into another program, such as\n"
                "    this one.\n";
    }
    else
    {
        Uint32 shortify_depth = 0;
        {
            istringstream in(argv[1]);
            in >> shortify_depth;
        }
        Uint32 indent_size = 4;
        if (argc >= 3)
        {
            istringstream in(argv[2]);
            in >> indent_size;
        }
        static Uint32 const BUFFER_SIZE = 0x10000;
        char buffer[BUFFER_SIZE];
        while (!cin.eof())
        {
            cin.getline(&buffer[0], BUFFER_SIZE, '\n');
            print_pretty_typestring(cout, buffer, shortify_depth, indent_size);
            cout << '\n';
        }
    }

    return 0;
}
