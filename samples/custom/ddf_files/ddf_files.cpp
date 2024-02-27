#include "ddf_files.h"

#include "img_archive.h"

const std::string default_model_name = "speeder.dff";

ddf_files::ddf_files()
{
	title        = "ddf_files";
}

ddf_files::~ddf_files()
{
	if (device)
	{
	}
}

bool ddf_files::prepare(const vkb::ApplicationOptions &options)
{
	if (!ApiVulkanSample::prepare(options))
	{
		return false;
	}
	load_assets();

	build_command_buffers();
	prepared = true;
	return true;
}

void ddf_files::request_gpu_features(vkb::PhysicalDevice &gpu)
{
	auto &requested_features = gpu.get_mutable_requested_features();
	if (gpu.get_features().samplerAnisotropy)
	{
		requested_features.samplerAnisotropy = VK_TRUE;
	}
}

void ddf_files::load_assets()
{
	gta3_archive = std::make_unique<img_archive>("/Users/hawkdeath/Games/Grand Theft Auto San Andreas/models/gta3.img");

	archive_file_list = gta3_archive->get_file_list();
	archive_file_list.erase(std::remove_if(archive_file_list.begin(), archive_file_list.end(), [&](const std::string &s) {
		                        return s.find(".dff") == std::string::npos;
	                        }),
	                        archive_file_list.end());

	model = std::make_shared<ddf_file>(*gta3_archive->get_archive_by_name(default_model_name).get());
}

void ddf_files::render(float delta_time)
{
	if (!prepared)
		return;
	draw();
}
void ddf_files::view_changed()
{
	// update_uniforms();
}

void ddf_files::on_update_ui_overlay(vkb::Drawer &drawer)
{
	std::string header_title = "Number of models: " + std::to_string(archive_file_list.size());
	if (drawer.header(header_title.c_str()))
	{
		for (auto &e : archive_file_list)
			if (drawer.button(e.c_str()))
			{
				LOGI("Current file: {}", e);
				model.reset();
				model = std::make_shared<ddf_file>(*gta3_archive->get_archive_by_name(e).get());
				build_command_buffers();
			}
	}
}

void ddf_files::build_command_buffers()
{
	VkCommandBufferBeginInfo cmd_begin_info = vkb::initializers::command_buffer_begin_info();

	VkClearValue clear_values[2];
	clear_values[0].color        = VkClearColorValue({1.0f, 0.0f, 0.0f, 1.0f});
	clear_values[1].depthStencil = {0.0f, 0};

	VkRenderPassBeginInfo render_pass_begin_info = vkb::initializers::render_pass_begin_info();
	render_pass_begin_info.renderPass            = render_pass;
	render_pass_begin_info.renderArea.offset     = {0, 0};
	render_pass_begin_info.renderArea.extent     = {width, height};
	render_pass_begin_info.clearValueCount       = 2u;
	render_pass_begin_info.pClearValues          = clear_values;

	for (uint32_t i = 0u; i < draw_cmd_buffers.size(); ++i)
	{
		render_pass_begin_info.framebuffer = framebuffers[i];

		auto &cmd = draw_cmd_buffers[i];

		VK_CHECK(vkBeginCommandBuffer(cmd, &cmd_begin_info));

		vkCmdBeginRenderPass(cmd, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport = vkb::initializers::viewport(static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f);
		VkRect2D   scissor  = vkb::initializers::rect2D(static_cast<int32_t>(width), static_cast<int32_t>(height), 0, 0);

		vkCmdSetViewport(cmd, 0u, 1u, &viewport);
		vkCmdSetScissor(cmd, 0u, 1u, &scissor);

		model->draw(cmd);

		draw_ui(cmd);

		vkCmdEndRenderPass(cmd);

		VK_CHECK(vkEndCommandBuffer(cmd));
	}
}

void ddf_files::draw()
{
	ApiVulkanSample::prepare_frame();

	submit_info.commandBufferCount = 1u;
	submit_info.pCommandBuffers    = &draw_cmd_buffers[current_buffer];

	VK_CHECK(vkQueueSubmit(queue, 1u, &submit_info, VK_NULL_HANDLE));

	ApiVulkanSample::submit_frame();
}

std::unique_ptr<vkb::Application> create_ddf_files()
{
	return std::make_unique<ddf_files>();
}