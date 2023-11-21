#pragma once

#include <unordered_set>

template <typename T>
class Set {
private:
    std::unordered_set<T> set;

public:
    void insertElement(T element);
    [[nodiscard]] bool findElement(T element);
    void removeElement(T element);
    bool replaceElement(T oldElement, T newElement);
    [[nodiscard]] std::unordered_set<T> getAllElements();
};

template<typename T>
bool Set<T>::findElement(T element) {
    if (set.find(element) == set.end()) {
        return false;
    }
    return true;
}

template<typename T>
void Set<T>::removeElement(T element) {
    set.erase(element);
}

template<typename T>
bool Set<T>::replaceElement(T oldElement, T newElement) {
    if (set.find(oldElement) == set.end()) {
        return false;
    }
    removeElement(oldElement);
    insertElement(newElement);
    return true;
}

template<typename T>
void Set<T>::insertElement(T element) {
    set.insert(element);
}

template<typename T>
std::unordered_set<T> Set<T>::getAllElements() {
    std::unordered_set<T> elements;
    elements.insert(set.begin(), set.end());
    return elements;
}
