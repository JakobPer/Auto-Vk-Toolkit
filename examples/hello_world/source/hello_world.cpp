#include <gvk.hpp>
#include <imgui.h>
#include <imgui_impl_vulkan.h>

class draw_a_triangle_app : public gvk::invokee
{
public: // v== cgb::invokee overrides which will be invoked by the framework ==v
	draw_a_triangle_app(avk::queue& aQueue) : mQueue{ &aQueue }
	{}

	void initialize() override
	{
		// Print some information about the available memory on the selected physical device:
		gvk::context().print_available_memory_types();
		
		// Create a graphics pipeline:
		mPipeline = gvk::context().create_graphics_pipeline_for(
			avk::vertex_shader("shaders/a_triangle.vert"),
			avk::fragment_shader("shaders/a_triangle.frag"),
			avk::cfg::front_face::define_front_faces_to_be_clockwise(),
			avk::cfg::viewport_depth_scissors_config::from_framebuffer(gvk::context().main_window()->backbuffer_reference_at_index(0)),
			gvk::context().create_renderpass({ 
				avk::attachment::declare(
					// Copy the format from the main window's swap chain images:
					gvk::format_from_window_color_buffer(gvk::context().main_window()), 
					// Load => use as color attachment => store (for imgui_manager which renders into this image too)
					avk::on_load::clear.from_previous_layout(avk::layout::undefined), avk::usage::color(0), avk::on_store::store
					//                                                      ^^^ don't care about the previous layout, hence undefined
				) 
			})
		);
		
		// We want to use an updater => gotta create one:
		mUpdater.emplace();
		mUpdater->on(
			gvk::swapchain_resized_event(gvk::context().main_window()), // In the case of window resizes,
			gvk::shader_files_changed_event(mPipeline.as_reference())   // or in the case of changes to the shader files (hot reloading), ...
		)
		.update(mPipeline); // ... it will recreate the pipeline.		

		auto imguiManager = gvk::current_composition()->element_by_type<gvk::imgui_manager>();
		if(nullptr != imguiManager) {
			imguiManager->add_callback([this](){	
				bool isEnabled = this->is_enabled();
		        ImGui::Begin("Hello, world!");
				ImGui::SetWindowPos(ImVec2(1.0f, 1.0f), ImGuiCond_FirstUseEver);
				ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
				ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
				ImGui::Checkbox("Enable/Disable invokee", &isEnabled);				
				if (isEnabled != this->is_enabled())
				{					
					if (!isEnabled) this->disable();
					else this->enable();					
				}
				static std::vector<float> values;
				values.push_back(1000.0f / ImGui::GetIO().Framerate);
		        if (values.size() > 90) {
			        values.erase(values.begin());
		        }
	            ImGui::PlotLines("ms/frame", values.data(), static_cast<int>(values.size()), 0, nullptr, 0.0f, FLT_MAX, ImVec2(0.0f, 100.0f));
		        ImGui::End();
			});
		}
	}

	void update() override
	{
		// On H pressed,
		if (gvk::input().key_pressed(gvk::key_code::h)) {
			// log a message:
			LOG_INFO_EM("Hello Gears-Vk! Hello Auto-Vk!");
		}

		// On C pressed,
		if (gvk::input().key_pressed(gvk::key_code::c)) {
			// center the cursor:
			auto resolution = gvk::context().main_window()->resolution();
			gvk::context().main_window()->set_cursor_pos({ resolution[0] / 2.0, resolution[1] / 2.0 });
		}

		// On Esc pressed,
		if (gvk::input().key_pressed(gvk::key_code::escape)) {
			// stop the current composition:
			gvk::current_composition()->stop();
		}
	}

