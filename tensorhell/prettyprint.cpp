#include <iostream>
#include <sstream>
#include <string>

#include "tenh/meta/typestringof.hpp"

using namespace std;
using namespace Tenh;

int main (int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "usage: " << argv[0] << " <shortify-depth>\n"
                "    Pretty-prints each line of input from stdin, meaning that template typenames\n"
                "    are printed as an indented hierarchy.  The required shortify-depth argument\n"
                "    indicates the depth to which the hierarchy will be collapsed (a value of 0\n"
                "    causes no collapsing).  Note that the bash command for redirecting stderr\n"
                "    to stdout (combining them) is `program 2>&1`, from which stdout can be piped\n"
                "    into another program, such as this one.\n";
    }
    else
    {
        Uint32 shortify_depth = 0;
        {
            istringstream in(argv[1]);
            in >> shortify_depth;
        }
        static Uint32 const BUFFER_SIZE = 0x10000;
        char buffer[BUFFER_SIZE];
        while (!cin.eof())
        {
            cin.getline(&buffer[0], BUFFER_SIZE, '\n');
            print_pretty_typestring(cout, buffer, 0, 4, shortify_depth);
            cout << '\n';
        }
    }

    return 0;
}
