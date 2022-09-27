import {
    Loader,
    ComponentDescriptor,
    PinType,
    PinDirection,
} from "../src/descriptor/loader";
import {
    Component,
    Pin
} from "../src/descriptor/components";

describe("Component can be constructed from Loader.", () => {
    let loader = Loader.fromFile(process.env.DESCRIPTOR_PATH as string);
    test("ALU1 component can be constructed from Loader", () => {
        expect(loader.getDescriptor("ALU1")).toBeTruthy();
        expect(loader.getDescriptor("ALU32")).toBeTruthy();
        expect(loader.getDescriptor("ALU33")).toBeFalsy();

        let alu1Desc = loader.getDescriptor("ALU1");
        if (!alu1Desc) {
            throw "Impossible to happen";
        }

        let alu1 = Component.fromDescriptor("alu1", null, alu1Desc, loader.descriptors);
        expect(alu1).toBeTruthy();

        expect(alu1.name).toBe("alu1");
        expect(alu1.id()).toBe("alu1");
        expect(alu1.parent).toBe(null);
        expect(alu1.pins.length).toBe(6);
        expect(alu1.pins[5].id()).toBe("alu1.Co");
        expect(alu1.subcomponents.length).toBe(3);
        expect(alu1.subcomponents[1].name).toBe("negater[0]");
        expect(alu1.subcomponents[1].id()).toBe("alu1.negater[0]");
    });
    test("ALU32 component can be constructed from Loader", () => {
        expect(loader.getDescriptor("ALU32")).toBeTruthy();

        let alu32Desc = loader.getDescriptor("ALU32");
        if (!alu32Desc) {
            throw "Impossible to happen";
        }

        let alu32 = Component.fromDescriptor("alu32", null, alu32Desc, loader.descriptors);
        expect(alu32).toBeTruthy();

        expect(alu32.name).toBe("alu32");
        expect(alu32.id()).toBe("alu32");
        expect(alu32.parent).toBe(null);
        expect(alu32.pins.length).toBe(99);
        expect(alu32.pins[5].id()).toBe("alu32.A[5]");
        expect(alu32.subcomponents.length).toBe(96);
        expect(alu32.subcomponents[1].name).toBe("adder[1]");
        expect(alu32.subcomponents[1].id()).toBe("alu32.adder[1]");
    });
    test("All component descriptors can be constructed", () => {
        loader.descriptors.forEach((desc, typeName) => {
            let compName = typeName.toLowerCase();
            let comp = Component.fromDescriptor(compName, null, desc, loader.descriptors);
            expect(comp).toBeTruthy();
            expect(comp.name).toBe(compName);
            expect(comp.id()).toBe(compName);

            expect(comp.pins.length).toBeGreaterThan(1);
        });
    });
    test("ComponentDescriptor.fromDescriptor throws when descriptor is not valid", () => {
        let loader = Loader.fromFile(process.env.DESCRIPTOR_PATH as string);
        let binaryCounter1Desc = loader.getDescriptor("BinaryCounter1");
        expect(binaryCounter1Desc).toBeTruthy();
        if (!binaryCounter1Desc) {
            throw "Impossible to happen";
        }

        // invalid subcomponents type
        let originalType = binaryCounter1Desc.subcomponents[0].type;
        binaryCounter1Desc.subcomponents[0].type = "notRegistered";
        expect(function() {
            // there is a bug in tsc, binaryCounter1Desc is ComponentDescriptor type, but identified
            // as ComponentDescriptor | undefined
            Component.fromDescriptor("foo", null, binaryCounter1Desc as ComponentDescriptor, loader.descriptors);
        }).toThrow("is not registered");
        binaryCounter1Desc.subcomponents[0].type = originalType;

        // invalid connection src componentIndex
        let originalIndex = binaryCounter1Desc.connections[0].src.componentIndex;
        binaryCounter1Desc.connections[0].src.componentIndex = 111;
        expect(function() {
            Component.fromDescriptor("foo", null, binaryCounter1Desc as ComponentDescriptor, loader.descriptors);
        }).toThrow("connection '0' invalid");
        binaryCounter1Desc.connections[0].src.componentIndex = originalIndex;

        // invalid connection src pinIndex
        originalIndex = binaryCounter1Desc.connections[0].src.pinIndex;
        binaryCounter1Desc.connections[0].src.pinIndex = 111;
        expect(function() {
            Component.fromDescriptor("foo", null, binaryCounter1Desc as ComponentDescriptor, loader.descriptors);
        }).toThrow("connection '0' invalid");
        binaryCounter1Desc.connections[0].src.pinIndex = originalIndex;

        // invalid connection dest componentIndex
        originalIndex = binaryCounter1Desc.connections[1].dest.componentIndex;
        binaryCounter1Desc.connections[1].dest.componentIndex = 111;
        expect(function() {
            Component.fromDescriptor("foo", null, binaryCounter1Desc as ComponentDescriptor, loader.descriptors);
        }).toThrow("connection '1' invalid");
        binaryCounter1Desc.connections[1].dest.componentIndex = originalIndex;

        // invalid connection dest pinIndex
        originalIndex = binaryCounter1Desc.connections[1].dest.pinIndex;
        binaryCounter1Desc.connections[1].dest.pinIndex = 111;
        expect(function() {
            Component.fromDescriptor("foo", null, binaryCounter1Desc as ComponentDescriptor, loader.descriptors);
        }).toThrow("connection '1' invalid");
        binaryCounter1Desc.connections[1].dest.pinIndex = originalIndex;
    });
    test("Pin can connect multiple times", () => {
        let pin0 = new Pin("pin0", PinType.ForwardInputPin, PinDirection.INPUT, null);
        let pin1 = new Pin("pin1", PinType.ForwardOutputPin, PinDirection.OUTPUT, null);
        expect(pin0.name).toBe("pin0");
        expect(pin0.id()).toBe("pin0");

        pin0.connect(pin1);
        pin0.connect(pin1);
        pin1.connect(pin0);
        pin1.connect(pin0);
    });
    test("Pin.connect is able to detect corrupted state.", () => {
        let pin0 = new Pin("pin0", PinType.ForwardInputPin, PinDirection.INPUT, null);
        let pin1 = new Pin("pin1", PinType.ForwardOutputPin, PinDirection.OUTPUT, null);

        pin0.peers.push(pin1);
        expect(() => pin0.connect(pin1)).toThrow("Program is in corrupted state");
        expect(() => pin1.connect(pin0)).toThrow("Program is in corrupted state");
    });
});
