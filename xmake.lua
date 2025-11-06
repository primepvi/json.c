add_rules("mode.debug", "mode.release")

target("json")
set_kind("static")
add_files("src/lib/*.c", "src/json.c")
