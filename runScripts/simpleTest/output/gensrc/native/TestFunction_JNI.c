/* !---- DO NOT EDIT: This file autogenerated by com/jogamp/gluegen/JavaEmitter.java on Fri Aug 23 16:47:28 KST 2024 ----! */
/* !---- C-Unit: TestFunction_JNI.c, output/gensrc/native/TestFunction_JNI.c ----! */

#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

JavaVM* JVMUtil_GetJavaVM();
JNIEnv* JVMUtil_GetJNIEnv(int asDaemon, int* jvmAttached);
void JVMUtil_ReleaseJNIEnv(JNIEnv* env, int detachJVM);


static jobject JVMUtil_NewDirectByteBufferCopy(JNIEnv *env, jclass clazzBuffers, void * source_address, size_t capacity); /* forward decl. */

 #include "function.h"

/*   Java->C glue code:
 *   Java package: testfunction.TestFunction
 *    Java method: int one_plus(int a)
 *     C function: int one_plus(int a)
 */
JNIEXPORT jint JNICALL 
Java_testfunction_TestFunction_one_1plus__I(JNIEnv *env, jclass _unused, jint a) {
  int _res;
  _res = (int) one_plus((int) a);
  jint _res_jni = _res;
  return _res_jni;
}


