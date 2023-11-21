#pragma once

#include <unordered_set>

// Wrapper around an unordered_set split into two.
template <typename T>
class HeadContainer {
private:
    std::unordered_set<T> head;
    std::unordered_set<T> body;
public:
    HeadContainer() = default;
    explicit HeadContainer(T head) : head{head}, body{head}
    {
    }

    std::unordered_set<T> getHead();
    std::unordered_set<T> getBody();
    void                  insert(T element);
    void                  insertHeadAndBody(T element);
    auto                  begin();
    auto                  begin() const;
    auto                  end();
    auto                  end() const;
    [[nodiscard]] bool                  empty() const;
};

template<typename T>
std::unordered_set<T> HeadContainer<T>::getHead() {
    return head;
}

template<typename T>
std::unordered_set<T> HeadContainer<T>::getBody() {
    return body;
}

template<typename T>
void HeadContainer<T>::insert(T element) {
    body.insert(element);
}

template<typename T>
void HeadContainer<T>::insertHeadAndBody(T element) {
    body.insert(element);
    head.insert(element);
}

template<typename T>
auto HeadContainer<T>::begin() {
    return body.begin();
}

template<typename T>
auto HeadContainer<T>::end() {
    return body.end();
}

template<typename T>
auto HeadContainer<T>::begin() const {
    return body.begin();
}

template<typename T>
auto HeadContainer<T>::end() const {
    return body.end();
}

template<typename T>
bool HeadContainer<T>::empty() const {
    return head.empty() && body.empty();
}
