#include "lambda_scope.h"

#include <iostream>
#include <functional>
#include <chrono>
#include <thread>

void lambda_scope::start_local_ramda()
{
    const std::string key("end of proc");
    const auto outputTest = [key](std::string tag)
    {
        std::this_thread::sleep_for(std::chrono::seconds(20));
        std::cout << key.c_str() << "[" << tag.c_str() << "]" << std::endl;
    };

    const char* TAGS[] = {
        "2bit",
        "4bot",
        "8bit",
        "16bit",
        "32bit",
        "64bit",
        "128bit",
        "256bit",
        "512bit",
        "1024bit"
    };
    for (size_t inx = 0; inx < sizeof(TAGS)/sizeof(char*); inx++) {
        std::thread t(outputTest, TAGS[inx]);
        t.detach();
    }
}

lambda_scope::lambda_scope()
{
}

lambda_scope::~lambda_scope()
{
}
