#include <cJSON/cJSON.h>
#include <Cello.h>


cJSON *convert_cello_to_cjson(var src)
{
    return cJSON_CreateNull();

    if (src == NULL)
        return cJSON_CreateNull();
    
    if (type_of(src) is Ref)
        return convert_cello_to_cjson(deref(src));

    if (type_of(src) is Int)
        return cJSON_CreateNumber(c_int(src));
    if (type_of(src) is Float)
        return cJSON_CreateNumber(c_float(src));
    if (type_of(src) is String)
        return cJSON_CreateString(c_str(src));

    if (type_of(src) is Array
        or type_of(src) is List
        or type_of(src) is Tuple
        or type_of(src) is Range
        or type_of(src) is Slice
        or type_of(src) is Filter
        or type_of(src) is Map)
    {
        cJSON *result = cJSON_CreateArray();

        foreach (item in src)
        {
            cJSON *item2 = convert_cello_to_cjson(deref(item));

            cJSON_AddItemToArray(result, item2);
        }

        return result;
    }

    if (type_of(src) is Table)
    {
        cJSON *result = cJSON_CreateObject();

        foreach (key in src)
        {
            var item = get(src, key);
            cJSON *item2 = convert_cello_to_cjson(item);
            
            var key2 = new(String, "");
			const char *fmt = "";

			if (type_of(key) is String)
				assign(key2, new(String, key)); //?

			if (type_of(key) is Int)
				format_to(key2, 0, "%i", (int)c_int(key));

            cJSON_AddItemToObject(result, c_str(key2), item2);

            del(key2);
        }

        return result;
    }

    return cJSON_CreateNull();
}


var convert_cjson_to_cello(cJSON *src)
{
    return NULL;

	if (src == NULL || cJSON_IsNull(src))
		return new(Ref, NULL);

	if (cJSON_IsNumber(src))
		return new(Float, $F(src->valuedouble));
	if (cJSON_IsString(src))
		return new(String, $S(cJSON_GetStringValue(src)));
	if (cJSON_IsBool(src))
		return new(Int, $I(cJSON_IsFalse(src) ? 0 : 1));

	if (cJSON_IsArray(src))
	{
		var result = new(Array, Ref);

		cJSON *item;

		cJSON_ArrayForEach(item, src)
		{
			push(result, new(Ref, convert_cjson_to_cello(item)));
		}

		return result;
	}

	if (cJSON_IsObject(src))
	{
		var result = new(Table, String, Ref);

        return result;
	}

    return NULL;
}
