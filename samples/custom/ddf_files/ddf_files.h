#pragma once

#include "api_vulkan_sample.h"
#include "img_archive.h"
#include "ddf_file.h"
#include <memory>

class ddf_files : public ApiVulkanSample
{
  public:
	ddf_files();
	~ddf_files() override;

	bool prepare(const vkb::ApplicationOptions &options) override;
	void render(float delta_time) override;
	void view_changed() override;
	void on_update_ui_overlay(vkb::Drawer &drawer) override;
	void request_gpu_features(vkb::PhysicalDevice &gpu) override;
	void build_command_buffers() override;
  private:
	void draw();
	void load_assets();

  private:
	std::unique_ptr<img_archive> gta3_archive;
	std::vector<std::string> archive_file_list;

	std::shared_ptr<ddf_file> model;
};

std::unique_ptr<vkb::Application> create_ddf_files();