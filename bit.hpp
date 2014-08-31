#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <utility>

namespace bit
{
  template <typename T>
  inline constexpr
  T log2_(const T remainder, T exponent) noexcept
  {
    return remainder <= T{1}
      ? T{0}
      : T{1} + log2_(remainder / T{2}, exponent + T{1});
  }

  template <typename T>
  inline constexpr
  T log2(const T value) noexcept
  {
    return log2_(value, T{0});
  }

  template <typename T>
  inline constexpr
  T width() noexcept
  {
    return log2(std::numeric_limits<T>::max() &
              ~(std::numeric_limits<T>::max() >> 1));
  }

  template <typename T>
  inline constexpr
  T mask(const unsigned int start, const unsigned int end) noexcept
  {
    return ((T{1} << ((end - start) + 1)) - 1) << start;
  }

  template <typename T>
  inline constexpr
  T pattern_(const unsigned int offset) noexcept
  {
    return offset > width<T>()
      ? 0
      : offset == width<T>()
        ? (T{1} << offset)
        : (T{1} << offset) | pattern_<T>(offset + 2);
  }

  template <typename T>
  inline constexpr
  T even_pattern() noexcept
  {
    return pattern_<T>(1);
  }

  template <typename T>
  inline constexpr
  T odd_pattern() noexcept
  {
    return pattern_<T>(0);
  }

  template <typename T>
  class proxy
  {
  public:
    constexpr proxy(T& data, unsigned int index) noexcept
      : data_(data), index_(index) {}

    constexpr operator bool() noexcept
    {
      return data_.get() & (1 << index_);
    }

    constexpr proxy& operator=(const bool bit) noexcept
    {
      data_ = bit ? (data_.get() |  (1 << index_))
                  : (data_.get() & ~(1 << index_));
      return *this;
    }

  private:
    std::reference_wrapper<T> data_;
    unsigned int              index_;
  };

  template <typename T>
  class iterator
  {
  public:
    using difference_type   = int;
    using value_type        = bool;
    using pointer           = proxy<T>*;
    using reference         = proxy<T>;
    using iterator_category = std::bidirectional_iterator_tag;

    static iterator begin(T& data)
    {
      return iterator{data, 0};
    }

    static iterator end(T& data)
    {
      return iterator{data, width<T>()};
    }

    friend void swap(iterator& i1, iterator& i2) noexcept
    {
      using std::swap;
      swap(i1.data_,  i2.data_);
      swap(i1.index_, i2.index_);
    }

    constexpr reference operator*() noexcept
    {
      return proxy<T>{data_, index_};
    }

    constexpr iterator& operator++() noexcept
    {
      ++index_;
      return *this;
    }

    constexpr iterator operator++(const int) noexcept
    {
      iterator i{*this};
      ++index_;
      return i;
    }

    constexpr iterator& operator--() noexcept
    {
      --index_;
      return *this;
    }

    constexpr iterator operator--(const int) noexcept
    {
      iterator i{*this};
      --index_;
      return i;
    }

    constexpr bool operator==(const iterator& other) noexcept
    {
      return (std::addressof(data_.get()) == std::addressof(other.data_.get())) &&
             (index_ == other.index_);
    }

    constexpr bool operator!=(const iterator& other) noexcept
    {
      return !operator==(other);
    }

  private:
    constexpr iterator(T& data, unsigned int index) noexcept
      : data_(data), index_(index) {}

    std::reference_wrapper<T> data_;
    unsigned int              index_;
  };

  template <typename T>
  class reverse_iterator
  {
  public:
    using difference_type   = int;
    using value_type        = bool;
    using pointer           = proxy<T>*;
    using reference         = proxy<T>;
    using iterator_category = std::bidirectional_iterator_tag;

    static reverse_iterator begin(T& data)
    {
      return reverse_iterator{data, width<T>() - 1};
    }

    static reverse_iterator end(T& data)
    {
      return reverse_iterator{data, std::numeric_limits<unsigned int>::max()};
    }

    friend void swap(reverse_iterator& i1, reverse_iterator& i2) noexcept
    {
      using std::swap;
      swap(i1.data_,  i2.data_);
      swap(i1.index_, i2.index_);
    }

    constexpr reference operator*() noexcept
    {
      return proxy<T>{data_, index_};
    }

    constexpr reverse_iterator& operator++() noexcept
    {
      --index_;
      return *this;
    }

    constexpr reverse_iterator operator++(const int) noexcept
    {
      reverse_iterator i{*this};
      --index_;
      return i;
    }

    constexpr reverse_iterator& operator--() noexcept
    {
      ++index_;
      return *this;
    }

    constexpr reverse_iterator operator--(const int) noexcept
    {
      reverse_iterator i{*this};
      ++index_;
      return i;
    }

