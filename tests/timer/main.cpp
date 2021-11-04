#include <NOTGate.hpp>
#include <Timer.hpp>
#include <gtest/gtest.h>
#include <thread>
#include <chrono>

TEST(TimerTest, APIs)
{
    component::Timer timer{10};
    gate::NOTGate notGate;
    notGate.compute();

    EXPECT_EQ(notGate.output(0)->value(), gate::PinState::High);

    timer.connect(notGate.input(0));

    std::thread schedThread([] () {
        sched::waitTillSteady();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    timer.halt();

    schedThread.join();
}
