function(zenith_enable_lto target)
    message(STATUS "Applying optimizations to: ${target}")

    if(IPO_SUPPORTED)
        set_target_properties(${target} PROPERTIES
                INTERPROCEDURAL_OPTIMIZATION_RELEASE ON
                INTERPROCEDURAL_OPTIMIZATION_DIST    ON
        )
        message(STATUS "  -> IPO enabled for Release/Dist: ${target}")
    endif()
endfunction()