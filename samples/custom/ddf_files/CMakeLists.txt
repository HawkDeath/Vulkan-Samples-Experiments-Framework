get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_LIST_DIR} PATH)
get_filename_component(CATEGORY_NAME ${PARENT_DIR} NAME)

add_sample_with_tags(
        ID ${FOLDER_NAME}
        CATEGORY ${CATEGORY_NAME}
        AUTHOR "Patryk Jastrzebski"
        NAME "ddf file loader"
        DESCRIPTION "ddf file loader"
        FILES
        "img_archive.h"
        "img_archive.cpp"
        "ddf_file.h"
        "ddf_file.cpp"

        #"ddf_files/base.vert"
#        "ddf_files/base.frag"
)