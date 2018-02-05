#ifndef INC_IO_IO_JSON_H_
#define INC_IO_IO_JSON_H_

#include "io.h"

namespace IO {

  // -------------------------------------------------
  class JsonWriter {
    struct TState {
      const char* name = "";
      bool inside_obj = false;
      bool inside_array = false;
      int  nitems = 0;
    };
    std::vector< TState > states;
  public:
    TState& bs() { return states.back(); }
    JsonWriter() {
      states.push_back(TState{});
    }
    void beginObj(const char* name) {
      indent(name);
      TState s;
      s.name = name;
      s.inside_obj = true;
      printf("{\n");
      states.push_back(s);
    }
    void endObj() {
      indent(nullptr);
      printf("}\n");
      states.pop_back();
    }
    void beginArray(const char* name) {
      indent(name);
      TState s;
      s.name = name;
      s.inside_array = true;
      printf("[\n");
      states.push_back(s);
    }
    void endArray() {
      indent(nullptr);
      printf("]\n");
      states.pop_back();
    }
    void indent(const char* title) {
      bs().nitems++;
      size_t n = states.size();
      if (bs().nitems > 1)
        n--;
      for (size_t i = 0; i < n; ++i)
        printf("  ");
      if (title) {
        if (bs().nitems > 1)
          printf(", ");
        if (bs().inside_obj ) {
          printf("\"%s\" : ", title);
        }
      }
    }
  };

  bool io_impl(JsonWriter& ar, int& t, const char* name) {
    ar.indent(name);
    printf("%d\n", t);
    return true;
  }
  bool io_impl(JsonWriter& ar, float& t, const char* name) {
    ar.indent(name);
    printf("%f\n", t);
    return true;
  }
  bool io_impl(JsonWriter& ar, bool& t, const char* name) {
    ar.indent(name);
    printf("%s\n", t ? "true" : "false");
    return true;
  }
  bool io_impl(JsonWriter& ar, std::string& t, const char* name) {
    ar.indent(name);
    printf("\"%s\"\n", t.c_str());
    return true;
  }

  // Serialization of all vectors
  template< typename T >
  bool io_impl(JsonWriter& ar, std::vector<T>& v, const char* name) {
    ar.beginArray(name);
    bool first = true;
    for (auto& item : v) {
      if (!io(ar, item, name))
        return false;
      first = false;
    }
    ar.endArray();
    return true;
  }

}

// ----------------------------------------------------------------------------
// By default, calls are forwarded to the member fn if exists or to the io_non_member
//template< typename T >
//bool io(IO::JsonWriter& ar, T& t, const char* name) {
//  printf("\"%s\" : {\n", name);
//  bool is_ok = IO::detail::io_has_member<
//    IO::detail::hasIO<IO::JsonWriter, T>()
//  >::io_call(ar, t, name);
//  printf("}\n");
//  return is_ok;
//}


#endif
