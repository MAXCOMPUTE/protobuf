// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This file defines the map container and its helpers to support protobuf maps.
//
// The Map and MapIterator types are provided by this header file.
// Please avoid using other types defined here, unless they are public
// types within Map or MapIterator, such as Map::value_type.

#ifndef GOOGLE_PROTOBUF_MAP_H__
#define GOOGLE_PROTOBUF_MAP_H__

#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>  // To support Visual Studio 2008
#include <map>
#include <type_traits>
#include <utility>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/generated_enum_util.h>
#include <google/protobuf/map_type_handler.h>
#include <google/protobuf/stubs/hash.h>

#ifdef SWIG
#error "You cannot SWIG proto headers"
#endif

#include <google/protobuf/port_def.inc>

PROTOBUF_NAMESPACE_OPEN

template <typename Key, typename T>
class Map;

class MapIterator;

template <typename Enum>
struct is_proto_enum;

namespace internal {
template <typename Derived, typename Key, typename T,
          WireFormatLite::FieldType key_wire_type,
          WireFormatLite::FieldType value_wire_type, int default_enum_value>
class MapFieldLite;

template <typename Derived, typename Key, typename T,
          WireFormatLite::FieldType key_wire_type,
          WireFormatLite::FieldType value_wire_type, int default_enum_value>
class MapField;

template <typename Key, typename T>
class TypeDefinedMapFieldBase;

class DynamicMapField;

class GeneratedMessageReflection;

// re-implement std::allocator to use arena allocator for memory allocation.
// Used for Map implementation. Users should not use this class
// directly.
template <typename U>
class MapAllocator {
 public:
  using value_type = U;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;
  using difference_type = ptrdiff_t;

  MapAllocator() : arena_(nullptr) {}
  explicit MapAllocator(Arena* arena) : arena_(arena) {}
  template <typename X>
  MapAllocator(const MapAllocator<X>& allocator)  // NOLINT(runtime/explicit)
      : arena_(allocator.arena()) {}

  pointer allocate(size_type n, const void* /* hint */ = nullptr) {
    // If arena is not given, malloc needs to be called which doesn't
    // construct element object.
    if (arena_ == nullptr) {
      return static_cast<pointer>(::operator new(n * sizeof(value_type)));
    } else {
      return reinterpret_cast<pointer>(
          Arena::CreateArray<uint8>(arena_, n * sizeof(value_type)));
    }
  }

  void deallocate(pointer p, size_type n) {
    if (arena_ == nullptr) {
#if defined(__GXX_DELETE_WITH_SIZE__) || defined(__cpp_sized_deallocation)
      ::operator delete(p, n * sizeof(value_type));
#else
      (void)n;
      ::operator delete(p);
#endif
    }
  }

#if __cplusplus >= 201103L && !defined(GOOGLE_PROTOBUF_OS_APPLE) && \
    !defined(GOOGLE_PROTOBUF_OS_NACL) &&                            \
    !defined(GOOGLE_PROTOBUF_OS_EMSCRIPTEN)
  template <class NodeType, class... Args>
  void construct(NodeType* p, Args&&... args) {
    // Clang 3.6 doesn't compile static casting to void* directly. (Issue
    // #1266) According C++ standard 5.2.9/1: "The static_cast operator shall
    // not cast away constness". So first the maybe const pointer is casted to
    // const void* and after the const void* is const casted.
    new (const_cast<void*>(static_cast<const void*>(p)))
        NodeType(std::forward<Args>(args)...);
  }

  template <class NodeType>
  void destroy(NodeType* p) {
    p->~NodeType();
  }
#else
  void construct(pointer p, const_reference t) { new (p) value_type(t); }

  void destroy(pointer p) { p->~value_type(); }
#endif

  template <typename X>
  struct rebind {
    using other = MapAllocator<X>;
  };

  template <typename X>
  bool operator==(const MapAllocator<X>& other) const {
    return arena_ == other.arena_;
  }

  template <typename X>
  bool operator!=(const MapAllocator<X>& other) const {
    return arena_ != other.arena_;
  }

  // To support Visual Studio 2008
  size_type max_size() const {
    // parentheses around (std::...:max) prevents macro warning of max()
    return (std::numeric_limits<size_type>::max)();
  }

  // To support gcc-4.4, which does not properly
  // support templated friend classes
  Arena* arena() const { return arena_; }

 private:
  using DestructorSkippable_ = void;
  Arena* const arena_;
};

template <typename T>
using KeyForTree =
    typename std::conditional<std::is_scalar<T>::value, T,
                              std::reference_wrapper<const T>>::type;

}  // namespace internal

// This is the class for Map's internal value_type. Instead of using
// std::pair as value_type, we use this class which provides us more control of
// its process of construction and destruction.
template <typename Key, typename T>
struct MapPair {
  using first_type = const Key;
  using second_type = T;

