#pragma once
#ifndef amoeba_json_cjson_neko__h
#define amoeba_json_cjson_neko__h


#include <stdlib.h>
#include <cJSON/cJSON.h>
#include <neko.h>


#ifndef AMOEBA_JSON_REC_DEPTH_LIM
#define AMOEBA_JSON_REC_DEPTH_LIM 256
#endif


extern value convert_cjson_to_neko(cJSON *src);
extern cJSON *convert_neko_to_cjson(value src);


#endif
