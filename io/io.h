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
        return t.io(ar, name);
      }
    };

    // Else call another fn called io_non_member
    template<>
    struct io_has_member<false> {
      template<typename AR, typename T>
      static bool io_call(AR& ar, T &t, const char* name) {
        return io_non_member(ar, t, name);
      }
    };
  }

}

// ----------------------------------------------------------------------------
// By default, calls are forwarded to the member fn if exists or to the io_non_member
template< typename AR, typename T >
bool io(AR& ar, T& t, const char* name) {
  return IO::detail::io_has_member<
    IO::detail::hasIO<AR, T>()
  >::io_call(ar, t, name);
}

#endif
