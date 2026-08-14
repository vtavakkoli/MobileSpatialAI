package ai.mobilespatial

/** Thin JNI facade. Performance-sensitive spatial operations stay in C++. */
class NativeSpatialCore : AutoCloseable {
    private var handle: Long = nativeCreate()
    data class NearestObject(val id: Long,val x: Float,val y: Float,val z: Float,val distanceMeters: Float,val confidence: Float)
    fun observe(label:String,confidence:Float,x:Float,y:Float,z:Float,timestampNs:Long=System.nanoTime()):Long { check(handle!=0L){"NativeSpatialCore is closed"}; return nativeObserve(handle,label,confidence,x,y,z,timestampNs) }
    fun nearest(label:String):NearestObject? { check(handle!=0L){"NativeSpatialCore is closed"}; val raw=nativeNearest(handle,label)?:return null; if(raw.size!=6)return null; return NearestObject(raw[0].toLong(),raw[1],raw[2],raw[3],raw[4],raw[5]) }
    override fun close(){val current=handle;if(current!=0L){nativeDestroy(current);handle=0L}}
    private external fun nativeCreate():Long
    private external fun nativeDestroy(handle:Long)
    private external fun nativeObserve(handle:Long,label:String,confidence:Float,x:Float,y:Float,z:Float,timestampNs:Long):Long
    private external fun nativeNearest(handle:Long,label:String):FloatArray?
    companion object { init { System.loadLibrary("mobilespatial_jni") } }
}
