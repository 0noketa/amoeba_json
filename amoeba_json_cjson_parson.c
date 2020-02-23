// something in parson conflicts to jansson and json-c

#include "amoeba_json_cjson_parson.h"


JSON_Value *convert_cjson_to_parson(cJSON *src)
{
	if (src == NULL || cJSON_IsNull(src))
		return json_value_init_null();

	if (cJSON_IsNumber(src))
		return json_value_init_number(src->valuedouble);

	if (cJSON_IsString(src))
		return json_value_init_string(cJSON_GetStringValue(src));

	if (cJSON_IsBool(src))
		return json_value_init_boolean(cJSON_IsFalse(src) ? 0 : 1);

	if (cJSON_IsArray(src))
	{
		JSON_Value *result = json_value_init_array();
		JSON_Array *result2 = json_array(result);

		cJSON *item;

		cJSON_ArrayForEach(item, src)
		{
			json_array_append_value(result2, convert_cjson_to_parson(item));
		}

		return result;
	}

	if (cJSON_IsObject(src))
	{
		JSON_Value *result = json_value_init_object();
		JSON_Object *result2 = json_object(result);

		cJSON *item;

		cJSON_ArrayForEach(item, src)
		{
			json_object_dotset_value(result2, item->string, convert_cjson_to_parson(item));
		}

		return result;
	}

	return json_value_init_null();
}

cJSON *convert_parson_to_cjson(JSON_Value *src)
{
	if (src == NULL)
		return cJSON_CreateNull();

	switch (json_value_get_type(src))
	{
	case JSONNull:
	case JSONError:
	case JSONSuccess:
		return cJSON_CreateNull();

	case JSONNumber:
		return cJSON_CreateNumber(json_number(src));

	case JSONString:
		return cJSON_CreateString(json_string(src));

	case JSONBoolean:
		return json_boolean(src) == 0 ? cJSON_CreateFalse() : cJSON_CreateTrue();

	case JSONArray:
		{
			cJSON *result = cJSON_CreateArray();

			JSON_Array *src2 = json_array(src);
			size_t size = json_array_get_count(src2);
			size_t i;

			for (i = 0; i < size; ++i)
			{
				JSON_Value *item = json_array_get_value(src2, i);

				cJSON_AddItemToArray(result, convert_parson_to_cjson(item));
			}

			return result;
		}
	case JSONObject:
		{
			cJSON *result = cJSON_CreateObject();

			JSON_Object *src2 = json_object(src);
			size_t size = json_object_get_count(src2);
			size_t i;

			for (i = 0; i < size; ++i)
			{
				const char *key = json_object_get_name(src2, i);
				JSON_Value *item = json_object_get_value_at(src2, i);

				cJSON_AddItemToObject(result, key, convert_parson_to_cjson(item));
			}

			return result;
		}
	default:
		return cJSON_CreateNull();
	}
}
