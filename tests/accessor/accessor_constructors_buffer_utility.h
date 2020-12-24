/*************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
//  Copyright:	(c) 2018 by Codeplay Software LTD. All Rights Reserved.
//
//  This file is a common utility for the implementation of
//  accessor_constructors.cpp and accessor_api.cpp.
//
**************************************************************************/
#ifndef SYCL_1_2_1_TESTS_ACCESSOR_ACCESSOR_CONSTRUCTORS_BUFFER_UTILITY_H
#define SYCL_1_2_1_TESTS_ACCESSOR_ACCESSOR_CONSTRUCTORS_BUFFER_UTILITY_H

#include "../common/common.h"
#include "accessor_constructors_utility.h"

#ifndef TEST_NAME
#error Invalid test namespace
#endif

namespace TEST_NAMESPACE {

using namespace sycl_cts;

/** @brief Creates a buffer accessor and checks all its members for correctness
 */
template <typename accTag>
class check_accessor_constructor_buffer {
public:
  /** @brief Overload to verify all constructors w/o range and offset
   */
  template <typename ... handlerArgsT>
  static void check(cl::sycl::buffer<typename accTag::dataT, accTag::dataDims>
                        &buffer,
                    sycl_cts::util::logger &log,
                    const std::string& constructorName,
                    const std::string& typeName,
                    handlerArgsT&& ... handler) {
    // construct the accessor
    typename accTag::type accessor(buffer,
                                   std::forward<handlerArgsT>(handler)...);
    const auto offset =
        sycl_cts::util::get_cts_object::id<accTag::dataDims>::get(0, 0, 0);

    // check the accessor
    check_accessor_members<accTag>::check(
        log, accessor, constructorName, typeName,
        accessor_members::size{buffer.get_size()},
        accessor_members::count{buffer.get_count()},
        accessor_members::offset<accTag::dataDims>{offset},
        accessor_members::range<accTag::dataDims>{buffer.get_range()},
        accessor_members::placeholder{accTag::placeholder});
  }
  /** @brief Overload to verify all constructors with range only
   */
  template <typename ... handlerArgsT>
  static void check(cl::sycl::buffer<typename accTag::dataT, accTag::dataDims>
                        &buffer,
                    cl::sycl::range<accTag::dataDims> range,
                    sycl_cts::util::logger &log,
                    const std::string& constructorName,
                    const std::string& typeName,
                    handlerArgsT&& ... handler) {
    // construct the accessor
    typename accTag::type accessor(buffer,
                                   std::forward<handlerArgsT>(handler)...,
                                   range);
    const auto offset =
        sycl_cts::util::get_cts_object::id<accTag::dataDims>::get(0, 0, 0);

    // check the accessor
    check_accessor_members<accTag>::check(
        log, accessor, constructorName, typeName,
        accessor_members::size{range.size() * sizeof(typename accTag::dataT)},
        accessor_members::count{range.size()},
        accessor_members::offset<accTag::dataDims>{offset},
        accessor_members::range<accTag::dataDims>{range},
        accessor_members::placeholder{accTag::placeholder});
  }
  /** @brief Overload to verify all constructors with range and offset
   */
  template <typename ... handlerArgsT>
  static void check(cl::sycl::buffer<typename accTag::dataT, accTag::dataDims>
                        &buffer,
                    cl::sycl::range<accTag::dataDims> range,
                    cl::sycl::id<accTag::dataDims> offset,
                    sycl_cts::util::logger &log,
                    const std::string& constructorName,
                    const std::string& typeName,
                    handlerArgsT&& ... handler) {
    // construct the accessor
    typename accTag::type accessor(buffer,
                                   std::forward<handlerArgsT>(handler)...,
                                   range, offset);

    // check the accessor
    check_accessor_members<accTag>::check(
        log, accessor, constructorName, typeName,
        accessor_members::size{range.size() * sizeof(typename accTag::dataT)},
        accessor_members::count{range.size()},
        accessor_members::offset<accTag::dataDims>{offset},
        accessor_members::range<accTag::dataDims>{range},
        accessor_members::placeholder{accTag::placeholder});
  }
};

/** @brief Checks all constructors available for non-zero dimensions
 */
template <typename T, size_t dims, cl::sycl::access::target target,
          cl::sycl::access::placeholder placeholder>
class check_all_accessor_constructors_buffer {
public:
  template <cl::sycl::access::mode mode, typename ... handlerArgsT>
  static void check(cl::sycl::buffer<T, dims> &buffer,
                    cl::sycl::range<dims> range,
                    cl::sycl::id<dims> offset,
                    sycl_cts::util::logger &log,
                    const std::string& typeName,
                    handlerArgsT&& ... handler) {
    // Run verification for accessors with dim > 0
    using accTag = accessor_type_info<T, dims, mode, target, placeholder>;
    using verifier = check_accessor_constructor_buffer<accTag>;

    constexpr bool usesHander = sizeof...(handlerArgsT) != 0;
    {
      const auto constructorName = usesHander ?
          "constructor(buffer, handler)" :
          "constructor(buffer)";
      verifier::check(buffer,
                      log, constructorName, typeName, handler...);
    }
    {
      const auto constructorName = usesHander ?
          "constructor(buffer, handler, range)" :
          "constructor(buffer, range)";
      verifier::check(buffer, range,
                      log, constructorName, typeName, handler...);
    }
    {
      const auto constructorName = usesHander ?
          "constructor(buffer, handler, range, offset)" :
          "constructor(buffer, range, offset)";
      verifier::check(buffer, range, offset,
                      log, constructorName, typeName, handler...);
    }
  }
};
/** @brief Checks all constructors available for 0 dimension
 */
template <typename T, cl::sycl::access::target target,
          cl::sycl::access::placeholder placeholder>
class check_all_accessor_constructors_buffer<T, 0, target, placeholder> {
  static constexpr size_t dims = 0;
public:
  template <cl::sycl::access::mode mode, typename ... handlerArgsT>
  static void check(cl::sycl::buffer<T, 1> &buffer,
                    cl::sycl::range<1> range,
                    cl::sycl::id<1> offset,
                    sycl_cts::util::logger &log,
                    const std::string& typeName,
                    handlerArgsT&& ... handler) {
    // Run verification for accessors with dim == 0
    static_cast<void>(range);
    static_cast<void>(offset);
    using accTag = accessor_type_info<T, dims, mode, target, placeholder>;
    using verifier = check_accessor_constructor_buffer<accTag>;

    constexpr bool usesHander = sizeof...(handlerArgsT) != 0;
    const auto constructorName = usesHander ?
        "constructor(buffer, handler)" :
        "constructor(buffer)";
    verifier::check(buffer, log, constructorName, typeName, handler...);
  }
};

/** @brief Check common-by-reference semantics for non-zero dimensions
 */
template <typename T, size_t dims, cl::sycl::access::target target,
          cl::sycl::access::placeholder placeholder>
class check_accessor_common_by_reference_buffer {
public:
  template <cl::sycl::access::mode mode, typename ... handlerArgsT>
  static void check(cl::sycl::buffer<T, dims> &buffer,
                    cl::sycl::range<dims> range,
                    cl::sycl::id<dims> offset,
                    sycl_cts::util::logger &log,
                    const std::string& typeName,
                    handlerArgsT&& ... handler) {
    // Run verification for accessors with dim > 0
    using accTag = accessor_type_info<T, dims, mode, target, placeholder>;
    {
      using verifier = check_accessor_copy_constructable<accTag>;

      typename accTag::type srcAccessor(buffer, handler..., range, offset);

      verifier::check(srcAccessor, log, typeName);
    }
    {
      using verifier = check_accessor_copy_assignable<accTag>;

      typename accTag::type srcAccessor(buffer, handler..., range, offset);
      typename accTag::type dstAccessor(buffer, handler...);

      verifier::check(srcAccessor, dstAccessor, log, typeName);
    }
    {
      using verifier = check_accessor_move_constructable<accTag>;

      typename accTag::type srcAccessor(buffer, handler...);

      verifier::check(srcAccessor, log, typeName);
    }
    {
      using verifier = check_accessor_move_assignable<accTag>;

      typename accTag::type srcAccessor(buffer, handler...);
      typename accTag::type dstAccessor(buffer, handler..., range, offset);

      verifier::check(srcAccessor, dstAccessor, log, typeName);
    }
  }
};
/** @brief Check common-by-reference semantics for 0 dimension
 */
template <typename T, cl::sycl::access::target target,
          cl::sycl::access::placeholder placeholder>
class check_accessor_common_by_reference_buffer<T, 0, target, placeholder> {
  static constexpr size_t dims = 0;
public:
  template <cl::sycl::access::mode mode, typename ... handlerArgsT>
  static void check(cl::sycl::buffer<T, 1> &buffer,
                    cl::sycl::range<1> range,
                    cl::sycl::id<1> offset,
                    sycl_cts::util::logger &log,
                    const std::string& typeName,
                    handlerArgsT&& ... handler) {
    // Neither range nor offset available for dim == 0
    static_cast<void>(range);
    static_cast<void>(offset);
    using accTag = accessor_type_info<T, dims, mode, target, placeholder>;
    {
      using verifier = check_accessor_copy_constructable<accTag>;

      typename accTag::type srcAccessor(buffer, handler...);

      verifier::check(srcAccessor, log, typeName);
    }
    {
      using verifier = check_accessor_copy_assignable<accTag>;

      typename accTag::type srcAccessor(buffer, handler...);
      typename accTag::type dstAccessor(buffer, handler...);

      verifier::check(srcAccessor, dstAccessor, log, typeName);
    }
    {
      using verifier = check_accessor_move_constructable<accTag>;

      typename accTag::type srcAccessor(buffer, handler...);

      verifier::check(srcAccessor, log, typeName);
    }
    {
      using verifier = check_accessor_move_assignable<accTag>;

      typename accTag::type srcAccessor(buffer, handler...);
      typename accTag::type dstAccessor(buffer, handler...);

      verifier::check(srcAccessor, dstAccessor, log, typeName);
    }
  }
};

/** @brief enum used to denote that the buffer_accessor_dims specialization
 *         performs checks either only for host_buffer or for any other buffer
 */
enum is_host_buffer : bool { false_t = false, true_t = true };

/** @brief Provide uniform way to initialize input data for different accessors
 */
template <typename T, size_t dims>
class buffer_accesor_input_data {
public:
  static constexpr size_t dataDims = dims;
  static constexpr int rangeSize = 32;

