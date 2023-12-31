# Windows SDK - библиотека для создания оконных приложений под Windows

Это библиотека на чистом C для создания оконных приложений под Windows в псевдо объектно-ориентированном стиле. В настоящее время графическим обработчиком служит GDI (Graphics Device Interface).

## Возможности

С помощью **Windows SDK** вы сможете:

- Создавать окна любого стиля и размера 🏙️
- Добавлять различные контролы в окно, такие как:
  - Кнопки 👉
  - Поля ввода (эдиты) 🖊️
  - Статический текст 📜
- Добавлять меню для вашего приложения 🍔

## Нереализованные возможности

Планируется добавить следующие функции:

- Поддержка других графических API, таких как OpenGL, DirectX 🎮
- Добавление всех видов контролов, таких как:
  - Скролл бары 📜
  - Прогресс бары 📊
- Добавление всех видов ресурсов для вашего приложения 📁

## Как использовать 

1. Скачайте файлы **Windows SDK** из репозитория.
2. Соберите проект как статическую библиотеку (.lib)
3. Подключите собранную библиотеку в свой проект
4. Подключите заголовочные файлы (.h)
5. Используйте функции Windows SDK в своем приложении 

```c
#include "window.h"
#include "GraphicsRenderer.h"
#include "GraphicObject.h"
#include <stdint.h>

int WinMain() {
	struct Window* win = window_create_object();
	window_ctor(win, 0, L"Test", 500, 500);
	struct Line* line = line_ctor(win, 100, 100);
	window_start_rendering(win);
	window_start_message_handling(win);
}
