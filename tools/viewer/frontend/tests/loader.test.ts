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
