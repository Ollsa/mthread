#pragma once
#include <iostream>
#include <vector>
#include <string>

//������� ������
struct Node {
    std::string data;                     //���� ������
    std::string crc;                      //CRC
    unsigned int process_id;              // ������������� �������� ��� �������
    Node* next;                           //��������� �� ������ ������� ������

    Node(std::string _data, unsigned int _process_id) : data(_data), process_id(_process_id), next(NULL) {} //�����������, ������������ ��� ������������� ���������� ������������ �� ������� �����
};


class FIFO
{
private:

    Node* front;                                        //��������� �� ����� ������ ������� ������
    Node* back;                                         //��������� �� ��������� ����������� ������� ������
    unsigned int size;

public:
    //���������� � ������ ������
    //�������� ��������, ������ �� � ������
    FIFO() : front(NULL), back(NULL), size(0) {}                 //����������� ������������ ��� ������������� ���������� ������������ �� ������� ������

    ~FIFO() {                                           //����������, ������������ ��� �������������� �������� ������ � ������ ����������� ������ ������
        while (front != NULL)                           //� ������� ����� �������� �������-���� ������, ������������� ������� ��������� ������ ������
            pop();                                      //����� �������-����� ������
    }

    void push(std::string _data);
    std::string pop();
    void print();
    unsigned int getSize();
    Node* popNode();
};

