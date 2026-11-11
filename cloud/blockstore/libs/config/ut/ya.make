# Blockstore configuration and overlay unit tests.
# The test target also exercises integration with the dynamic control board.

UNITTEST_FOR(cloud/blockstore/libs/config)

SRCS(
    blockstore_config_ut.cpp
    opaque_config_parser_ut.cpp
    sections_processing_ut.cpp
    sections_processing/features_ut.cpp
    sections_processing/volume_throttling_ut.cpp
)

PEERDIR(
    cloud/storage/core/libs/config

    contrib/ydb/core/control
)

END()
