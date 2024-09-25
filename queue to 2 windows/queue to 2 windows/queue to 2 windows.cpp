/*
Задание 1
Очередь клиентов
Вам нужно создать приложение, которое имитирует очередь в окошко. Для этого нужно создать два потока, работающие с одной разделяемой переменной.

Первый поток имитирует клиента: раз в секунду он обращается к счётчику клиентов и увеличивает его на 1. Максимальное количество клиентов должно быть параметризировано.

Второй поток имитирует операциониста: раз в 2 секунды он обращается к счётчику клиентов и уменьшает его на 1. «Операционист» работает до последнего клиента.

Атомарная очередь клиентов:

Нужно модифицировать задание 1 к первому уроку так, чтобы счётчик клиентов был атомарным.

Все операции со счётчиков должны быть атомарными.

Проверьте работу различными способами упорядочения доступа к памяти.

*/

#include <iostream>
#include <Windows.h>
#include<thread>
#include <ctime>
#include<atomic>
#include<mutex>

std::mutex m;// для корректного вывода текста, так же получил четкий порядок действий 1 к 2, потоки по сути синхронизируются иногда сразу, иногда через итерацию.



void secunda(std::atomic<int> &data)
{
    int end = data;
    m.lock();
    std::cout << "First window: start to work!\n";    
    std::cout << "Всего клиентов: " << data << std::endl;
    m.unlock();
    for (int i = 0; i < end; ++i)
    {        
        ++data; // инкрементные операции являются атомарными
        m.lock();
        std::cout  << " Очередь увеличелась (+), "<< "вего клиентов: " << data << std::endl;
        m.unlock();
        Sleep(500);
    }
    std::cout << "First window: work is down!\n" << std::endl;
}
void to_the_last(std::atomic<int> &data)
{
    m.lock();
    std::cout << "Second window: start to work!\n";    
    std::cout << "Всего клиентов: " << data << std::endl;
    m.unlock();
    do
    {
        m.lock();
        --data;        
        std::cout << " Очередь уменшилась (-), " << "вего клиентов: " << data << std::endl;
        m.unlock();
        Sleep(1000);
    } while (data > 0);
    std::cout << "Second window: work is down!\n" << std::endl;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::atomic<int> queue=10;
    auto now = std::chrono::system_clock::now();
    time_t now_t = std::chrono::system_clock::to_time_t(now);
    std::cout << "Время начала работы: ";
    printf("%02d:%02d:%02d \n", (now_t / 3600 + 3) % 24, (now_t / 60 % 60), now_t % 60);// текущее время 
    std::thread window1(secunda, std::ref(queue));
    std::thread window2(to_the_last, std::ref(queue));
    window1.join();
    window2.join();    
    return 0;
}


