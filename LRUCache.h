#ifndef __COM_ZETTIX_LRUCACHE__
#define __COM_ZETTIX_LRUCACHE__

#include <iterator>
#include <unordered_map>
#include <typeinfo>
#include <string>
#include <sstream>

namespace com_zettix {

// Utility: A double linked list node:
template<class S, class T>
class DLLNode {
 public:
  DLLNode<S, T>(S in_key, T in_data) : key(in_key), data(in_data) {
    child = nullptr;
    parent = nullptr;
  }
  DLLNode<S, T> *parent;
  DLLNode<S, T> *child;
  S key;
  T data;
  std::string str();
};

template<class S, class T>
std::string DLLNode<S, T>::str() {
  std::stringstream ss;
  ss << "K:" << key << " V: ";
  ss << data;
  return ss.str();
}

/* Least Recently Used Cache

A least recently used cache maintains a maximum cache size,
and removed items based on their last reference.  Items
referenced are moved to the front of the queue, and items
at the back of the queue are removed as necessary to maintain
the cache size.

Usage: see public section, much like unordered_map with string keys.
  LRUCache<S, T> me(100);  // LRUCache with capacity of 100 T's
  me.insert(std::make_pair("key", T value);
*/
template<class S, class T>
class LRUCache {

 public:
  class iterator : public std::iterator<std::bidirectional_iterator_tag, std::pair<S, T>>  {
   public:
    iterator(DLLNode<S, T> *in_node): n(in_node) {}
    iterator(const iterator& mit) : n(mit.n) {}
    std::pair<S, T>  operator*() {return std::make_pair(n->key, n->data);}

    bool has_next() {
      if (n->child != nullptr) {
        return true;
      }
      return false;
    }

    iterator next() {
      if (n->child != nullptr) {
        n = n->child;
        return *this;
      } else {
        return nullptr;
      }
    }

    iterator operator++(int) {
      return this->next();
    }

   private:
    DLLNode<S, T> *n;
  };
 
  // Member functions
  LRUCache(long capacity): cap(capacity) {}
  ~LRUCache();
  // Capacity
  void set_capacity(long max_elements);
  bool empty();
  long size();
  long max_size();
  // Iterators
  iterator begin() { return iterator(head); };
  iterator end() { return iterator(nullptr); };
  // Element Access
  T operator[](const S &key);
  T at(const S &key);
  // Element lookup
  iterator find(S key) {
    auto search = data_.find(key);
    if (search != data_.end()) {
      DLLNode<S, T> *n = search->second;
      update_node(n);
      iterator it(n);
      return it;
    }
    iterator it(nullptr);
    return it;
  }
  long count(const S &key);
  // Modifiers
  void insert(std::pair<S, T> item);
  long erase(const S &key);
  void clear();
  // Debug
  std::string str();

 private:
   std::unordered_map<S, DLLNode<S, T> *> data_;
   DLLNode<S, T> *head = nullptr;
   DLLNode<S, T> *tail = nullptr;
   long cap = 10;
   void update_node(DLLNode<S, T> *n);
   void remove_node(DLLNode<S, T> *n);
};

template<class S, class T>
void LRUCache<S, T>::set_capacity(long capacity) {
  cap = capacity;
  while (data_.size() > cap) {
    data_.erase(tail->key);
    remove_node(tail);
  }
}

template<class S, class T>
LRUCache<S, T>::~LRUCache() {
   for (auto it = data_.begin(); it !=data_.end(); it++) {
     delete (it->second);
   }
}

template<class S, class T>
long LRUCache<S, T>::size() {
  return data_.size();
}

template<class S, class T>
long LRUCache<S, T>::max_size() {
  return data_.max_size();
}

template<class S, class T>
void LRUCache<S, T>::clear() {
   for (auto it = data_.begin(); it !=data_.end(); it++) {
     delete it->second;
   }
  data_.clear();
}

template<class S, class T>
void LRUCache<S, T>::update_node(DLLNode<S, T> *n) {
  if (n->child != nullptr && n->parent != nullptr) { // middle a-x-b->x..a-b
     n->child->parent = n->parent;
     n->parent->child = n->child;
     n->child = head;
     n->parent = nullptr;
  } else if (n->child != nullptr) { // n->parent is null, x-...
     // I'm already the head
  } else if (n->parent != nullptr) { // n->child is null a-b-x
     // I'm at the very end.
     n->parent->child = nullptr;
     tail = n->parent;
     n->child = head;
     n->parent = nullptr;
  } else  { // parent and child are null
     tail = n;
  }
  head = n;
}

template<class S, class T>
void LRUCache<S, T>::remove_node(DLLNode<S, T> *n) {
  if (n->child != nullptr && n->parent != nullptr) { // middle a-x-b->x..a-b
     n->child->parent = n->parent;
     n->parent->child = n->child;
  } else if (n->child != nullptr) { // n->parent is null, x-...  I'm the head
     head = head->child; 
  } else if (n->parent != nullptr) { // n->child is null a-b-x  I'm the tail
     tail = n->parent;
     tail->child = nullptr;
  } else  { // parent and child are null.
     tail = nullptr;
     head = nullptr;
  }
  delete(n);
}

template<class S, class T>
T LRUCache<S, T>::at(const S &key) {
  auto search = data_.find(key);
  if (search != data_.end()) {  // Head, Middle, or End, special cases?
    DLLNode<S, T> *n = search->second;
    update_node(n);
    return head->data;
  }
  throw std::out_of_range("std::string not found.");
}

template<class S, class T>
T  LRUCache<S, T>::operator[](const S &key) {
  return at(key);
}

template<class S, class T>
long LRUCache<S, T>::count(const S &key) {
  auto search = data_.find(key);
  if (search == data_.end()) {
    return 0;
  }
  return 1;
}

template<class S, class T>
void LRUCache<S, T>::insert(std::pair<S, T> item) {
  auto search = data_.find(item.first);
  if (search != data_.end()) {
    DLLNode<S, T> *n = search->second;
    update_node(n);
    n->data = item.second;
  } else {
    DLLNode<S, T> *n = new DLLNode<S, T>(item.first, item.second);
    n->parent = nullptr;
    n->child = head;
    if (head != nullptr) {
      head->parent = n;
    }
    head = n;
    data_.insert(std::make_pair(item.first, n));
    if (tail == nullptr) {
      tail = n;
    }
  }
  while (data_.size() > cap) {
    data_.erase(tail->key);
    remove_node(tail);
  }
}

template<class S, class T>
long LRUCache<S, T>::erase(const S &key) {
  auto search = data_.find(key);
  if (search != data_.end()) {
    DLLNode<S, T> *node = search->second;
    data_.erase(key);
    remove_node(node);
    return 1;
  }
  return 0;
}

template<class S, class T>
std::string LRUCache<S, T>::str() {
  std::stringstream ss;
  DLLNode<S, T> *n = head;
  while (n != nullptr) {
    ss << n->str() << std::endl;
    n = n->child;
  }
  return ss.str();
}

} // namespace
#endif
