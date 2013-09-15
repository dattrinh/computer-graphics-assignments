#pragma once

#include "gui/Window.hpp"
#include "gui/CommonControls.hpp"

#include <string>
#include <vector>


namespace FW {

struct Vertex
{
	Vec3f position;
	Vec3f normal;
};

struct glGeneratedIndices
{
	GLuint static_vao, dynamic_vao;
	GLuint shader_program;
	GLuint static_vertex_buffer, dynamic_vertex_buffer;
	GLuint model_to_world_uniform, world_to_clip_uniform, shading_toggle_uniform;
};

class App : public Window::Listener
{
private:
	enum CurrentModel
	{
		MODEL_EXAMPLE,
		MODEL_USER_GENERATED,
		MODEL_FROM_INDEXED_DATA,
		MODEL_FROM_FILE
	};

public:
						App();		// constructor
	virtual				~App() {}	// destructor

	virtual bool		handleEvent(const Window::Event& ev);

private:
						App(const App&);		// forbid copy
	App&				operator=(const App&);	// forbid assignment

	void				initRendering();
	void				render();
	std::vector<Vertex>	loadObjFileModel(std::string filename);

	Window				window_;
	CommonControls		common_ctrl_;

	CurrentModel		current_model_;
	bool				model_changed_;
	bool				shading_toggle_;
	bool				shading_mode_changed_;

	glGeneratedIndices	gl_;

	std::vector<Vertex>	vertices_;

	float				camera_rotation_angle_;

	// YOUR CODE HERE (R1)
	// Add a class member to store the current translation.
	Vec3f				translation_;

	float				scaling_;
	float				rotation_x_;
	float				rotation_y_;
	float				rotation_z_;


	
	std::vector<Vertex>	loadPLYFileModel(std::string filename);


	// EXTRA:
	// For animation extra credit you can use the framework's Timer class.
	// The .start() and .unstart() methods start and stop the timer; when it's
	// running, .end() gives you seconds passed after last call to .end().
	Timer timer_;
};

}
