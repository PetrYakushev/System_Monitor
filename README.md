# System Monitor

System Monitor - приложение на C++20 под Linux для мониторинга загрузки системных ресурсов в реальном времени.

Программа:

- собирает информацию о системе через `/proc` и `statvfs()`;
- предоставляет HTTP API в формате JSON;
- отображает данные в Web UI (HTML + CSS + TypeScript);
- автоматически обновляет показатели каждую секунду.

---

## Отображаемые показатели

### System
- Hostname
- Uptime

### CPU
- Общая загрузка процессора (%)

### Memory
- Использовано / Всего (MB)
- Процент использования (%)

### Disk
- Использовано / Всего (GB)
- Процент использования (%)

### Network
- Получено (RX)
- Отправлено (TX)

---

## Формат JSON

```json
{
    "system": {
        "hostname": "ubuntu-vm",
        "uptime": "2d 5h 17m"
    },
    "cpu": {
        "totalUsage": 14.25
    },
    "memory": {
        "totalMB": 15920,
        "usedMB": 4231,
        "freeMB": 11689,
        "usagePercent": 26.58
    },
    "disk": {
        "totalGB": 100.00,
        "usedGB": 37.42,
        "freeGB": 62.58,
        "usagePercent": 37.42
    },
    "network": {
        "rxBytes": 123456789,
        "txBytes": 98765432
    }
}
```
## Используемые библиотеки

Проект использует следующие сторонние библиотеки:

- **Crow** - header-only C++ framework для создания HTTP-серверов и REST API.
- **nlohmann/json** - библиотека для сериализации и десериализации JSON

## Требования

Для сборки необходимы:

 - GCC 11+ или Clang 14+
 - CMake 3.16+
 - nlohmann/json

Установка зависимостей:
```
sudo apt update
sudo apt install -y \
    build-essential \
    cmake \
    g++ \
    nlohmann-json3-dev
```

## Сборка проекта

Из директории backend:

```
cd backend
mkdir build
cd build
cmake ..
make -j
```
После успешной сборки будет создан исполняемый файл:
`backend/build/system_monitor`

## Запуск

Из директории backend/build:

```
./system_monitor
```

После запуска сервер будет доступен по адресу:

`http://localhost:8080`

## Доступ к приложению

Web-интерфейс

`http://localhost:8080/`

JSON API

`http://localhost:8080/api/system`

Health Check

`http://localhost:8080/health`

## Завершение работы

Остановка приложения:

`Ctrl + C`
