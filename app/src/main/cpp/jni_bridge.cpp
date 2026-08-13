#include <jni.h>
#include <cstdint>
#include <memory>
#include <string>
#include "mobile_spatial_ai/spatial_core.hpp"
namespace { msai::SpatialCore* fromHandle(jlong h){return reinterpret_cast<msai::SpatialCore*>(static_cast<std::uintptr_t>(h));} std::string toString(JNIEnv*e,jstring v){if(v==nullptr)return{};const char*c=e->GetStringUTFChars(v,nullptr);if(c==nullptr)return{};std::string r(c);e->ReleaseStringUTFChars(v,c);return r;} }
extern "C" JNIEXPORT jlong JNICALL Java_ai_mobilespatial_NativeSpatialCore_nativeCreate(JNIEnv*,jobject){auto c=std::make_unique<msai::SpatialCore>();return static_cast<jlong>(reinterpret_cast<std::uintptr_t>(c.release()));}
extern "C" JNIEXPORT void JNICALL Java_ai_mobilespatial_NativeSpatialCore_nativeDestroy(JNIEnv*,jobject,jlong h){delete fromHandle(h);}
extern "C" JNIEXPORT jlong JNICALL Java_ai_mobilespatial_NativeSpatialCore_nativeObserve(JNIEnv*e,jobject,jlong h,jstring label,jfloat confidence,jfloat x,jfloat y,jfloat z,jlong ts){auto*c=fromHandle(h);if(!c)return 0;return static_cast<jlong>(c->observe3D(toString(e,label),confidence,{x,y,z},ts));}
extern "C" JNIEXPORT jfloatArray JNICALL Java_ai_mobilespatial_NativeSpatialCore_nativeNearest(JNIEnv*e,jobject,jlong h,jstring label){auto*c=fromHandle(h);if(!c)return nullptr;auto r=c->nearest(toString(e,label));if(!r.found)return nullptr;const jfloat v[6]={static_cast<jfloat>(r.object.id),r.object.position.x,r.object.position.y,r.object.position.z,r.distance_m,r.object.confidence};jfloatArray out=e->NewFloatArray(6);if(out)e->SetFloatArrayRegion(out,0,6,v);return out;}
