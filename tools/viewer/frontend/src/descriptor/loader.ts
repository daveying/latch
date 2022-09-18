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

import fs from "fs";

export enum PinType {
    DelayedInputPin_0,
    DelayedOutputPin_0,
    DelayedInputPin_1,
    DelayedInputPin_5,
    InertialInputPin_1,
    ForwardInputPin,
    ForwardOutputPin,
}

export class Pin {
    type: PinType;
    constructor(type: PinType) {
        this.type = type;
    }
}

export class Gate {
    inputPins: Pin[];
    outputPins: Pin[];
    constructor(inputPins: Pin[], outputPins: Pin[]) {
        this.inputPins = inputPins;
        this.outputPins = outputPins;
    }
}

export class ComponentDescriptor {
    name: string;
    rawDescriptor: any;
    constructor(compName: string, rawDesc: any) {
        this.name = compName;
        this.rawDescriptor = rawDesc;
    }
}

export class Loader {
    descriptors: ComponentDescriptor[];
    constructor(fileName: string) {
        this.descriptors = [];
        let jsonData = JSON.parse(fs.readFileSync(fileName).toString());
        for (let compName in jsonData) {
            this.descriptors.push(new ComponentDescriptor(compName, jsonData[compName]));
        }
    }
}