    constexpr bool operator==(const reverse_iterator& other) noexcept
    {
      return (std::addressof(data_.get()) == std::addressof(other.data_.get())) &&
             (index_ == other.index_);
    }

    constexpr bool operator!=(const reverse_iterator& other) noexcept
    {
      return !operator==(other);
    }

  private:
    constexpr reverse_iterator(T& data, unsigned int index) noexcept
      : data_(data), index_(index) {}

    std::reference_wrapper<T> data_;
    unsigned int              index_;
  };

  template <typename T>
  class const_proxy
  {
  public:
    constexpr const_proxy(const T& data, unsigned int index) noexcept
      : data_(data), index_(index) {}

    constexpr operator bool() const noexcept
    {
      return data_.get() & (1 << index_);
    }

  private:
    std::reference_wrapper<const T> data_;
    unsigned int                    index_;
  };

  template <typename T>
  class const_iterator
  {
  public:
    using difference_type   = int;
    using value_type        = bool;
    using pointer           = const_proxy<T>*;
    using reference         = const_proxy<T>;
    using iterator_category = std::bidirectional_iterator_tag;

    static const_iterator begin(const T& data)
    {
      return const_iterator{data, 0};
    }

    static const_iterator end(const T& data)
    {
      return const_iterator{data, width<T>()};
    }

    friend void swap(const_iterator& i1, const_iterator& i2) noexcept
    {
      using std::swap;
      swap(i1.data_,  i2.data_);
      swap(i1.index_, i2.index_);
    }

    constexpr reference operator*() const noexcept
    {
      return const_proxy<T>{data_, index_};
    }

    constexpr const_iterator& operator++() noexcept
    {
      ++index_;
      return *this;
    }

    constexpr const_iterator operator++(const int) noexcept
    {
      const_iterator i{*this};
      ++index_;
      return i;
    }

    constexpr const_iterator& operator--() noexcept
    {
      --index_;
      return *this;
    }

    constexpr const_iterator operator--(const int) noexcept
    {
      const_iterator i{*this};
      --index_;
      return i;
    }

    constexpr bool operator==(const const_iterator& other) noexcept
    {
      return (std::addressof(data_.get()) == std::addressof(other.data_.get())) &&
             (index_ == other.index_);
    }

    constexpr bool operator!=(const const_iterator& other) noexcept
    {
      return !operator==(other);
    }

  private:
    constexpr const_iterator(const T& data, unsigned int index) noexcept
      : data_(data), index_(index) {}

    std::reference_wrapper<const T> data_;
    unsigned int                    index_;
  };

  template <typename T>
  class const_reverse_iterator
  {
  public:
    using difference_type   = int;
    using value_type        = bool;
    using pointer           = const_proxy<T>*;
    using reference         = const_proxy<T>;
    using iterator_category = std::bidirectional_iterator_tag;

    static const_reverse_iterator begin(const T& data)
    {
      return const_reverse_iterator{data, width<T>() - 1};
    }

    static const_reverse_iterator end(const T& data)
    {
      return const_reverse_iterator{data, std::numeric_limits<unsigned int>::max()};
    }

    friend void swap(const_reverse_iterator& i1, const_reverse_iterator& i2) noexcept
    {
      using std::swap;
      swap(i1.data_,  i2.data_);
      swap(i1.index_, i2.index_);
    }

    constexpr reference operator*() const noexcept
    {
      return const_proxy<T>{data_, index_};
    }

    constexpr const_reverse_iterator& operator++() noexcept
    {
      --index_;
      return *this;
    }

    constexpr const_reverse_iterator operator++(const int) noexcept
    {
      const_reverse_iterator i{*this};
      --index_;
      return i;
    }

    constexpr const_reverse_iterator& operator--() noexcept
    {
      ++index_;
      return *this;
    }

    constexpr const_reverse_iterator operator--(const int) noexcept
    {
      const_reverse_iterator i{*this};
      ++index_;
      return i;
    }

    constexpr bool operator==(const const_reverse_iterator& other) noexcept
    {
      return (std::addressof(data_.get()) == std::addressof(other.data_.get())) &&
             (index_ == other.index_);
    }

    constexpr bool operator!=(const const_reverse_iterator& other) noexcept
    {
      return !operator==(other);
    }

  private:
    constexpr const_reverse_iterator(const T& data, unsigned int index) noexcept
      : data_(data), index_(index) {}

    std::reference_wrapper<const T> data_;
    unsigned int                    index_;
  };

