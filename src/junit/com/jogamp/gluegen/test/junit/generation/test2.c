#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int value;
} T2_UndefStruct;

#include "test2.h"

static int32_t CustomFuncA1(void* aptr) {
    (void)aptr;
    return 0xa001;
}
static int32_t CustomFuncA2(void* aptr) {
    (void)aptr;
    return 0xa002;
}

static int32_t CustomFuncB1(T2_UserData* pUserData) {
    return pUserData->balance;
}

static int32_t CustomFuncB2(T2_UserData* pUserData) {
    return -pUserData->balance;
}

static int32_t StaticInt32Array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
static T2_UndefStruct StaticUndefStructArray[] = { { 0 }, { 1 }, { 2 }, { 3 }, { 4 }, { 5 }, { 6 }, { 7 }, { 8 }, { 9 } };

T2_PointerStorage * createT2PointerStorage() {
    T2_PointerStorage * s = calloc(1, sizeof(T2_PointerStorage));
    for(int i=0; i<10; ++i) {
        s->int32PtrArray[i] = &StaticInt32Array[i];
    }
    s->undefStructPtr = (T2_UndefStructPtr) &StaticUndefStructArray[0];
    for(int i=0; i<10; ++i) {
        s->undefStructPtrArray[i] = (T2_UndefStructPtr) &StaticUndefStructArray[i];
    }

    for(int i=0; i<10; ++i) {
        s->customFuncAVariantsArray[i] = (T2_CustomFuncA) ( ( i %2 == 0 ) ? CustomFuncA1 : CustomFuncA2 );
    }
    for(int i=0; i<10; ++i) {
        s->customFuncBVariantsArray[i] = (T2_CustomFuncB) ( ( i %2 == 0 ) ? CustomFuncB1 : CustomFuncB2 );
    }
    return s;
}

void destroyT2PointerStorage(T2_PointerStorage * s) {
    assert(NULL!=s);
    memset(s, 0, sizeof(T2_PointerStorage));
    free(s);
}

int Initialize(T2_InitializeOptions* Options) {
    Options->ProductName = calloc(100, sizeof(char));
    Options->ProductVersion = calloc(100, sizeof(char));
    strncpy((char*)Options->ProductName, "Product Name", 100); // yuck: nonsense-warning
    strncpy((char*)Options->ProductVersion, "Product Version", 100); // yuck: nonsense-warning
    Options->ApiVersion = 1;
    Options->Reserved1 = (void*) 0x0000CAFFEEBEEFUL;
    Options->CustomFuncA1 = (T2_CustomFuncA) CustomFuncA1;
    *( (T2_CustomFuncA*) &Options->CustomFuncA2 ) = (T2_CustomFuncA) CustomFuncA2; // yuck: real yuck
    Options->CustomFuncB1 = CustomFuncB1;
    Options->CustomFuncB2 = CustomFuncB2;
    Options->customFuncBVariants[0] = CustomFuncB1;
    Options->customFuncBVariants[1] = CustomFuncB2;
    
    Options->OverrideThreadAffinity = NULL;
}

int Release(T2_InitializeOptions* Options) {
    if( NULL != Options->ProductName ) {
        free( (void*) Options->ProductName ); // yuck: nonsense-warning
        Options->ProductName = NULL;
    }
    if( NULL != Options->ProductVersion ) {
        free( (void*) Options->ProductVersion ); // yuck: nonsense-warning
        Options->ProductVersion = NULL;
    }
    Options->CustomFuncA1 = NULL;
    // Options->CustomFuncA2 = NULL; // keep const
    Options->CustomFuncB1 = NULL;
    Options->CustomFuncB2 = NULL;
}

//
//
//

static T2_CallbackFunc01 t2_callback01 = NULL;
static void* t2_callback01_userparam = NULL;

void MessageCallback01(T2_CallbackFunc01 func, void* userParam) {
    t2_callback01 = func;
    t2_callback01_userparam = userParam;
    fprintf(stderr, "XXX MessageCallback01 func %p, user %p\n", func, userParam);
    fflush(NULL);
}

