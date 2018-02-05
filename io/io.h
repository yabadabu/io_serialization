#ifndef INC_IO_IO_H_
#define INC_IO_IO_H_

#include <type_traits>

#define IOAuto(ar,n)    ::io(ar,n,#n)

namespace IO {

  // ----------------------------------------------------------------------------
  namespace detail {

    // To be used as template argument: hasIO<AR, T>()
    // returns true if T has a method called io receiving an AR& and const char arg
    template < 
        typename AR
      , typename T
      , bool result = std::is_same<decltype(((T*)nullptr)->io(*(AR*)nullptr, "string")), bool>::value>
    constexpr bool hasIO() {
      return result;
    }

    // All other forms/args of hasIOHelper will return false
    template <typename AR, typename T>
    constexpr bool hasIO(...) { return false; }

    // ----------------------------------------------------------------------------
    template<bool>
    struct io_has_member;

    // when is true, call the member function on the obj itself
    template<>
    struct io_has_member<true> {
      template<typename AR, typename T>
      static bool io_call(AR& ar, T &t, const char* name) {
        ar.beginObj(name);
        bool is_ok = t.io(ar, name);
        ar.endObj();
        return is_ok;
      }
    };

    // Else call another fn called io_non_member
    template<>
    struct io_has_member<false> {
      template<typename AR, typename T>
      static bool io_call(AR& ar, T &t, const char* name) {
        bool is_ok = io_impl(ar, t, name);
        return is_ok;
      }
    };
  }
}

//// https://jguegant.github.io/blogs/tech/sfinae-introduction.html
template<bool B, class T = void> // Default template version.
struct enable_if {};             // This struct doesn't define "type" and the substitution will fail if you try to access it.

template<class T>                // A specialisation used if the expression is true. 
struct enable_if<true, T> { typedef T type; }; // This struct do have a "type" and won't fail on access.

// ----------------------------------------------------------------------------
// By default, calls are forwarded to the member fn if exists or to the io_impl
template< typename AR, typename T >
bool io(AR& ar, T& t, const char* name) {

  bool is_ok = IO::detail::io_has_member<
    IO::detail::hasIO<AR, T>()
  >::io_call(ar, t, name);


  return is_ok;
}

#endif
