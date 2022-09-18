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

#include <GateComponents.hpp>
#include <ComponentFactory.hpp>

namespace component
{

REGISTER_COMPONENT(ANDGateComponent);
REGISTER_COMPONENT(ANDGate3Component);
REGISTER_COMPONENT(ANDGate4Component);
REGISTER_COMPONENT(ANDGate5Component);
REGISTER_COMPONENT(BusBufferComponent);
REGISTER_COMPONENT(BusBufferDataDelayedComponent);
REGISTER_COMPONENT(NANDGateComponent);
REGISTER_COMPONENT(NORGateComponent);
REGISTER_COMPONENT(NORGate3Component);
REGISTER_COMPONENT(NORGate4Component);
REGISTER_COMPONENT(NORGateDelayedComponent);
REGISTER_COMPONENT(NORGateUnitInertial4Component);
REGISTER_COMPONENT(NOTGateComponent);
REGISTER_COMPONENT(NOTGateUnitInertialComponent);
REGISTER_COMPONENT(NOTGateDelayedComponent);
REGISTER_COMPONENT(ORGateComponent);
REGISTER_COMPONENT(XORGateComponent);

} // namespace component
