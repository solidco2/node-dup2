node-dup2
=========
There isn't any way for us to redirect #1(fd) to any other file, before I born.

好吧，我是说，node里面没有dup2函数（当然你可以先关掉一个文件描述符，然后立即打开一个文件），我的出现就是用来提供dup2支持的。
Demo
require("node-dup2").invoke(oldfd, newfd);
此句会先关掉newfd文件描述符，而克隆oldfd的文件打开状态到newfd上。
失败时返回-1
