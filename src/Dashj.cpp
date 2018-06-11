//
// Created by shine jiang on 2018/6/10.
//

#include "com_okcoin_vault_jni_digibyte_Digibytej.h"
#include "jni.h"
#include <stdio.h>
#include <string>
#include <list>

#include "JavaAutoByteArray.h"

using namespace std;

char* jstring2char(JNIEnv* env, jstring jstr);
jstring char2jstring(JNIEnv* env, const char* pat);
jobjectArray getJstringArray(JNIEnv* env, const char* param);
jobjectArray stringList2jobjectArray(JNIEnv* env, const list<string>& stringList);
char** jobjectArray2charArray(JNIEnv *env, jobjectArray javaParams);
jbyteArray string2byteArray(JNIEnv *env, const string& cppString);

extern string SignTx(const string& inputList,
                     const string& outAddrList,
                     const string& outDataList,
                     const string& priKeysJson,
                     const string& prevTxsJson,
                     const string& signType);
extern bool GetAddressFromPrivateKey(const string& strSert, string& address);

JNIEXPORT jstring JNICALL Java_com_okcoin_vault_jni_digibyte_Digibytej_GetAddressByPrivateKey
        (JNIEnv * env, jobject obj, jstring priKey) {

    string address;
    char *priSert = jstring2char(env, priKey);
    bool is_succ = GetAddressFromPrivateKey(priSert, address);

    return char2jstring(env, address.c_str());
}

JNIEXPORT jstring JNICALL Java_com_okcoin_vault_jni_digibyte_Digibytej_SignTranscation
        (JNIEnv *env, jobject obj,
         jstring inputList,
         jstring outAddrList,
         jstring outDataList,
         jstring priKeysJson,
         jstring prevTxsJson,
         jstring signType) {

    char *pInputList = jstring2char(env, inputList);
    char *pOutAddrList = jstring2char(env, outAddrList);
    char *pOutDataList = jstring2char(env, outDataList);
    char *pPriKeysJson = jstring2char(env, priKeysJson);
    char *pPrevTxsJson = jstring2char(env, prevTxsJson);
    char *psignJson = jstring2char(env, signType);

    string txSignHexStr = SignTx(pInputList, pOutAddrList, pOutDataList, pPriKeysJson, pPrevTxsJson, psignJson);
    return char2jstring(env, txSignHexStr.c_str());
}

jbyteArray string2byteArray(JNIEnv *env, const string& cppString) {

    int unsignedTxLen = cppString.length();

    jbyteArray jByteArray = env->NewByteArray(cppString.length());
    env->SetByteArrayRegion(jByteArray, 0, unsignedTxLen, (jbyte *) cppString.c_str());
    return jByteArray;
}

jobjectArray stringList2jobjectArray(JNIEnv* env, const list<string>& stringList)
{
    size_t size = stringList.size();
    jclass objClass = env->FindClass("java/lang/String");
    jobjectArray resArray = env->NewObjectArray(size, objClass, 0);
    jsize i = 0;
    for (auto &s: stringList) {
        env->SetObjectArrayElement(resArray, i++, char2jstring(env, s.c_str()));
    }
    return resArray;
}

char* jstring2char(JNIEnv* env, jstring jstr)
{
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);

    if (alen > 0)
    {
        rtn = (char*)malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

jstring char2jstring(JNIEnv* env, const char* pat)
{
    jclass strClass = env->FindClass("Ljava/lang/String;");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");

//    printf("char2jstring: par strlen<%zu>, sizeof<%lu>\n", strlen(pat), sizeof(pat));
    jbyteArray bytes = env->NewByteArray(strlen(pat));
    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
    jstring encoding = env->NewStringUTF("utf-8");
    return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}


char** jobjectArray2charArray(JNIEnv *env, jobjectArray javaParams) {
    int paramsSize = env->GetArrayLength(javaParams);
    char** cppParam = new char*[paramsSize + 1];

    static char tmp[] = "dummy";
    cppParam[0] = tmp; // bypass argv[0] in monero_entry() method
    printf("paramId<%d>, param<%s>\n", 0, cppParam[0]);

    int paramIdx = 1;

    for(int i = 0; i < paramsSize; ++i) {
        jstring javaP = (jstring)env->GetObjectArrayElement(javaParams, i);
        char* p = jstring2char(env, javaP);
        printf("paramId<%d>, param<%s>\n", paramIdx, p);
        cppParam[paramIdx++] = p;
    }
    return cppParam;
}

