#include <node_api.h>
#include <fcntl.h>
#include <assert.h>

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

/**
 * Changes a old file descriptor to a new one (rather duplicates it)
 */
napi_value change (napi_env env, napi_callback_info info) {
    napi_status   status = napi_generic_failure;
    napi_deferred deferred;     // this will be either resolved or rejected and freed after
    napi_value    promise;      // promise which will be returned
    napi_value    argv[2];      // arg value 
    napi_value    ret;          // refers to the number that was returnd by fcntl
    napi_value    This;         // refers to the js this
    
    napi_valuetype argType1;
    napi_valuetype argType2;

    uint32_t oldfd = 0;
    uint32_t newfd = 0;
    size_t   argc = 2;
    
    // create promise
    status = napi_create_promise(env, &deferred, &promise);
    assert(status == napi_ok);

    // get args
    status = napi_get_cb_info(env, info, &argc, argv, &This, nullptr);
    assert(status == napi_ok);

    // argc is an "in-out" argument of "napi_get_cb_info" you pass the 
    // expected arg count in and get the actual count so you can compare it
    // against something
    if (argc < 2) {
        napi_throw_range_error(env, nullptr, "The function needs two arguments");
        return nullptr;
    } 

    // type checking
    status = napi_typeof(env, argv[0], &argType1);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Old FileDescriptor must be a Number");
        return nullptr;
    }

    status = napi_typeof(env, argv[1], &argType2);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "New FileDescriptor must be a Number");
        return nullptr;
    }

    // get the actual values and convert them to C types
    status = napi_get_value_uint32(env, argv[0], &oldfd);
    assert(status == napi_ok);

    status = napi_get_value_uint32(env, argv[1], &newfd);
    assert(status == napi_ok);

    // all the duplicating magic
    close(newfd);

    int retfd = fcntl(oldfd, F_DUPFD, newfd);
    
    // either resolve or reject with an error
    if (newfd == (uint32_t)retfd) {
        status = napi_create_uint32(env, retfd, &ret);
        assert(status == napi_ok);

        status = napi_resolve_deferred(env, deferred, ret);
        assert(status == napi_ok);
    } else {
        napi_value error;
        napi_value nReason;
        const char* reason = "Changed FileDescriptor not matches the chosen";

        status = napi_create_string_utf8(env, reason, sizeof(reason), &nReason);
        assert(status == napi_ok);

        status = napi_create_error(env, nullptr, nReason, &error);
        assert(status == napi_ok);

        status = napi_reject_deferred(env, deferred, error);
        assert(status == napi_ok);
    }

    // the deferred object was freed by "napi_resolve_deferred" or
    // "napi_reject_deferred" so we can set it to NULL
    deferred = NULL;

    // return the promise to the js land so we can do async stuff with it
    return promise;
}

// helper to create a function on the exports value object
#define DECLARE_NAPI_METHOD(name, func) \
    { name, 0, func, 0, 0, 0, napi_default, 0 }

/**
 *  Initializes the module and sets up the change property on the exports object
 */
napi_value init (napi_env env, napi_value exports) {
    napi_status status;

    napi_property_descriptor changeDescriptor = DECLARE_NAPI_METHOD("change", change);

    status = napi_define_properties(env, exports, 1, &changeDescriptor);
    assert(status == napi_ok);

    return exports;

}

// NODE_GYP_MODULE_NAME refers to the name of the addon in the binding.gyp file 
NAPI_MODULE(NODE_GYP_MODULE_NAME, init)