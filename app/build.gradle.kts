plugins { id("com.android.application") }
android {
    namespace = "ai.mobilespatial"
    compileSdk = 36
    ndkVersion = "28.2.13676358"
    defaultConfig {
        applicationId = "ai.mobilespatial"
        minSdk = 29
        targetSdk = 36
        versionCode = 1
        versionName = "1.0.0-alpha01"
        externalNativeBuild { cmake { cppFlags += listOf("-std=c++17"); arguments += listOf("-DMSAI_BUILD_TESTS=OFF","-DMSAI_BUILD_BENCHMARK=OFF","-DMSAI_ENABLE_FAST_MATH=OFF") } }
        ndk { abiFilters += listOf("arm64-v8a","x86_64") }
    }
    buildTypes {
        debug { isMinifyEnabled = false }
        release { isMinifyEnabled = true; isShrinkResources = true; proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"),"proguard-rules.pro"); ndk.debugSymbolLevel = "SYMBOL_TABLE" }
    }
    externalNativeBuild { cmake { path = file("src/main/cpp/CMakeLists.txt"); version = "3.22.1" } }
    buildFeatures { buildConfig = false }
    compileOptions { sourceCompatibility = JavaVersion.VERSION_17; targetCompatibility = JavaVersion.VERSION_17 }
    packaging { jniLibs.useLegacyPackaging = false }
}