  template <typename T> inline constexpr auto begin  (      T& data) { return iterator<T>::begin(data);               }
  template <typename T> inline constexpr auto begin  (const T& data) { return const_iterator<T>::begin(data);         }
  template <typename T> inline constexpr auto cbegin (const T& data) { return const_iterator<T>::begin(data);         }
  template <typename T> inline constexpr auto end    (      T& data) { return iterator<T>::end(data);                 }
  template <typename T> inline constexpr auto end    (const T& data) { return const_iterator<T>::end(data);           }
  template <typename T> inline constexpr auto cend   (const T& data) { return const_iterator<T>::end(data);           }
  template <typename T> inline constexpr auto rbegin (      T& data) { return reverse_iterator<T>::begin(data);       }
  template <typename T> inline constexpr auto rbegin (const T& data) { return const_reverse_iterator<T>::begin(data); }
  template <typename T> inline constexpr auto crbegin(const T& data) { return const_reverse_iterator<T>::begin(data); }
  template <typename T> inline constexpr auto rend   (      T& data) { return reverse_iterator<T>::end(data);         }
  template <typename T> inline constexpr auto rend   (const T& data) { return const_reverse_iterator<T>::end(data);   }
  template <typename T> inline constexpr auto crend  (const T& data) { return const_reverse_iterator<T>::end(data);   }

  template <typename T>
  class const_range
  {
  public:
    constexpr const_range(const T& data) noexcept : data_(data) {}

    constexpr auto begin()   const noexcept { return bit::cbegin(data_.get());  }
    constexpr auto cbegin()  const noexcept { return bit::cbegin(data_.get());  }

    constexpr auto end()     const noexcept { return bit::cend(data_.get());    }
    constexpr auto cend()    const noexcept { return bit::cend(data_.get());    }

    constexpr auto rbegin()  const noexcept { return bit::crbegin(data_.get()); }
    constexpr auto crbegin() const noexcept { return bit::crbegin(data_.get()); }

    constexpr auto rend()    const noexcept { return bit::crend(data_.get());   }
    constexpr auto crend()   const noexcept { return bit::crend(data_.get());   }

  protected:
    std::reference_wrapper<T> data_;
  };

  template <typename T>
  class const_reverse_range
  {
  public:
    constexpr const_reverse_range(const T& data) noexcept : data_(data) {}

    constexpr auto begin()   const noexcept { return bit::crbegin(data_.get()); }
    constexpr auto cbegin()  const noexcept { return bit::crbegin(data_.get()); }

    constexpr auto end()     const noexcept { return bit::crend(data_.get());   }
    constexpr auto cend()    const noexcept { return bit::crend(data_.get());   }

    constexpr auto rbegin()  const noexcept { return bit::cbegin(data_.get());  }
    constexpr auto crbegin() const noexcept { return bit::cbegin(data_.get());  }

    constexpr auto rend()    const noexcept { return bit::cend(data_.get());    }
    constexpr auto crend()   const noexcept { return bit::cend(data_.get());    }

  protected:
    std::reference_wrapper<const T> data_;
  };

  template <typename T>
  class range : public const_range<const T>
  {
  public:
    constexpr range(T& data) noexcept : const_range<const T>(data) {}

    constexpr auto begin()  noexcept { return bit::begin (const_cast<T&>(const_range<const T>::data_.get())); }
    constexpr auto end()    noexcept { return bit::end   (const_cast<T&>(const_range<const T>::data_.get())); }
    constexpr auto rbegin() noexcept { return bit::rbegin(const_cast<T&>(const_range<const T>::data_.get())); }
    constexpr auto rend()   noexcept { return bit::rend  (const_cast<T&>(const_range<const T>::data_.get())); }
  };

  template <typename T>
  class reverse_range : public const_reverse_range<const T>
  {
  public:
    constexpr reverse_range(T& data) noexcept : const_reverse_range<const T>(data) {}

    constexpr auto begin()  noexcept { return bit::rbegin(const_cast<T&>(const_reverse_range<const T>::data_.get())); }
    constexpr auto end()    noexcept { return bit::rend  (const_cast<T&>(const_reverse_range<const T>::data_.get())); }
    constexpr auto rbegin() noexcept { return bit::begin (const_cast<T&>(const_reverse_range<const T>::data_.get())); }
    constexpr auto rend()   noexcept { return bit::end   (const_cast<T&>(const_reverse_range<const T>::data_.get())); }
  };

  template <typename T>
  inline constexpr
  auto make_range(T& data) noexcept
  {
    return range<T>(data);
  }

  template <typename T>
  inline constexpr
  auto make_range(const T& data) noexcept
  {
    return const_range<T>{data};
  }

  template <typename T>
  inline constexpr
  auto make_reverse_range(T& data) noexcept
  {
    return reverse_range<T>{data};
  }

  template <typename T>
  inline constexpr
  auto make_reverse_range(const T& data) noexcept
  {
    return const_reverse_range<T>{data};
  }

  template <typename T>
  inline constexpr
  unsigned int count(const T data) noexcept
  {
    return std::count(cbegin(data), cend(data), true);
  }
}
