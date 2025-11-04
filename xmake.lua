add_rules("mode.debug", "mode.release")

add_includedirs("include")

target("json")
set_kind("binary")
add_files("src/**/*.c", "src/main.c")
