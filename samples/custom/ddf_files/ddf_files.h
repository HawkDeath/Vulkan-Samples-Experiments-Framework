#pragma once

#include "api_vulkan_sample.h"

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
};

std::unique_ptr<vkb::Application> create_ddf_files();