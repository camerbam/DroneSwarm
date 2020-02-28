#define BOOST_TEST_MODULE MsgTest
#include <boost/test/unit_test.hpp>

#include <google/protobuf/stubs/common.h>

struct Fixture
{
  Fixture() {}
  ~Fixture() 
  {
    google::protobuf::ShutdownProtobufLibrary();
  }
};

BOOST_GLOBAL_FIXTURE(Fixture);