  using range_t = cl::sycl::range<dataDims>;
  using offset_t = cl::sycl::id<dataDims>;
  using data_t = std::vector<cl::sycl::cl_uchar>;
  using buffer_t = cl::sycl::buffer<T, dataDims>;

public:
  buffer_accesor_input_data():
    dataRange(sycl_cts::util::get_cts_object::range<dataDims>::get(rangeSize,
                                                                   rangeSize,
                                                                   rangeSize)),
    data(dataRange.size() * sizeof(T)),
    range(dataRange / 2),
    offset(dataRange / 2) {
    std::iota(std::begin(data), std::end(data), 0);
  }

  inline T* getDataPointer() {
    return reinterpret_cast<T *>(data.data());
  }
  inline const range_t& getDataRange() const {
    return dataRange;
  }
  inline const range_t& getRange() const {
    return range;
  }
  inline const offset_t& getOffset() const {
    return offset;
  }
private:
  range_t dataRange;
  data_t data;
  range_t range;
  offset_t offset;
};
/** @brief Specialization to implement input data initialization for 0 dimension
 *         in an uniform way with the non-zero dimensions
 */
template <typename T>
class buffer_accesor_input_data<T, 0> {
public:
  static constexpr size_t dataDims = 1;
  static constexpr int rangeSize = 1;

