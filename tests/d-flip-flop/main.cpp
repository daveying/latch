/////////////////////////////////////////////////////////////////////////////////
// MIT License
//
// Copyright (c) 2022 Xingpeng Da
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/////////////////////////////////////////////////////////////////////////////////

#include <NORGate.hpp>
#include <ANDGate.hpp>
#include <NOTGate.hpp>
#include <IScheduler.hpp>

#include <gtest/gtest.h>

TEST(DFlipFlop, DFlipFlopTruthTable)
{
    component::ZeroDelayOutputPin D{nullptr};
    component::ZeroDelayOutputPin E{nullptr};

    component::NOTGateDelayed invertor;
    component::ANDGate pulseGate;

    component::NOTGate notGate;
    component::ANDGate andGate0;
    component::ANDGate andGate1;
    component::NORGate norGate0;
    component::NORGate norGate1;

    E.connect(pulseGate.input(0));
    E.connect(invertor.input(0));
    invertor.output(0)->connect(pulseGate.input(1));

    D.connect(andGate0.input(0));
    D.connect(notGate.input(0));

    auto E2 = pulseGate.output(0);
    E2->connect(andGate0.input(1));
    E2->connect(andGate1.input(0));

    notGate.output(0)->connect(andGate1.input(1));

    andGate0.output(0)->connect(norGate0.input(0));
    andGate1.output(0)->connect(norGate1.input(1));

    norGate0.output(0)->connect(norGate1.input(0));
    norGate1.output(0)->connect(norGate0.input(1));

    invertor.compute();
    pulseGate.compute();
    notGate.compute();
    andGate0.compute();
    andGate1.compute();
    norGate0.compute();
    sched::waitTillSteady();
    norGate1.compute();

    sched::waitTillSteady();

    auto Q = norGate1.output(0);

    sched::waitTillSteady();
    // D Low, E Low
    EXPECT_EQ(Q->value(), component::PinState::Low);

    // D -> High, E Low
    D.value(component::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), component::PinState::Low);

    // E -> High
    E.value(component::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), component::PinState::High);

    // D -> Low
    D.value(component::PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), component::PinState::High);

    // E -> Low
    E.value(component::PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), component::PinState::High);

    // E -> pulse
    E.value(component::PinState::High);
    sched::waitTillSteady();
    E.value(component::PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), component::PinState::Low);
}

