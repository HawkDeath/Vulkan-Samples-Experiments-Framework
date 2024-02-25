#pragma once

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

const int32_t IMG_SECTOR_SIZE = 2048;

struct img_directory
{
	int32_t offset; // offset in sectors; (offset * SECTOR_SIZE)
	int16_t stream_size; // size in sectors; (stream_size * SECTOR_SIZE)
	int16_t not_to_use_size_in_archive {0}; // not used, always 0
	char name[24]; // null terminated; 23 + 1 (string + '\0')
};

struct img_archive_file
{
	std::shared_ptr<img_directory> file_directory {nullptr};
	uint64_t  full_file_offset; // offset in file
	uint64_t  full_file_size; // actual size of file
	std::vector<uint8_t> file_buffer; // raw data
};

class img_archive
{
  public:
	explicit img_archive(const std::string &file_path);
	~img_archive() = default;

	std::shared_ptr<img_archive_file> get_archive_by_name(const std::string &archive_name);
	uint32_t get_number_of_entries() const { return archive_entries.size(); }

	void print_all_archives();

  private:
	void open_file_and_parse();

  private:
	std::ifstream file_handle;
	std::string file_name_full_path;
	std::vector<std::shared_ptr<img_archive_file>> archive_entries;
};