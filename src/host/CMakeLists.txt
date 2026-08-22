add_executable(andows_host WIN32 main.cpp)
target_compile_definitions(andows_host PRIVATE UNICODE _UNICODE)
if(MSVC)
  # 源文件以 UTF-8 保存,强制 MSVC 按 UTF-8 解析,避免代码页 936 下中文乱码
  target_compile_options(andows_host PRIVATE /utf-8)
endif()
target_link_libraries(andows_host PRIVATE user32 gdi32)
