#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>   // std::random_access_iterator_tag
#include <cstddef>     // size_t
#include <stdexcept>   // std::out_of_range
#include <type_traits> // std::is_same
#include <vector>

template <class T> class Vector {
public:
  class iterator;

private:
  T *array;
  size_t _capacity, _size;

  // You may want to write a function that grows the vector
  void grow() { /* TODO */
  }

public:
  Vector() noexcept { /* TODO */
    array = new T[0];
    _capacity = 0;
    _size = 0;
  }

  Vector(size_t count, const T &value) { /* TODO */
    array = new T[count];
    _capacity = count;
    _size = count;
    int i = 0;
    int tempCap = _capacity;
    for (i = 0; i < tempCap; i++) {
      array[i] = value;
    }
  }
  explicit Vector(size_t count) { /* TODO */
    _capacity = count;
    array = new T[_capacity];
    _size = count;
    int tempCap = _capacity;
    for (int i = 0; i < tempCap; i++) {
      array[i] = T();
    }
  }
  Vector(const Vector &other) { /* TODO */
    int i = 0;
    array = new T[other.size()];
    _size = other.size();
    _capacity = _size;
    for (i = 0; i < other.size(); i++) {
      array[i] = other[i];
    }
  }
  Vector(Vector &&other) noexcept { /* TODO */
    _capacity = other.capacity();
    _size = other.size();
  }

  ~Vector() { /* TODO */
    delete[] array;
  }

  Vector &operator=(const Vector &other) { /* TODO */
    int i = 0;
    int newSize = other.size();
    if (other.size() <= _size) {
      for (i = 0; i < newSize; i++) {
        array[i] = other[i];
        _size = other.size();
        return *this;
      }
    }

    T *copyArray = new T[other.size()];

    int j = 0;
    for (j = 0; j < newSize; j++) {
      copyArray[j] = other[j];
    }

    delete[] array;
    _capacity = other.size();
    _size = other.size();
    array = copyArray;
    return *this;
  }
  Vector &operator=(Vector &&other) noexcept { /* TODO */
  }

  iterator begin() noexcept { /* TODO */
    return iterator(array);
  }
  iterator end() noexcept { /* TODO */
    return iterator(array + _size);
  }

  [[nodiscard]] bool empty() const noexcept {/* TODO */
                                             return _size == 0}

  size_t size() const noexcept { /* TODO */
    return _size;
  }
  size_t capacity() const noexcept { /* TODO */
    return _capacity;
  }

  T &at(size_t pos) { /* TODO */
    if (pos < 0 || pos >= _size) {
      throw std::out_of_range("Out of range error!");
    }

    return array[pos];
  }
  const T &at(size_t pos) const { /* TODO */
    if (pos < 0 || pos >= _size) {
      throw std::out_of_range("Out of range error!");
    }

    return array[pos];
  }
  T &operator[](size_t pos) { /* TODO */
    return array[pos];
  }
  const T &operator[](size_t pos) const { /* TODO */
    return array[pos];
  }
  T &front() { /* TODO */
    return array[0];
  }
  const T &front() const { /* TODO */
    return array[0];
  }
  T &back() { /* TODO */
    return array[_size - 1];
  }
  const T &back() const { /* TODO */
    return array[_size - 1];
  }

  void push_back(const T &value) { /* TODO */
    if (_size == _capacity) {
      _capacity *= 2;
      T *temp = new T[_capacity];
      for (int i = 0; i < _size; i++) {
        temp[i] = array[i];
      }
      delete[] array;
      array = temp;
    }
    insert(end(), value);

    // _capacity *= 2;
    // array[_size++] = std::move(value);
  }

  void push_back(T &&value) { /* TODO */
    if (_size == _capacity) {
      _capacity *= 2;
      T *temp = new T[_capacity];
      for (int i = 0; i < _size; i++) {
        temp[i] = array[i];
      }
      delete[] array;
      array = temp;
    }
    array[_size++] = std::move(value);
    // _capacity *= 2;
    // array[_size++] = std::move(value);
  }
  void pop_back() { /* TODO */
    if (_size > 0) {
      --_size;
    }
  }
  // goofy function
  void add(T &&value) {
    int *temp = new int[_capacity + 1];
    int tempCap = _capacity;
    for (int i = 0; i < tempCap; i++) {
      temp[i] = array[i];
    }
    temp[_capacity] = value;
    delete[] array;
    _capacity++;
  }

  iterator insert(iterator pos, const T &value) { /* TODO */
    auto diff = pos - begin();
    if (_size == _capacity) {
      _capacity *= 2;
      T *temp = new T[_capacity];
      for (int i = 0; i < _size; i++) {
        temp[i] = array[i];
      }
      delete[] array;
      array = temp;
    }
    for (int i = _size; i > diff; i--) {
      array[i] = array[i - 1];
    }
    array[diff] = value;
    _size++;
    return iterator(array + diff);
  }
  iterator insert(iterator pos, T &&value) { /* TODO */
    auto diff = pos - begin();
  }
  iterator insert(iterator pos, size_t count, const T &value) { /* TODO */
  }
  iterator erase(iterator pos) { /* TODO */
    auto diff = pos - begin();
    for (auto i = pos; i < _size - 1; i++) {
      array[i] = array[i + 1];
    }
    _size--;
    return iterator(array + pos);
  }
  iterator erase(iterator first, iterator last) { /* TODO */
  }

  class iterator {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T *;
    using reference = T &;

  private:
    // Add your own data members here
    pointer vptr;
    // HINT: For random_access_iterator, the data member is a pointer 99.9% of
    // the time
  public:
    iterator(pointer ptr) : vptr(ptr) { /* TODO */
    }
    // Add any constructors that you may need

    // This assignment operator is done for you, please do not add more
    iterator &operator=(const iterator &) noexcept = default;

    [[nodiscard]] reference operator*() const noexcept { /* TODO */
      return *vptr;
    }
    [[nodiscard]] pointer operator->() const noexcept { /* TODO */
      return vptr;
    }

    // Prefix Increment: ++a
    iterator &operator++() noexcept { /* TODO */
      ++vptr;
      return *this;
    }
    // Postfix Increment: a++
    iterator operator++(int) noexcept { /* TODO */
      iterator i;
      i.vptr = this->ptr;
      ++(*this);
      return *this;
    }
    // Prefix Decrement: --a
    iterator &operator--() noexcept { /* TODO */
      --(this->vptr);
      return *this;
    }
    // Postfix Decrement: a--
    iterator operator--(int) noexcept { /* TODO */
      auto i = *this;
      this->vptr--;
      return i;
    }

    iterator &operator+=(difference_type offset) noexcept { /* TODO */
      for (auto i = 0; i < offset; i++) {
        this->vptr++;
      }
      return *this;
    }
    [[nodiscard]] iterator
    operator+(difference_type offset) const noexcept { /* TODO */
      iterator it(this->vptr);
      for (auto i = 0; i < offset; i++) {
        it.vptr++;
      }
      return it;
    }

    iterator &operator-=(difference_type offset) noexcept { /* TODO */
      vptr -= offset;
      return *this;
    }
    [[nodiscard]] iterator
    operator-(difference_type offset) const noexcept { /* TODO */
      return iterator(vptr - offset);
    }
    [[nodiscard]] difference_type
    operator-(const iterator &rhs) const noexcept { /* TODO */
      return std::distance(rhs.vptr, this->vptr);
    }

    [[nodiscard]] reference
    operator[](difference_type offset) const noexcept { /* TODO */
      return *(*this + offset);
    }

    [[nodiscard]] bool
    operator==(const iterator &rhs) const noexcept { /* TODO */
      return vptr == rhs.vptr;
    }
    [[nodiscard]] bool
    operator!=(const iterator &rhs) const noexcept { /* TODO */
      return vptr != rhs.vptr;
    }
    [[nodiscard]] bool
    operator<(const iterator &rhs) const noexcept { /* TODO */
      return vptr < rhs.vptr;
    }
    [[nodiscard]] bool
    operator>(const iterator &rhs) const noexcept { /* TODO */
      return vptr > rhs.vptr;
    }
    [[nodiscard]] bool
    operator<=(const iterator &rhs) const noexcept { /* TODO */
      return vptr <= rhs.vptr;
    }
    [[nodiscard]] bool
    operator>=(const iterator &rhs) const noexcept { /* TODO */
      return vptr >= rhs.vptr;
    }
  };

  void clear() noexcept { /* TODO */
    _size = 0;
    delete[] array;
    array = new T[this->_capacity];
  }
};

// This ensures at compile time that the deduced argument _Iterator is a
// Vector<T>::iterator There is no way we know of to back-substitute template
// <typename T> for external functions because it leads to a non-deduced context
namespace {
template <typename _Iterator>
using is_vector_iterator =
    std::is_same<typename Vector<typename _Iterator::value_type>::iterator,
                 _Iterator>;
}

template <typename _Iterator,
          bool _enable = is_vector_iterator<_Iterator>::value>
[[nodiscard]] _Iterator
operator+(typename _Iterator::difference_type offset,
          _Iterator const &iterator) noexcept { /* TODO */
}

#endif
