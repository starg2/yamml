
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
#include "file.hpp"
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
        ("version", "print version info")
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

    try
    {
#ifdef _WIN32

        auto args = PO::split_winmain(YAMML::Driver::W2UTF8(::GetCommandLineW()));
        std::vector<char*> argPointers(args.size());
        std::transform(args.begin(), args.end(), argPointers.begin(), [] (auto&& x) { return &x[0]; });

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

        if (vm.count("help") || !vm.count("input"))
        {
            std::cout << "Usage: yamml [<options>...] <input_file> [<output_file>]\n\n";
            std::cout << visibleOptions << std::endl;
            return 0;
        }

        if (vm.count("version"))
        {
#if YAMML_VERSION_DATE + 0
            std::cout << "YAMML v" << YAMML_VERSION_MAJOR << "." << YAMML_VERSION_MINOR << "." << YAMML_VERSION_DATE << "." << YAMML_VERSION_BUILD << std::endl;
#else
            std::cout << "YAMML dev" << std::endl;
#endif // YAMML_VERSION_DATE + 0

            std::cout << "Copyright (C) 2016 Starg." << std::endl;
            return 0;
        }

        std::cout.flush();
    }
    catch (const YAMML::Driver::FileOpenException& e)
    {
        std::cout << "Unable to open file '" << e.FilePath << "'" << std::endl;
    }
    catch (const YAMML::Driver::IOException&)
    {
        std::cout << "Unable to read/write file" << std::endl;
    }
    catch (const PO::unknown_option& e)
    {
        std::cout << "Unknown option: " << e.get_option_name() << std::endl;
    }
    catch (const PO::error&)
    {
        std::cout << "Invalid command line" << std::endl;
    }
    catch (const std::exception&)
    {
        std::cout << "Unknown error" << std::endl;
    }

    return 0;
}
