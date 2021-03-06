#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <string>
#include <cassert>
#include "io/io.h"
#include "io/io_binary.h"
#include "io/io_imgui.h"
#include "io/io_json.h"

struct C1 {

  struct TPOD {
    float value;
    char  name[16];
    bool  magic_bool;

    template< typename AR >
    bool io(AR& ar, const char* name) {
      IOAuto(ar, value);
      IOAuto(ar, magic_bool);
      return true;
    }
  };

  struct TNonPOD {
    std::string name;
    float       value;

  };

  int                  unique_id;
  float                number;
  std::vector<int>     ids;
  std::vector<TPOD>    pods;
  std::vector<TNonPOD> non_pods;
  std::vector<std::string> names;

  template< typename AR >
  bool io(AR& ar, const char* name) {
    IOAuto(ar, unique_id);
    IOAuto(ar, number);
    IOAuto(ar, ids);
    IOAuto(ar, pods);
    IOAuto(ar, names);
    IOAuto(ar, non_pods);
    return true;
  }
};

template< typename AR >
bool io(AR& ar, C1::TNonPOD& np, const char* name) {
  ar.beginObj(name);
  IOAuto(ar, np.name);
  IOAuto(ar, np.value);
  ar.endObj();
  return true;
}

int main( int argc, char** argv)
{

  C1 c1;
  c1.ids.push_back(1);
  c1.ids.push_back(2);
  c1.ids.push_back(3);
  c1.names.push_back("john");
  c1.names.push_back("peter");
  c1.names.push_back("eva");
  c1.unique_id = 7;
  c1.number = 3.14f;
  c1.pods.push_back({ 1.2f, "pod_name", true });
  c1.pods.push_back({ 1.3f, "pod_other_name", false });
  c1.non_pods.push_back({ "non_pod_name1", 10.f });
  c1.non_pods.push_back({ "non_pod_name2", 20.f });

  if (0) {
    printf("int... %d\n", std::is_pod< int >::value);
    printf("names... %d\n", std::is_pod< std::vector<std::string> >::value);
    printf("pods[0]... %d\n", std::is_trivial< C1::TPOD >::value);
    printf("non_pods[0]... %d\n", std::is_pod< C1::TNonPOD >::value );
  }

  bool test_bin = false;
  bool test_imgui = false;
  bool test_json = true;

  if (test_bin) {
    if( 1 ) {
      printf("Writing to binary...\n");
      IO::BinWriter bw("out1.bin");
      IOAuto(bw, c1);
    }

    C1 c2;
    {
      printf("Reading binary...\n");
      IO::BinReader br("out1.bin");
      IOAuto(br, c2);
      assert(c1.unique_id == c2.unique_id);
      assert(c1.number == c2.number);
      assert(c1.ids.size() == c2.ids.size());
      assert(c1.pods.size() == c2.pods.size());
      for (size_t i = 0; i < c1.ids.size(); ++i) {
        assert(c1.ids[i] == c2.ids[i]);
      }
    }
  }

  if (test_imgui) {
    IO::IOImGUi io_imgui;
    IOAuto(io_imgui, c1);
  }

  if (test_json) {
    IO::JsonWriter jw;
    IOAuto(jw, c1);
  }

  return 0;
}

