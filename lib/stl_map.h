#ifndef YCSB_C_LIB_STL_MAP_H_
#define YCSB_C_LIB_STL_MAP_H_

#include "lib/string_hashtable.h"

#include <unordered_map>
#include <vector>
#include "lib/string.h"

namespace vmp {

template <class V, class MA = MemAlloc,
    class PA = std::allocator<std::pair<const String, V>>>
class StlMap : public StringHashtable<V> {
 public:
  typedef typename StringHashtable<V>::KVPair KVPair;

  V Get(const char *key) const; ///< Returns NULL if the key is not found
  bool Insert(const char *key, V value);
  V Update(const char *key, V value);
  V Remove(const char *key);
  std::vector<KVPair> Entries(const char *key = NULL,
                              std::size_t n = -1) const;
  std::size_t Size() const { return table_.size(); }

 private:

  typedef std::map<String, V, std::less<String>, PA> Hashtable;
  Hashtable table_;
};

template<class V, class MA, class PA>
V StlMap<V, MA, PA>::Get(const char *key) const {
  typename Hashtable::const_iterator pos = table_.find(String::Wrap(key));
  if (pos == table_.end()) return NULL;
  else return pos->second;
}

template<class V, class MA, class PA>
bool StlMap<V, MA, PA>::Insert(const char *key, V value) {
  if (!key) return false;
  String skey = String::Copy<MA>(key);
  return table_.insert(std::make_pair(skey, value)).second;
}

template<class V, class MA, class PA>
V StlMap<V, MA, PA>::Update(const char *key, V value) {
  typename Hashtable::iterator pos = table_.find(String::Wrap(key));
  if (pos == table_.end()) return NULL;
  V old = pos->second;
  pos->second = value;
  return old;
}

template<class V, class MA, class PA>
V StlMap<V, MA, PA>::Remove(const char *key) {
  typename Hashtable::const_iterator pos = table_.find(String::Wrap(key));
  if (pos == table_.end()) return NULL;
  String::Free<MA>(pos->first);
  V old = pos->second;
  table_.erase(pos);
  return old;
}

template<class V, class MA, class PA>
std::vector<typename StlMap<V, MA, PA>::KVPair>
StlMap<V, MA, PA>::Entries(const char *key, std::size_t n) const {
  std::vector<KVPair> pairs;
  typename Hashtable::const_iterator pos;
  if (!key) {
    pos = table_.cbegin();
  } else {
    pos = table_.find(String::Wrap(key));
  }
  for (std::size_t i = 0; pos != table_.end() && i < n; ++pos, ++i) {
    pairs.push_back(std::make_pair(pos->first.value(), pos->second));
  }
  return pairs;
}

} // vmp

#endif // YCSB_C_LIB_STL_MAP_H_
