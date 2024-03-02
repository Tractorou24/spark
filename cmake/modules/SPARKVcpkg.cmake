#########
# Include guard to prevent repeated library definition
#########
include_guard(GLOBAL)

#########
# Sets the vcpkg triplet depending of the platform
#########
block()
	if(LINUX)
		set(VCPKG_TARGET_TRIPLET x64-linux PARENT_SCOPE)
	else()
		set(VCPKG_TARGET_TRIPLET x64-windows PARENT_SCOPE)
	endif()
endblock()

#########
# Sets the cmake toolchain file
#########
block()
	#########
	# Helper function used below to check if the vcpkg install is usable by spark
	# _spark_validate_vcpkg_install(
	#   result_var
	#   current_item
	# )
	# An installation is usable if it's not a mount point (or windows vcpkg will be found first on WSL)
	#########
	function(_spark_validate_vcpkg_install result_var item)
		if(item MATCHES "^/mnt")
			set(${result_var} FALSE PARENT_SCOPE)
		endif()
	endfunction()

	# Find vcpkg root directory from the vcpkg executable
	find_program(VCPKG_EXECUTABLE vcpkg VALIDATOR _spark_validate_vcpkg_install REQUIRED)
	get_filename_component(VCPKG_DIR ${VCPKG_EXECUTABLE} DIRECTORY)

	# Set the cmake toolchain files
	set(VCPKG_TOOLCHAIN_FILE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake")
	message(STATUS "Using vcpkg toolchain file: ${VCPKG_TOOLCHAIN_FILE}")

	set(CMAKE_TOOLCHAIN_FILE ${VCPKG_TOOLCHAIN_FILE} PARENT_SCOPE)
endblock()

# Set the VCPKG options
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)

# Include the toolchain file
include(${CMAKE_TOOLCHAIN_FILE})
