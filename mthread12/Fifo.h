#pragma once
#include <iostream>
#include <mutex>
#include <string>
#include <queue>
#include <vector>

struct Node {
    std::string data;
    std::vector<std::string> crc;
    unsigned int process_id;              // for debug

    Node(std::string _data, unsigned int _process_id) : data(_data), process_id(_process_id), crc('\0') {};
};

class Fifo
{
private:
    std::queue<Node> q;
    std::mutex mt1;
    std::mutex mt2;
    unsigned int max_size;
public:

    Fifo(unsigned int size)
    {
        max_size = size;
    };
    ~Fifo() {};

    void setNode(std::string data, unsigned int id)
    {
        if (q.size() > max_size) throw std::length_error("The item has not been added to the queue. Maximum queue size exceeded");
 
        mt1.lock();
        //???q->push({ RandomNumber(data_size).getRandomStr(), prId });
        q.push({ data, id });
        mt1.unlock();
    };

    Node getNode()
    {
        if (q.empty()) throw std::out_of_range("Queue is empty");
        
        mt2.lock();
        Node node = q.front();
        q.pop();
        mt2.unlock();
        return node;
    };

    unsigned int getSize()
    {
        return q.size();
    };

};


