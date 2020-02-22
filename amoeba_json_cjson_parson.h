#pragma once
#ifndef amoeba_json_cjson_parson__h
#define amoeba_json_cjson_parson__h


#include <stdlib.h>
#include <cjson/cJSON.h>
#include <parson.h>


JSON_Value *convert_cjson_to_parson(cJSON *src);
cJSON *convert_parson_to_cjson(JSON_Value *src);


#endif
