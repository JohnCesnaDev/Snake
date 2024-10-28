add_repositories("nazara-repo https://github.com/NazaraEngine/xmake-repo.git")
add_requires("nazaraengine", { debug = is_mode("debug") })

add_rules("mode.debug", "mode.release")

set_runtimes(is_mode("debug") and "MDd" or "MD")
set_languages("c++20")

add_defines("NAZARA_ENTT")

target("Snake")
    add_files("src/main.cpp")
    add_files("src/States/*.cpp")
    add_packages("nazaraengine")

        after_install(function (target)
        local nazaraengine = target:pkg("nazaraengine")
        if not nazaraengine then
            return
        end

        local bin = path.join(nazaraengine:installdir(), "bin")
        os.vcp(path.join(bin, "*.dll"), target:installdir("bin"))
    end)