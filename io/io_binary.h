#ifndef INC_IO_IO_BINARY_H_
#define INC_IO_IO_BINARY_H_

#include "io.h"

namespace IO {

  // -------------------------------------------------
  struct BinArchive {
  protected:
    FILE* f = nullptr;
  public:
    void beginObj(const char* name) {}
    void endObj() {}
    ~BinArchive() {
      if (f) {
        fclose(f);
        f = nullptr;
      }
    }
  };

  // -------------------------------------------------
  class BinReader : public BinArchive {
  public:
    BinReader(const char* aname) {
      f = fopen(aname, "rb");
    }
    bool isReading() const { return true; }
    bool ioBytes(void* addr, size_t nbytes, const char* name) {
      printf("BinReader::ioBytes(%zd,%s)\n", nbytes, name );
      if (!f)
        return false;
      auto nbytes_processed = fread(addr, 1, nbytes, f);
      return nbytes_processed == nbytes;
    }
  };

  // -------------------------------------------------
  class BinWriter : public BinArchive {
  public: 
    BinWriter(const char* aname) {
      f = fopen(aname, "wb");
    }
    bool isReading() const { return false; }
    bool ioBytes(void* addr, size_t nbytes, const char* name) {
      printf("BinWriter::ioBytes(%zd,%s)\n", nbytes, name);
      if (!f)
        return false;
      auto nbytes_processed = fwrite(addr, 1, nbytes, f);
      return nbytes_processed == nbytes;
    }
  };

  // -------------------------------------------------
  // if std::is_base_of<BinArchive,AR>::value && std::is_pod<T>::value == true
  // then enable_if will return the second arg: bool, from the enable_if  we want 
  // the type, so we add ::type, but we also need to add typename to avoid confusion
  // with a static var for example
  template< typename AR, typename T >
  typename enable_if< std::is_base_of<BinArchive,AR>::value && std::is_pod<T>::value, bool >::type
  io_impl(AR& ar, T& t, const char* name)
  {
    return ar.ioBytes(&t, sizeof(T), name);
  }

  // Serialization of all vectors for BinArchives
  template< typename AR, typename T >
  typename enable_if< std::is_base_of<BinArchive, AR>::value, bool >::type
  io_impl(AR& ar, std::vector<T>& v, const char* name) {

    size_t sz = v.size();
    printf("At io_non_member of vector. Size:%zd\n", sz);
    if (!io(ar, sz, "count"))
      return false;

    if (ar.isReading())
      v.resize(sz);

    if (std::is_pod<T>::value && !IO::detail::hasIO<AR, T>()) {
      printf("vector of pods: ");
      if (!ar.ioBytes(v.data(), v.size() * sizeof(T), name))
        return false;
    }
    else {
      for (auto& item : v) {
        if (!io(ar, item, name))
          return false;
      }
    }

    printf("End of vector\n");
    return true;
  }

  // --------------------------------------------------------------------
  // Save a string in general form: Save the size, then the bytes
  template< typename AR >
  typename enable_if< std::is_base_of<BinArchive, AR>::value, bool >::type
  io_impl(AR& ar, std::string& t, const char* name) {
    size_t sz = t.size();
    if (ar.isReading())
      t.resize(sz);
    return ar.ioBytes((void*)t.data(), sz, name);
  }

}

#endif