void InjectMessageCallback01(size_t id, const char* msg) {
    if( NULL != t2_callback01 ) {
        fprintf(stderr, "XXX InjectMessageCallback01 func %p, user %p\n", t2_callback01, t2_callback01_userparam);
        fflush(NULL);
        (*t2_callback01)(id, msg, t2_callback01_userparam);
    }
}

//
//
//

static ALEVENTPROCSOFT alEventCallback_cb = NULL;
static void* alEventCallback_up = NULL;

void alEventCallback(ALEVENTPROCSOFT callback, void *userParam) {
    alEventCallback_cb = callback;
    alEventCallback_up = userParam;
}
void alEventCallbackInject(int eventType, int object, int param, const char* msg) {
    if( NULL != alEventCallback_cb ) {
        fprintf(stderr, "XXX InjectMessageCallback01 func %p, user %p\n", alEventCallback_cb, alEventCallback_up);
        fflush(NULL);
        (*alEventCallback_cb)(eventType, object, param, strlen(msg), msg, alEventCallback_up);
    }
}

//
//
//

static const int MAX_AL_BUFFER = 5;
static ALBUFFERCALLBACKTYPESOFT alBufferCallback0_callback[] = { NULL, NULL, NULL, NULL, NULL };
static void* alBufferCallback0_userptr[] = { NULL, NULL, NULL, NULL, NULL };

void alBufferCallback0(int buffer /* key */, int format, int freq, ALBUFFERCALLBACKTYPESOFT callback, void *userptr) {
    if( buffer < 0 || MAX_AL_BUFFER <= buffer ) {
        fprintf(stderr, "Error: alBufferCallback0: buffer not in range [0..%d), is %d\n", MAX_AL_BUFFER, buffer);
    } else {
        alBufferCallback0_callback[buffer] = callback;
        alBufferCallback0_userptr[buffer] = userptr;
        fprintf(stderr, "XXX alBufferCallback0 buffer %d -> func %p, user %p\n", buffer, callback, userptr);
    }
    fflush(NULL);
}
void alBufferCallback0Inject(int buffer, int sampledata, int numbytes) {
    if( buffer < 0 || MAX_AL_BUFFER <= buffer ) {
        fprintf(stderr, "Error: alBufferCallback0Inject: buffer not in range [0..%d), is %d\n", MAX_AL_BUFFER, buffer);
        fflush(NULL);
        return;
    }
    if( NULL != alBufferCallback0_callback[buffer] ) {
        fprintf(stderr, "XXX alBufferCallback0Inject: buffer %d, func %p, user %p\n", buffer, alBufferCallback0_callback[buffer], alBufferCallback0_userptr[buffer]);
        fflush(NULL);
        (*alBufferCallback0_callback[buffer])(buffer, alBufferCallback0_userptr[buffer], sampledata, numbytes);
    }
}

//
//
//

static ALBUFFERCALLBACKTYPESOFT alBufferCallback1_callback[] = { NULL, NULL, NULL, NULL, NULL };
static void* alBufferCallback1_userptr[] = { NULL, NULL, NULL, NULL, NULL };

void alBufferCallback1(int buffer /* key */, int format, int freq, ALBUFFERCALLBACKTYPESOFT callback, void *userptr) {
    if( buffer < 0 || MAX_AL_BUFFER <= buffer ) {
        fprintf(stderr, "Error: alBufferCallback1: buffer not in range [0..%d), is %d\n", MAX_AL_BUFFER, buffer);
    } else {
        alBufferCallback1_callback[buffer] = callback;
        alBufferCallback1_userptr[buffer] = userptr;
        fprintf(stderr, "XXX alBufferCallback1 buffer %d -> func %p, user %p\n", buffer, callback, userptr);
    }
    fflush(NULL);
}
void alBufferCallback1Inject(int buffer, int sampledata, int numbytes) {
    if( buffer < 0 || MAX_AL_BUFFER <= buffer ) {
        fprintf(stderr, "Error: alBufferCallback1Inject: buffer not in range [0..%d), is %d\n", MAX_AL_BUFFER, buffer);
        fflush(NULL);
        return;
    }
    if( NULL != alBufferCallback1_callback[buffer] ) {
        fprintf(stderr, "XXX alBufferCallback1Inject: buffer %d, func %p, user %p\n", buffer, alBufferCallback1_callback[buffer], alBufferCallback1_userptr[buffer]);
        fflush(NULL);
        (*alBufferCallback1_callback[buffer])(buffer, alBufferCallback1_userptr[buffer], sampledata, numbytes);
    }
}

