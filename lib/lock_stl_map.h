#ifndef YCSB_C_LIB_LOCK_STL_MAP_H_
#define YCSB_C_LIB_LOCK_STL_MAP_H_

#include "lib/stl_map.h"

#include <vector>
#include <mutex>

namespace vmp {

template<class V>
class LockStlMap : public StlMap<V> {
 public:
  typedef typename StringHashtable<V>::KVPair KVPair;

  V Get(const char *key) const; ///< Returns NULL if the key is not found
  bool Insert(const char *key, V value);
  V Update(const char *key, V value);
  V Remove(const char *key);
  std::vector<KVPair> Entries(const char *key = NULL, size_t n = -1) const;
  std::size_t Size() const;

 private:
  mutable std::mutex mutex_;
};

template<class V>
inline V LockStlMap<V>::Get(const char *key) const {
  std::lock_guard<std::mutex> lock(mutex_);
  return StlMap<V>::Get(key);
}

template<class V>
inline bool LockStlMap<V>::Insert(const char *key, V value) {
  std::lock_guard<std::mutex> lock(mutex_);
  return StlMap<V>::Insert(key, value);
}

template<class V>
inline V LockStlMap<V>::Update(const char *key, V value) {
  std::lock_guard<std::mutex> lock(mutex_);
  return StlMap<V>::Update(key, value);
}

template<class V>
inline V LockStlMap<V>::Remove(const char *key) {
  std::lock_guard<std::mutex> lock(mutex_);
  return StlMap<V>::Remove(key);
}

template<class V>
inline std::size_t LockStlMap<V>::Size() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return StlMap<V>::Size();
}

template<class V>
inline std::vector<typename LockStlMap<V>::KVPair>
LockStlMap<V>::Entries(const char *key, size_t n) const {
  std::lock_guard<std::mutex> lock(mutex_);
  return StlMap<V>::Entries(key, n);
}

} // vmp

#endif // YCSB_C_LIB_LOCK_STL_MAP_H_
