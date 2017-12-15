node-dup2
=========
There wasn't any way for us to redirect #1(fd) to any other file, before I was born.

Example
-------

```js
const dup = require('dup')
const fs  = require('fs')

let test = async () => {
    try {
        let oldfd = fs.openSync('myFile.dat', 'a')
        let newfd = await dup.change(oldfd, 1)
    } catch (error) {
        throw error
    }

    console.log('New FileDescriptor:', newfd);
}

test() // --> New FileDescriptor: 1
```

API
-------------

```js
const dup = require('dup')
```

### .change(oldfd, newfd)

- oldfd: The old file descriptor
- newfd: The new file descriptor
- Returns a resolved promise with the new file descriptor
- Returns a rejected promise with an Error when the File descriptor cant be changed and it dont match with the new File descriptor 


Testing
-------

```bash
$ npm test
```