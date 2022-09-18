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

TEST(EdgeDetectorUnitDelay, Pulse)
{
    sched::waitTillSteady();
    auto beginTime = sched::getCurrentTimestamp();

    component::ZeroDelayOutputPin D{nullptr};
    component::ZeroDelayOutputPin E{nullptr};

    component::NOTGateDelayed invertor{"invertor"};
    component::ANDGate pulseGate{"pulseGate"};

    E.connect(pulseGate.input(0));
    E.connect(invertor.input(0));
    invertor.output(0)->connect(pulseGate.input(1));

    sched::waitTillSteady();
    // here converge at timestamp 0 because invertor has no
    // value change when E connects to it.
    EXPECT_EQ(beginTime + 0, sched::getCurrentTimestamp());

    auto Q = pulseGate.output(0);

    EXPECT_EQ(Q->value(), component::PinState::Low);

    // E -> High
    sched::addEvent(1, sched::Event::create("E goes high", [&](sched::Timestamp) { E.value(component::PinState::High); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), component::PinState::Low);
    EXPECT_EQ(beginTime + 2, sched::getCurrentTimestamp());

    // E -> Low
    sched::addEvent(1, sched::Event::create("E goes low", [&](sched::Timestamp) { E.value(component::PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), component::PinState::Low);
    EXPECT_EQ(beginTime + 4, sched::getCurrentTimestamp());
}

TEST(EdgeDetectorNonDelayed, Pulse)
{
    sched::waitTillSteady();
    auto beginTime = sched::getCurrentTimestamp();

    component::ZeroDelayOutputPin D{nullptr};
    component::ZeroDelayOutputPin E{nullptr};

    component::NOTGate invertor{"invertor"};
    component::ANDGate pulseGate{"pulseGate"};

    // connect sequence causes reset before compute (RBC) issue
    // but if E.connects pulseGate first, there is no RBC issue
    E.connect(invertor.input(0));
    E.connect(pulseGate.input(0));
    invertor.output(0)->connect(pulseGate.input(1));

    sched::waitTillSteady();
    // here converge at timestamp 0 because invertor has no
    // value change when E connects to it.
    EXPECT_EQ(beginTime + 0, sched::getCurrentTimestamp());

    auto Q = pulseGate.output(0);

    EXPECT_EQ(Q->value(), component::PinState::Low);

    // E -> High
    sched::addEvent(1, sched::Event::create("E goes high", [&](sched::Timestamp) { E.value(component::PinState::High); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), component::PinState::Low);
    EXPECT_EQ(beginTime + 1, sched::getCurrentTimestamp());

    // E -> Low
    sched::addEvent(1, sched::Event::create("E goes low", [&](sched::Timestamp) { E.value(component::PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), component::PinState::Low);
    EXPECT_EQ(beginTime + 2, sched::getCurrentTimestamp());
}
