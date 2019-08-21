#pragma once
// ReSharper disable CppUnusedIncludeDirective

// ------ commonly used stuff from the standard library ------
#include <vector>
#include <deque>
#include <array>
#include <string>
#include <string_view>
#include <exception>
#include <stdexcept>
#include <unordered_map>
#include <map>
#include <functional>
#include <memory>
#include <iostream>
#include <ostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <variant>
#include <iomanip>
#include <optional>
#include <typeinfo>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <typeindex>
#include <type_traits>
#include <utility>
#include <cstdint>

#include <stdio.h>
#include <assert.h>

// ----------------------- externals -----------------------
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <stb_image.h>

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/anim.h>

#include <imgui.h>

#include "cpplinq.hpp"

// GLM: 
#if defined(USE_VULKAN_CONTEXT)
// DEFINES:
#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
// The perspective projection matrix generated by GLM will use the OpenGL depth range of -1.0 to 1.0 by default. 
// We need to configure it to use the Vulkan range of 0.0 to 1.0 using the GLM_FORCE_DEPTH_ZERO_TO_ONE definition. [1]
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/quaternion_exponential.hpp>
#include <glm/ext/quaternion_relational.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/mat4x4.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/hash.hpp>

// -------------------- windows include ---------------------
#ifdef _WIN32
// Include that after the assimp includes due to some conflict
// with the global scope operator :: in material.inl
#define NOMINMAX
#include <windows.h>
#endif

// -------------------- CG-Base includes --------------------
#include "string_utils.hpp"
#include "log.hpp"
#include "essential_utils.hpp"
#include "memory_usage.hpp"
#include "context_state.hpp"
#include "context_tracker.hpp"
#include "device_queue_selection_strategy.hpp"
#include "shader_type.hpp"
#include "shader_info.hpp"

#if defined(USE_OPENGL_CONTEXT)
#include <glad/glad.h>
#include "buffer_member_format_opengl.hpp"
#include "image_opengl.hpp"
#include "image_view_opengl.hpp"
#elif defined(USE_VULKAN_CONTEXT)
#include <vulkan/vulkan.hpp>
#include "buffer_member_format_vulkan.hpp"
#include "image_vulkan.hpp"
#include "image_view_vulkan.hpp"
#endif

#include "attachment.hpp"
#include "buffer_data.hpp"
#include "buffer.hpp"
#include "input_description.hpp"
#include "push_constants.hpp"
#include "filter_mode.hpp"
#include "border_handling_mode.hpp"

#if defined(USE_OPENGL_CONTEXT)
#include "context_generic_glfw_types.hpp"
#include "window_base.hpp"
#include "context_generic_glfw.hpp"
#include "sampler_opengl.hpp"
#include "image_sampler_opengl.hpp"
#include "command_buffer_opengl.hpp"
#include "semaphore_opengl.hpp"
#include "fence_opengl.hpp"
#include "buffer_opengl.hpp"
#include "renderpass_opengl.hpp"
#include "framebuffer_opengl.hpp"
#include "window_opengl.hpp"
#include "shader_opengl.hpp"
#include "binding_data_opengl.hpp"
#include "graphics_pipeline_config.hpp"
#include "pipeline_opengl.hpp"
#include "bindings_opengl.hpp"

#include "imgui_impl_opengl3.h"

#include "context_opengl.hpp"
#include "context.hpp"
#elif defined(USE_VULKAN_CONTEXT)
#include "context_generic_glfw_types.hpp"
#include "window_base.hpp"
#include "context_generic_glfw.hpp"
#include "sampler_vulkan.hpp"
#include "image_sampler_vulkan.hpp"
#include "command_buffer_vulkan.hpp"
#include "command_pool_vulkan.hpp"
#include "semaphore_vulkan.hpp"
#include "fence_vulkan.hpp"
#include "buffer_vulkan.hpp"
#include "queue_vulkan.hpp"
#include "renderpass_vulkan.hpp"
#include "framebuffer_vulkan.hpp"
#include "window_vulkan.hpp"
#include "acceleration_structure_vulkan.hpp"
#include "shader_vulkan.hpp"
#include "binding_data_vulkan.hpp"
#include "graphics_pipeline_config.hpp"
#include "descriptor_set_vulkan.hpp"
#include "descriptor_pool_vulkan.hpp"
#include "pipeline_vulkan.hpp"
#include "shader_binding_table_vulkan.hpp"
#include "bindings_vulkan.hpp"
#include "vulkan_helper_functions.hpp"

#include "imgui_impl_vulkan.h"

#include "context_vulkan.hpp"
#include "context.hpp"
// [1] Vulkan Tutorial, Rendering and presentation, https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Rendering_and_presentation
// [2] Vulkan Tutorial, Vertex buffer creation, https://vulkan-tutorial.com/Vertex_buffers/Vertex_buffer_creation
// [3] Vulkan Tutorial, Images, https://vulkan-tutorial.com/Texture_mapping/Images
// [4] Vulkan Tutorial, Image view and sampler, https://vulkan-tutorial.com/Texture_mapping/Image_view_and_sampler
#endif

#include "graphics_pipeline_config_convenience_functions.hpp"

#include "image.hpp"
#include "various_utils.hpp"
#include "math_utils.hpp"
#include "key_code.hpp"
#include "key_state.hpp"
#include "timer_interface.hpp"
#include "timer_frame_type.hpp"
#include "fixed_update_timer.hpp"
#include "varying_update_timer.hpp"

#include "input_buffer.hpp"
#include "composition_interface.hpp"

namespace cgb
{
#pragma region global data representing the currently active composition
	/**	@brief Get the current timer, which represents the current game-/render-time
	 *	\remark This is just a shortcut to @ref composition_interface::current()->time();
	 */
	inline timer_interface& time()
	{
		return composition_interface::current()->time();
	}

	/** @brief Get the current frame's input data
	 *	\remark This is just a shortcut to @ref composition_interface::current()->input();
	 */
	inline input_buffer& input()
	{
		return composition_interface::current()->input();
	}

	/** @brief Get access to the currently active objects
	 *	\remark This is just a shortcut to @ref *composition_interface::current();
	 */
	inline composition_interface& current_composition()
	{
		return *composition_interface::current();
	}
#pragma endregion 
}

#include "cg_element.hpp"
#include "sequential_executor.hpp"
#include "composition.hpp"

#include "transform.hpp"
#include "material_config.hpp"
#include "material_gpu_data.hpp"
#include "material.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "quake_camera.hpp"

// ReSharper restore CppUnusedIncludeDirective
