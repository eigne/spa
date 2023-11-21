#pragma once

#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <functional>
#include <deque>

/**
 * A data structure that contains two maps: a forward and backward map.
 * Both maps store keys mapped to Containers of values.
 * The backward map is a copy of the forward map, but in reverse.
 * This allows O(1) reverse lookup of the map at the cost of using twice the memory.
 * @tparam T The key type.
 * @tparam U The value type.
 * @tparam Container Any set-like container class. Defaults to unordered_set.
 */
template<typename T, typename U, template<typename, typename...> class Container = std::unordered_set>
class DoubleMap {
private:
    std::unordered_map<T, Container<U>> forward;
    std::unordered_map<U, Container<T>> backward;

public:
    void                                     insertForward(T key, U value);
    void                                     insertForwardHeadAndBody(T key, U value);
    void                                     insertBackward(U key, T value);
    void                                     insertBackwardHeadAndBody(U key, T value);
    void                                     insertBoth(T forwardKey, U forwardValue);
    void                                     insertBothHeadAndBody(T forwardKey, U forwardValue);
    void                                     insertEmpty(T forwardKey);
    [[nodiscard]] Container<U>               lookupForward(T key);
    [[nodiscard]] Container<T>               lookupBackward(U key);
    [[nodiscard]] std::unordered_set<T>      getAllForwardKeys();
    [[nodiscard]] std::unordered_set<U>      getAllBackwardKeys();
    std::unordered_map<T, Container<U>>      getForwardMap();
    [[nodiscard]] bool                       empty();
};

/**
 * Inserts into the forward map only.
 * @param key The key to insert.
 * @param value The value to insert into the Container associated with key.
 */
template<typename T, typename U, template<typename, typename...> class Container>
void DoubleMap<T, U, Container>::insertForward(T key, U value) {
    if (forward.find(key) != forward.end()) {
        forward[key].insert(value);
        return;
    }
    forward.insert({ key, Container<U>{ value }});
}

/**
 * Inserts into the forward map only.
 * Inserts into both the head and body sets of Container, assuming Container is a HeadContainer.
 * This is a duck-typed function that assumes Container supports this method.
 * Only call this method on DoubleMaps initialised with HeadContainer as a Container, or this will not compile.
 * @param key The key to insert.
 * @param value The value to insert into the Container associated with key.
 */
template<typename T, typename U, template<typename, typename...> class Container>
void DoubleMap<T, U, Container>::insertForwardHeadAndBody(T key, U value) {
    if (forward.find(key) != forward.end()) {
        forward[key].insertHeadAndBody(value);
        return;
    }
    forward.insert({ key, Container<U>{ value }});
}

/**
 * Inserts into the backward map only.
 * @param key The key to insert.
 * @param value The value to insert into the Container associated with key.
 */
template<typename T, typename U, template<typename, typename...> class Container>
void DoubleMap<T, U, Container>::insertBackward(U key, T value) {
    if (backward.find(key) != backward.end()) {
        backward[key].insert(value);
        return;
    }

    backward.insert({key, Container<T>{value}});
}

/**
 * Inserts into the backward map only.
 * Inserts into both the head and body sets of Container, assuming Container is a HeadContainer.
 * This is a duck-typed function that assumes Container supports this method.
 * Only call this method on DoubleMaps initialised with HeadContainer as a Container, or this will not compile.
 * @param key The key to insert.
 * @param value The value to insert into the Container associated with key.
 */
template<typename T, typename U, template<typename, typename...> class Container>
void DoubleMap<T, U, Container>::insertBackwardHeadAndBody(U key, T value) {
    if (backward.find(key) != backward.end()) {
        backward[key].insertHeadAndBody(value);
        return;
    }

    backward.insert({key, Container<T>{value}});
}

/**
 * Inserts into both maps, forward and backward.
 * Equivalent to calling both insertForward and insertBackward, swapping the arguments appropriately.
 * @param forwardKey The key to insert into the forward map, which is also the value to insert into the backward map.
 * @param forwardValue The value to insert into the forward map, which is also the key to insert into the backward map.
 */
template<typename T, typename U, template<typename, typename...> class Container>
void DoubleMap<T, U, Container>::insertBoth(T forwardKey, U forwardValue) {
    insertForward(forwardKey, forwardValue);
    insertBackward(forwardValue, forwardKey);
}

/**
 * Inserts a key into the forward map only with no values. An empty Container is associated with it. Values can be inserted
 * into this empty Container later with insertForward or insertBoth.
 * This method does not call insertBackward as it would not make sense to have an empty Container as a key.
 * @param forwardKey The key to insert into the forward map.
 */
template<typename T, typename U, template<typename, typename...> class Container>
void DoubleMap<T, U, Container>::insertEmpty(T forwardKey) {
    forward.emplace(forwardKey, Container<U>{});
}

/**
 * Inserts into both maps.
 * Inserts into both the head and body sets of Container, assuming Container is a HeadContainer.
 * This is a duck-typed function that assumes Container supports this method.
 * Only call this method on DoubleMaps initialised with HeadContainer as a Container, or this will not compile.
 * @param key The key to insert.
 * @param value The value to insert into the Container associated with key.
 */
template<typename T, typename U, template<typename, typename...> class Container>
void DoubleMap<T, U, Container>::insertBothHeadAndBody(T forwardKey, U forwardValue) {
    insertForwardHeadAndBody(forwardKey, forwardValue);
    insertBackwardHeadAndBody(forwardValue, forwardKey);
}

/**
 * Queries the forward map only.
 * @param key The key to search for.
 * @return The Container associated with key.
 */
template<typename T, typename U, template<typename, typename...> class Container>
Container<U> DoubleMap<T, U, Container>::lookupForward(T key) {
    if (forward.find(key) != forward.end()) {
        return forward.at(key);
    }
    return Container<U>{ };
}

template<typename T, typename U, template<typename, typename...> class Container>
Container<T> DoubleMap<T, U, Container>::lookupBackward(U key) {
    if (backward.find(key) != backward.end()) {
        return backward.at(key);
    }
    return Container<T>{ };
}

template<typename T, typename U, template<typename, typename...> class Container>
std::unordered_set<T> DoubleMap<T, U, Container>::getAllForwardKeys() {
    std::unordered_set<T> keys;
    for (auto iter{ forward.begin() }; iter != forward.end(); ++iter) {
        keys.insert(iter->first);
    }
    return keys;
}

template<typename T, typename U, template<typename, typename...> class Container>
std::unordered_set<U> DoubleMap<T, U, Container>::getAllBackwardKeys() {
    std::unordered_set<U> keys;
    for (auto iter{ backward.begin() }; iter != backward.end(); ++iter) {
        keys.insert(iter->first);
    }
    return keys;
}

template<typename T, typename U, template<typename, typename...> class Container>
std::unordered_map<T, Container<U>> DoubleMap<T, U, Container>::getForwardMap() {
    return forward;
}

template<typename T, typename U, template<typename, typename...> class Container>
bool DoubleMap<T, U, Container>::empty() {
    return forward.empty();
}