  MapPair(const Key& other_first, const T& other_second)
      : first(other_first), second(other_second) {}
  explicit MapPair(const Key& other_first) : first(other_first), second() {}
  MapPair(const MapPair& other) : first(other.first), second(other.second) {}

  ~MapPair() {}

  // Implicitly convertible to std::pair of compatible types.
  template <typename T1, typename T2>
  operator std::pair<T1, T2>() const {  // NOLINT(runtime/explicit)
    return std::pair<T1, T2>(first, second);
  }

  const Key first;
  T second;

 private:
  friend class Arena;
  friend class Map<Key, T>;
};

// Map is an associative container type used to store protobuf map
// fields.  Each Map instance may or may not use a different hash function, a
// different iteration order, and so on.  E.g., please don't examine
// implementation details to decide if the following would work:
//  Map<int, int> m0, m1;
//  m0[0] = m1[0] = m0[1] = m1[1] = 0;
//  assert(m0.begin()->first == m1.begin()->first);  // Bug!
//
// Map's interface is similar to std::unordered_map, except that Map is not
// designed to play well with exceptions.
template <typename Key, typename T>
class Map {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = MapPair<Key, T>;

  using pointer = value_type*;
  using const_pointer = const value_type*;
  using reference = value_type&;
  using const_reference = const value_type&;

  using size_type = size_t;
  using hasher = std::hash<Key>;

  Map() : arena_(nullptr), default_enum_value_(0) { Init(); }
  explicit Map(Arena* arena) : arena_(arena), default_enum_value_(0) { Init(); }

  Map(const Map& other)
      : arena_(nullptr), default_enum_value_(other.default_enum_value_) {
    Init();
    insert(other.begin(), other.end());
  }

  Map(Map&& other) noexcept : Map() {
    if (other.arena_) {
      *this = other;
    } else {
      swap(other);
    }
  }
  Map& operator=(Map&& other) noexcept {
    if (this != &other) {
      if (arena_ != other.arena_) {
        *this = other;
      } else {
        swap(other);
      }
    }
    return *this;
  }

  template <class InputIt>
  Map(const InputIt& first, const InputIt& last)
      : arena_(nullptr), default_enum_value_(0) {
    Init();
    insert(first, last);
  }

  ~Map() {
    clear();
    if (arena_ == nullptr) {
      delete elements_;
    }
  }

 private:
  void Init() { elements_ = Arena::CreateMessage<InnerMap>(arena_, 0); }

  using Allocator = internal::MapAllocator<void*>;

  // InnerMap is a generic hash-based map.  It doesn't contain any
  // protocol-buffer-specific logic.  It is a chaining hash map with the
  // additional feature that some buckets can be converted to use an ordered
  // container.  This ensures O(lg n) bounds on find, insert, and erase, while
  // avoiding the overheads of ordered containers most of the time.
  //
  // The implementation doesn't need the full generality of unordered_map,
  // and it doesn't have it.  More bells and whistles can be added as needed.
  // Some implementation details:
  // 1. The hash function has type hasher and the equality function
  //    equal_to<Key>.  We inherit from hasher to save space
  //    (empty-base-class optimization).
  // 2. The number of buckets is a power of two.
  // 3. Buckets are converted to trees in pairs: if we convert bucket b then
  //    buckets b and b^1 will share a tree.  Invariant: buckets b and b^1 have
  //    the same non-null value iff they are sharing a tree.  (An alternative
  //    implementation strategy would be to have a tag bit per bucket.)
  // 4. As is typical for hash_map and such, the Keys and Values are always
  //    stored in linked list nodes.  Pointers to elements are never invalidated
  //    until the element is deleted.
  // 5. The trees' payload type is pointer to linked-list node.  Tree-converting
  //    a bucket doesn't copy Key-Value pairs.
  // 6. Once we've tree-converted a bucket, it is never converted back. However,
  //    the items a tree contains may wind up assigned to trees or lists upon a
  //    rehash.
  // 7. The code requires no C++ features from C++14 or later.
  // 8. Mutations to a map do not invalidate the map's iterators, pointers to
  //    elements, or references to elements.
  // 9. Except for erase(iterator), any non-const method can reorder iterators.
  // 10. InnerMap uses KeyForTree<Key> when using the Tree representation, which
  //    is either `Key`, if Key is a scalar, or `reference_wrapper<const Key>`
  //    otherwise. This avoids unncessary copies of string keys, for example.
  class InnerMap : private hasher {
   public:
    explicit InnerMap(size_type n) : InnerMap(nullptr, n) {}
    InnerMap(Arena* arena, size_type n)
        : hasher(),
          num_elements_(0),
          seed_(Seed()),
          table_(nullptr),
          alloc_(arena) {
      n = TableSize(n);
      table_ = CreateEmptyTable(n);
      num_buckets_ = index_of_first_non_null_ = n;
    }

