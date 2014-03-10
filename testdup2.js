#!/usr/bin/env node
var dup2 = require("./build/Release/dup2");
var fs = require("fs");
var fd = fs.openSync("test.log", "a");
dup2.invoke(fd, 1);
console.log("hello, file");
