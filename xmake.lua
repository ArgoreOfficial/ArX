

set_languages( "cxx17" )


add_rules("mode.debug")
add_rules("mode.release")

target( "unordered_array" )
    set_kind( "binary" )
    
    set_basename("unordered_array_test_$(mode)_$(arch)")

    set_targetdir "bin"
    set_objectdir "build/obj"

    add_headerfiles( 
        "**.h",
        "**.hpp" 
        )
    add_files( "**.cpp" )
    add_includedirs( "unordered_array" )
target_end()