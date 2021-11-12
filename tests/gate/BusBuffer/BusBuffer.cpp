#include <BusBuffer.hpp>
#include <NOTGate.hpp>
#include <IScheduler.hpp>

#include <gtest/gtest.h>

TEST(BusBuffer, DefaultState)
{
    gate::BusBuffer buffer;
    buffer.compute();
    EXPECT_EQ(buffer.input(0)->value(), gate::PinState::Low);
    EXPECT_EQ(buffer.input(1)->value(), gate::PinState::Low);
    EXPECT_EQ(buffer.output(0)->value(), gate::PinState::Low);
}

TEST(BusBuffer, TruthTable)
{
    gate::BusBuffer buffer;
    buffer.compute();

    auto in   = buffer.input(0);
    auto ctrl = buffer.input(1);
    auto out  = buffer.output(0);

    // in = Low, ctrl = Low
    EXPECT_EQ(out->value(), gate::PinState::Low);

    // in = High, ctrl = Low
    in->value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), gate::PinState::Low);
    // in = High, ctrl = High
    ctrl->value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), gate::PinState::High);
    // in = Low, ctrl = High
    in->value(gate::PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), gate::PinState::Low);
}

TEST(BusBuffer, HighImpedance)
{
    gate::BusBuffer buffer;
    gate::NOTGate notGate;

    buffer.output(0)->connect(notGate.input(0));
    buffer.compute();
    notGate.compute();

    sched::waitTillSteady();
    auto bufferData = buffer.input(0);
    auto bufferCtrl = buffer.input(1);
    auto bufferOut  = buffer.output(0);
    auto notGateIn  = notGate.input(0);
    auto notGateOut = notGate.output(0);
    EXPECT_EQ(bufferData->value(), gate::PinState::Low);
    EXPECT_EQ(bufferCtrl->value(), gate::PinState::Low);
    EXPECT_EQ(bufferOut->value(), gate::PinState::Low);
    EXPECT_EQ(notGateIn->value(), gate::PinState::Low);
    EXPECT_EQ(notGateOut->value(), gate::PinState::High);

    // set notGateIn High
    notGateIn->value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(bufferData->value(), gate::PinState::Low);
    EXPECT_EQ(bufferCtrl->value(), gate::PinState::Low);
    EXPECT_EQ(bufferOut->value(), gate::PinState::Low);
    EXPECT_EQ(notGateIn->value(), gate::PinState::High);
    EXPECT_EQ(notGateOut->value(), gate::PinState::Low);

    // set buffer data High
    bufferData->value(gate::PinState::High);
    sched::waitTillSteady();
    // expect high impedance
    EXPECT_EQ(bufferData->value(), gate::PinState::High);
    EXPECT_EQ(bufferCtrl->value(), gate::PinState::Low);
    EXPECT_EQ(bufferOut->value(), gate::PinState::Low);
    EXPECT_EQ(notGateIn->value(), gate::PinState::High);
    EXPECT_EQ(notGateOut->value(), gate::PinState::Low);

    // set buffer ctrl High
    bufferCtrl->value(gate::PinState::High);
    sched::waitTillSteady();
    // data High will be forwarded
    EXPECT_EQ(bufferData->value(), gate::PinState::High);
    EXPECT_EQ(bufferCtrl->value(), gate::PinState::High);
    EXPECT_EQ(bufferOut->value(), gate::PinState::High);
    EXPECT_EQ(notGateIn->value(), gate::PinState::High);
    EXPECT_EQ(notGateOut->value(), gate::PinState::Low);

    // set buffer data Low
    bufferData->value(gate::PinState::Low);
    sched::waitTillSteady();
    // data Low will be forwarded
    EXPECT_EQ(bufferData->value(), gate::PinState::Low);
    EXPECT_EQ(bufferCtrl->value(), gate::PinState::High);
    EXPECT_EQ(bufferOut->value(), gate::PinState::Low);
    EXPECT_EQ(notGateIn->value(), gate::PinState::Low);
    EXPECT_EQ(notGateOut->value(), gate::PinState::High);

    // set buffer data and ctrl at the same time
    bufferData->value(gate::PinState::High);
    bufferCtrl->value(gate::PinState::Low);
    sched::waitTillSteady();
    // data high happened first, ctrl low happened next, high will
    // be forwarded. (NOTE, multiple thread schedule causes intermittency)
    EXPECT_EQ(bufferData->value(), gate::PinState::High);
    EXPECT_EQ(bufferCtrl->value(), gate::PinState::Low);
    EXPECT_EQ(bufferOut->value(), gate::PinState::High);
    EXPECT_EQ(notGateIn->value(), gate::PinState::High);
    EXPECT_EQ(notGateOut->value(), gate::PinState::Low);

    // set buffer data and ctrl at the same time
    bufferData->value(gate::PinState::Low);
    bufferCtrl->value(gate::PinState::High);
    sched::waitTillSteady();
    // data low happened first, ctrl high happened next, low will still
    // be forwarded because ctrl pin high will also trigger recompute.
    // (NOTE, multiple thread schedule causes intermittency)
    EXPECT_EQ(bufferData->value(), gate::PinState::Low);
    EXPECT_EQ(bufferCtrl->value(), gate::PinState::High);
    EXPECT_EQ(bufferOut->value(), gate::PinState::Low);
    EXPECT_EQ(notGateIn->value(), gate::PinState::Low);
    EXPECT_EQ(notGateOut->value(), gate::PinState::High);

    // set buffer data and ctrl at the same time
    bufferCtrl->value(gate::PinState::Low);
    bufferData->value(gate::PinState::High);
    sched::waitTillSteady();
    // ctrl low happened first, data high happened next, high will not
    // be forwarded. (NOTE, multiple thread schedule causes intermittency)
    EXPECT_EQ(bufferData->value(), gate::PinState::High);
    EXPECT_EQ(bufferCtrl->value(), gate::PinState::Low);
    EXPECT_EQ(bufferOut->value(), gate::PinState::Low);
    EXPECT_EQ(notGateIn->value(), gate::PinState::Low);
    EXPECT_EQ(notGateOut->value(), gate::PinState::High);

    // set buffer data and ctrl at the same time
    bufferCtrl->value(gate::PinState::High);
    bufferData->value(gate::PinState::Low);
    sched::waitTillSteady();
    // ctrl high happened first, data low happened next, low will
    // be forwarded because ctrl pin high will also trigger recompute.
    // (NOTE, multiple thread schedule causes intermittency)
    EXPECT_EQ(bufferData->value(), gate::PinState::Low);
    EXPECT_EQ(bufferCtrl->value(), gate::PinState::High);
    EXPECT_EQ(bufferOut->value(), gate::PinState::Low);
    EXPECT_EQ(notGateIn->value(), gate::PinState::Low);
    EXPECT_EQ(notGateOut->value(), gate::PinState::High);
}

