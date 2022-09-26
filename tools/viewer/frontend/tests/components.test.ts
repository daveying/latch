import { Loader } from "../src/descriptor/loader";
import { Component } from "../src/descriptor/components";

describe("Component can be constructed from Loader.", () => {
    test("ALU1 component can be constructed from Loader", () => {
        let loader = Loader.fromFile(process.env.DESCRIPTOR_PATH as string);
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
        expect(alu1.parent).toBe(null);
        expect(alu1.pins.length).toBe(6);
        expect(alu1.pins[5].id()).toBe("alu1.Co");
        expect(alu1.subcomponents.length).toBe(3);
        expect(alu1.subcomponents[1].name).toBe("negater[0]");
        expect(alu1.subcomponents[1].id()).toBe("alu1.negater[0]");
    });
});
