
#include "amoeba_json_cjson_yajl.h"


cJSON *convert_yajl_to_cjson(yajl_val src)
{
	if (src == NULL || YAJL_IS_NULL(src))
		return cJSON_CreateNull();

	if (YAJL_IS_DOUBLE(src))
		return cJSON_CreateNumber(YAJL_GET_DOUBLE(src));

	if (YAJL_IS_INTEGER(src))
		return cJSON_CreateNumber(YAJL_GET_INTEGER(src));

	if (YAJL_IS_STRING(src))
		return cJSON_CreateString(YAJL_GET_STRING(src));

	if (YAJL_IS_FALSE(src))
		return cJSON_CreateFalse();

	if (YAJL_IS_TRUE(src))
		return cJSON_CreateTrue();

	if (YAJL_IS_ARRAY(src))
	{
		cJSON *result = cJSON_CreateArray();

		size_t size = src->u.array.len;
		size_t i;

		for (i = 0; i < size; ++i)
		{
			cJSON_AddItemToArray(result, convert_yajl_to_cjson(src->u.array.values[i]));
		}

		return result;
	}

	if (YAJL_IS_OBJECT(src))
	{
		cJSON *result = cJSON_CreateObject();

		size_t size = src->u.object.len;
		size_t i;

		for (i = 0; i < size; ++i)
		{
			const char *key = src->u.object.keys[i];
			yajl_val val = src->u.object.values[i];

			cJSON_AddItemToObject(result, key, convert_yajl_to_cjson(val));
		}

		return result;
	}

	return cJSON_CreateNull();
}