TEST(BusBuffer, AsSelector)
{
    gate::BusBuffer buffer0{"buffer0"};
    gate::BusBuffer buffer1{"buffer1"};

    gate::ZeroDelayInputPin input{nullptr};

    // two BusBuffer output connect to the same input
    buffer0.output(0)->connect(&input);
    buffer1.output(0)->connect(&input);

    buffer0.input(0)->value(gate::PinState::High); // buffer0 High
    buffer1.input(0)->value(gate::PinState::Low);  // buffer1 Low

    sched::waitTillSteady();
    EXPECT_EQ(input.value(), gate::PinState::Low);

    buffer0.input(1)->value(gate::PinState::High); // select buffer0
    buffer1.input(1)->value(gate::PinState::Low);

    sched::waitTillSteady();

    EXPECT_EQ(input.value(), gate::PinState::High); // value from buffer0

    //==============================
    buffer1.input(1)->value(gate::PinState::High);  // select buffer1
    // at this point buffer0.ctrl still High, as long as buffer0.data
    // don't change, there should be no problem
    buffer0.input(1)->value(gate::PinState::Low);
    sched::waitTillSteady();

    EXPECT_EQ(input.value(), gate::PinState::Low);

    //==============================
    buffer1.input(1)->value(gate::PinState::Low);  // select none
    // at this point buffer0.ctrl still High, as long as buffer0.data
    // don't change, there should be no problem
    buffer0.input(1)->value(gate::PinState::Low);
    sched::waitTillSteady();

    // keeps the same
    EXPECT_EQ(input.value(), gate::PinState::Low);

    //==============================
    buffer0.input(1)->value(gate::PinState::High);  // select buffer0
    buffer1.input(1)->value(gate::PinState::Low);
    sched::waitTillSteady();

    EXPECT_EQ(input.value(), gate::PinState::High);

    //==============================
    buffer0.input(1)->value(gate::PinState::Low);  // select none
    buffer1.input(1)->value(gate::PinState::Low);
    sched::waitTillSteady();

    // keeps the same
    EXPECT_EQ(input.value(), gate::PinState::High);

    //==============================
    buffer0.input(1)->value(gate::PinState::Low);  // select none
    buffer1.input(1)->value(gate::PinState::High);
    sched::waitTillSteady();

    EXPECT_EQ(input.value(), gate::PinState::Low);
}
