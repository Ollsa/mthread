#include "FIFO.h"
#include <mutex>

std::mutex m1;
std::mutex m2;
std::mutex m3;

unsigned int FIFO::getSize() {
    return this->size;
}

/*
* �������� ������� � ������
*/
void FIFO::push(std::string _data) {
    m1.lock();
    auto id = std::this_thread::get_id();
    uint32_t* ptr = (uint32_t*)&id;
    unsigned int prId = *ptr;
    Node* newNode = new Node(_data, prId);               //��������� ������ ��� ������ ��������
    std::cout <<" Push " << prId << std::endl;

    if (front == NULL) {                              //���� ������� ������
        front = newNode;                            //���������� ��������� front �� ����
    }
    else {
        back->next = newNode;                       //� ��������� ������ ���������� ��������� back->next  �� ��������� ����
    }

    back = newNode;                                 //� ����������, ��� ��������� ���� � ��� ���������, ���������, ������� ������
    this->size++;
    m1.unlock();
}

/*
* ����� ������ �������� �� ������
*/
std::string FIFO:: pop() {
    m2.lock();
    std::string str;
    //   if (front == NULL)           throw std::underflow_error("Nothing to dequeue");

    if (front == NULL) {            //��� ���������� ������ ����, ��� �� ������ � ������������������ ���� �������, �� ��������� ����
                                    //������� ����������, �� �� ������ ����� ����� ��������, ��� ���, ������� ���
    //����� ����� �������� �������������� ��������� � ���, ��� ������ ������//
        //return ;
    }
    else {

        Node* temp = front;
        front = front->next;                            //������� ��������� �� ������ ������ �� ��������� �������
        str = temp->data;
        this->size--;
        delete(temp);               //������������ ������ �� ��������, ������� ���� ��� ������ ������
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

    if (front == NULL) {            //��� ���������� ������ ����, ��� �� ������ � ������������������ ���� �������, �� ��������� ����
                                    //������� ����������, �� �� ������ ����� ����� ��������, ��� ���, ������� ���
    //����� ����� �������� �������������� ��������� � ���, ��� ������ ������//
        //return ;
    }
    else {

        temp = front;
        front = front->next;                            //������� ��������� �� ������ ������ �� ��������� �������
        str = temp->data;
        this->size--;
    }
    m3.unlock();
    return temp;
}

void FIFO:: print() {
    Node* temp = front;                             //��������� ���� ��������� � ���������� ��� �� ������ ������
    while (temp != NULL) {                          //� ������� ����� ������� ���� ������
        std::cout << temp->data << " Process Id: "<< temp ->process_id <<std::endl;             //Выводим информацию на экран
        temp = temp->next;                          //���������� ��������� �� ��������� �������
    }
}