# Tutorial 04: HelloSDL - 기본 SDL2 프로그램

[![SDL2](https://img.shields.io/badge/SDL2-2.0.8-blue.svg)](https://www.libsdl.org/)
[![C++](https://img.shields.io/badge/C++-17-orange.svg)](https://en.cppreference.com/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)](https://github.com/wawworld/gameframework-tutorial-branches)

> **SDL2를 이용한 가장 기본적인 "Hello World" 프로그램**  
> 이 튜토리얼은 SDL2 라이브러리의 기본 구조와 사용법을 익히는 첫 번째 단계입니다.

## 🎯 학습 목표

- SDL2 라이브러리의 기본 초기화 과정 이해
- 윈도우 생성과 렌더러 설정 방법 학습
- 기본적인 렌더링 파이프라인 구조 파악
- 리소스 관리와 메모리 해제 패턴 학습

## 📁 프로젝트 구조

```
tutorial-04-hello-sdl/
├── src/
│   └── main.cpp         # 메인 소스 파일 (1572 bytes)
├── include/             # 헤더 파일 (현재 비어있음)
├── assets/              # 리소스 파일
├── 3rdParty/            # SDL2 라이브러리
├── build/               # 빌드 출력 (생성됨)
├── bin/                 # 실행 파일 (생성됨)
├── Makefile             # Linux/macOS 빌드 설정
├── HoseoSDL.sln         # Visual Studio 솔루션
├── HoseoSDL.vcxproj     # Visual Studio 프로젝트
├── command              # 빌드 스크립트
└── README.md            # 이 파일
```

## 🔍 핵심 코드 분석

### main.cpp 구조

```cpp
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;
constexpr Uint32 DELAY_TIME = 5000;  // 5초
```

### SDL 기본 패턴

이 프로그램은 모든 SDL 애플리케이션의 기본 패턴을 따릅니다:

1. **초기화 (Initialization)**
   ```cpp
   SDL_Init(SDL_INIT_VIDEO)
   ```

2. **윈도우 생성 (Window Creation)**
   ```cpp
   SDL_CreateWindow("HelloSDL", 
                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN)
   ```

3. **렌더러 생성 (Renderer Creation)**
   ```cpp
   SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)
   ```

4. **렌더링 (Rendering)**
   ```cpp
   SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // 검은색
   SDL_RenderClear(renderer);                       // 화면 지우기
   SDL_RenderPresent(renderer);                     // 화면 표시
   ```

5. **정리 (Cleanup)**
   ```cpp
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   SDL_Quit();
   ```

## 🚀 빌드 및 실행

### Linux/macOS (Makefile 사용)

```bash
# 이 브랜치 클론
git clone https://github.com/wawworld/gameframework-tutorial-branches.git
cd gameframework-tutorial-branches
git checkout tutorial-04-hello-sdl

# 빌드 및 실행
make run
```

### Windows (Visual Studio)

1. `HoseoSDL.sln` 파일을 Visual Studio로 열기
2. 솔루션 빌드 (Ctrl+Shift+B)
3. 디버그 시작 (F5)

### 수동 빌드 (Linux/macOS)

```bash
# SDL2 설치 (Ubuntu/Debian)
sudo apt-get install libsdl2-dev

# 컴파일
g++ -Iinclude -I3rdParty/SDL/include src/main.cpp -lSDL2 -o bin/hello_sdl

# 실행
./bin/hello_sdl
```

## 📊 실행 결과

프로그램을 실행하면:

1. **640×480 픽셀**의 검은색 윈도우가 화면 중앙에 나타남
2. **"HelloSDL"** 제목의 창이 표시됨
3. **5초 후** 자동으로 프로그램 종료

![HelloSDL 실행 화면](https://via.placeholder.com/640x480/000000/FFFFFF?text=HelloSDL)

## 🔧 주요 설정값

| 설정 | 값 | 설명 |
|------|-----|------|
| 창 크기 | 640×480 | 표준 VGA 해상도 |
| 배경색 | RGB(0,0,0) | 검은색 |
| 표시 시간 | 5초 | 자동 종료까지의 시간 |
| 렌더러 | 하드웨어 가속 | GPU 활용 |

## 📚 주요 SDL 함수 설명

### 초기화 및 생성

- **`SDL_Init(SDL_INIT_VIDEO)`**: 비디오 서브시스템 초기화
- **`SDL_CreateWindow()`**: 게임 윈도우 생성
- **`SDL_CreateRenderer()`**: 하드웨어 가속 렌더링 컨텍스트 생성

### 렌더링

- **`SDL_SetRenderDrawColor()`**: 렌더링에 사용할 색상 설정
- **`SDL_RenderClear()`**: 설정된 색상으로 전체 화면 지우기
- **`SDL_RenderPresent()`**: 백 버퍼 내용을 화면에 표시 (더블 버퍼링)

### 정리

- **`SDL_DestroyRenderer()`**: 렌더러 메모리 해제
- **`SDL_DestroyWindow()`**: 윈도우 메모리 해제
- **`SDL_Quit()`**: SDL 라이브러리 완전 종료

## ⚠️ 주의사항

1. **리소스 순서**: 생성 순서의 **역순**으로 해제해야 함
2. **에러 처리**: 각 SDL 함수의 반환값을 확인하여 에러 처리
3. **메모리 누수**: 모든 SDL 객체는 명시적으로 해제 필요
4. **플랫폼 의존성**: Windows에서는 추가 DLL 파일이 필요할 수 있음

## 🔄 에러 처리 패턴

```cpp
if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "SDL 초기화 실패: %s\n", SDL_GetError());
    return 1;
}

if (window == nullptr) {
    fprintf(stderr, "윈도우 생성 실패: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
}

if (renderer == nullptr) {
    fprintf(stderr, "렌더러 생성 실패: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
}
```

- **반환값 확인**: NULL이나 0이 아닌 값이면 실패
- **에러 메시지**: `SDL_GetError()`로 구체적인 오류 확인
- **정리 후 종료**: 실패 시에도 이미 할당된 리소스 해제

## 🎯 다음 단계 미리보기

이 기본 구조에서 발전할 예정인 내용들:

- **tutorial-05-game-basic-structure**: 게임 루프와 이벤트 처리
- **tutorial-06-game-class-structure**: 객체지향 Game 클래스 설계
- **tutorial-07-sdl-basic-drawing**: 기본 도형과 색상 그리기
- **tutorial-08-source-destination-rect**: 텍스처 영역 관리

## 🔍 코드 라인별 분석

### 상수 정의
```cpp
constexpr int WINDOW_WIDTH = 640;    // 윈도우 너비
constexpr int WINDOW_HEIGHT = 480;   // 윈도우 높이  
constexpr Uint32 DELAY_TIME = 5000;  // 5초 지연
```

### 변수 선언
```cpp
SDL_Window* window = nullptr;        // 윈도우 포인터
SDL_Renderer* renderer = nullptr;    // 렌더러 포인터
```

### 메인 로직 흐름
1. SDL 비디오 시스템 초기화
2. 중앙 위치에 640x480 윈도우 생성
3. 하드웨어 가속 렌더러 생성
4. 검은색으로 화면 칠하기
5. 5초 대기
6. 모든 리소스 해제 및 종료

## 🐛 문제 해결

### 일반적인 문제들

1. **SDL2 라이브러리를 찾을 수 없음**
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libsdl2-dev
   
   # macOS (Homebrew)
   brew install sdl2
   
   # Windows
   # 3rdParty 폴더의 SDL 라이브러리 경로 확인
   ```

2. **실행 파일이 생성되지 않음**
   - Makefile의 경로 설정 확인
   - 컴파일러 설치 여부 확인 (`g++ --version`)
   - 권한 문제 확인 (`chmod +x make`)

3. **창이 나타나지 않음**
   - 그래픽 드라이버 업데이트
   - X11 포워딩 설정 (SSH 사용 시)
   - 가상머신에서는 3D 가속 활성화

4. **5초 후 자동 종료되지 않음**
   - 시스템 시계 확인
   - SDL_Delay 함수 정상 작동 여부 확인

## 💡 학습 팁

1. **코드 수정 실험**
   - `WINDOW_WIDTH`, `WINDOW_HEIGHT` 값 변경해보기
   - `DELAY_TIME` 값을 조정해보기
   - 배경색 (0,0,0,255)을 다른 색으로 변경해보기

2. **에러 발생시키기**
   - SDL_Init 호출 전에 SDL_CreateWindow 호출해보기
   - 리소스 해제 순서를 바꿔보기
   - 의도적으로 잘못된 값 전달해보기

3. **디버깅 연습**
   - printf를 추가하여 실행 흐름 확인
   - 각 단계에서 성공/실패 메시지 출력
   - SDL_GetError() 활용하여 오류 원인 파악

## 📖 참고 자료

- [SDL2 공식 문서](https://wiki.libsdl.org/)
- [SDL2 튜토리얼 (LazyFoo)](http://lazyfoo.net/tutorials/SDL/)
- [SDL2 API 레퍼런스](https://wiki.libsdl.org/SDL2/APIByCategory)
- [SDL2 예제 코드](https://github.com/libsdl-org/SDL/tree/main/test)

## 🔄 브랜치별 튜토리얼 진행

현재 위치: **tutorial-04-hello-sdl**

```bash
# 다른 튜토리얼 단계로 이동
git checkout tutorial-05-game-basic-structure      # 다음 단계
git checkout tutorial-06-game-class-structure      # 객체지향 설계
git checkout tutorial-07-sdl-basic-drawing         # 그리기 기초

# 전체 브랜치 목록 확인
git branch -a
```

## 👨‍💻 작성자

**wawworld** (wawworld@imrlab.hoseo.edu)  
호서대학교 게임 프레임워크 튜토리얼

---

> 💡 **튜토리얼 완료 체크리스트**
> - [ ] SDL2 라이브러리 설치 완료
> - [ ] 프로그램 정상 컴파일 확인
> - [ ] 검은색 윈도우 5초간 표시 확인
> - [ ] 에러 없이 자동 종료 확인
> - [ ] 코드의 각 부분 역할 이해
> - [ ] 다음 튜토리얼 준비 완료

**다음 단계**: [tutorial-05-game-basic-structure](../tutorial-05-game-basic-structure/README.md)로 이동하여 게임 루프를 학습하세요!