    ~InnerMap() {
      if (table_ != nullptr) {
        clear();
        Dealloc<void*>(table_, num_buckets_);
      }
    }

   private:
    enum { kMinTableSize = 8 };

    // Linked-list nodes, as one would expect for a chaining hash table.
    struct Node {
      value_type kv;
      Node* next;
    };

    // Trees. The payload type is a copy of Key, so that we can query the tree
    // with Keys that are not in any particular data structure.
    // The value is a void* pointing to Node. We use void* instead of Node* to
    // avoid code bloat. That way there is only one instantiation of the tree
    // class per key type.
    using TreeAllocator = typename Allocator::template rebind<
        std::pair<const internal::KeyForTree<Key>, void*>>::other;
    using Tree = std::map<internal::KeyForTree<Key>, void*, std::less<Key>,
                          TreeAllocator>;
    using TreeIterator = typename Tree::iterator;

    static Node* NodeFromTreeIterator(TreeIterator it) {
      return static_cast<Node*>(it->second);
    }

    // iterator and const_iterator are instantiations of iterator_base.
    template <typename KeyValueType>
    class iterator_base {
     public:
      using reference = KeyValueType&;
      using pointer = KeyValueType*;

      // Invariants:
      // node_ is always correct. This is handy because the most common
      // operations are operator* and operator-> and they only use node_.
      // When node_ is set to a non-null value, all the other non-const fields
      // are updated to be correct also, but those fields can become stale
      // if the underlying map is modified.  When those fields are needed they
      // are rechecked, and updated if necessary.
      iterator_base() : node_(nullptr), m_(nullptr), bucket_index_(0) {}

      explicit iterator_base(const InnerMap* m) : m_(m) {
        SearchFrom(m->index_of_first_non_null_);
      }

      // Any iterator_base can convert to any other.  This is overkill, and we
      // rely on the enclosing class to use it wisely.  The standard "iterator
      // can convert to const_iterator" is OK but the reverse direction is not.
      template <typename U>
      explicit iterator_base(const iterator_base<U>& it)
          : node_(it.node_), m_(it.m_), bucket_index_(it.bucket_index_) {}

      iterator_base(Node* n, const InnerMap* m, size_type index)
          : node_(n), m_(m), bucket_index_(index) {}

      iterator_base(TreeIterator tree_it, const InnerMap* m, size_type index)
          : node_(NodeFromTreeIterator(tree_it)), m_(m), bucket_index_(index) {
        // Invariant: iterators that use buckets with trees have an even
        // bucket_index_.
        GOOGLE_DCHECK_EQ(bucket_index_ % 2, 0u);
      }

      // Advance through buckets, looking for the first that isn't empty.
      // If nothing non-empty is found then leave node_ == nullptr.
      void SearchFrom(size_type start_bucket) {
        GOOGLE_DCHECK(m_->index_of_first_non_null_ == m_->num_buckets_ ||
               m_->table_[m_->index_of_first_non_null_] != nullptr);
        node_ = nullptr;
        for (bucket_index_ = start_bucket; bucket_index_ < m_->num_buckets_;
             bucket_index_++) {
          if (m_->TableEntryIsNonEmptyList(bucket_index_)) {
            node_ = static_cast<Node*>(m_->table_[bucket_index_]);
            break;
          } else if (m_->TableEntryIsTree(bucket_index_)) {
            Tree* tree = static_cast<Tree*>(m_->table_[bucket_index_]);
            GOOGLE_DCHECK(!tree->empty());
            node_ = NodeFromTreeIterator(tree->begin());
            break;
          }
        }
      }

      reference operator*() const { return node_->kv; }
      pointer operator->() const { return &(operator*()); }

      friend bool operator==(const iterator_base& a, const iterator_base& b) {
        return a.node_ == b.node_;
      }
      friend bool operator!=(const iterator_base& a, const iterator_base& b) {
        return a.node_ != b.node_;
      }

      iterator_base& operator++() {
        if (node_->next == nullptr) {
          TreeIterator tree_it;
          const bool is_list = revalidate_if_necessary(&tree_it);
          if (is_list) {
            SearchFrom(bucket_index_ + 1);
          } else {
            GOOGLE_DCHECK_EQ(bucket_index_ & 1, 0u);
            Tree* tree = static_cast<Tree*>(m_->table_[bucket_index_]);
            if (++tree_it == tree->end()) {
              SearchFrom(bucket_index_ + 2);
            } else {
              node_ = NodeFromTreeIterator(tree_it);
            }
          }
        } else {
          node_ = node_->next;
        }
        return *this;
      }

      iterator_base operator++(int /* unused */) {
        iterator_base tmp = *this;
        ++*this;
        return tmp;
      }

