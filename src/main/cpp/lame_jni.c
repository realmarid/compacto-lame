#include <jni.h>
#include <stdint.h>
#include <stdlib.h>
#include "lame.h"

/* JNI bridge for com.nujiwave.compacto.engine.audio.LameNative. CBR encoder only. */

#define JNI_FN(name) Java_com_nujiwave_compacto_engine_audio_LameNative_##name

static void throw_ise(JNIEnv *env, const char *msg) {
    jclass cls = (*env)->FindClass(env, "java/lang/IllegalStateException");
    if (cls != NULL) (*env)->ThrowNew(env, cls, msg);
}

JNIEXPORT jlong JNICALL JNI_FN(nativeInit)(JNIEnv *env, jobject thiz, jint sampleRate, jint channels,
                                           jint bitrateKbps, jint quality) {
    (void) thiz;
    lame_t g = lame_init();
    if (g == NULL) return 0;
    lame_set_in_samplerate(g, sampleRate);
    lame_set_num_channels(g, channels);
    lame_set_mode(g, channels == 1 ? MONO : JOINT_STEREO);
    lame_set_VBR(g, vbr_off);
    lame_set_brate(g, bitrateKbps);
    lame_set_quality(g, quality);
    /* No Xing/Info header: we stream bytes and never seek back to patch it. */
    lame_set_bWriteVbrTag(g, 0);
    if (lame_init_params(g) < 0) {
        lame_close(g);
        return 0;
    }
    return (jlong) (intptr_t) g;
}

static jbyteArray to_java(JNIEnv *env, const unsigned char *data, int n) {
    jbyteArray arr = (*env)->NewByteArray(env, n);
    if (arr != NULL && n > 0) (*env)->SetByteArrayRegion(env, arr, 0, n, (const jbyte *) data);
    return arr;
}

static int encode_pcm(lame_t g, const short *pcm, int frames, unsigned char *out, int cap) {
    if (lame_get_num_channels(g) == 1) return lame_encode_buffer(g, pcm, pcm, frames, out, cap);
    return lame_encode_buffer_interleaved(g, (short *) pcm, frames, out, cap);
}

JNIEXPORT jbyteArray JNICALL JNI_FN(nativeEncode)(JNIEnv *env, jobject thiz, jlong handle,
                                                  jshortArray pcm, jint frames) {
    (void) thiz;
    lame_t g = (lame_t) (intptr_t) handle;
    if (g == NULL) { throw_ise(env, "lame closed"); return NULL; }
    int cap = (int) (1.25 * frames + 7200);
    unsigned char *out = (unsigned char *) malloc((size_t) cap);
    if (out == NULL) { throw_ise(env, "out of memory"); return NULL; }
    jshort *in = (*env)->GetShortArrayElements(env, pcm, NULL);
    if (in == NULL) { free(out); return NULL; }
    int n = encode_pcm(g, (const short *) in, frames, out, cap);
    (*env)->ReleaseShortArrayElements(env, pcm, in, JNI_ABORT);
    jbyteArray res = NULL;
    if (n < 0) throw_ise(env, "lame_encode_buffer failed"); else res = to_java(env, out, n);
    free(out);
    return res;
}

JNIEXPORT jbyteArray JNICALL JNI_FN(nativeEncodeDirect)(JNIEnv *env, jobject thiz, jlong handle,
                                                        jobject buffer, jint frames) {
    (void) thiz;
    lame_t g = (lame_t) (intptr_t) handle;
    if (g == NULL) { throw_ise(env, "lame closed"); return NULL; }
    void *addr = (*env)->GetDirectBufferAddress(env, buffer);
    if (addr == NULL) { throw_ise(env, "not a direct buffer"); return NULL; }
    int cap = (int) (1.25 * frames + 7200);
    unsigned char *out = (unsigned char *) malloc((size_t) cap);
    if (out == NULL) { throw_ise(env, "out of memory"); return NULL; }
    int n = encode_pcm(g, (const short *) addr, frames, out, cap);
    jbyteArray res = NULL;
    if (n < 0) throw_ise(env, "lame_encode_buffer failed"); else res = to_java(env, out, n);
    free(out);
    return res;
}

JNIEXPORT jbyteArray JNICALL JNI_FN(nativeFlush)(JNIEnv *env, jobject thiz, jlong handle) {
    (void) thiz;
    lame_t g = (lame_t) (intptr_t) handle;
    if (g == NULL) { throw_ise(env, "lame closed"); return NULL; }
    unsigned char out[7200];
    int n = lame_encode_flush(g, out, (int) sizeof(out));
    if (n < 0) { throw_ise(env, "lame_encode_flush failed"); return NULL; }
    return to_java(env, out, n);
}

JNIEXPORT void JNICALL JNI_FN(nativeClose)(JNIEnv *env, jobject thiz, jlong handle) {
    (void) env;
    (void) thiz;
    lame_t g = (lame_t) (intptr_t) handle;
    if (g != NULL) lame_close(g);
}
