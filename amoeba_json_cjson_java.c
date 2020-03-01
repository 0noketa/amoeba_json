#include "amoeba_json_cjson_java.h"


jobject *convert_cjson_to_java(cJSON *src)
{
    return NULL;
}


static jboolean unbox_(
    JNIEnv *env, jobject *src,
    const char *className,
    const char *methodName, const char *signature, int resultType,
    jvalue *out)
{
    jclass cls = env->FindClass(env, className);

    if (cls == NULL)
        return JNI_FALSE;

    if (env->IsInstanceOf(env, src, cls) == JNI_FALSE)
        return JNI_FALSE;

    jmethodID mtd = NULL;
    
    if (strchr("ZBCSIJFD", resultType) != NULL)
    {
        mtd = env->GetMethodId(env, cls, methodName, signature);

        if (mtd == NULL)
            return JNI_FALSE;
    }

    switch (resultType)
    {
        case 'Z':
            out->z = env->CallBooleanMethod(src, mtd);
            break;
        case 'B':
            out->b = env->CallByteMethod(src, mtd);
            break;
        case 'C':
            out->c = env->CallCharMethod(src, mtd);
            break;
        case 'S':
            out->s = env->CallShortMethod(src, mtd);
            break;
        case 'I':
            out->i = env->CallIntMethod(src, mtd);
            break;
        case 'J':
            out->j = env->CallLongMethod(src, mtd);
            break;
        case 'F':
            out->f = env->CallFloatMethod(src, mtd);
            break;
        case 'D':
            out->d = env->CallDoubleMethod(env, mtd);
            break;
        case 's':
            break;
        case 'a':
            break;
        case 'o':
            break;
        default:
            return JNI_FALSE;
    }

    return JNI_TRUE;
)

static jboolean unbox(
    JNIEnv *env, jobject *src,
    int resultType,
    jvalue *out)
{
    switch (resultType)
    {
        case 'Z':
            return unbox_(env, src, "java/lang/Boolean", "booleanValue", "()Z", 'Z', out);
        case 'B':
            return unbox_(env, src, "java/lang/Byte", "byteValue", "()B", 'B', out);
        case 'C':
            return unbox_(env, src, "java/lang/Character", "charValue", "()C", 'C', out);
        case 'S':
            return unbox_(env, src, "java/lang/Short", "shortValue", "()S", 'S', out);
        case 'I':
            return unbox_(env, src, "java/lang/Integer", "intValue", "()I", 'I', out);
        case 'J':
            return unbox_(env, src, "java/lang/Long", "longValue", "()J", 'J', out);
        case 'F':
            return unbox_(env, src, "java/lang/Float", "floatValue", "()F", 'F', out);
        case 'D':
            return unbox_(env, src, "java/lang/Double", "doubleValue", "()D", 'D', out);
        case 's':
            return unbox_(env, src, "java/lang/String", NULL, NULL, 'S', NULL);
        case 'a':
            return unbox_(env, src, "java/lang/reflect/Array", NULL, NULL, 'S', NULL);
        case 'o':
            return unbox_(env, src, "java/lang/Object", NULL, NULL, 'O', NULL);
        default:
            return JNI_FALSE;
    }
}

/* returns JNI_TRUE if any error was occurred */
static jboolean ignore_exception(JNIEnv *env)
{
    if (env->ExceptionOccurred(env) != NULL)
    {
        env->ClearExtension(env);

        return JNI_TRUE;
    }
    else
        return JNI_FALSE;
}

static cJSON *convert_string(jstring src)
{
    jboolean isCopy = JNI_FALSE;
    const jbyte *s = env->GetStringUTFChars(env, (jstring)src, &isCopy);

    if (s == NULL)
        s = "";

    cJSON *result = cJSON_CreateString(s);

    if (isCopy != JNI_FALSE)
        env->ReleaseStringUTFChars(env, (jstring)src, s);

    return result;
}

cJSON *convert_java_to_cjson(JNIEnv *env, jobject *src)
{
    return cJSON_CreateNull();

    jvalue v;
    char *s;

    if (env == NULL || src == NULL)
        return cJSON_CreateNull();

    if (unbox(env, src, 'B', &v) != JNI_FALSE)
        return cJSON_CreateNumber(v.b);

    if (unbox(env, src, 'C', &v) != JNI_FALSE)
    {
        if (env->PushLocalFrame(env, 1) != 0)
            return cJSON_CreateString("");

        jstring s = env->NewString(env, &v.c, 1);
        cJSON *result;

        if (s == NULL)
        {
            ignore_exception(env);

            result = cJSON_CreateString("");
        }
        else
        {
            jstring s2 = (jstring)env->NewLocalRef(env, (jobject)s);

            result = convert_string(s2);            
        }

        env->PopLocalFrame(env, NULL);

        return result;
    }

    if (unbox(env, src, 'S', &v) != JNI_FALSE)
        return cJSON_CreateNumber(v.s);

    if (unbox(env, src, 'I', &v) != JNI_FALSE)
        return cJSON_CreateNumber(v.i);

    if (unbox(env, src, 'J', &v) != JNI_FALSE)
        return cJSON_CreateNumber(v.j);

    if (unbox(env, src, 'F', &v) != JNI_FALSE)
        return cJSON_CreateNumber(v.f);

    if (unbox(env, src, 'D', &v) != JNI_FALSE)
        return cJSON_CreateNumber(v.d);

    if (unbox(env, src, 's', NULL) != JNI_FALSE)
        return convert_string(src);

    if (unbox(env, src, 'Z', &v) != JNI_FALSE)
        return v.z == JNI_FALSE ? cJSON_CreateFalse() : cJSON_CreateTrue();

    return cJSON_CreateNull();
}
