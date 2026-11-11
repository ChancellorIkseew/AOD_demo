# Arts of Destruction — 3D Renderer Demo

Первая демонстрация: кубик с управлением WASD над плоскостью, free-look камера мышью (зажать ПКМ).

## Сборка

### Требуемые библиотеки (положить в `libs/`)

```
libs/
  include/
    SDL3/                  # заголовки SDL3
    glm/                   # заголовки glm (только нужные подмодули)
  MSVC_lib/
    SDL3.lib               # для Windows/MSVC
  GCC_so/
    libSDL3.so.0           # для Linux/GCC
```

### GLAD

Положить папку `glad/` (с `glad.c`, `glad.h`, `khrplatform.h`) в:
```
src/engine/render/glad/
```
GLAD должен быть сгенерирован для OpenGL 4.6 Core profile на https://glad.dav1d.de/

### Сборка (Ninja)

```
cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Запуск

Перед запуском скопировать папку `res/` в директорию с исполняемым файлом (рядом с `build/AoD_Demo`).

## Управление

- **WASD** — движение кубика относительно взгляда камеры (по горизонтали)
- **Space / LShift** — вверх / вниз
- **ПКМ (зажать)** — free-look камеры (захват мыши)
- **Esc** — выход

## Архитектура

См. `doc/design_doc.pdf` (отдельный файл) — раздел «Архитектура движка» и «План реализации».
