/*******************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
//  Copyright (c) 2017-2022 Codeplay Software LTD. All Rights Reserved.
//  Copyright (c) 2022 The Khronos Group Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
*******************************************************************************/

#include "../common/common.h"

#define TEST_NAME header_test

namespace header_test__ {
using namespace sycl_cts;

/** test SYCL header for compilation and macro definitions
 */
class TEST_NAME : public util::test_base {
 public:
  /** return information about this test
  */
  void get_info(test_base::info &out) const override {
    set_test_info(out, TOSTRING(TEST_NAME), TEST_FILE);
  }

  /** execute this test
  */
  void run(util::logger& log) override {
/** checks that __FAST_RELAXED_MATH__ is defined
*/
#if defined(__FAST_RELAXED_MATH__)
    log.note("__FAST_RELAXED_MATH__ defined");
#endif

/** checks that __SYCL_DEVICE_ONLY__ is defined
*/
#if defined(__SYCL_DEVICE_ONLY__)
    log.note("__SYCL_DEVICE_ONLY__ defined");
#endif

/** checks that __SYCL_SINGLE_SOURCE__ is defined
*/
#if defined(__SYCL_SINGLE_SOURCE__)
    log.note("__SYCL_SINGLE_SOURCE__ defined");
#endif

#if defined(TEST_FAIL)
    FAIL(log, "sycl macro undefined, see previous error");
#endif
  }
};

TEST_CASE(
    "The implementation defines the correct SYCL_LANGUAGE_VERSION macro") {
#ifndef SYCL_LANGUAGE_VERSION
  FAIL("SYCL_LANGUAGE_VERSION is not defined");
#else
  STATIC_REQUIRE(std::is_same_v<decltype(SYCL_LANGUAGE_VERSION), long>);
  STATIC_REQUIRE(SYCL_LANGUAGE_VERSION == 202012L);
#endif
}

// register this test with the test_collection
util::test_proxy<TEST_NAME> proxy;

} /* namespace header_test__ */
