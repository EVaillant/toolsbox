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
	# test_type_uid
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
	# test_any
	add_executable(
		test_any
		test_any.cc
	)
	target_link_libraries(
		test_any
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	)
	add_test(test_any test_any)

endif (Boost_FOUND)
