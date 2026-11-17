// Excerpt from Compacto's engine-audio/build.gradle.kts: only the parts that build libcompactolame.so.
// Not a complete build script. Paste the android { } block into an Android library module whose
// src/main/cpp/ holds this repository's src/main/cpp/ folder.
//
// Values that come from Compacto's own convention plugin (build-logic, compacto.android.library):
//   compileSdk = 36
//   defaultConfig { minSdk = 26 }        -> AGP passes -DANDROID_PLATFORM=android-26 to CMake
// Toolchain used for Compacto 1.0.0: Android Gradle Plugin 8.11.1, Gradle 8.13, CMake 3.22.1 (from the SDK).
// The release build uses CMAKE_BUILD_TYPE=RelWithDebInfo; AGP strips debug symbols when it packages the app.

android {
    ndkVersion = "28.2.13676358"

    defaultConfig {
        ndk {
            // The app module filters to the same three ABIs (no 32-bit x86), see app/build.gradle.kts.
            abiFilters += listOf("arm64-v8a", "armeabi-v7a", "x86_64")
        }
        externalNativeBuild {
            cmake {
                // Plain C JNI library; no STL needed.
                arguments += listOf("-DANDROID_STL=none", "-DANDROID_SUPPORT_FLEXIBLE_PAGE_SIZES=ON")
            }
        }
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
}
