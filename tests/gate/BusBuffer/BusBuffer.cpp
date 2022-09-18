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

#include <BusBuffer.hpp>
#include <NOTGate.hpp>
#include <IScheduler.hpp>

#include <gtest/gtest.h>

TEST(BusBuffer, DefaultState)
{
    component::BusBuffer buffer;
    buffer.compute();
    EXPECT_EQ(buffer.input(0)->value(), component::PinState::Low);
    EXPECT_EQ(buffer.input(1)->value(), component::PinState::Low);
    EXPECT_EQ(buffer.output(0)->value(), component::PinState::Low);
}

TEST(BusBuffer, TruthTable)
{
    component::BusBuffer buffer;
    buffer.compute();

    auto in   = buffer.input(0);
    auto ctrl = buffer.input(1);
    auto out  = buffer.output(0);

    // in = Low, ctrl = Low
    EXPECT_EQ(out->value(), component::PinState::Low);

    // in = High, ctrl = Low
    in->value(component::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), component::PinState::Low);
    // in = High, ctrl = High
    ctrl->value(component::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), component::PinState::High);
    // in = Low, ctrl = High
    in->value(component::PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), component::PinState::Low);
}

TEST(BusBuffer, HighImpedance)
{
    component::BusBuffer buffer;
    component::NOTGate notGate;

    buffer.output(0)->connect(notGate.input(0));
    buffer.compute();
    notGate.compute();

    sched::waitTillSteady();
    auto bufferData = buffer.input(0);
    auto bufferCtrl = buffer.input(1);
    auto bufferOut  = buffer.output(0);
    auto notGateIn  = notGate.input(0);
    auto notGateOut = notGate.output(0);
    EXPECT_EQ(bufferData->value(), component::PinState::Low);
    EXPECT_EQ(bufferCtrl->value(), component::PinState::Low);
    EXPECT_EQ(bufferOut->value(), component::PinState::Low);
    EXPECT_EQ(notGateIn->value(), component::PinState::Low);
    EXPECT_EQ(notGateOut->value(), component::PinState::High);

    // set notGateIn High
    notGateIn->value(component::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(bufferData->value(), component::PinState::Low);
    EXPECT_EQ(bufferCtrl->value(), component::PinState::Low);
    EXPECT_EQ(bufferOut->value(), component::PinState::Low);
    EXPECT_EQ(notGateIn->value(), component::PinState::High);
    EXPECT_EQ(notGateOut->value(), component::PinState::Low);

    // set buffer data High
    bufferData->value(component::PinState::High);
    sched::waitTillSteady();
    // expect high impedance
    EXPECT_EQ(bufferData->value(), component::PinState::High);
    EXPECT_EQ(bufferCtrl->value(), component::PinState::Low);
    EXPECT_EQ(bufferOut->value(), component::PinState::Low);
    EXPECT_EQ(notGateIn->value(), component::PinState::High);
    EXPECT_EQ(notGateOut->value(), component::PinState::Low);

    // set buffer ctrl High
    bufferCtrl->value(component::PinState::High);
    sched::waitTillSteady();
    // data High will be forwarded
    EXPECT_EQ(bufferData->value(), component::PinState::High);
    EXPECT_EQ(bufferCtrl->value(), component::PinState::High);
    EXPECT_EQ(bufferOut->value(), component::PinState::High);
    EXPECT_EQ(notGateIn->value(), component::PinState::High);
    EXPECT_EQ(notGateOut->value(), component::PinState::Low);

    // set buffer data Low
    bufferData->value(component::PinState::Low);
    sched::waitTillSteady();
    // data Low will be forwarded
    EXPECT_EQ(bufferData->value(), component::PinState::Low);
    EXPECT_EQ(bufferCtrl->value(), component::PinState::High);
    EXPECT_EQ(bufferOut->value(), component::PinState::Low);
    EXPECT_EQ(notGateIn->value(), component::PinState::Low);
    EXPECT_EQ(notGateOut->value(), component::PinState::High);

    // set buffer data and ctrl at the same time
    bufferData->value(component::PinState::High);
    bufferCtrl->value(component::PinState::Low);
    sched::waitTillSteady();
    // data high happened first, ctrl low happened next, high will
    // be forwarded. (NOTE, multiple thread schedule causes intermittency)
    EXPECT_EQ(bufferData->value(), component::PinState::High);
    EXPECT_EQ(bufferCtrl->value(), component::PinState::Low);
    EXPECT_EQ(bufferOut->value(), component::PinState::High);
    EXPECT_EQ(notGateIn->value(), component::PinState::High);
    EXPECT_EQ(notGateOut->value(), component::PinState::Low);

    // set buffer data and ctrl at the same time
    bufferData->value(component::PinState::Low);
    bufferCtrl->value(component::PinState::High);
    sched::waitTillSteady();
    // data low happened first, ctrl high happened next, low will still
    // be forwarded because ctrl pin high will also trigger recompute.
    // (NOTE, multiple thread schedule causes intermittency)
    EXPECT_EQ(bufferData->value(), component::PinState::Low);
    EXPECT_EQ(bufferCtrl->value(), component::PinState::High);
    EXPECT_EQ(bufferOut->value(), component::PinState::Low);
    EXPECT_EQ(notGateIn->value(), component::PinState::Low);
    EXPECT_EQ(notGateOut->value(), component::PinState::High);

    // set buffer data and ctrl at the same time
    bufferCtrl->value(component::PinState::Low);
    bufferData->value(component::PinState::High);
    sched::waitTillSteady();
    // ctrl low happened first, data high happened next, high will not
    // be forwarded. (NOTE, multiple thread schedule causes intermittency)
    EXPECT_EQ(bufferData->value(), component::PinState::High);
    EXPECT_EQ(bufferCtrl->value(), component::PinState::Low);
    EXPECT_EQ(bufferOut->value(), component::PinState::Low);
    EXPECT_EQ(notGateIn->value(), component::PinState::Low);
    EXPECT_EQ(notGateOut->value(), component::PinState::High);

    // set buffer data and ctrl at the same time
    bufferCtrl->value(component::PinState::High);
    bufferData->value(component::PinState::Low);
    sched::waitTillSteady();
    // ctrl high happened first, data low happened next, low will
    // be forwarded because ctrl pin high will also trigger recompute.
    // (NOTE, multiple thread schedule causes intermittency)
    EXPECT_EQ(bufferData->value(), component::PinState::Low);
    EXPECT_EQ(bufferCtrl->value(), component::PinState::High);
    EXPECT_EQ(bufferOut->value(), component::PinState::Low);
    EXPECT_EQ(notGateIn->value(), component::PinState::Low);
    EXPECT_EQ(notGateOut->value(), component::PinState::High);
}

TEST(BusBuffer, AsSelector)
{
    component::BusBuffer buffer0{"buffer0"};
    component::BusBuffer buffer1{"buffer1"};

    component::ZeroDelayInputPin input{nullptr};

    // two BusBuffer output connect to the same input
    buffer0.output(0)->connect(&input);
    buffer1.output(0)->connect(&input);

    buffer0.input(0)->value(component::PinState::High); // buffer0 High
    buffer1.input(0)->value(component::PinState::Low);  // buffer1 Low

    sched::waitTillSteady();
    EXPECT_EQ(input.value(), component::PinState::Low);

    buffer0.input(1)->value(component::PinState::High); // select buffer0
    buffer1.input(1)->value(component::PinState::Low);

    sched::waitTillSteady();

    EXPECT_EQ(input.value(), component::PinState::High); // value from buffer0

    //==============================
    buffer1.input(1)->value(component::PinState::High);  // select buffer1
    // at this point buffer0.ctrl still High, as long as buffer0.data
    // don't change, there should be no problem
    buffer0.input(1)->value(component::PinState::Low);
    sched::waitTillSteady();

    EXPECT_EQ(input.value(), component::PinState::Low);

    //==============================
    buffer1.input(1)->value(component::PinState::Low);  // select none
    // at this point buffer0.ctrl still High, as long as buffer0.data
    // don't change, there should be no problem
    buffer0.input(1)->value(component::PinState::Low);
    sched::waitTillSteady();

    // keeps the same
    EXPECT_EQ(input.value(), component::PinState::Low);

    //==============================
    buffer0.input(1)->value(component::PinState::High);  // select buffer0
    buffer1.input(1)->value(component::PinState::Low);
    sched::waitTillSteady();

    EXPECT_EQ(input.value(), component::PinState::High);

    //==============================
    buffer0.input(1)->value(component::PinState::Low);  // select none
    buffer1.input(1)->value(component::PinState::Low);
    sched::waitTillSteady();

    // keeps the same
    EXPECT_EQ(input.value(), component::PinState::High);

    //==============================
    buffer0.input(1)->value(component::PinState::Low);  // select none
    buffer1.input(1)->value(component::PinState::High);
    sched::waitTillSteady();

    EXPECT_EQ(input.value(), component::PinState::Low);
}