  using range_t = cl::sycl::range<dataDims>;
  using offset_t = cl::sycl::id<dataDims>;
  using data_t = std::vector<cl::sycl::cl_uchar>;
  using buffer_t = cl::sycl::buffer<T, dataDims>;

public:
  buffer_accesor_input_data():
    dataRange(rangeSize),
    data(sizeof(T), 0),
    range(dataRange),
    offset(0) {
  }

  inline T* getDataPointer() {
    return reinterpret_cast<T *>(data.data());
  }
  inline const range_t& getDataRange() const {
    return dataRange;
  }
  inline const range_t& getRange() const {
    return range;
  }
  inline const offset_t& getOffset() const {
    return offset;
  }
private:
  range_t dataRange;
  data_t data;
  range_t range;
  offset_t offset;
};

/** @brief Used to test the buffer accessor combinations for global_buffer and
 *         constant_buffer
 */
template <typename T, size_t dims, is_host_buffer isHostBuffer = false_t,
          cl::sycl::access::placeholder placeholder =
              cl::sycl::access::placeholder::false_t>
class buffer_accessor_dims {
public:
  static void check(util::logger &log, cl::sycl::queue &queue,
                    const std::string& typeName) {
    using input_type = buffer_accesor_input_data<T, dims>;
    input_type input;

    const auto dataRange = input.getDataRange();
    typename input_type::buffer_t buffer(input.getDataPointer(), dataRange);

    const auto r = input.getRange();
    const auto offset = input.getOffset();

    /** check buffer accessor constructors for global_buffer
     */
    {
      constexpr auto target = cl::sycl::access::target::global_buffer;
      using verifier =
          check_all_accessor_constructors_buffer<T, dims, target, placeholder>;
      using semantics_verifier =
          check_accessor_common_by_reference_buffer<T, dims, target, placeholder>;

      queue.submit([&](cl::sycl::handler &h) {
        /** check global_buffer constructors for different modes
         */
        {
          constexpr auto mode = cl::sycl::access::mode::read;
          verifier::template check<mode>(buffer, r, offset, log, typeName, h);
        }
        {
          constexpr auto mode = cl::sycl::access::mode::write;
          verifier::template check<mode>(buffer, r, offset, log, typeName, h);
        }
        {
          constexpr auto mode = cl::sycl::access::mode::read_write;
          verifier::template check<mode>(buffer, r, offset, log, typeName, h);
        }
        {
          constexpr auto mode = cl::sycl::access::mode::discard_write;
          verifier::template check<mode>(buffer, r, offset, log, typeName, h);
        }
        {
          constexpr auto mode = cl::sycl::access::mode::discard_read_write;
          verifier::template check<mode>(buffer, r, offset, log, typeName, h);
        }
        {
          constexpr auto mode = cl::sycl::access::mode::atomic;
          verifier::template check<mode>(buffer, r, offset, log, typeName, h);
        }
        /** check common-by-reference semantics
         */
        {
          constexpr auto mode = cl::sycl::access::mode::discard_read_write;
          semantics_verifier::template check<mode>(buffer, r, offset, log, typeName, h);
        }
        /** dummy kernel as no kernel is required for these checks
         */
        h.single_task(
            dummy_functor<T, cl::sycl::access::target::global_buffer>{});
      });
      queue.wait_and_throw();
    }

    /** check buffer accessor constructors for constant_buffer
     */
    {
      constexpr auto target = cl::sycl::access::target::constant_buffer;
      using verifier =
          check_all_accessor_constructors_buffer<T, dims, target, placeholder>;
      using semantics_verifier =
          check_accessor_common_by_reference_buffer<T, dims, target, placeholder>;

      queue.submit([&](cl::sycl::handler &h) {
        /** check constant_buffer constructors for different modes
         */
        {
          constexpr auto mode = cl::sycl::access::mode::read;
          verifier::template check<mode>(buffer, r, offset, log, typeName, h);
        }
        /** check common-by-reference semantics
         */
        {
          constexpr auto mode = cl::sycl::access::mode::read;
          semantics_verifier::template check<mode>(buffer, r, offset, log, typeName, h);
        }
        /** dummy kernel as no kernel is required for these checks
         */
        h.single_task(
            dummy_functor<T, cl::sycl::access::target::constant_buffer>{});

      });
      queue.wait_and_throw();
    }
  }
};

/** @brief Specialization of buffer_accessor_dims for host_buffer
 */
template <typename T, size_t dims, cl::sycl::access::placeholder placeholder>
class buffer_accessor_dims<T, dims, is_host_buffer::true_t, placeholder> {
 public:
  static void check(util::logger &log, cl::sycl::queue &queue,
                    const std::string& typeName) {
    using input_type = buffer_accesor_input_data<T, dims>;
    input_type input;

    const auto dataRange = input.getDataRange();
    typename input_type::buffer_t buffer(input.getDataPointer(), dataRange);

    const auto r = input.getRange();
    const auto offset = input.getOffset();

    /** check buffer accessor constructors for host_buffer
     */
    {
      constexpr auto target = cl::sycl::access::target::host_buffer;
      using verifier =
          check_all_accessor_constructors_buffer<T, dims, target, placeholder>;
      using semantics_verifier =
          check_accessor_common_by_reference_buffer<T, dims, target, placeholder>;

      /** check host_buffer constructors for different modes
       */
      {
        constexpr auto mode = cl::sycl::access::mode::read;
        verifier::template check<mode>(buffer, r, offset, log, typeName);
      }
      {
        constexpr auto mode = cl::sycl::access::mode::write;
        verifier::template check<mode>(buffer, r, offset, log, typeName);
      }
      {
        constexpr auto mode = cl::sycl::access::mode::read_write;
        verifier::template check<mode>(buffer, r, offset, log, typeName);
      }
      {
        constexpr auto mode = cl::sycl::access::mode::discard_write;
        verifier::template check<mode>(buffer, r, offset, log, typeName);
      }
      {
        constexpr auto mode = cl::sycl::access::mode::discard_read_write;
        verifier::template check<mode>(buffer, r, offset, log, typeName);
      }
      /** check common-by-reference semantics
       */
      {
        constexpr auto mode = cl::sycl::access::mode::read;
        semantics_verifier::template check<mode>(buffer, r, offset, log, typeName);
      }
    }
  }
};

/** @brief Used to test the buffer accessor combinations for placeholder
 *         global_buffer and placeholder constant_buffer
 */
template <typename T, size_t dims>
class buffer_accessor_dims<T, dims, is_host_buffer::false_t,
                           cl::sycl::access::placeholder::true_t> {
  static constexpr auto placeholder = cl::sycl::access::placeholder::true_t;
public:
  static void check(util::logger &log, cl::sycl::queue &queue,
                    const std::string& typeName) {
    using input_type = buffer_accesor_input_data<T, dims>;
    input_type input;

    const auto dataRange = input.getDataRange();
    typename input_type::buffer_t buffer(input.getDataPointer(), dataRange);

    const auto r = input.getRange();
    const auto offset = input.getOffset();

    /** check buffer accessor constructors for global_buffer
     */
    {
      constexpr auto target = cl::sycl::access::target::global_buffer;
      using verifier =
          check_all_accessor_constructors_buffer<T, dims, target, placeholder>;
      using semantics_verifier =
          check_accessor_common_by_reference_buffer<T, dims, target, placeholder>;

      /** check global_buffer constructors for different modes
       */
      {
        constexpr auto mode = cl::sycl::access::mode::read;
        verifier::template check<mode>(buffer, r, offset, log, typeName);
      }
      {
        constexpr auto mode = cl::sycl::access::mode::write;
        verifier::template check<mode>(buffer, r, offset, log, typeName);
      }
      {
        constexpr auto mode = cl::sycl::access::mode::read_write;
        verifier::template check<mode>(buffer, r, offset, log, typeName);
      }
      {
        constexpr auto mode = cl::sycl::access::mode::discard_write;
        verifier::template check<mode>(buffer, r, offset, log, typeName);
      }
      {
        constexpr auto mode = cl::sycl::access::mode::discard_read_write;
        verifier::template check<mode>(buffer, r, offset, log, typeName);
      }
      {
        constexpr auto mode = cl::sycl::access::mode::atomic;
        verifier::template check<mode>(buffer, r, offset, log, typeName);
      }
      /** check common-by-reference semantics
       */
      {
        constexpr auto mode = cl::sycl::access::mode::read;
        semantics_verifier::template check<mode>(buffer, r, offset, log, typeName);
      }

      queue.submit([&](cl::sycl::handler &h) {
        /** dummy kernel as no kernel is required for these checks
         */
        h.single_task(
            dummy_functor<T, cl::sycl::access::target::global_buffer>{});
      });
      queue.wait_and_throw();
    }

    /** check buffer accessor constructors for constant_buffer
     */
    {
      constexpr auto target = cl::sycl::access::target::constant_buffer;
      using verifier =
          check_all_accessor_constructors_buffer<T, dims, target, placeholder>;
      using semantics_verifier =
          check_accessor_common_by_reference_buffer<T, dims, target, placeholder>;

      /** check constant_buffer constructors for different modes
       */
      {
        constexpr auto mode = cl::sycl::access::mode::read;
        verifier::template check<mode>(buffer, r, offset, log, typeName);
      }
      /** check common-by-reference semantics
       */
      {
        constexpr auto mode = cl::sycl::access::mode::read;
        semantics_verifier::template check<mode>(buffer, r, offset, log, typeName);
      }

      queue.submit([&](cl::sycl::handler &h) {
        /** dummy kernel as no kernel is required for these checks
         */
        h.single_task(
            dummy_functor<T, cl::sycl::access::target::constant_buffer>{});

      });
      queue.wait_and_throw();
    }
  }
};
}  // namespace accessor_utility__

#endif  // SYCL_1_2_1_TESTS_ACCESSOR_ACCESSOR_CONSTRUCTORS_BUFFER_UTILITY_H