      // Assumes node_ and m_ are correct and non-null, but other fields may be
      // stale.  Fix them as needed.  Then return true iff node_ points to a
      // Node in a list.  If false is returned then *it is modified to be
      // a valid iterator for node_.
      bool revalidate_if_necessary(TreeIterator* it) {
        GOOGLE_DCHECK(node_ != nullptr && m_ != nullptr);
        // Force bucket_index_ to be in range.
        bucket_index_ &= (m_->num_buckets_ - 1);
        // Common case: the bucket we think is relevant points to node_.
        if (m_->table_[bucket_index_] == static_cast<void*>(node_)) return true;
        // Less common: the bucket is a linked list with node_ somewhere in it,
        // but not at the head.
        if (m_->TableEntryIsNonEmptyList(bucket_index_)) {
          Node* l = static_cast<Node*>(m_->table_[bucket_index_]);
          while ((l = l->next) != nullptr) {
            if (l == node_) {
              return true;
            }
          }
        }
        // Well, bucket_index_ still might be correct, but probably
        // not.  Revalidate just to be sure.  This case is rare enough that we
        // don't worry about potential optimizations, such as having a custom
        // find-like method that compares Node* instead of the key.
        iterator_base i(m_->find(node_->kv.first, it));
        bucket_index_ = i.bucket_index_;
        return m_->TableEntryIsList(bucket_index_);
      }

      Node* node_;
      const InnerMap* m_;
      size_type bucket_index_;
    };

   public:
    using iterator = iterator_base<value_type>;
    using const_iterator = iterator_base<const value_type>;

    iterator begin() { return iterator(this); }
    iterator end() { return iterator(); }
    const_iterator begin() const { return const_iterator(this); }
    const_iterator end() const { return const_iterator(); }

    void clear() {
      for (size_type b = 0; b < num_buckets_; b++) {
        if (TableEntryIsNonEmptyList(b)) {
          Node* node = static_cast<Node*>(table_[b]);
          table_[b] = nullptr;
          do {
            Node* next = node->next;
            DestroyNode(node);
            node = next;
          } while (node != nullptr);
        } else if (TableEntryIsTree(b)) {
          Tree* tree = static_cast<Tree*>(table_[b]);
          GOOGLE_DCHECK(table_[b] == table_[b + 1] && (b & 1) == 0);
          table_[b] = table_[b + 1] = nullptr;
          typename Tree::iterator tree_it = tree->begin();
          do {
            Node* node = NodeFromTreeIterator(tree_it);
            typename Tree::iterator next = tree_it;
            ++next;
            tree->erase(tree_it);
            DestroyNode(node);
            tree_it = next;
          } while (tree_it != tree->end());
          DestroyTree(tree);
          b++;
        }
      }
      num_elements_ = 0;
      index_of_first_non_null_ = num_buckets_;
    }

    const hasher& hash_function() const { return *this; }

    static size_type max_size() {
      return static_cast<size_type>(1) << (sizeof(void**) >= 8 ? 60 : 28);
    }
    size_type size() const { return num_elements_; }
    bool empty() const { return size() == 0; }

    iterator find(const Key& k) { return iterator(FindHelper(k).first); }
    const_iterator find(const Key& k) const { return find(k, nullptr); }
    bool contains(const Key& k) const { return find(k) != end(); }

    // Insert the key into the map, if not present. In that case, the value will
    // be value initialized.
    std::pair<iterator, bool> insert(const Key& k) {
      std::pair<const_iterator, size_type> p = FindHelper(k);
      // Case 1: key was already present.
      if (p.first.node_ != nullptr)
        return std::make_pair(iterator(p.first), false);
      // Case 2: insert.
      if (ResizeIfLoadIsOutOfRange(num_elements_ + 1)) {
        p = FindHelper(k);
      }
      const size_type b = p.second;  // bucket number
      Node* node;
      if (alloc_.arena() == nullptr) {
        node = new Node{value_type(k), nullptr};
      } else {
        node = Alloc<Node>(1);
        Arena::CreateInArenaStorage(const_cast<Key*>(&node->kv.first),
                                    alloc_.arena(), k);
        Arena::CreateInArenaStorage(&node->kv.second, alloc_.arena());
      }

      iterator result = InsertUnique(b, node);
      ++num_elements_;
      return std::make_pair(result, true);
    }

    value_type& operator[](const Key& k) { return *insert(k).first; }

    void erase(iterator it) {
      GOOGLE_DCHECK_EQ(it.m_, this);
      typename Tree::iterator tree_it;
      const bool is_list = it.revalidate_if_necessary(&tree_it);
      size_type b = it.bucket_index_;
      Node* const item = it.node_;
      if (is_list) {
        GOOGLE_DCHECK(TableEntryIsNonEmptyList(b));
        Node* head = static_cast<Node*>(table_[b]);
        head = EraseFromLinkedList(item, head);
        table_[b] = static_cast<void*>(head);
      } else {
        GOOGLE_DCHECK(TableEntryIsTree(b));
        Tree* tree = static_cast<Tree*>(table_[b]);
        tree->erase(tree_it);
        if (tree->empty()) {
          // Force b to be the minimum of b and b ^ 1.  This is important
          // only because we want index_of_first_non_null_ to be correct.
          b &= ~static_cast<size_type>(1);
          DestroyTree(tree);
          table_[b] = table_[b + 1] = nullptr;
        }
      }
      DestroyNode(item);
      --num_elements_;
      if (PROTOBUF_PREDICT_FALSE(b == index_of_first_non_null_)) {
        while (index_of_first_non_null_ < num_buckets_ &&
               table_[index_of_first_non_null_] == nullptr) {
          ++index_of_first_non_null_;
        }
      }
    }

