/*******************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
//  Provides group async_work_group_copy tests for half and cl_half
//
*******************************************************************************/

#include "group_async_work_group_copy_common.h"

#define TEST_NAME group_async_work_group_copy_fp16

using namespace sycl_cts;
using namespace group_async_work_group_copy;

namespace TEST_NAMESPACE {

class TEST_NAME : public util::test_base {
 public:
  /** return information about this test
  */
  void get_info(test_base::info &out) const override {
    set_test_info(out, TOSTRING(TEST_NAME), TEST_FILE);
  }

  /** execute the test
  */
  void run(util::logger &log) override {
    try {
      auto queue = util::get_cts_object::queue();

      if (!queue.get_device().has_extension("cl_khr_fp16")) {
        log.note(
            "Device does not support half precision floating point operations");
        return;
      }
      // Test using queue constructed already
      for_type_and_vectors<check_type, cl::sycl::half>(queue, log,
          "cl::sycl::half");
      for_type_and_vectors<check_type, cl::sycl::cl_half>(queue, log,
          "cl::sycl::cl_half");

    } catch (const cl::sycl::exception &e) {
      log_exception(log, e);
      auto errorMsg = std::string("a SYCL exception was caught: ") + e.what();
      FAIL(log, errorMsg);
    }
  }
};

// construction of this proxy will register the above test
util::test_proxy<TEST_NAME> proxy;

}  // namespace TEST_NAMESPACE
