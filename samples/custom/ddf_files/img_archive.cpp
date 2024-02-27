#include "img_archive.h"
#include "common/logging.h"

struct img_archive_header
{
	char    version[4];
	int32_t number_of_entries;
};

img_archive::img_archive(const std::string &file_path) :
    file_name_full_path{file_path}
{
	open_file_and_parse();
}

img_archive::~img_archive()
{
	file_handle.close();
}

void img_archive::open_file_and_parse()
{
	file_handle.open(file_name_full_path);
	if (!file_handle.is_open())
	{
		LOGE("Failed to open {}", file_name_full_path);
		return;
	}
	else
	{
		LOGI("{} has been loaded", file_name_full_path);
	}

	img_archive_header header{};
	file_handle.read((char *) &header, sizeof(img_archive_header));
	if (header.version[0] != 'V' && header.version[3] != '2')
	{
		LOGE("Wrong version of archive");
		return;
	}

	for (size_t i = 0; i < header.number_of_entries; ++i)
	{
		auto entry            = std::make_shared<img_archive_file>();
		entry->file_directory = std::make_shared<img_directory>();
		file_handle.read((char *) entry->file_directory.get(), sizeof(img_directory));
		archive_entries.push_back(entry);
	}
}

std::shared_ptr<img_archive_file> img_archive::get_archive_by_name(const std::string &archive_name)
{
	for (size_t i = 0; i < archive_entries.size(); ++i)
	{
		auto &archive_entry = archive_entries[i];
		if (archive_name != archive_entry->file_directory->name) continue;

		LOGI("Requested file has been found in archive {}", archive_name);
		archive_entry->full_file_offset = archive_entry->file_directory->offset * IMG_SECTOR_SIZE;
		archive_entry->full_file_size = archive_entry->file_directory->stream_size * IMG_SECTOR_SIZE;
		archive_entry->file_buffer.resize(archive_entry->full_file_size);
		file_handle.seekg(std::ios_base::beg); // make sure is reading from begin of file
		file_handle.seekg(archive_entry->full_file_offset);
		file_handle.read((char*)archive_entry->file_buffer.data(), archive_entry->full_file_size);
		return archive_entry;
	}
	LOGE("Cannot find the {} in archive", archive_name);

	return nullptr;
}

void img_archive::print_all_archives()
{
	if (archive_entries.empty())
	{
		LOGW("No entries available in {} a file", file_name_full_path);
		return;
	}

	for (size_t i = 0; i < archive_entries.size(); ++i)
	{
		auto entry = archive_entries[i];
		LOGI(entry->file_directory->name);
	}
}

std::vector<std::string> img_archive::get_file_list()
{
	if (archive_entries.empty())
	{
		LOGW("No entries available in {} a file", file_name_full_path);
		return {};
	}

	std::vector<std::string> list;
	list.reserve(archive_entries.size());
	for (auto &e : archive_entries)
		list.emplace_back(e->file_directory->name);

	return list;
}