TEST(DFlipFlop, Pulse)
{
    sched::waitTillSteady();
    auto beginTime = sched::getCurrentTimestamp();

    component::ZeroDelayOutputPin D{nullptr};
    component::ZeroDelayOutputPin E{nullptr};

    component::NOTGateDelayed invertor{"invertor"};
    component::ANDGate pulseGate{"pulseGate"};

    component::NOTGate notGate{"notGate"};
    component::ANDGate andGate0{"andGate0"};
    component::ANDGate andGate1{"andGate1"};
    component::NORGate norGate0{"norGate0"};
    component::NORGate norGate1{"norGate1"};

    E.connect(pulseGate.input(0));
    E.connect(invertor.input(0));
    invertor.output(0)->connect(pulseGate.input(1));

    D.connect(andGate0.input(0));
    D.connect(notGate.input(0));

    auto E2 = pulseGate.output(0);
    E2->connect(andGate0.input(1));
    E2->connect(andGate1.input(0));

    notGate.output(0)->connect(andGate1.input(1));

    andGate0.output(0)->connect(norGate0.input(0));
    andGate1.output(0)->connect(norGate1.input(1));

    norGate0.output(0)->connect(norGate1.input(0));
    norGate1.output(0)->connect(norGate0.input(1));

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

    auto Q = norGate1.output(0);

    // D Low, E Low
    EXPECT_EQ(Q->value(), component::PinState::Low);

    // D -> High, E Low
    D.value(component::PinState::High);

    // E -> pulse
    sched::addEvent(1, sched::Event::create("E goes high", [&](sched::Timestamp) { E.value(component::PinState::High); }));
    sched::addEvent(2, sched::Event::create("E goes low", [&](sched::Timestamp) { E.value(component::PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), component::PinState::High);
    EXPECT_EQ(E.value(), component::PinState::Low);
    EXPECT_EQ(beginTime + 3, sched::getCurrentTimestamp());

    // D -> High, E Low
    D.value(component::PinState::Low);

    // E -> pulse
    sched::addEvent(1, sched::Event::create("E goes high", [&](sched::Timestamp) { E.value(component::PinState::High); }));
    sched::addEvent(3, sched::Event::create("E goes low", [&](sched::Timestamp) { E.value(component::PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), component::PinState::Low);
    EXPECT_EQ(E.value(), component::PinState::Low);
    EXPECT_EQ(beginTime + 7, sched::getCurrentTimestamp());
}

TEST(DFlipFlopNonDelayed, Pulse)
{
    sched::waitTillSteady();
    auto beginTime = sched::getCurrentTimestamp();

    component::ZeroDelayOutputPin D{nullptr};
    component::ZeroDelayOutputPin E{nullptr};

    // if all gate is non delayed pin, then need several not gate
    // wire together to have enough time delay
    component::NOTGate invertor0{"invertor0"};
    component::NOTGate invertor1{"invertor1"};
    component::NOTGate invertor2{"invertor2"};
    component::ANDGate pulseGate{"pulseGate"};

    component::NOTGate notGate{"notGate"};
    component::ANDGate andGate0{"andGate0"};
    component::ANDGate andGate1{"andGate1"};
    component::NORGate norGate0{"norGate0"};
    component::NORGate norGate1{"norGate1"};

    E.connect(invertor0.input(0));
    E.connect(pulseGate.input(0));
    invertor0.output(0)->connect(invertor1.input(0));
    invertor1.output(0)->connect(invertor2.input(0));
    invertor2.output(0)->connect(pulseGate.input(1));

    D.connect(andGate0.input(0));
    D.connect(notGate.input(0));

    auto E2 = pulseGate.output(0);
    E2->connect(andGate0.input(1));
    E2->connect(andGate1.input(0));

    notGate.output(0)->connect(andGate1.input(1));

    andGate0.output(0)->connect(norGate0.input(0));
    andGate1.output(0)->connect(norGate1.input(1));

    norGate0.output(0)->connect(norGate1.input(0));
    norGate1.output(0)->connect(norGate0.input(1));

    invertor0.compute();
    invertor1.compute();
    invertor2.compute();
    pulseGate.compute();
    notGate.compute();
    andGate0.compute();
    andGate1.compute();
    norGate0.compute();
    sched::waitTillSteady();
    norGate1.compute();

    sched::waitTillSteady();
    EXPECT_EQ(beginTime + 0, sched::getCurrentTimestamp());

    auto Q = norGate1.output(0);

    // D Low, E Low
    EXPECT_EQ(Q->value(), component::PinState::Low);

    // D -> High, E Low
    D.value(component::PinState::High);
    sched::waitTillSteady();

    // E -> pulse
    sched::addEvent(1, sched::Event::create("E goes high", [&](sched::Timestamp) { E.value(component::PinState::High); }));
    sched::addEvent(2, sched::Event::create("E goes low", [&](sched::Timestamp) { E.value(component::PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), component::PinState::High);
    EXPECT_EQ(E.value(), component::PinState::Low);
    EXPECT_EQ(beginTime + 2, sched::getCurrentTimestamp());

    // D -> High, E Low
    D.value(component::PinState::Low);

    // E -> pulse
    sched::addEvent(1, sched::Event::create("E goes high", [&](sched::Timestamp) { E.value(component::PinState::High); }));
    sched::addEvent(3, sched::Event::create("E goes low", [&](sched::Timestamp) { E.value(component::PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), component::PinState::Low);
    EXPECT_EQ(E.value(), component::PinState::Low);
    EXPECT_EQ(beginTime + 5, sched::getCurrentTimestamp());
}
