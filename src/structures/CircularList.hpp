#pragma once
#include <iostream>    
#include "../models/Product.hpp"
using namespace std;

struct Node {
    Product data;
    Node* next;
    Node(Product p) : data(p), next(nullptr) {}  // ✅ implementación inline
};


class CircularList {
private:
    Node* head;
public:
    CircularList() : head(nullptr) {}

    void insert(Product p) {
        Node* nuevo = new Node(p);
        if (!head) {
            head = nuevo;
            head->next = head;
        } else {
            Node* temp = head;
            while (temp->next != head) temp = temp->next;
            temp->next = nuevo;
            nuevo->next = head;
        }
    }

    void display() {
        if (!head) return;
        Node* temp = head;
        do {
            std::cout << temp->data.codigo << " - " << temp->data.nombre
                      << " - $" << temp->data.precio
                      << " - stock: " << temp->data.stock << "\n";
            temp = temp->next;
        } while (temp != head);
    }
};


