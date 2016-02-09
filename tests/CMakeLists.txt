if(NOT DISABLE_UNITTEST AND Boost_FOUND)
	enable_testing()

	#
	# variant
	add_executable(
		test_variant
		test_variant.cc
	)
	target_link_libraries(
		test_variant
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	)
	add_test(test_variant test_variant)

	#
	# type_uid
	add_executable(
		test_type_uid
		test_type_uid.cc
	)
	target_link_libraries(
		test_type_uid
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	)
	add_test(test_type_uid test_type_uid)

	#
	# any
	add_executable(
		test_any
		test_any.cc
	)
	target_link_libraries(
		test_any
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	)
	add_test(test_any test_any)

	#
	# lockfree
	add_executable(
		test_lockfree
		test_lockfree.cc
	)
	target_link_libraries(
		test_lockfree
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	)
	add_test(test_lockfree test_lockfree)

	#
	# pool 
	add_executable(
		test_pool
		test_pool.cc
	)
	target_link_libraries(
		test_pool
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	)
	add_test(test_pool test_pool)

  #
  # perf prog
	if(ENABLE_PERF_APPL_BUILD)
		add_executable(
			perf_lockfree_01
			perf_lockfree_01.cc
		)
		add_executable(
			perf_object_pool_01
			perf_object_pool_01.cc
		)
	endif(ENABLE_PERF_APPL_BUILD)

	#
	# sig
	add_executable(
		test_sig
		test_sig.cc
	)
	target_link_libraries(
		test_sig
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	)
	add_test(test_sig test_sig)

	#
	# print_type
	add_executable(
		test_print_type
		test_print_type.cc
	)
	target_link_libraries(
		test_print_type
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	)
	add_test(test_print_type test_print_type)

	#
	# for_each
	add_executable(
		test_for_each
		test_for_each.cc
	)
	target_link_libraries(
		test_for_each
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	)
	add_test(test_for_each test_for_each)

	#
	# zip
	add_executable(
		test_zip
		test_zip.cc
	)
	target_link_libraries(
		test_zip
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	)
	add_test(test_zip test_zip)

	#
	# zip_tuple
	add_executable(
		test_zip_tuple
		test_zip_tuple.cc
	)
	target_link_libraries(
		test_zip_tuple
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	)
	add_test(test_zip_tuple test_zip_tuple)

	#
	# zip_tuple
	add_executable(
		test_static_map
		test_static_map.cc
	)
	target_link_libraries(
		test_static_map
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	)
	add_test(test_static_map test_static_map)

endif (NOT DISABLE_UNITTEST AND Boost_FOUND) 
