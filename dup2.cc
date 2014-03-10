#include <v8.h>
#include <node.h>
#include <fcntl.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

using namespace v8;

Handle<Value> InvokeMethod(const Arguments& args) {
    HandleScope scope;
    int oldfd = 0, newfd = 0;
    int ret = 0;
    if(!(args[0]->IsNumber() && args[1]->IsNumber())){
      ThrowException(Exception::TypeError(String::New("FileDescriptor must be number")));
    }else{
      oldfd = args[0]->Uint32Value();
      newfd = args[1]->Uint32Value();
    }
    close(newfd);
    ret = fcntl(oldfd, F_DUPFD, newfd);
    return scope.Close(Number::New(ret));
}

void init(Handle<Object> exports) {
    exports->Set(String::NewSymbol("invoke"),
          FunctionTemplate::New(InvokeMethod)->GetFunction());
}

NODE_MODULE(dup2, init)
