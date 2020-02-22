
#include "amoeba_json_cjson_jansson.h"


json_t *convert_cjson_to_jansson(cJSON *src)
{
	if (src == NULL || cJSON_IsNull(src) || cJSON_IsInvalid(src))
		return json_null();

	if (cJSON_IsNumber(src))
		return json_real(src->valuedouble);

	if (cJSON_IsString(src))
		return json_string(cJSON_GetStringValue(src));

	if (cJSON_IsBool(src))
		return cJSON_IsFalse(src) ? json_false() : json_true();

	if (cJSON_IsArray(src))
	{
		json_t *result = json_array();

		cJSON *item;

		cJSON_ArrayForEach(item, src)
		{
			json_array_append(result, convert_cjson_to_jansson(item));
		}

		return result;
	}

	if (cJSON_IsObject(src))
	{
		json_t *result = json_object();

		cJSON *item;

		cJSON_ArrayForEach(item, src)
		{
			json_object_set(result, item->string, convert_cjson_to_jansson(item));
		}

		return result;
	}

	return json_null();
}

cJSON *convert_jansson_to_cjson(json_t *src)
{
	if (src == NULL || json_is_null(src))
		return cJSON_CreateNull();

	if (json_is_number(src))
		return cJSON_CreateNumber(json_number_value(src));

	if (json_is_string(src))
		return cJSON_CreateString(json_string_value(src));

	if (json_is_boolean(src))
		return json_is_false(src) ? cJSON_CreateFalse() : cJSON_CreateTrue();

	if (json_is_array(src))
	{
		cJSON *result = cJSON_CreateArray();

		size_t i;
		json_t *item;

		json_array_foreach(src, i, item)
		{
			cJSON_AddItemToArray(result, convert_jansson_to_cjson(item));
		}

		return result;
	}

	if (json_is_object(src))
	{
		cJSON *result = cJSON_CreateObject();

		const char *key;
		json_t *item;

		json_object_foreach(src, key, item)
		{
			cJSON_AddItemToObject(result, key, convert_jansson_to_cjson(item));
		}

		return result;
	}

	return cJSON_CreateNull();
}
