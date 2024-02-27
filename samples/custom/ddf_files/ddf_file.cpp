#include "ddf_file.h"

ddf_file::ddf_file(img_archive_file &archive)
{
	file_name = archive.file_directory->name;
	LOGI("Parsing {}", archive.file_directory->name);
	parse_archive(archive.file_buffer);
	LOGI("Parsing {} - done", archive.file_directory->name);

}
ddf_file::~ddf_file()
{
	LOGW("Release object {}", file_name);
}

void ddf_file::draw(VkCommandBuffer cmd)
{
}

void ddf_file::parse_archive(std::vector<uint8_t> &raw_data)
{
}
