import { Loader } from "../src/descriptor/loader";
import * as fs from "fs";

describe("Make sure the descriptor file is ready for loader tests", () => {
    test("Env var DESCRIPTOR_PATH is set", () => {
        expect(process.env.DESCRIPTOR_PATH).toBeTruthy();
    });

    test("Can open component descriptor file", () => {
        fs.readFile(process.env.DESCRIPTOR_PATH as string, (err, data) => {
            expect(err).toBeFalsy();
            expect(data).toBeTruthy();
        });
    });
});

describe("Loader can load component descriptor file and convert it to ComponentDescriptor instances", () => {
    test("Loader can load component descriptor file", () => {
        let loader = Loader.fromFile(process.env.DESCRIPTOR_PATH as string);
        expect(loader).toBeTruthy();
    });
});

describe("Descriptor classes can catch invalid JSON descriptor", () => {
    let componentDescriptors = JSON.parse(fs.readFileSync(process.env.DESCRIPTOR_PATH as string).toString());
    expect(componentDescriptors).toBeTruthy();
    let ALU1Desc = componentDescriptors.descriptors.ALU1;
    expect(ALU1Desc).toBeTruthy();

    function getTestData() {
        return JSON.parse(JSON.stringify(ALU1Desc));
    }
    test("PinDescriptor throws when pin JSON invalid", () => {
        let loader = new Loader();
        let jsonData = getTestData();
        loader.loadSingle(jsonData);

        loader = new Loader();
        jsonData.pins[0].type = 123;
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for PinDescriptor");
        jsonData.pins[0].type = undefined;
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for PinDescriptor");

        jsonData = getTestData();
        jsonData.pins[0].name = 123;
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for PinDescriptor");
        jsonData.pins[0].name = undefined;
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for PinDescriptor");

        jsonData = getTestData();
        jsonData.pins[0].direction = "IN";
        expect(() => loader.loadSingle(jsonData)).toThrow("direction (IN) is invalid");
    });

    test("SubcomponentDescriptor throws when JSON invalid", () => {
        let loader = new Loader();

        let jsonData = getTestData();
        jsonData.subcomponents[0].name = 123;
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for SubcomponentDescriptor");
        jsonData.subcomponents[0].name = undefined;
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for SubcomponentDescriptor");

        jsonData = getTestData();
        jsonData.subcomponents[0].type = 123;
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for SubcomponentDescriptor");
        jsonData.subcomponents[0].type = undefined;
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for SubcomponentDescriptor");
    });

    test("Endpoint throws when JSON invalid", () => {
        let loader = new Loader();

        let jsonData = getTestData();
        jsonData.connections[0].src.componentIndex = "";
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for Endpoint");
        jsonData.connections[0].src.componentIndex = undefined
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for Endpoint");

        jsonData = getTestData();
        jsonData.connections[0].dest.pinIndex = "";
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for Endpoint");
        jsonData.connections[0].dest.pinIndex = undefined;
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for Endpoint");
    });

    test("ConnectionDescriptor throws when JSON invalid", () => {
        let loader = new Loader();

        let jsonData = getTestData();
        jsonData.connections[0].src = undefined;
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for ConnectionDescriptor");

        jsonData = getTestData();
        jsonData.connections[0].dest = undefined;
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for ConnectionDescriptor");
    });

    test("ComponentDescriptor throws when JSON invalid", () => {
        let loader = new Loader();

        let jsonData = getTestData();
        jsonData.pins = {};
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for ComponentDescriptor");
        jsonData.pins = undefined;
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for ComponentDescriptor");

        jsonData = getTestData();
        jsonData.subcomponents = {};
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for ComponentDescriptor");
        jsonData.subcomponents = undefined;
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for ComponentDescriptor");

        jsonData = getTestData();
        jsonData.connections = {};
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for ComponentDescriptor");
        jsonData.connections = undefined;
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for ComponentDescriptor");

        jsonData = getTestData();
        jsonData.type = {};
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for ComponentDescriptor");
        jsonData.type = undefined;
        expect(() => loader.loadSingle(jsonData)).toThrow("Invalid JSON data for ComponentDescriptor");
    });

    test("Loader throws when component names conflicts", () => {
        let loader = new Loader();

        let jsonData = getTestData();
        loader.loadSingle(jsonData);
        expect(() => loader.loadSingle(jsonData)).toThrow("has already loaded");
    });
})
