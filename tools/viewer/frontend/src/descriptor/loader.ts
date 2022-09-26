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

import * as fs from "fs";

export enum PinType {
    DelayedInputPin_0,
    DelayedOutputPin_0,
    DelayedInputPin_1,
    DelayedInputPin_5,
    InertialInputPin_1,
    ForwardInputPin,
    ForwardOutputPin,
}

export enum PinDirection {
    INPUT,
    OUTPUT,
}

export class PinDescriptor {
    type: PinType;
    name: string;
    direction: PinDirection;
    constructor(name: string, type: PinType, direction: PinDirection) {
        this.name = name;
        this.type = type;
        this.direction = direction;
    }
    static fromJSON(jsonData: any): PinDescriptor {
        if (typeof jsonData.name !== "string" ||
            typeof jsonData.type !== "string" ||
            typeof jsonData.direction !== "string") {
            throw `Invalid JSON data for PinDescriptor. ${JSON.stringify(jsonData)}`;
        }
        let type = PinType[<keyof typeof PinType>jsonData.type];
        let direction = PinDirection[<keyof typeof PinDirection>jsonData.direction];
        if (type === undefined || direction === undefined) {
            throw `Pin type (${jsonData.type}) or direction (${jsonData.direction}) is invalid`;
        }
        return new PinDescriptor(
            jsonData.name,
            type,
            direction
        );
    }
}

export class SubcomponentDescriptor {
    type: string;
    name: string;
    constructor(name: string, type: string) {
        this.name = name;
        this.type = type;
    }
    static fromJSON(jsonData: any): SubcomponentDescriptor {
        if (typeof jsonData.name !== "string" ||
            typeof jsonData.type !== "string") {
            throw `Invalid JSON data for SubcomponentDescriptor. ${JSON.stringify(jsonData)}`;
        }
        return new SubcomponentDescriptor(jsonData.name, jsonData.type);
    }
}

export class Endpoint {
    componentIndex: number;
    pinIndex: number;
    constructor(componentIndex: number, pinIndex: number) {
        this.componentIndex = componentIndex;
        this.pinIndex = pinIndex;
    }
    static fromJSON(jsonData: any): Endpoint {
        if (typeof jsonData.componentIndex !== "number" ||
            typeof jsonData.pinIndex !== "number") {
            throw `Invalid JSON data for Endpoint. ${JSON.stringify(jsonData)}`;
        }
        return new Endpoint(jsonData.componentIndex, jsonData.pinIndex);
    }
}

export class ConnectionDescriptor {
    src: Endpoint;
    dest: Endpoint;
    constructor(src: Endpoint, dest: Endpoint) {
        this.src = src;
        this.dest = dest;
    }
    static fromJSON(jsonData: any): ConnectionDescriptor {
        if (!jsonData.src || !jsonData.dest) {
            throw `Invalid JSON data for ConnectionDescriptor. ${JSON.stringify(jsonData)}`;
        }
        return new ConnectionDescriptor(
            Endpoint.fromJSON(jsonData.src),
            Endpoint.fromJSON(jsonData.dest)
        );
    }
}

export class ComponentDescriptor {
    type: string;
    pins: PinDescriptor[];
    subcomponents: SubcomponentDescriptor[];
    connections: ConnectionDescriptor[];
    constructor(
        type: string,
        pins: PinDescriptor[],
        subcomponents: SubcomponentDescriptor[],
        connections: ConnectionDescriptor[]) {
        this.type = type;
        this.pins = pins;
        this.subcomponents = subcomponents;
        this.connections = connections;
    }
    static fromJSON(jsonData: any): ComponentDescriptor {
        if (!Array.isArray(jsonData.pins) ||
            !Array.isArray(jsonData.subcomponents) ||
            !Array.isArray(jsonData.connections) ||
            typeof jsonData.type !== "string") {
            throw `Invalid JSON data for ComponentDescriptor. ${JSON.stringify(jsonData.type)}`;
        }
        return new ComponentDescriptor(
            jsonData.type,
            jsonData.pins.map((pinDesc: any) => PinDescriptor.fromJSON(pinDesc)),
            jsonData.subcomponents.map((subcompDesc: any) => SubcomponentDescriptor.fromJSON(subcompDesc)),
            jsonData.connections.map((connectionDesc: any) => ConnectionDescriptor.fromJSON(connectionDesc)),
        );
    }
}

export class Loader {
    descriptors: Map<string, ComponentDescriptor>;
    constructor(jsonData?: any) {
        this.descriptors = new Map();
        for (let compName in jsonData) {
            this.descriptors.set(compName, ComponentDescriptor.fromJSON(jsonData[compName]));
        }
    }
    static fromFile(fileName: string): Loader {
        let jsonData = JSON.parse(fs.readFileSync(fileName).toString());
        return new Loader(jsonData["descriptors"]);
    }
    loadSingle(jsonData: any): void {
        if (!jsonData.type || this.descriptors.has(jsonData.type)) {
            throw jsonData.type ? `Component type ${jsonData.type} has already loaded.`
                                : "Invalid JSON data for ComponentDescriptor";
        }
        let compDesc = ComponentDescriptor.fromJSON(jsonData);
        this.descriptors.set(compDesc.type, compDesc);
    }
    getDescriptor(name: string): ComponentDescriptor | undefined {
        return this.descriptors.get(name);
    }
}