   private:
    const_iterator find(const Key& k, TreeIterator* it) const {
      return FindHelper(k, it).first;
    }
    std::pair<const_iterator, size_type> FindHelper(const Key& k) const {
      return FindHelper(k, nullptr);
    }
    std::pair<const_iterator, size_type> FindHelper(const Key& k,
                                                    TreeIterator* it) const {
      size_type b = BucketNumber(k);
      if (TableEntryIsNonEmptyList(b)) {
        Node* node = static_cast<Node*>(table_[b]);
        do {
          if (IsMatch(node->kv.first, k)) {
            return std::make_pair(const_iterator(node, this, b), b);
          } else {
            node = node->next;
          }
        } while (node != nullptr);
      } else if (TableEntryIsTree(b)) {
        GOOGLE_DCHECK_EQ(table_[b], table_[b ^ 1]);
        b &= ~static_cast<size_t>(1);
        Tree* tree = static_cast<Tree*>(table_[b]);
        auto tree_it = tree->find(k);
        if (tree_it != tree->end()) {
          if (it != nullptr) *it = tree_it;
          return std::make_pair(const_iterator(tree_it, this, b), b);
        }
      }
      return std::make_pair(end(), b);
    }

    // Insert the given Node in bucket b.  If that would make bucket b too big,
    // and bucket b is not a tree, create a tree for buckets b and b^1 to share.
    // Requires count(*KeyPtrFromNodePtr(node)) == 0 and that b is the correct
    // bucket.  num_elements_ is not modified.
    iterator InsertUnique(size_type b, Node* node) {
      GOOGLE_DCHECK(index_of_first_non_null_ == num_buckets_ ||
             table_[index_of_first_non_null_] != nullptr);
      // In practice, the code that led to this point may have already
      // determined whether we are inserting into an empty list, a short list,
      // or whatever.  But it's probably cheap enough to recompute that here;
      // it's likely that we're inserting into an empty or short list.
      iterator result;
      GOOGLE_DCHECK(find(node->kv.first) == end());
      if (TableEntryIsEmpty(b)) {
        result = InsertUniqueInList(b, node);
      } else if (TableEntryIsNonEmptyList(b)) {
        if (PROTOBUF_PREDICT_FALSE(TableEntryIsTooLong(b))) {
          TreeConvert(b);
          result = InsertUniqueInTree(b, node);
          GOOGLE_DCHECK_EQ(result.bucket_index_, b & ~static_cast<size_type>(1));
        } else {
          // Insert into a pre-existing list.  This case cannot modify
          // index_of_first_non_null_, so we skip the code to update it.
          return InsertUniqueInList(b, node);
        }
      } else {
        // Insert into a pre-existing tree.  This case cannot modify
        // index_of_first_non_null_, so we skip the code to update it.
        return InsertUniqueInTree(b, node);
      }
      // parentheses around (std::min) prevents macro expansion of min(...)
      index_of_first_non_null_ =
          (std::min)(index_of_first_non_null_, result.bucket_index_);
      return result;
    }

    // Helper for InsertUnique.  Handles the case where bucket b is a
    // not-too-long linked list.
    iterator InsertUniqueInList(size_type b, Node* node) {
      node->next = static_cast<Node*>(table_[b]);
      table_[b] = static_cast<void*>(node);
      return iterator(node, this, b);
    }

    // Helper for InsertUnique.  Handles the case where bucket b points to a
    // Tree.
    iterator InsertUniqueInTree(size_type b, Node* node) {
      GOOGLE_DCHECK_EQ(table_[b], table_[b ^ 1]);
      // Maintain the invariant that node->next is null for all Nodes in Trees.
      node->next = nullptr;
      return iterator(
          static_cast<Tree*>(table_[b])->insert({node->kv.first, node}).first,
          this, b & ~static_cast<size_t>(1));
    }

