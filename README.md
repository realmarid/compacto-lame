# compacto-lame

LAME 3.100 source and build files for `libcompactolame.so`, the MP3 encoder library in the Android app **Compacto** (`com.nujiwave.compacto`).

[한국어](#한국어) · [English](#english)

---

## 한국어

### 이 저장소는

Compacto(Android 앱, 패키지 이름 `com.nujiwave.compacto`)가 영상·소리를 MP3로 만들 때 쓰는 공유 라이브러리 `libcompactolame.so`의 대응 소스 전체입니다. 이 저장소만으로 같은 라이브러리를 다시 빌드할 수 있습니다.

| 경로 | 내용 |
|---|---|
| `src/main/cpp/lame/` | LAME 3.100 원본 중 빌드에 쓰는 부분(48개 파일). **수정 없음** |
| `src/main/cpp/lame_jni.c` | 앱과 LAME을 잇는 JNI 연결 코드 (Compacto 작성) |
| `src/main/cpp/CMakeLists.txt` | 라이브러리 빌드 설정 (Compacto 작성) |
| `gradle/engine-audio-native.gradle.kts` | 앱 Gradle 빌드 설정 중 이 라이브러리 부분 발췌 |
| `LICENSES/SOURCE.txt` | 원본 압축 파일 정보와 실제 빌드 옵션 기록 |
| `LICENSES/LAME-NOTICE.txt` | 앱에 넣은 LAME 고지문 (영문) |
| `MODIFICATIONS.md` | 수정 여부와 확인 방법 |
| `CHECKSUMS.sha256` | 이 저장소 모든 파일의 SHA-256 |
| `LICENSE` | GNU Library General Public License 버전 2 전문 |

앱의 나머지 코드는 들어 있지 않습니다. 앱은 이 라이브러리를 `System.loadLibrary("compactolame")`로 불러 쓰기만 합니다.

### 라이선스

- **LAME**: GNU Library General Public License 버전 2, 또는 (선택에 따라) 그 이후 버전 (SPDX: `LGPL-2.0-or-later`). 전문은 `LICENSE`이며, LAME의 `src/main/cpp/lame/COPYING`과 같은 파일입니다. "그 이후 버전" 조항에 따라 LGPL 2.1 이후 조건으로 쓰셔도 됩니다.
- **`lame_jni.c`, `CMakeLists.txt`, Gradle 발췌**: LAME과 한 라이브러리(.so)로 묶이므로 같은 조건(`LGPL-2.0-or-later`)으로 배포합니다.
- LAME 저작권: The LAME Project, Mark Taylor, Michael Cheng 외 (각 소스 파일과 `src/main/cpp/lame/README` 참고).

### 원본 소스와 수정 여부

| 항목 | 값 |
|---|---|
| 원본 파일 | `lame-3.100.tar.gz` (1,524,133 bytes) |
| SHA-256 | `ddfe36cab873794038ae2c1210557ad34857a4b6bdc515785d1da9e175b1da1e` |
| 받는 곳 | https://sourceforge.net/projects/lame/files/lame/3.100/lame-3.100.tar.gz/download (프로젝트 사이트 https://lame.sourceforge.io/) |
| 수정 | **없음.** `src/main/cpp/lame/`의 48개 파일은 원본 압축 파일의 같은 경로 파일과 바이트 단위로 같습니다 |

빌드에 쓰지 않는 부분(명령줄 프로그램 `frontend/`, 디코더 `mpglib/`와 `libmp3lame/mpglib_interface.c`, 문서 등)은 넣지 않았습니다. 확인 방법은 `MODIFICATIONS.md`에 있습니다.

### 다시 빌드하기 (NDK 28.2 — arm64-v8a, armeabi-v7a, x86_64)

준비물: Android SDK Manager에서 **NDK 28.2.13676358**과 **CMake 3.22.1**(Ninja 포함)을 설치합니다.

Windows (PowerShell, 이 저장소 폴더에서):

```powershell
$SDK = "$env:LOCALAPPDATA\Android\Sdk"
$NDK = "$SDK\ndk\28.2.13676358"
$CM  = "$SDK\cmake\3.22.1\bin"
foreach ($abi in 'arm64-v8a','armeabi-v7a','x86_64') {
  & "$CM\cmake.exe" -S src/main/cpp -B "out/$abi" -G Ninja "-DCMAKE_MAKE_PROGRAM=$CM\ninja.exe" `
    "-DCMAKE_TOOLCHAIN_FILE=$NDK\build\cmake\android.toolchain.cmake" "-DANDROID_ABI=$abi" `
    -DANDROID_PLATFORM=android-26 -DANDROID_STL=none -DANDROID_SUPPORT_FLEXIBLE_PAGE_SIZES=ON `
    -DCMAKE_BUILD_TYPE=RelWithDebInfo
  & "$CM\cmake.exe" --build "out/$abi"
  & "$NDK\toolchains\llvm\prebuilt\windows-x86_64\bin\llvm-strip.exe" --strip-unneeded "out/$abi/libcompactolame.so"
}
```

Linux / macOS (bash, 이 저장소 폴더에서):

```bash
SDK="$HOME/Android/Sdk"          # macOS: $HOME/Library/Android/sdk
HOST=linux-x86_64                # macOS: darwin-x86_64
NDK="$SDK/ndk/28.2.13676358"
CM="$SDK/cmake/3.22.1/bin"
for ABI in arm64-v8a armeabi-v7a x86_64; do
  "$CM/cmake" -S src/main/cpp -B "out/$ABI" -G Ninja -DCMAKE_MAKE_PROGRAM="$CM/ninja" \
    -DCMAKE_TOOLCHAIN_FILE="$NDK/build/cmake/android.toolchain.cmake" -DANDROID_ABI=$ABI \
    -DANDROID_PLATFORM=android-26 -DANDROID_STL=none -DANDROID_SUPPORT_FLEXIBLE_PAGE_SIZES=ON \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo
  "$CM/cmake" --build "out/$ABI"
  "$NDK/toolchains/llvm/prebuilt/$HOST/bin/llvm-strip" --strip-unneeded "out/$ABI/libcompactolame.so"
