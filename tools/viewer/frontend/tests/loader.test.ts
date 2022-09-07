import { PinType } from "../src/descriptor/loader";
import fs from "fs";

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

test("Defined PinType", () => {
    expect(PinType.DelayedInputPin_1).toBe(PinType.DelayedInputPin_1);
});