    // Returns whether it did resize.  Currently this is only used when
    // num_elements_ increases, though it could be used in other situations.
    // It checks for load too low as well as load too high: because any number
    // of erases can occur between inserts, the load could be as low as 0 here.
    // Resizing to a lower size is not always helpful, but failing to do so can
    // destroy the expected big-O bounds for some operations. By having the
    // policy that sometimes we resize down as well as up, clients can easily
    // keep O(size()) = O(number of buckets) if they want that.
    bool ResizeIfLoadIsOutOfRange(size_type new_size) {
      const size_type kMaxMapLoadTimes16 = 12;  // controls RAM vs CPU tradeoff
      const size_type hi_cutoff = num_buckets_ * kMaxMapLoadTimes16 / 16;
      const size_type lo_cutoff = hi_cutoff / 4;
      // We don't care how many elements are in trees.  If a lot are,
      // we may resize even though there are many empty buckets.  In
      // practice, this seems fine.
      if (PROTOBUF_PREDICT_FALSE(new_size >= hi_cutoff)) {
        if (num_buckets_ <= max_size() / 2) {
          Resize(num_buckets_ * 2);
          return true;
        }
      } else if (PROTOBUF_PREDICT_FALSE(new_size <= lo_cutoff &&
                                        num_buckets_ > kMinTableSize)) {
        size_type lg2_of_size_reduction_factor = 1;
        // It's possible we want to shrink a lot here... size() could even be 0.
        // So, estimate how much to shrink by making sure we don't shrink so
        // much that we would need to grow the table after a few inserts.
        const size_type hypothetical_size = new_size * 5 / 4 + 1;
        while ((hypothetical_size << lg2_of_size_reduction_factor) <
               hi_cutoff) {
          ++lg2_of_size_reduction_factor;
        }
        size_type new_num_buckets = std::max<size_type>(
            kMinTableSize, num_buckets_ >> lg2_of_size_reduction_factor);
        if (new_num_buckets != num_buckets_) {
          Resize(new_num_buckets);
          return true;
        }
      }
      return false;
    }

    // Resize to the given number of buckets.
    void Resize(size_t new_num_buckets) {
      GOOGLE_DCHECK_GE(new_num_buckets, kMinTableSize);
      void** const old_table = table_;
      const size_type old_table_size = num_buckets_;
      num_buckets_ = new_num_buckets;
      table_ = CreateEmptyTable(num_buckets_);
      const size_type start = index_of_first_non_null_;
      index_of_first_non_null_ = num_buckets_;
      for (size_type i = start; i < old_table_size; i++) {
        if (TableEntryIsNonEmptyList(old_table, i)) {
          TransferList(old_table, i);
        } else if (TableEntryIsTree(old_table, i)) {
          TransferTree(old_table, i++);
        }
      }
      Dealloc<void*>(old_table, old_table_size);
    }

    void TransferList(void* const* table, size_type index) {
      Node* node = static_cast<Node*>(table[index]);
      do {
        Node* next = node->next;
        InsertUnique(BucketNumber(node->kv.first), node);
        node = next;
      } while (node != nullptr);
    }

    void TransferTree(void* const* table, size_type index) {
      Tree* tree = static_cast<Tree*>(table[index]);
      typename Tree::iterator tree_it = tree->begin();
      do {
        InsertUnique(BucketNumber(tree_it->first),
                     NodeFromTreeIterator(tree_it));
      } while (++tree_it != tree->end());
      DestroyTree(tree);
    }

    Node* EraseFromLinkedList(Node* item, Node* head) {
      if (head == item) {
        return head->next;
      } else {
        head->next = EraseFromLinkedList(item, head->next);
        return head;
      }
    }

    bool TableEntryIsEmpty(size_type b) const {
      return TableEntryIsEmpty(table_, b);
    }
    bool TableEntryIsNonEmptyList(size_type b) const {
      return TableEntryIsNonEmptyList(table_, b);
    }
    bool TableEntryIsTree(size_type b) const {
      return TableEntryIsTree(table_, b);
    }
    bool TableEntryIsList(size_type b) const {
      return TableEntryIsList(table_, b);
    }
    static bool TableEntryIsEmpty(void* const* table, size_type b) {
      return table[b] == nullptr;
    }
    static bool TableEntryIsNonEmptyList(void* const* table, size_type b) {
      return table[b] != nullptr && table[b] != table[b ^ 1];
    }
    static bool TableEntryIsTree(void* const* table, size_type b) {
      return !TableEntryIsEmpty(table, b) &&
             !TableEntryIsNonEmptyList(table, b);
    }
    static bool TableEntryIsList(void* const* table, size_type b) {
      return !TableEntryIsTree(table, b);
    }

    void TreeConvert(size_type b) {
      GOOGLE_DCHECK(!TableEntryIsTree(b) && !TableEntryIsTree(b ^ 1));
      Tree* tree =
          Arena::Create<Tree>(alloc_.arena(), typename Tree::key_compare(),
                              typename Tree::allocator_type(alloc_));
      size_type count = CopyListToTree(b, tree) + CopyListToTree(b ^ 1, tree);
      GOOGLE_DCHECK_EQ(count, tree->size());
      table_[b] = table_[b ^ 1] = static_cast<void*>(tree);
    }