done
```

결과: `out/<ABI>/libcompactolame.so`

- 옵션은 앱 빌드와 같습니다: `RelWithDebInfo`, `android-26`(minSdk 26), STL 없음, 16 KB 페이지 정렬. 마지막 `llvm-strip`은 Android Gradle Plugin이 앱을 패키징할 때 디버그 기호를 지우는 단계와 같습니다. 전체 옵션 기록은 `LICENSES/SOURCE.txt`에 있습니다.
- 확인 결과 (2026-11-16): 위 방법으로 빌드한 파일은 Compacto 1.0.0 빌드의 라이브러리와 세 ABI 모두 크기, 코드(`.text`, `.rodata`), 내보낸 기호가 같고, 빌드 경로에 따라 달라지는 GNU Build ID 20바이트만 다릅니다.
- Gradle로 빌드하려면 `gradle/engine-audio-native.gradle.kts`의 `android { }` 블록을 Android 라이브러리 모듈(minSdk 26)에 넣고, 이 저장소의 `src/main/cpp/`를 그 모듈의 `src/main/cpp/`로 두면 됩니다.

### 바꿔 넣는 라이브러리가 지켜야 할 것

- 파일 이름 `libcompactolame.so`, 위 세 ABI 중 해당 ABI용으로 빌드.
- 아래 JNI 함수 5개를 같은 이름·시그니처로 내보낼 것. 앱 쪽 선언은 Kotlin `object com.nujiwave.compacto.engine.audio.LameNative`의 `external fun`(인스턴스 메서드)입니다.

| JNI 함수 (`Java_com_nujiwave_compacto_engine_audio_LameNative_` + 이름) | Kotlin 선언 |
|---|---|
| `nativeInit` | `(sampleRate: Int, channels: Int, bitrateKbps: Int, quality: Int): Long` — 0이면 실패 |
| `nativeEncode` | `(handle: Long, pcm: ShortArray, frames: Int): ByteArray?` |
| `nativeEncodeDirect` | `(handle: Long, pcm: ByteBuffer, frames: Int): ByteArray?` — direct buffer |
| `nativeFlush` | `(handle: Long): ByteArray?` |
| `nativeClose` | `(handle: Long)` |

- 기기에 기본으로 있는 시스템 라이브러리(`libc`, `libm`, `libdl`)에만 의존할 것.

### APK 안의 라이브러리 바꾸기 (일반적인 방법)

1. 기기에서 APK를 꺼냅니다: `adb shell pm path com.nujiwave.compacto`로 경로를 보고 `adb pull`. Google Play로 설치했다면 `base.apk`와 `split_config.*.apk` 여러 개이고, 라이브러리는 ABI split(예: `split_config.arm64_v8a.apk`)의 `lib/<ABI>/libcompactolame.so`에 있습니다.
2. 그 APK 안의 `lib/<ABI>/libcompactolame.so`를 새로 빌드한 파일로 바꿉니다. Compacto는 라이브러리를 압축하지 않고 담아 APK에서 바로 읽으므로(`extractNativeLibs=false`), 새 파일도 **압축 없이(store)** 넣어야 합니다. 예: 작업 폴더에 `lib/arm64-v8a/libcompactolame.so`를 두고 `zip -0 split_config.arm64_v8a.apk lib/arm64-v8a/libcompactolame.so`.
3. 정렬합니다: `zipalign -P 16 -f 4 in.apk aligned.apk` (Android SDK build-tools 35 이상).
4. 직접 만든 서명 키로 **모든** APK(base와 split 전부)를 같은 키로 서명합니다: `apksigner sign --ks my.keystore aligned.apk`.
5. 서명이 달라 원래 앱 위에 덮어 설치할 수 없으므로, 원래 앱을 지운 뒤(앱 설정 등 앱 데이터가 지워집니다) `adb install-multiple base.apk split_config.*.apk`로 설치합니다. APK가 하나면 `adb install`.

- 이렇게 설치한 앱은 Google Play 업데이트를 받지 못합니다.
- Compacto에는 앱 서명이나 라이브러리를 검사해 실행을 막는 기능이 없습니다.
- 수정한 라이브러리를 디버깅하기 위한 분석(리버스 엔지니어링)을 허용합니다.

### 소스 제공 약속 (3년)

Compacto를 배포하는 동안, 그리고 이 라이브러리가 든 Compacto 버전을 마지막으로 배포한 날부터 **최소 3년 동안**, 요청하시는 누구에게나 `libcompactolame.so`의 대응 소스 전체(이 저장소의 내용)를 이메일로 무료로 보내 드립니다. 실물 매체를 원하시면 그 실비 이하만 받습니다.

- 문의: **nujiwave.biz@gmail.com** (앱 버전을 함께 적어 주세요)

### 버전 대응

| Compacto | LAME | 비고 |
|---|---|---|
| 1.0.0 (versionCode 10000) | 3.100 | 이 저장소의 첫 공개본 |

앱에서 LAME 버전이나 이 라이브러리의 빌드 설정이 바뀌면 이 저장소도 함께 갱신합니다.

---

## English

### What this is

This is the complete corresponding source of `libcompactolame.so`, the shared library that the Android app Compacto (package name `com.nujiwave.compacto`) uses to make MP3 files from video or audio. This repository alone is enough to rebuild that library.

| Path | Contents |
|---|---|
| `src/main/cpp/lame/` | The part of the LAME 3.100 source that is built (48 files). **Unmodified** |
| `src/main/cpp/lame_jni.c` | JNI bridge between the app and LAME (written for Compacto) |
| `src/main/cpp/CMakeLists.txt` | Build script for the library (written for Compacto) |
| `gradle/engine-audio-native.gradle.kts` | Excerpt of the app's Gradle build: the part that builds this library |
| `LICENSES/SOURCE.txt` | Upstream archive details and the exact build options used |
| `LICENSES/LAME-NOTICE.txt` | The LAME notice shipped in the app |
| `MODIFICATIONS.md` | Modification status and how to check it |
| `CHECKSUMS.sha256` | SHA-256 of every file in this repository |
| `LICENSE` | Full text of the GNU Library General Public License, version 2 |

No other app code is included. The app only loads this library with `System.loadLibrary("compactolame")`.

### Licence

- **LAME**: GNU Library General Public License, version 2, or (at your option) any later version (SPDX: `LGPL-2.0-or-later`). The full text is `LICENSE`, the same file as LAME's `src/main/cpp/lame/COPYING`. Under the "any later version" clause you may also use it under LGPL 2.1 or later.
- **`lame_jni.c`, `CMakeLists.txt` and the Gradle excerpt**: they are linked into the same library (.so) as LAME, so they are distributed under the same terms (`LGPL-2.0-or-later`).
- LAME copyright: The LAME Project, Mark Taylor, Michael Cheng and others (see each source file and `src/main/cpp/lame/README`).

### Upstream source and modifications

| Item | Value |
|---|---|
| Upstream archive | `lame-3.100.tar.gz` (1,524,133 bytes) |
| SHA-256 | `ddfe36cab873794038ae2c1210557ad34857a4b6bdc515785d1da9e175b1da1e` |
| Download | https://sourceforge.net/projects/lame/files/lame/3.100/lame-3.100.tar.gz/download (project site https://lame.sourceforge.io/) |
| Modifications | **None.** The 48 files under `src/main/cpp/lame/` are byte-identical to the files at the same paths in the upstream archive |

Parts that are not built (the `frontend/` command-line program, the `mpglib/` decoder and `libmp3lame/mpglib_interface.c`, documentation and so on) are left out. `MODIFICATIONS.md` explains how to check this yourself.

### Rebuilding (NDK 28.2 — arm64-v8a, armeabi-v7a, x86_64)

Install **NDK 28.2.13676358** and **CMake 3.22.1** (which includes Ninja) with the Android SDK Manager.

Windows (PowerShell, in this repository's folder):

```powershell
$SDK = "$env:LOCALAPPDATA\Android\Sdk"
$NDK = "$SDK\ndk\28.2.13676358"
$CM  = "$SDK\cmake\3.22.1\bin"
foreach ($abi in 'arm64-v8a','armeabi-v7a','x86_64') {
  & "$CM\cmake.exe" -S src/main/cpp -B "out/$abi" -G Ninja "-DCMAKE_MAKE_PROGRAM=$CM\ninja.exe" `
    "-DCMAKE_TOOLCHAIN_FILE=$NDK\build\cmake\android.toolchain.cmake" "-DANDROID_ABI=$abi" `
    -DANDROID_PLATFORM=android-26 -DANDROID_STL=none -DANDROID_SUPPORT_FLEXIBLE_PAGE_SIZES=ON `
    -DCMAKE_BUILD_TYPE=RelWithDebInfo
  & "$CM\cmake.exe" --build "out/$abi"
  & "$NDK\toolchains\llvm\prebuilt\windows-x86_64\bin\llvm-strip.exe" --strip-unneeded "out/$abi/libcompactolame.so"
}
```

Linux / macOS (bash, in this repository's folder):

```bash
SDK="$HOME/Android/Sdk"          # macOS: $HOME/Library/Android/sdk
HOST=linux-x86_64                # macOS: darwin-x86_64
NDK="$SDK/ndk/28.2.13676358"
CM="$SDK/cmake/3.22.1/bin"
for ABI in arm64-v8a armeabi-v7a x86_64; do
  "$CM/cmake" -S src/main/cpp -B "out/$ABI" -G Ninja -DCMAKE_MAKE_PROGRAM="$CM/ninja" \
    -DCMAKE_TOOLCHAIN_FILE="$NDK/build/cmake/android.toolchain.cmake" -DANDROID_ABI=$ABI \
    -DANDROID_PLATFORM=android-26 -DANDROID_STL=none -DANDROID_SUPPORT_FLEXIBLE_PAGE_SIZES=ON \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo
  "$CM/cmake" --build "out/$ABI"
  "$NDK/toolchains/llvm/prebuilt/$HOST/bin/llvm-strip" --strip-unneeded "out/$ABI/libcompactolame.so"
done
```

Output: `out/<ABI>/libcompactolame.so`

- The options match the app build: `RelWithDebInfo`, `android-26` (minSdk 26), no STL, 16 KB page alignment. The final `llvm-strip` matches the step in which the Android Gradle Plugin removes debug symbols when it packages the app. `LICENSES/SOURCE.txt` lists every option.
- Checked on 2026-11-16: for all three ABIs, a library built this way has the same size, the same code (`.text`, `.rodata`) and the same exported symbols as the library in the Compacto 1.0.0 build. Only the 20-byte GNU build ID differs, because it depends on the build path.
- To build with Gradle instead, put the `android { }` block from `gradle/engine-audio-native.gradle.kts` in an Android library module (minSdk 26) and use this repository's `src/main/cpp/` as that module's `src/main/cpp/`.

### What a replacement library must keep

- The file name `libcompactolame.so`, built for the ABI it replaces (one of the three above).
- These five JNI functions, with the same names and signatures. On the app side they are `external fun` instance methods of the Kotlin `object com.nujiwave.compacto.engine.audio.LameNative`.

| JNI function (`Java_com_nujiwave_compacto_engine_audio_LameNative_` + name) | Kotlin declaration |
|---|---|
| `nativeInit` | `(sampleRate: Int, channels: Int, bitrateKbps: Int, quality: Int): Long` — 0 means failure |
| `nativeEncode` | `(handle: Long, pcm: ShortArray, frames: Int): ByteArray?` |
| `nativeEncodeDirect` | `(handle: Long, pcm: ByteBuffer, frames: Int): ByteArray?` — direct buffer |
| `nativeFlush` | `(handle: Long): ByteArray?` |
| `nativeClose` | `(handle: Long)` |

- Depend only on system libraries that every device has (`libc`, `libm`, `libdl`).

### Replacing the library in the APK (general steps)

1. Pull the APK from your device: `adb shell pm path com.nujiwave.compacto`, then `adb pull` each path. An install from Google Play is a `base.apk` plus several `split_config.*.apk` files; the library is in the ABI split (for example `split_config.arm64_v8a.apk`) at `lib/<ABI>/libcompactolame.so`.
2. Replace `lib/<ABI>/libcompactolame.so` in that APK with your build. Compacto stores native libraries uncompressed and loads them straight from the APK (`extractNativeLibs=false`), so the new file must also be **stored without compression**. For example, with `lib/arm64-v8a/libcompactolame.so` in your working folder: `zip -0 split_config.arm64_v8a.apk lib/arm64-v8a/libcompactolame.so`.
3. Align it: `zipalign -P 16 -f 4 in.apk aligned.apk` (Android SDK build-tools 35 or later).
4. Sign **every** APK (the base and all splits) with one signing key of your own: `apksigner sign --ks my.keystore aligned.apk`.
5. The signature differs from the original, so it cannot be installed over it. Uninstall the original app (this deletes its app data, such as settings), then install with `adb install-multiple base.apk split_config.*.apk`, or `adb install` for a single APK.

- An app installed this way does not receive Google Play updates.
- Compacto has no check of its signature or of this library that would stop it from running.
- Reverse engineering for debugging such modifications is permitted.

### Written offer (3 years)

While we distribute Compacto, and for **at least three years** after we last distribute a Compacto version that contains this library, we will send anyone who asks the complete corresponding source of `libcompactolame.so` (the contents of this repository) by email, free of charge. On physical media, we charge no more than our cost of making and sending it.

- Contact: **nujiwave.biz@gmail.com** (please tell us your app version)

### Version map

| Compacto | LAME | Note |
|---|---|---|
| 1.0.0 (versionCode 10000) | 3.100 | First public release of this repository |

When the app changes its LAME version or how this library is built, this repository is updated with it.
