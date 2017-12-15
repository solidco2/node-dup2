#!/usr/bin/env node

let fs   = require('fs');
let dup2 = require('bindings')('dup2');

async function test () {

    let fd = fs.openSync('test.log', 'a');
    let newfd;

    try {
        newfd = await dup2.change(fd, 2);
    } catch (error) {
        throw error;
    }

    console.log('hello, file');
    console.log('New FileDescriptor:', newfd);
}

test()