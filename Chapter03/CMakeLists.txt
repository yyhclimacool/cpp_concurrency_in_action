aux_source_directory(. chap03_srcs)
set(target_list)
foreach(file IN LISTS chap03_srcs)
    string(REPLACE ".cc" "" target_name_temp ${file})
    string(REPLACE "./" "" target_name ${target_name_temp})
    add_executable(${target_name} ${file})
    target_link_libraries(${target_name} pthread)
    list(APPEND target_list ${target_name})
endforeach()

install(TARGETS ${target_list}
    RUNTIME DESTINATION bin/chapter03
)
