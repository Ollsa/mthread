#include "FIFO.h"
#include <mutex>

std::mutex m1;
std::mutex m2;
std::mutex m3;

unsigned int FIFO::getSize() {
    return this->size;
}

/*
* Добавить элемент в список
*/
void FIFO::push(std::string _data) {
    m1.lock();
    auto id = std::this_thread::get_id();
    uint32_t* ptr = (uint32_t*)&id;
    unsigned int prId = *ptr;
    Node* newNode = new Node(_data, prId);               //выделение памяти для нового элемента
    std::cout <<" Push " << prId << std::endl;

    if (front == NULL) {                              //если элемент первый
        front = newNode;                            //направляем указатель front на него
    }
    else {
        back->next = newNode;                       //в противном случае направляем указатель back->next  на созданный узел
    }

    back = newNode;                                 //и обозначаем, что созданный узел — это последний, хвостовой, элемент списка
    this->size++;
    m1.unlock();
}

/*
* Взять данные элемента из списка
*/
std::string FIFO:: pop() {
    m2.lock();
    std::string str;
    //   if (front == NULL)           throw std::underflow_error("Nothing to dequeue");

    if (front == NULL) {            //Это упрощённая запись того, что вы видите в закомментированной выше строчке, то обработка путём
                                    //выброса исключения, но мы скорее всего плохо понимаем, что это, поэтому так
    //здесь можно вставить информационное сообщение о том, что список пустой//
        //return ;
    }
    else {

        Node* temp = front;
        front = front->next;                            //Смещаем указатель на начало списка на следующий элемент
        str = temp->data;
        this->size--;
        delete(temp);               //Высвобождаем память от элемента, который стал уже бывшим первым
    }
    m2.unlock();
    return str;
}

/*
* Взять элемента из списка Для отладки
*/
Node* FIFO::popNode() {
    m3.lock();
    std::string str;
    Node* temp = NULL;
    //   if (front == NULL)           throw std::underflow_error("Nothing to dequeue");

    if (front == NULL) {            //Это упрощённая запись того, что вы видите в закомментированной выше строчке, то обработка путём
                                    //выброса исключения, но мы скорее всего плохо понимаем, что это, поэтому так
    //здесь можно вставить информационное сообщение о том, что список пустой//
        //return ;
    }
    else {

        temp = front;
        front = front->next;                            //Смещаем указатель на начало списка на следующий элемент
        str = temp->data;
        this->size--;
    }
    m3.unlock();
    return temp;
}

void FIFO:: print() {
    Node* temp = front;                             //Объявляем себе указатель и направляем его на начало списка
    while (temp != NULL) {                          //С помощью цикла обходим весь список
        std::cout << temp->data << " Process Id: "<< temp ->process_id <<std::endl;             //Выводим информацию на экран
        temp = temp->next;                          //Направляем указатель на следующий элемент
    }
}