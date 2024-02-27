#pragma once

#include "img_archive.h"

#include "scene_graph/components/sub_mesh.h"
#include <vulkan/vulkan.h>

#include <memory>

class ddf_file
{
  public:
	explicit ddf_file(img_archive_file &archive);
	~ddf_file();

	void draw(VkCommandBuffer cmd);

  private:
	void parse_archive(std::vector<uint8_t> &raw_data);

  private:
	std::shared_ptr<vkb::sg::SubMesh> mesh;
	std::string file_name;
};