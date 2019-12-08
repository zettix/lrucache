# LRUCache
A c++ LRU (Least Recently Used) Cache.  (C) 2019 Sean Brennan
At the moment, only std::string keys are supported, with a template data type.
The following methods are currently supported.  Methods with a # move the key/value pair
to the front of the queue, where the back of the queue is delete to make room for new
insertions.
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
  T operator[](const std::string &key)
  T at(const std::string &key)
  // Element lookup
  iterator find(std::string key)
  long count(const std::string &key)
  // Modifiers
  void insert(std::pair<std::string, T> item)
  long erase(const std::string &key)
  void clear()
  // Debug
  std::string str()
```
See LRUCache_test.cpp for example usage.  