    // Copy a linked list in the given bucket to a tree.
    // Returns the number of things it copied.
    size_type CopyListToTree(size_type b, Tree* tree) {
      size_type count = 0;
      Node* node = static_cast<Node*>(table_[b]);
      while (node != nullptr) {
        tree->insert({node->kv.first, node});
        ++count;
        Node* next = node->next;
        node->next = nullptr;
        node = next;
      }
      return count;
    }

    // Return whether table_[b] is a linked list that seems awfully long.
    // Requires table_[b] to point to a non-empty linked list.
    bool TableEntryIsTooLong(size_type b) {
      const size_type kMaxLength = 8;
      size_type count = 0;
      Node* node = static_cast<Node*>(table_[b]);
      do {
        ++count;
        node = node->next;
      } while (node != nullptr);
      // Invariant: no linked list ever is more than kMaxLength in length.
      GOOGLE_DCHECK_LE(count, kMaxLength);
      return count >= kMaxLength;
    }

    size_type BucketNumber(const Key& k) const {
      size_type h = hash_function()(k);
      return (h + seed_) & (num_buckets_ - 1);
    }

    bool IsMatch(const Key& k0, const Key& k1) const { return k0 == k1; }

    // Return a power of two no less than max(kMinTableSize, n).
    // Assumes either n < kMinTableSize or n is a power of two.
    size_type TableSize(size_type n) {
      return n < static_cast<size_type>(kMinTableSize)
                 ? static_cast<size_type>(kMinTableSize)
                 : n;
    }

    // Use alloc_ to allocate an array of n objects of type U.
    template <typename U>
    U* Alloc(size_type n) {
      using alloc_type = typename Allocator::template rebind<U>::other;
      return alloc_type(alloc_).allocate(n);
    }

    // Use alloc_ to deallocate an array of n objects of type U.
    template <typename U>
    void Dealloc(U* t, size_type n) {
      using alloc_type = typename Allocator::template rebind<U>::other;
      alloc_type(alloc_).deallocate(t, n);
    }

    void DestroyNode(Node* node) {
      if (alloc_.arena() == nullptr) {
        delete node;
      }
    }

    void DestroyTree(Tree* tree) {
      if (alloc_.arena() == nullptr) {
        delete tree;
      }
    }

    void** CreateEmptyTable(size_type n) {
      GOOGLE_DCHECK(n >= kMinTableSize);
      GOOGLE_DCHECK_EQ(n & (n - 1), 0);
      void** result = Alloc<void*>(n);
      memset(result, 0, n * sizeof(result[0]));
      return result;
    }

    // Return a randomish value.
    size_type Seed() const {
      size_type s = static_cast<size_type>(reinterpret_cast<uintptr_t>(this));
#if defined(__x86_64__) && defined(__GNUC__) && \
    !defined(GOOGLE_PROTOBUF_NO_RDTSC)
      uint32 hi, lo;
      asm("rdtsc" : "=a"(lo), "=d"(hi));
      s += ((static_cast<uint64>(hi) << 32) | lo);
#endif
      return s;
    }

    friend class Arena;
    using InternalArenaConstructable_ = void;
    using DestructorSkippable_ = void;

    size_type num_elements_;
    size_type num_buckets_;
    size_type seed_;
    size_type index_of_first_non_null_;
    void** table_;  // an array with num_buckets_ entries
    Allocator alloc_;
    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(InnerMap);
  };  // end of class InnerMap

 public:
  // Iterators
  class const_iterator {
    using InnerIt = typename InnerMap::const_iterator;

   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = typename Map::value_type;
    using difference_type = ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    const_iterator() {}
    explicit const_iterator(const InnerIt& it) : it_(it) {}

    const_reference operator*() const { return *it_; }
    const_pointer operator->() const { return &(operator*()); }

    const_iterator& operator++() {
      ++it_;
      return *this;
    }
    const_iterator operator++(int) { return const_iterator(it_++); }

    friend bool operator==(const const_iterator& a, const const_iterator& b) {
      return a.it_ == b.it_;
    }
    friend bool operator!=(const const_iterator& a, const const_iterator& b) {
      return !(a == b);
    }

   private:
    InnerIt it_;
  };

  class iterator {
    using InnerIt = typename InnerMap::iterator;

   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = typename Map::value_type;
    using difference_type = ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    iterator() {}
    explicit iterator(const InnerIt& it) : it_(it) {}

    reference operator*() const { return *it_; }
    pointer operator->() const { return &(operator*()); }

    iterator& operator++() {
      ++it_;
      return *this;
    }
    iterator operator++(int) { return iterator(it_++); }

    // Allow implicit conversion to const_iterator.
    operator const_iterator() const {  // NOLINT(runtime/explicit)
      return const_iterator(typename InnerMap::const_iterator(it_));
    }

