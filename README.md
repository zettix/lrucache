# LRUCache Map
## A Template C++ LRU (Least Recently Used) Cache Map.

(C) 2019 Sean Brennan

Apache License

Similar to unordered_map

Usage:

```
  LRUCache<string, float> me(100);  // LRUCache with capacity of 100 T's
  me.insert(std::make_pair("key",  0.123f);
```


The following methods are currently supported.  Methods with a hash (#) move the key/value pair
to the front of the queue, where the back of the queue is delete to make room for new
insertions.  Method function should match that in std::unordered_map
```
  // Member functions
  LRUCache(long capacity)
  ~LRUCache()
  // Capacity
  void set_capacity(long max_elements)
  bool empty()
  long size()
  long max_size()
  // Iterators
  iterator begin()
  iterator end()
  // Element Access
  T operator[](const std::string &key)  #
  T at(const std::string &key)  #
  // Element lookup
  iterator find(std::string key)  #
  long count(const std::string &key)
  // Modifiers
  void insert(std::pair<std::string, T> item) #
  long erase(const std::string &key)
  void clear()
  // Debug
  std::string str()
```
See LRUCache_test.cpp for example usage.  
