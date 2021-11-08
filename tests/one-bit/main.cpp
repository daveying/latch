#include <ORGate.hpp>
#include <NORGate.hpp>
#include <ANDGate.hpp>
#include <NOTGate.hpp>
#include <IScheduler.hpp>

#include <gtest/gtest.h>

TEST(OneBit, TruthTable)
{
    sched::waitTillSteady();
    auto beginTime = sched::getCurrentTimestamp();

    gate::ZeroDelayOutputPin D{nullptr};
    gate::ZeroDelayOutputPin Load{nullptr};
    gate::ZeroDelayOutputPin Clock{nullptr};

    gate::NOTGate dataInvertor{"dataInvertor"};
    gate::ANDGate feedbackAnd{"feedbackAnd"};
    gate::ANDGate dataAnd{"dataAnd"};
    gate::ORGate dataSelect{"dataSelect"};

    // D flip flop gates
    gate::NOTGateDelayed invertor{"dff.invertor"};
    gate::ANDGate pulseGate{"dff.pulseGate"};

    gate::NOTGate notGate{"dff.notGate"};
    gate::ANDGate andGate0{"dff.andGate0"};
    gate::ANDGate andGate1{"dff.andGate1"};
    gate::NORGate norGate0{"dff.norGate0"};
    gate::NORGate norGate1{"dff.norGate1"};

    Load.connect(dataInvertor.input(0));
    Load.connect(dataAnd.input(0));
    D.connect(dataAnd.input(1));
    Clock.connect(pulseGate.input(0));
    Clock.connect(invertor.input(0));

    dataInvertor.output(0)->connect(feedbackAnd.input(1));

    feedbackAnd.output(0)->connect(dataSelect.input(0));
    dataAnd.output(0)->connect(dataSelect.input(0));

    invertor.output(0)->connect(pulseGate.input(1));

    dataSelect.output(0)->connect(andGate0.input(0));
    dataSelect.output(0)->connect(notGate.input(0));

    pulseGate.output(0)->connect(andGate0.input(1));
    pulseGate.output(0)->connect(andGate1.input(0));

    notGate.output(0)->connect(andGate1.input(1));

    andGate0.output(0)->connect(norGate0.input(0));
    andGate1.output(0)->connect(norGate1.input(1));

    norGate0.output(0)->connect(norGate1.input(0));
    norGate1.output(0)->connect(norGate0.input(1));

    auto Q = norGate1.output(0);
    Q->connect(feedbackAnd.input(0));

    dataInvertor.compute();
    feedbackAnd.compute();
    dataAnd.compute();
    dataSelect.compute();

    invertor.compute();
    pulseGate.compute();
    notGate.compute();
    andGate0.compute();
    andGate1.compute();
    norGate0.compute();
    sched::waitTillSteady();
    norGate1.compute();

    sched::waitTillSteady();
    // here converge at timestamp 0 because invertor has no
    // value change when E connects to it.
    EXPECT_EQ(beginTime + 0, sched::getCurrentTimestamp());

    // D Low, Load Low
    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // D -> High, Load Low
    D.value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock.value(gate::PinState::High); }));
    sched::addEvent(2, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock.value(gate::PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), gate::PinState::Low);
    EXPECT_EQ(Clock.value(), gate::PinState::Low);
    EXPECT_EQ(beginTime + 3, sched::getCurrentTimestamp());

    // D High, Load High
    Load.value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock.value(gate::PinState::High); }));
    sched::addEvent(3, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock.value(gate::PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), gate::PinState::High);
    EXPECT_EQ(beginTime + 7, sched::getCurrentTimestamp());
}