    friend bool operator==(const iterator& a, const iterator& b) {
      return a.it_ == b.it_;
    }
    friend bool operator!=(const iterator& a, const iterator& b) {
      return !(a == b);
    }

   private:
    friend class Map;

    InnerIt it_;
  };

  iterator begin() { return iterator(elements_->begin()); }
  iterator end() { return iterator(elements_->end()); }
  const_iterator begin() const {
    return const_iterator(iterator(elements_->begin()));
  }
  const_iterator end() const {
    return const_iterator(iterator(elements_->end()));
  }
  const_iterator cbegin() const { return begin(); }
  const_iterator cend() const { return end(); }

  // Capacity
  size_type size() const { return elements_->size(); }
  bool empty() const { return size() == 0; }

  // Element access
  T& operator[](const key_type& key) { return (*elements_)[key].second; }
  const T& at(const key_type& key) const {
    const_iterator it = find(key);
    GOOGLE_CHECK(it != end()) << "key not found: " << key;
    return it->second;
  }
  T& at(const key_type& key) {
    iterator it = find(key);
    GOOGLE_CHECK(it != end()) << "key not found: " << key;
    return it->second;
  }

  // Lookup
  size_type count(const key_type& key) const {
    const_iterator it = find(key);
    GOOGLE_DCHECK(it == end() || key == it->first);
    return it == end() ? 0 : 1;
  }
  const_iterator find(const key_type& key) const {
    return const_iterator(iterator(elements_->find(key)));
  }
  iterator find(const key_type& key) { return iterator(elements_->find(key)); }
  bool contains(const Key& key) const { return elements_->contains(key); }
  std::pair<const_iterator, const_iterator> equal_range(
      const key_type& key) const {
    const_iterator it = find(key);
    if (it == end()) {
      return std::pair<const_iterator, const_iterator>(it, it);
    } else {
      const_iterator begin = it++;
      return std::pair<const_iterator, const_iterator>(begin, it);
    }
  }
  std::pair<iterator, iterator> equal_range(const key_type& key) {
    iterator it = find(key);
    if (it == end()) {
      return std::pair<iterator, iterator>(it, it);
    } else {
      iterator begin = it++;
      return std::pair<iterator, iterator>(begin, it);
    }
  }

  // insert
  std::pair<iterator, bool> insert(const value_type& value) {
    std::pair<typename InnerMap::iterator, bool> p =
        elements_->insert(value.first);
    if (p.second) {
      p.first->second = value.second;
    }
    return std::pair<iterator, bool>(iterator(p.first), p.second);
  }
  template <class InputIt>
  void insert(InputIt first, InputIt last) {
    for (InputIt it = first; it != last; ++it) {
      iterator exist_it = find(it->first);
      if (exist_it == end()) {
        operator[](it->first) = it->second;
      }
    }
  }
  void insert(std::initializer_list<value_type> values) {
    insert(values.begin(), values.end());
  }

  // Erase and clear
  size_type erase(const key_type& key) {
    iterator it = find(key);
    if (it == end()) {
      return 0;
    } else {
      erase(it);
      return 1;
    }
  }
  iterator erase(iterator pos) {
    iterator i = pos++;
    elements_->erase(i.it_);
    return pos;
  }
  void erase(iterator first, iterator last) {
    while (first != last) {
      first = erase(first);
    }
  }
  void clear() { erase(begin(), end()); }

  // Assign
  Map& operator=(const Map& other) {
    if (this != &other) {
      clear();
      insert(other.begin(), other.end());
    }
    return *this;
  }

  void swap(Map& other) {
    if (arena_ == other.arena_) {
      std::swap(default_enum_value_, other.default_enum_value_);
      std::swap(elements_, other.elements_);
    } else {
      // TODO(zuguang): optimize this. The temporary copy can be allocated
      // in the same arena as the other message, and the "other = copy" can
      // be replaced with the fast-path swap above.
      Map copy = *this;
      *this = other;
      other = copy;
    }
  }

  // Access to hasher.  Currently this returns a copy, but it may
  // be modified to return a const reference in the future.
  hasher hash_function() const { return elements_->hash_function(); }

 private:
  // Set default enum value only for proto2 map field whose value is enum type.
  void SetDefaultEnumValue(int default_enum_value) {
    default_enum_value_ = default_enum_value;
  }

  Arena* arena_;
  int default_enum_value_;
  InnerMap* elements_;

  friend class Arena;
  using InternalArenaConstructable_ = void;
  using DestructorSkippable_ = void;
  template <typename Derived, typename K, typename V,
            internal::WireFormatLite::FieldType key_wire_type,
            internal::WireFormatLite::FieldType value_wire_type,
            int default_enum_value>
  friend class internal::MapFieldLite;
};

PROTOBUF_NAMESPACE_CLOSE

#include <google/protobuf/port_undef.inc>

#endif  // GOOGLE_PROTOBUF_MAP_H__
