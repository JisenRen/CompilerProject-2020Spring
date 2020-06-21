# CMake generated Testfile for 
# Source directory: /home/rjs/CompilerProject-2020Spring/3rdparty/jsoncpp/src/jsontestrunner
# Build directory: /home/rjs/CompilerProject-2020Spring/build/jsoncpp-build/src/jsontestrunner
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(jsoncpp_readerwriter "/usr/bin/python3.8" "-B" "/home/rjs/CompilerProject-2020Spring/3rdparty/jsoncpp/src/jsontestrunner/../../test/runjsontests.py" "/home/rjs/CompilerProject-2020Spring/build/bin/jsontestrunner_exe" "/home/rjs/CompilerProject-2020Spring/3rdparty/jsoncpp/src/jsontestrunner/../../test/data")
set_tests_properties(jsoncpp_readerwriter PROPERTIES  WORKING_DIRECTORY "/home/rjs/CompilerProject-2020Spring/3rdparty/jsoncpp/src/jsontestrunner/../../test/data" _BACKTRACE_TRIPLES "/home/rjs/CompilerProject-2020Spring/3rdparty/jsoncpp/src/jsontestrunner/CMakeLists.txt;41;add_test;/home/rjs/CompilerProject-2020Spring/3rdparty/jsoncpp/src/jsontestrunner/CMakeLists.txt;0;")
add_test(jsoncpp_readerwriter_json_checker "/usr/bin/python3.8" "-B" "/home/rjs/CompilerProject-2020Spring/3rdparty/jsoncpp/src/jsontestrunner/../../test/runjsontests.py" "--with-json-checker" "/home/rjs/CompilerProject-2020Spring/build/bin/jsontestrunner_exe" "/home/rjs/CompilerProject-2020Spring/3rdparty/jsoncpp/src/jsontestrunner/../../test/data")
set_tests_properties(jsoncpp_readerwriter_json_checker PROPERTIES  WORKING_DIRECTORY "/home/rjs/CompilerProject-2020Spring/3rdparty/jsoncpp/src/jsontestrunner/../../test/data" _BACKTRACE_TRIPLES "/home/rjs/CompilerProject-2020Spring/3rdparty/jsoncpp/src/jsontestrunner/CMakeLists.txt;45;add_test;/home/rjs/CompilerProject-2020Spring/3rdparty/jsoncpp/src/jsontestrunner/CMakeLists.txt;0;")
