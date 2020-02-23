#include <stdlib.h>
#include <cJSON/cJSON.h>
#include <dnako_import.h>


PHiValue convert_cjson_to_nako(cJSON *src)
{
	PHiValue result = nako_var_new(NULL);

	if (src == NULL || cJSON_IsNull(src))
		return result;

	if (cJSON_IsNumber(src))
	{
		nako_double2var(src->valuedouble, result);

		return result;
	}

	if (cJSON_IsString(src))
	{
		nako_str2var(cJSON_GetStringValue(src), result);

		return result;
	}

	if (cJSON_IsBool(src))
	{
		hi_setBool(result, cJSON_IsFalse(src) ? FALSE : TRUE);

		return result;
	}

	if (cJSON_IsArray(src))
	{
		nako_ary_create(result);

		cJSON *item;

		cJSON_ArrayForEach(item, src)
		{
			nako_ary_add(result, convert_cjson_to_nako(item));
		}

		return result;
	}

	if (cJSON_IsObject(src))
	{
		nako_group_create(result);

		cJSON *item;

		cJSON_ArrayForEach(item, src)
		{
			PHiValue val = convert_cjson_to_nako(item);

			val->VarID = nako_tango2id(item->string);

			nako_group_addMember(result, val);
		}

		return result;
	}

	return result;
}


cJSON *convert_nako_to_cjson(PHiValue src)
{
	if (src == NULL)
		return cJSON_CreateNull();

	switch (src->VType)
	{
	case varNil:
	case varPointer:
	case varFunc:
	case varLink:
		return cJSON_CreateNull();

	case varInt:
		return cJSON_CreateNumber(hi_int(src));

	case varFloat:
		return cJSON_CreateNumber(hi_float(src));

	case varStr:
		return cJSON_CreateString(hi_str(src));

	case varArray:
		{
			cJSON *result = cJSON_CreateArray();

			size_t size = nako_ary_count(src);
			size_t i;

			for (i = 0; i < size; ++i)
			{
				cJSON_AddItemToArray(result, convert_nako_to_cjson(nako_ary_get(src, i)));
			}

			return result;
		}
	case varGroup:
		{
			cJSON *result = cJSON_CreateObject();

			// can not get list of keys

			return result;
		}
	default:
		return cJSON_CreateNull();
	}
}


