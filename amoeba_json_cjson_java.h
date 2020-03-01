#ifndef amoeba_json_cjson_java__h
#define amoeba_json_cjson_java__h


#include <stdlib.h>
#include <cJSON/cJSON.h>
#include <jni.h>


extern jobject *convert_cjson_to_java(JNIEnv *env, cJSON *src);
extern cJSON *convert_java_to_cjson(JNIEnv *env, jobject *src);


#endif
