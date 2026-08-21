-- include subprojects
includes("lib/commonlibsse")

-- set project constants
set_project("CCExtender")
set_version("2.0.0")
set_license("MIT")
set_languages("c++23")
set_warnings("allextra")

-- add common rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- set configs
set_config("commonlib_json", true)

-- define targets
target("CCExtender")
    add_rules("commonlibsse.plugin", {
        name = "CCExtender",
        author = "ryan-rsm-mckenzie"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")

    -- add extra files
    add_extrafiles(".clang-format")
