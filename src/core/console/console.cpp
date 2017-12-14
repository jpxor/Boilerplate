

#include <stdexcept>
#include <iostream>

#include "util/strutil.h"
#include "window/window.h"
#include "console/console.h"
#include "events/eventdispatch.h"
#include "events/eventenums.h"


using namespace Events; 

namespace Console
{

void listCommands()
{
    std::cout << "help" << std::endl;
    std::cout << "exit" << std::endl;
}

void parseCommand(std::string cmd)
{
}

void start()
{
    std::cout << "Console Started" << std::endl;
    std::cout << "Enter Command:" << std::endl;
    while (true)
    {
        try
        {
            std::string cmd;
            getline(std::cin, cmd);
            if (StringUtil::startswith("exit", cmd))
            {
                Events::dispatch_now(Window_Event, std::make_unique<Event>(WINDOW_CLOSE));
                break;
            }
            if (StringUtil::startswith("help", cmd))
            {
                listCommands();
                continue;
            }
            parseCommand(cmd);
        }
        catch (const std::exception &e)
        {
            std::cout << "Exception Caught: " << e.what() << std::endl;
        }
    }
}
}