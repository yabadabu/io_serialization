#ifndef INC_IO_IO_IMGUI_H_
#define INC_IO_IO_IMGUI_H_

#include "io.h"

namespace IO {

  // -------------------------------------------------
  class IOImGUi {
  };

  bool io_impl(IOImGUi& ar, int& t, const char* name) {
    printf("ImGUI::int  %s -> ( %d ) \n", name, t);
    return true;
  }
  bool io_impl(IOImGUi& ar, float& t, const char* name) {
    printf("ImGUI::float  %s -> ( %f ) \n", name, t);
    return true;
  }
  bool io_impl(IOImGUi& ar, bool& t, const char* name) {
    printf("ImGUI::bool  %s -> ( %s ) \n", name, t ? "yes" : "no");
    return true;
  }
  bool io_impl(IOImGUi& ar, std::string& t, const char* name) {
    printf("ImGUI::string  %s -> ( %s ) \n", name, t.c_str());
    return true;
  }

  // Serialization of all vectors
  template< typename T >
  bool io_impl(IOImGUi& ar, std::vector<T>& v, const char* name) {

    size_t sz = v.size();
    
    printf("At IOImGUi of vector. Size:%zd\n", sz);

    for (auto& item : v) {
      if (!io(ar, item, name))
        return false;
    }

    return true;
  }

}

#endif
