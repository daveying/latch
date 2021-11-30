/////////////////////////////////////////////////////////////////////////////////
// MIT License
//
// Copyright (c) 2021 Xingpeng Da
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

#ifndef TRUTH_TABLE_HPP_
#define TRUTH_TABLE_HPP_

#include <array>
#include <memory>
#include <type_traits>

#include <IGate.hpp>
#include <IPin.hpp>

namespace component
{

#define DEFINE_GATE_TRUTH_TABLE(_Name, _InputSize, _OutputSize)                   \
    class _Name##_TruthTable : public TruthTableBase                              \
    {                                                                             \
    public:                                                                       \
        using InputArray = std::array<std::unique_ptr<IPin>, _InputSize>;         \
        using OutputArray = std::array<std::unique_ptr<ISourcePin>, _OutputSize>; \
        void compute(const InputArray& input, const OutputArray& output) const
#define END_GATE_TRUTH_TABLE };

} // namespace component

#endif
