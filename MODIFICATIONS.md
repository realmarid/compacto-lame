# Modifications to LAME 3.100 / LAME 3.100 수정 내역

## Summary / 요약

**None. / 없음.**

- All 48 files under `src/main/cpp/lame/` are byte-identical to the files at the same paths in `lame-3.100.tar.gz` (SHA-256 `ddfe36cab873794038ae2c1210557ad34857a4b6bdc515785d1da9e175b1da1e`). Checked file by file with SHA-256 on 2026-11-16. A patch against upstream would be empty, so none is included.
- `src/main/cpp/lame/`의 48개 파일은 모두 `lame-3.100.tar.gz`의 같은 경로 파일과 바이트 단위로 같습니다 (2026-11-16, 파일별 SHA-256 비교). 원본 대비 패치는 비어 있으므로 따로 넣지 않았습니다.

## What Compacto adds (new files, not changes to LAME files) / Compacto가 더한 것 (새 파일이며 LAME 파일 변경 아님)

| File | Purpose |
|---|---|
| `src/main/cpp/lame_jni.c` | JNI bridge for `com.nujiwave.compacto.engine.audio.LameNative`. CBR encoding only, calls the public `lame.h` API. Linked into the same `libcompactolame.so` / 같은 .so에 함께 링크되는 JNI 연결 코드 |
| `src/main/cpp/CMakeLists.txt` | Replaces LAME's autotools build. `HAVE_CONFIG_H` is not defined, so no `config.h` is used; the compile definitions that `config.h` would provide are passed instead (`STDC_HEADERS`, `HAVE_LIMITS_H`, `HAVE_STDINT_H`, `HAVE_INTTYPES_H`, `HAVE_ERRNO_H`, `HAVE_FCNTL_H`, `HAVE_MEMCPY`, `HAVE_STRCHR`, `ieee754_float32_t=float`, `LAME_LIBRARY_BUILD`, `NDEBUG`) / autotools 대신 쓰는 빌드 설정. `config.h` 대신 같은 정의를 컴파일 옵션으로 넘김 |

## What is compiled / 실제로 컴파일하는 파일

- `src/main/cpp/lame/libmp3lame/*.c` (19 files) and `src/main/cpp/lame_jni.c`.
- Headers from `include/` and `libmp3lame/` (including `libmp3lame/vector/lame_intrin.h`, which `fft.c` and `quantize.c` include).
- `libmp3lame/vector/xmm_quantize_sub.c`, `Makefile.am` and `Makefile.in` are present as copied from upstream but are not compiled (the CMake glob takes only `libmp3lame/*.c`).

## Left out of the subset / 넣지 않은 원본 부분

Not compiled, so not included: `frontend/` (command-line program), `mpglib/` and `libmp3lame/mpglib_interface.c` (MP3 decoder), `libmp3lame/i386/`, `ACM/`, `Dll/`, `dshow/`, `mac/`, `macosx/`, `vc_solution/`, `debian/`, `doc/`, `misc/`, autotools files and test files. The library therefore contains no MP3 decoder.

빌드하지 않으므로 넣지 않았습니다. 따라서 이 라이브러리에는 MP3 디코더가 없습니다.

## Check it yourself / 직접 확인하기

Download `lame-3.100.tar.gz` (see README) into this repository's folder, then:

```bash
sha256sum lame-3.100.tar.gz        # ddfe36cab873794038ae2c1210557ad34857a4b6bdc515785d1da9e175b1da1e
tar -xzf lame-3.100.tar.gz
cd src/main/cpp/lame
find . -type f | while read -r f; do cmp -s "$f" "../../../../lame-3.100/$f" || echo "DIFF $f"; done
```

```powershell
(Get-FileHash lame-3.100.tar.gz -Algorithm SHA256).Hash   # DDFE36CA...
tar -xzf lame-3.100.tar.gz
$v = (Resolve-Path src/main/cpp/lame).Path; $u = (Resolve-Path lame-3.100).Path
Get-ChildItem $v -Recurse -File | ForEach-Object {
  $rel = $_.FullName.Substring($v.Length + 1)
  if ((Get-FileHash $_.FullName).Hash -ne (Get-FileHash (Join-Path $u $rel)).Hash) { "DIFF $rel" }
}
```

No output means every file matches. / 아무것도 출력되지 않으면 모든 파일이 같습니다.
