#pragma once
#include <iostream>
#include <vector>
#include <string>

//Элемент списка
struct Node {
    std::string data;                     //блок данных
    std::string crc;                      //CRC
    unsigned int process_id;              // идентификатор процесса Для отладки
    Node* next;                           //указатель на другой элемент списка

    Node(std::string _data, unsigned int _process_id) : data(_data), process_id(_process_id), next(NULL) {} //конструктор, используется для инициализации указателей направлением на нулевой адрес
};


class FIFO
{
private:

    Node* front;                                        //указатель на самый первый элемент списка
    Node* back;                                         //указатель на последний добавленный элемент списка
    unsigned int size;

public:
    //Количество и размер блоков
    //Добавить проверку, влезет ли в память
    FIFO() : front(NULL), back(NULL), size(0) {}                 //конструктор используется для инициализации указателей направлением на нулевые адреса

    ~FIFO() {                                           //деструктор, используется для автоматической зачистки памяти в момент прекращения работы класса
        while (front != NULL)                           //с помощью цикла вызываем функцию-член класса, освобождающую занятую элементом списка память
            pop();                                      //вызов функции-члена класса
    }

    void push(std::string _data);
    std::string pop();
    void print();
    unsigned int getSize();
    Node* popNode();
};

