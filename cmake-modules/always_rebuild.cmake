# Dummy output which is never actually produced. Anything that depends on this will always be rebuilt.
add_custom_command(
        OUTPUT always_rebuild
        COMMAND cmake -E echo
)