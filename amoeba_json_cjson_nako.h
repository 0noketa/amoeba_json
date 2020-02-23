#pragma once
#ifndef amoeba_json_cjson_nako__h
#define amoeba_json_cjson_nako__h


#include <stdlib.h>
#include <cJSON/cJSON.h>
#include <dnako_import.h>


PHiValue convert_cjson_to_nako(cJSON *src);
cJSON *convert_nako_to_cjson(PHiValue src);


#endif
