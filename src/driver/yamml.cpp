
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <boost/program_options.hpp>

#ifdef _WIN32

#include <windows.h>

#include "cconv.hpp"

#endif // _WIN32

#include "driver.hpp"
#include "msgcallback.hpp"
#include "stderrwriter.hpp"

int main(int argc, char** argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    namespace PO = boost::program_options;

    PO::options_description visibleOptions("Options");
    visibleOptions.add_options()
        ("help", "print this help message")
        ("entry", PO::value<std::string>(), "specify entry point")
        ("out", PO::value<std::string>(), "name output file");

    PO::options_description invisibleOptions;
    invisibleOptions.add_options()
        ("input", PO::value<std::string>());

    PO::options_description allOptions;
    allOptions.add(visibleOptions).add(invisibleOptions);

    PO::positional_options_description positional;
    positional.add("input", 1).add("out", 1);

    PO::variables_map vm;

#ifdef _WIN32

    auto args = PO::split_winmain(YAMML::Driver::W2UTF8(::GetCommandLineW()));
    std::vector<char*> argPointers(args.size());
    std::transform(args.begin(), args.end(), argPointers.begin(), [] (auto&& x) { return const_cast<char*>(x.data()); });

    PO::store(
        PO::command_line_parser(argPointers.size(), argPointers.data())
        .options(allOptions)
        .positional(positional)
        .style(
            PO::command_line_style::allow_long
            | PO::command_line_style::allow_short
            | PO::command_line_style::allow_dash_for_short
            | PO::command_line_style::allow_slash_for_short
            | PO::command_line_style::long_allow_adjacent
            | PO::command_line_style::long_allow_next
            | PO::command_line_style::short_allow_adjacent
            | PO::command_line_style::short_allow_next
            | PO::command_line_style::case_insensitive
            | PO::command_line_style::allow_long_disguise
        )
        .run(),
        vm
    );

#else // !_WIN32

    PO::store(
        PO::command_line_parser(argc, argv)
        .options(allOptions)
        .positional(positional)
        .style(
            PO::command_line_style::allow_long
            | PO::command_line_style::allow_short
            | PO::command_line_style::allow_dash_for_short
            | PO::command_line_style::long_allow_adjacent
            | PO::command_line_style::long_allow_next
            | PO::command_line_style::short_allow_adjacent
            | PO::command_line_style::short_allow_next
            | PO::command_line_style::allow_long_disguise
        )
        .run(),
        vm
    );

#endif // !_WIN32

    PO::notify(vm);

    if (vm.count("help"))
    {
        std::cout << "Usage: yamml [<options>...] <input_file> [<output_file>]\n\n";
        std::cout << visibleOptions << std::endl;
        return 0;
    }



    return 0;
}
