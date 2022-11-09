#ifndef YCSB_C_LOCK_STL_MAP_DB_H_
#define YCSB_C_LOCK_STL_MAP_DB_H_

#include "db/hashtable_db.h"

#include <string>
#include <vector>
#include "lib/lock_stl_map.h"

namespace ycsbc {

class LockStlMapDB : public HashtableDB {
 public:
  LockStlMapDB() : HashtableDB(
      new vmp::LockStlMap<HashtableDB::FieldHashtable *>) { }

  ~LockStlMapDB() {
    std::vector<KeyHashtable::KVPair> key_pairs = key_table_->Entries();
    for (auto &key_pair : key_pairs) {
      DeleteFieldHashtable(key_pair.second);
    }
    delete key_table_;
  }

 protected:
  HashtableDB::FieldHashtable *NewFieldHashtable() {
    return new vmp::LockStlMap<const char *>;
  }

  void DeleteFieldHashtable(HashtableDB::FieldHashtable *table) {
    std::vector<FieldHashtable::KVPair> pairs = table->Entries();
    for (auto &pair : pairs) {
      DeleteString(pair.second);
    }
    delete table;
  }

  const char *CopyString(const std::string &str) {
    char *value = new char[str.length() + 1];
    strcpy(value, str.c_str());
    return value;
  }

  void DeleteString(const char *str) {
    delete[] str;
  }
};

} // ycsbc

#endif // YCSB_C_LOCK_STL_MAP_DB_H_