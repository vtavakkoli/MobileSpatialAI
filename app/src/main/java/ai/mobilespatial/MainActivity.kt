package ai.mobilespatial

import android.app.Activity
import android.graphics.Color
import android.os.Bundle
import android.view.Gravity
import android.widget.LinearLayout
import android.widget.TextView

class MainActivity : Activity() {
    private val spatialCore = NativeSpatialCore()
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        spatialCore.observe("chair",0.94f,1.2f,0f,2.1f)
        val chair=spatialCore.nearest("chair")
        val layout=LinearLayout(this).apply{orientation=LinearLayout.VERTICAL;gravity=Gravity.CENTER;setPadding(48,48,48,48);setBackgroundColor(Color.rgb(12,18,28))}
        layout.addView(TextView(this).apply{text="MobileSpatialAI";textSize=30f;setTextColor(Color.WHITE)})
        layout.addView(TextView(this).apply{text="Native C++ spatial core loaded\n🔒 On-device by default";textSize=18f;setTextColor(Color.rgb(120,220,180));gravity=Gravity.CENTER})
        layout.addView(TextView(this).apply{text=if(chair!=null)"3D world-model smoke test\nNearest chair: %.2f m\nConfidence: %.0f%%".format(chair.distanceMeters,chair.confidence*100f) else "Native query unavailable";textSize=16f;setTextColor(Color.LTGRAY);gravity=Gravity.CENTER})
        setContentView(layout)
    }
    override fun onDestroy(){spatialCore.close();super.onDestroy()}
}
