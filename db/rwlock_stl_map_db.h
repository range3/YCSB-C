#ifndef YCSB_C_RWLOCK_STL_MAP_DB_H_
#define YCSB_C_RWLOCK_STL_MAP_DB_H_

#include "db/hashtable_db.h"

#include <string>
#include <vector>
#include "lib/lock_stl_map.h"

namespace ycsbc {

// TODO: Implement this class.
class RWLockStlMapDB : public HashtableDB {
 public:
  RWLockStlMapDB()
      : HashtableDB(new vmp::LockStlMap<HashtableDB::FieldHashtable*>) {}

  ~RWLockStlMapDB() {
    std::vector<KeyHashtable::KVPair> key_pairs = key_table_->Entries();
    for (auto& key_pair : key_pairs) {
      DeleteFieldHashtable(key_pair.second);
    }
    delete key_table_;
  }

 protected:
  HashtableDB::FieldHashtable* NewFieldHashtable() {
    return new vmp::LockStlMap<const char*>;
  }

  void DeleteFieldHashtable(HashtableDB::FieldHashtable* table) {
    std::vector<FieldHashtable::KVPair> pairs = table->Entries();
    for (auto& pair : pairs) {
      DeleteString(pair.second);
    }
    delete table;
  }

  const char* CopyString(const std::string& str) {
    char* value = new char[str.length() + 1];
    strcpy(value, str.c_str());
    return value;
  }

  void DeleteString(const char* str) { delete[] str; }
};

}  // namespace ycsbc

#endif  // YCSB_C_RQLOCK_STL_MAP_DB_H_
