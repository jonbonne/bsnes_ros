#include <iostream>

#include "fugasichi_server_core/transform_manager.hpp"
#include "fugasichi_server_core/zone_manager.hpp"

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto zm = std::make_shared<universal_state::ZoneManager>("TestZM");
    if (zm->initialize())
    {
        while (rclcpp::ok())
        {
            //std::cout << "cool bro\n";
            rclcpp::spin_some(zm);
        }
        zm->teardown();
    }
    rclcpp::shutdown();
    return 0;
}