//
//
//

static const int MAX_C11_BUFFER = 5;

static T2_CallbackFunc11 MessageCallback11a_callback[] = { NULL, NULL, NULL, NULL, NULL };
static T2_Callback11UserType* MessageCallback11a_userptr[] = { NULL, NULL, NULL, NULL, NULL };

void MessageCallback11a(size_t id /* key */, T2_CallbackFunc11 cbFunc, const T2_Callback11UserType* usrParam) {
    if( id < 0 || MAX_C11_BUFFER <= id ) {
        fprintf(stderr, "Error: MessageCallback11a: id not in range [0..%d), is %d\n", MAX_C11_BUFFER, id);
    } else {
        MessageCallback11a_callback[id] = cbFunc;
        MessageCallback11a_userptr[id] = (T2_Callback11UserType*)usrParam;
        if( NULL != usrParam ) {
            MessageCallback11a_userptr[id]->ApiVersion = 42;
        }
        fprintf(stderr, "XXX MessageCallback11a id %d -> func %p, user %p\n", id, cbFunc, usrParam);
    }
    fflush(NULL);
}
void MessageCallback11aInject(size_t id, long val) {
    if( id < 0 || MAX_C11_BUFFER <= id ) {
        fprintf(stderr, "Error: MessageCallback11aInjecta: id not in range [0..%d), is %d\n", MAX_C11_BUFFER, id);
        fflush(NULL);
        return;
    }
    if( NULL != MessageCallback11a_callback[id] ) {
        fprintf(stderr, "XXX MessageCallback11aInjecta: id %d, func %p, user %p\n", id, MessageCallback11a_callback[id], MessageCallback11a_userptr[id]);
        fflush(NULL);
        (*MessageCallback11a_callback[id])(id, MessageCallback11a_userptr[id], val);
    }
}

//
//
//

static T2_CallbackFunc11 MessageCallback11b_callback[] = { NULL, NULL, NULL, NULL, NULL };
static T2_Callback11UserType MessageCallback11b_userptr[];

void MessageCallback11b(size_t id /* key */, T2_CallbackFunc11 cbFunc, void* Data) {
    if( id < 0 || MAX_C11_BUFFER <= id ) {
        fprintf(stderr, "Error: MessageCallback11b: id not in range [0..%d), is %d\n", MAX_C11_BUFFER, id);
    } else {
        MessageCallback11b_callback[id] = cbFunc;
        MessageCallback11b_userptr[id].ApiVersion = 42;
        MessageCallback11b_userptr[id].Data = Data;
        fprintf(stderr, "XXX MessageCallback11b id %d -> func %p, user %p\n", id, cbFunc, Data);
    }
    fflush(NULL);
}
void MessageCallback11bInject(size_t id, long val) {
    if( id < 0 || MAX_C11_BUFFER <= id ) {
        fprintf(stderr, "Error: MessageCallback11bInject: id not in range [0..%d), is %d\n", MAX_C11_BUFFER, id);
        fflush(NULL);
        return;
    }
    if( NULL != MessageCallback11b_callback[id] ) {
        fprintf(stderr, "XXX MessageCallback11bInject: id %d, func %p, user %p\n", id, MessageCallback11b_callback[id], &MessageCallback11b_userptr[id]);
        fflush(NULL);
        (*MessageCallback11b_callback[id])(id, &MessageCallback11b_userptr[id], val);
    }
}

