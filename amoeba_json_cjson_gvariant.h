#pragma once
#ifndef amoeba_json_cjson_gvariant__h
#define amoeba_json_cjson_gvariant__h


#include <stdlib.h>
#include <cJSON/cJSON.h>
#include <glib-object.h>


/**
 * convert_cjson_to_gvariant:
 * @src: read-only cJSON.
 * Returns: GVariant. caller should delete. converts every null to empty string.
 * Stability: Stable
 */
extern GVariant *convert_cjson_to_gvariant(cJSON *src);

/**
 * convert_gvariant_to_cjson:
 * @src: read-only GValiant.
 * Returns: cJSON. caller should delete.
 * Stability: Stable
 */
extern cJSON *convert_gvariant_to_cjson(GVariant *src);


#endif
