
set(tests_src
	${tests_src}
	${CMAKE_CURRENT_SOURCE_DIR}/MockFunction.h
	${CMAKE_CURRENT_SOURCE_DIR}/TestBlock.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TestConnection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TestDatatype.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TestErrors.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TestFunction.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TestPrimitiveFunction.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TestProgram.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TestReturnBlock.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TestStatement.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/testValue.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TestVariableReadBlock.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TestVariableWriteBlock.cpp
	PARENT_SCOPE)