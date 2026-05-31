# Optimization Menu — Geode Mod

Mod simple de optimización para Geometry Dash hecho con Geode SDK.

## Funciones

| Setting | Descripción |
|---|---|
| Disable Particles | Desactiva partículas para mejorar FPS |
| Disable Backgrounds | Oculta el fondo animado |
| Show FPS Counter | Muestra el contador de FPS en pantalla |
| FPS Limit | Limita el FPS máximo (30–360) |
| Disable Shaders | Desactiva shaders para mejor rendimiento |
| Reduce Texture Quality | Reduce calidad de texturas |

## Compilación

1. Instalar [Geode SDK](https://geode-sdk.org)
2. Instalar CMake y Visual Studio (Windows)
3. Clonar este repositorio
4. Compilar:
```bash
mkdir build && cd build
cmake ..
cmake --build .
```
4. El archivo `.geode` se generará automáticamente en `build/`

## Instalación

Renombrar el `.zip` a `.geode` y colocarlo en la carpeta `mods` de Geode,
o usar el instalador de Geode directamente.

## Compatibilidad

- GD 2.2074
- Geode 4.0.0+
- Windows / Android / macOS