	/**	Render callback which is invoked by the framework every frame after every update() callback has been invoked.
	 *
	 *	Important: We must establish a dependency to the "swapchain image available" condition, i.e., we must wait for the
	 *	           next swap chain image to become available before we may start to render into it.
	 *			   This dependency is expressed through a semaphore, and the framework demands us to use it via the function:
	 *			   context().main_window()->consume_current_image_available_semaphore() for the main_window (our only window).
	 *
	 *			   More background information: At one point, we also must tell the presentation engine when we are done
	 *			   with rendering by the means of a semaphore. Actually, we would have to use the framework function:
	 *			   mainWnd->add_present_dependency_for_current_frame() for that purpose, but we don't have to do it in our case
	 *			   since we are rendering a GUI. imgui_manager will add a semaphore as dependency for the presentation engine.
	 */
	void render() override
	{
		auto mainWnd = gvk::context().main_window();

		// The main window's swap chain provides us with an "image available semaphore" for the current frame.
		// Only after the swapchain image has become available, we may start rendering into it.
		auto imageAvailableSemaphore = mainWnd->consume_current_image_available_semaphore();

		// Get a command pool to allocate command buffers from:
		auto& commandPool = gvk::context().get_command_pool_for_single_use_command_buffers(*mQueue);

		// Create a command buffer and render into the *current* swap chain image:
		auto cmdBfr = commandPool->alloc_command_buffer(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
		
		gvk::context().record({
				// Begin and end one renderpass:
				avk::command::render_pass(mPipeline->renderpass_reference(), gvk::context().main_window()->current_backbuffer_reference(), {
					// And within, bind a pipeline and draw three vertices:
					avk::command::bind_pipeline(mPipeline.as_reference()),
					avk::command::draw(3u, 1u, 0u, 0u)
				})
			})
			.into_command_buffer(cmdBfr)
			.then_submit_to(mQueue)
			.waiting_for(imageAvailableSemaphore >> avk::stage::color_attachment_output)
			.submit();

		// Use a convenience function of gvk::window to take care of the command buffer's lifetime:
		// It will get deleted in the future after #concurrent-frames have passed by.
		gvk::context().main_window()->handle_lifetime(std::move(cmdBfr));
	}

private: // v== Member variables ==v

	avk::queue* mQueue;
	avk::graphics_pipeline mPipeline;

}; // draw_a_triangle_app

int main() // <== Starting point ==
{
	int result = EXIT_FAILURE;
	try {
		// Create a window and open it
		auto mainWnd = gvk::context().create_window("Hello World");
		mainWnd->set_resolution({ 640, 480 });
		mainWnd->enable_resizing(true);
		mainWnd->set_presentaton_mode(gvk::presentation_mode::mailbox);
		mainWnd->set_number_of_concurrent_frames(3u);
		mainWnd->set_number_of_presentable_images(3u);
		mainWnd->open();

		auto& singleQueue = gvk::context().create_queue({}, avk::queue_selection_preference::versatile_queue, mainWnd);
		mainWnd->add_queue_family_ownership(singleQueue);
		mainWnd->set_present_queue(singleQueue);
		
		// Create an instance of our main "invokee" which contains all the functionality:
		auto app = draw_a_triangle_app(singleQueue);
		// Create another invokee for drawing the UI with ImGui
		auto ui = gvk::imgui_manager(singleQueue);

		// Compile all the configuration parameters and the invokees into a "composition":
		auto composition = configure_and_compose(
			gvk::application_name("Hello, Gears-Vk + Auto-Vk World!"),
			[](gvk::validation_layers& config) {
				config.enable_feature(vk::ValidationFeatureEnableEXT::eSynchronizationValidation);
			},
			// Pass windows:
			mainWnd,
			// Pass invokees:
			app, ui
		);

		// Create an invoker object, which defines the way how invokees/elements are invoked
		// (In this case, just sequentially in their execution order):
		gvk::sequential_invoker invoker;

		// With everything configured, let us start our render loop:
		composition.start_render_loop(
			// Callback in the case of update:
			[&invoker](const std::vector<gvk::invokee*>& aToBeInvoked) {
				// Call all the update() callbacks:
				invoker.invoke_updates(aToBeInvoked);
			},
			// Callback in the case of render:
			[&invoker](const std::vector<gvk::invokee*>& aToBeInvoked) {
				// Sync (wait for fences and so) per window BEFORE executing render callbacks
				gvk::context().execute_for_each_window([](gvk::window* wnd) {
					wnd->sync_before_render();
				});

				// Call all the render() callbacks:
				invoker.invoke_renders(aToBeInvoked);

				// Render per window:
				gvk::context().execute_for_each_window([](gvk::window* wnd) {
					wnd->render_frame();
				});
			}
		); // This is a blocking call, which loops until gvk::current_composition()->stop(); has been called (see update())
	
		result = EXIT_SUCCESS;
	}
	catch (gvk::logic_error&) {}
	catch (gvk::runtime_error&) {}
	catch (avk::logic_error&) {}
	catch (avk::runtime_error&) {}
	return result;
}
