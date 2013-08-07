
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(
        -DDEBUG
        )
endif()

include_directories(
    .
    ./foundation
    ./handler
    ./handler/av_codec
    ./handler/av_container
    ./handler/av_display
    ./handler/av_sync
    ./media_info
    ${SMEDIA_INC_DIRS}
    ${PROJECT_SOURCE_DIR}/sdk/include/
    ${PROJECT_SOURCE_DIR}/sdk/include/tscam_ctrl
    )

    #foundation
FILE(GLOB foundation
    foundation/cmd_msg.c
    foundation/cmd_msg.h
    foundation/list_template.h
    foundation/looper.c
    foundation/looper.h
    foundation/looper_roster.c
    foundation/looper_roster.h
    foundation/register_template.h
    foundation/ring_buffer.c
    foundation/ring_buffer.h
    foundation/basic_type.h
    foundation/handler_desc.h
    )   
source_group("foundation" FILES ${foundation})

    #av_codec
FILE(GLOB av_codec
    handler/av_codec/av_codec_desc.h
    )
source_group("av_codec" FILES ${av_codec})

    #av_container
FILE(GLOB av_container
    handler/av_container/av_container_desc.h
    )
source_group("av_container" FILES ${av_container})

    #av_display
FILE(GLOB av_display
    handler/av_display/av_display_desc.h
    )
source_group("av_display" FILES ${av_display})

    #av_sync
FILE(GLOB av_sync
    handler/av_sync/av_sync_desc.h
    )
source_group("av_sync" FILES ${av_sync})

    #handler
FILE(GLOB handler
    handler/handler_mm_a_dec.c
    handler/handler_mm_a_dec.h
    handler/handler_mm_demux.c
    handler/handler_mm_demux.h
    handler/handler_mm_v_dec.c
    handler/handler_mm_v_dec.h
    )
source_group("handler" FILES ${handler})

    #media_info
FILE(GLOB media_info
    media_info/audio_info.c
    media_info/audio_info.h
    media_info/video_info.c
    media_info/video_info.h
    )
source_group("media_info" FILES ${media_info})


add_library(imp STATIC
    cmd_router.c
    cmd_router.h
    info_reply.c
    info_reply.h
    ite_media_player.c
    ite_media_player.h
    media_player_cfg.h
    media_player_def.h
    media_player_err.h
    ${foundation}
    ${av_codec}
    ${av_container}
    ${av_display}
    ${av_sync}
    ${handler}
    ${media_info}
    )


