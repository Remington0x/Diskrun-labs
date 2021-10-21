#ifndef TVECTOR_HPP
#define TVECTOR_HPP

#include <memory>

template <class T>
class TVectorNode {
public:
    T data;
    std::shared_ptr< TVectorNode<T> > next;
    TVectorNode() noexcept : data(), next(nullptr) {};
	explicit TVectorNode(const T & elem) noexcept : data(elem), next(nullptr) {};
};

template <class T>
class TVector {
private:
    int size;
    std::shared_ptr< TVectorNode<T> > leftNode;
public:
    void insert(T data, int pos);
    T at(int pos);
    void erase(int pos);
    void erase(int pos1, int pos2);
    int getSize();

    TVector() noexcept : leftNode(nullptr) {};
};

template <class T>
int TVector<T>::getSize() {
    int i = 0;
    std::shared_ptr< TVectorNode<T> > curNode = leftNode;
    while (curNode != nullptr) {
        ++i;
        curNode = curNode->next;
    }
    return i;
}

template <class T>
void TVector<T>::erase(const int pos) {
    int i = 0;
    if (pos == 0) {
        leftNode = leftNode->next;
        return;
    }
    std::shared_ptr< TVectorNode<T> > curNode = leftNode;
    while (i < (pos - 1) && curNode != nullptr) {
        curNode = curNode->next;
        ++i;
    }
    if (i == (pos - 1) && curNode != nullptr) {
        curNode->next = curNode->next->next;
    } else {
        throw(std::runtime_error("erase error\n"));
    }
    return;
}

template <class T>
void TVector<T>::erase(const int pos1, const int pos2) {
    int i = 0;
    std::shared_ptr< TVectorNode<T> > node1 = leftNode;
    std::shared_ptr< TVectorNode<T> > node2;
    if (pos1 == 0) {
        node2 = node1;
        while (i < pos2 && node2 != nullptr) {
            node2 = node2->next;
            ++i;
        }
        if (i == pos2 && node2 != nullptr) {
            leftNode = node2->next;
            return;
        } else {
            //std::cout << "node2 = " << node2 << std::endl;
            throw(std::runtime_error("erase error"));
        }
    }
    while (i < (pos1 - 1) && node1 != nullptr) {
        node1 = node1->next;
        ++i;
    }
    if (i == (pos1 - 1) && node1 != nullptr) {
        node2 = node1;
        while (i < pos2 && node2 != nullptr) {
            node2 = node2->next;
            ++i;
        }
        if (i == pos2 && node2 != nullptr) {
            node1->next = node2->next;
        } else {
            //std::cout << "node2 = " << node2 << std::endl;
            throw(std::runtime_error("erase error\n"));
        }
    } else {
        throw(std::runtime_error("erase error\n"));
    }

}

template <class T>
T TVector<T>::at(int pos) {
    int i = 0;
    std::shared_ptr< TVectorNode<T> > curNode = leftNode;
    while (i < pos && curNode != nullptr) {
        curNode = curNode->next;
        ++i;
    }
    if (i == pos && curNode != nullptr) {
        return curNode->data;
    } else {
        throw(std::runtime_error("at error\n"));
    }
}

template <class T>
void TVector<T>::insert(T data, int pos) {
    int i = 0;
    std::shared_ptr< TVectorNode<T> > curNode = leftNode;
    if (pos == 0) {
        TVectorNode<T>* newNode = new TVectorNode<T>(data);
        std::shared_ptr< TVectorNode<T> > newNodeShared(newNode);
        newNodeShared->next = leftNode;
        leftNode = newNodeShared;

        return;
    }
    while (i < (pos - 1) && curNode != nullptr) {
        ++i;
        curNode = curNode->next;
    }

    if (i == (pos - 1) && curNode != nullptr) {
        TVectorNode<T>* newNode = new TVectorNode<T>(data);
        std::shared_ptr< TVectorNode<T> > newNodeShared(newNode);
        newNodeShared->next = curNode->next;
        curNode->next = newNodeShared;
    } else {
        std::cout << "i = " << i << ", curNode = " << curNode << ", leftNode = " << leftNode << std::endl;
        throw(std::runtime_error("insert error"));
    }
}

#endif
