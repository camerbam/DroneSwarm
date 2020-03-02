#define BOOST_TEST_MODULE MsgTest
#include <boost/test/unit_test.hpp>

#pragma warning(push)
#pragma warning(disable : 4267)
#include <google/protobuf/stubs/common.h>
#pragma warning(pop)

struct Fixture
{
  Fixture() {}
  ~Fixture() { google::protobuf::ShutdownProtobufLibrary(); }
};

BOOST_GLOBAL_FIXTURE(Fixture);
