# Курсовой проект

# Вариант:
Тип файла конфигурации: yaml

# Ссылка на отчет
- [Отчёт](https://docs.google.com/document/d/1dERlmWumxFXfXgWbYhdP8XfGf2BjPCrFBjIgY8Sr_TY/edit?usp=sharing)


# Лабораторная работа № 5-7

Реализовать распределенную систему по асинхронной обработке запросов. В данной распределенной системе должно существовать 2 вида узлов: «управляющий» и «вычислительный». Необходимо объединить данные узлы в соответствии с той топологией, которая определена вариантом. Связь между узлами необходимо осуществить при помощи технологии очередей сообщений. Также в данной системе необходимо предусмотреть проверку доступности узлов в соответствии с вариантом. При убийстве («kill -9») любого вычислительного узла система должна пытаться максимально сохранять свою работоспособность, а именно все дочерние узлы убитого узла могут стать недоступными, но родительские узлы должны сохранить свою работоспособность. 

# Вариант 25    

1. Топология - дерево общего вида.
2. Набор команд - локальный целочисленный словарь.
3. Тип проверки доступности узлов - heartbeat time
4. Технология очереди сообщений: zero message queue


# Ссылка на отчет
- [Отчёт](https://docs.google.com/document/d/1tHzIaSzQ_1XU-Pon3RTI0i2OBZv1cy5UYxs7DtwJx1I/edit?usp=sharing)

## Запуск: 

- `cd lab5-7`
- `mkdir build`
- `cd build`
- `cmake ../ .`
- `make`
- Запускаем `./client_exe`
