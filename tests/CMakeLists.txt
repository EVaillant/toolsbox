if (Boost_FOUND)
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

	if(ENABLE_PERF_APPL_BUILD)
		add_executable(
			perf_lockfree_01
			perf_lockfree_01.cc
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

endif (Boost_FOUND)
