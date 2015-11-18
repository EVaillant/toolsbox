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
endif (Boost_FOUND)
