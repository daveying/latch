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

import type {
    ComponentDescriptor,
    Endpoint,
    PinDescriptor,
    PinDirection,
    PinType
} from "src/descriptor/loader";

type Optional<Type> = Type | null;

export class Pin {
    name: string;
    type: PinType;
    direction: PinDirection;
    parent: Optional<Component>;
    peers: Pin[];
    constructor(name: string, type: PinType, direction: PinDirection, parent: Optional<Component>) {
        this.name = name;
        this.type = type;
        this.direction = direction;
        this.parent = parent;
        this.peers = [];
    }
    setParent(parent: Optional<Component>) {
        this.parent = parent;
    }
    connect(other: Pin) {
        let foundInThisPeer = this.peers.find(p => p === other);
        let foundInOtherPeer = other.peers.find(p => p === this);

        if (foundInThisPeer && foundInOtherPeer) {
            // already connected
            return;
        } else if (foundInOtherPeer || foundInThisPeer) {
            // partially connected, error
            throw "Program is in corrupted state."
        } else {
            this.peers.push(other);
            other.peers.push(this);
        }
    }
    id(): string {
        return this.parent ? `${this.parent.id()}.${this.name}` : this.name;
    }
    static fromDescriptor(desc: PinDescriptor, parent: Optional<Component>) {
        return new Pin(desc.name, desc.type, desc.direction, parent);
    }
}

export class Component {
    name: string;
    subcomponents: Component[];
    pins: Pin[];
    parent: Optional<Component>;
    constructor(
        name: string,
        subcomponents: Component[],
        pins: Pin[],
        parent: Optional<Component>
    ) {
        this.name = name;
        this.subcomponents = subcomponents;
        this.pins = pins;
        this.parent = parent;
    }
    setParent(parent: Optional<Component>) {
        this.parent = parent;
    }
    id(): string {
        return this.parent ? `${this.parent.id()}.${this.name}` : this.name;
    }
    static fromDescriptor(
        name: string,
        parent: Optional<Component>,
        desc: ComponentDescriptor,
        descriptors: Map<string, ComponentDescriptor>
    ): Component {
        let pins: Pin[] = desc.pins.map(pinDesc => Pin.fromDescriptor(pinDesc, null));

        let subcomponents: Component[] = desc.subcomponents.map(subcompDesc => {
            let compDesc = descriptors.get(subcompDesc.type);
            if (!compDesc) {
                throw `Invalid ComponentDescriptor '${desc.type}': subcomponent type '${subcompDesc.type}' is not registered.`;
            }
            return Component.fromDescriptor(subcompDesc.name, null, compDesc, descriptors);
        });

        let comp = new Component(name, subcomponents, pins, parent);

        pins.forEach((pin) => pin.setParent(comp));
        subcomponents.forEach((c) => c.setParent(comp));

        desc.connections.map((conn, index): [Pin, Pin] => {
            function getPin(endpoint: Endpoint): Optional<Pin> {
                let pinComp = endpoint.componentIndex === -1 ?
                    comp : subcomponents[endpoint.componentIndex];
                if (pinComp) {
                    let pin = pinComp.pins[endpoint.pinIndex];
                    return pin || null;
                }
                return null;
            }
            let srcPin = getPin(conn.src);
            let destPin = getPin(conn.dest);
            if (!srcPin || !destPin) {
                throw `Invalid ComponentDescriptor '${desc.type}': connection '${index}' invalid.`;
            }
            return [srcPin, destPin];
        }).forEach((pins) => {
            pins[0].connect(pins[1]);
        });

        return comp;
    }
}
