#pragma once
#ifndef amoeba_json_cjson_yajl__h
#define amoeba_json_cjson_yajl__h


#include <cJSON/cJSON.h>
#include <yajl/yajl_tree.h>
//#include <yajl/yajl_gen.h>


cJSON *convert_yajl_to_cjson(yajl_val src);


#endif
