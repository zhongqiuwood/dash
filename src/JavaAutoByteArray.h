//
// Created by shine jiang on 2018/6/10.
//
#ifndef _JAVA_AUTO_BYTE_ARRAY_H_
#define _JAVA_AUTO_BYTE_ARRAY_H_

#include "jni.h"

class JavaAutoByteArray
{
public:
    JavaAutoByteArray(JNIEnv *env, jbyteArray javaByteArray)
    {
        m_env = env;
        m_javaByteArray = javaByteArray;
        m_cppByteArray = 0;
    }

    virtual ~JavaAutoByteArray()
    {
        if (m_cppByteArray != 0)
        {
            m_env->ReleaseByteArrayElements
                    (m_javaByteArray, (jbyte *)m_cppByteArray, JNI_ABORT); // Does not throw
        }
    }

    void marshalJavaByteArray()
    {
        m_length = m_env->GetArrayLength(m_javaByteArray);
        jboolean isCopy = JNI_FALSE;
        m_cppByteArray = (char*)m_env->GetByteArrayElements(m_javaByteArray, &isCopy);
    }

    unsigned int getLength()
    {
        return m_length;
    }

    char* getCppByteArray()
    {
        return m_cppByteArray;
    }

private:

    jbyteArray m_javaByteArray;
    JNIEnv *m_env;
    char *m_cppByteArray;
    unsigned int m_length;

};

#endif // _JAVA_AUTO_BYTE_ARRAY_H_