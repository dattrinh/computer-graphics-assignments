#define _CRT_SECURE_NO_WARNINGS

#include "App.hpp"

#include "utility.hpp"
#include "base/Main.hpp"
#include "gpu/GLContext.hpp"
#include "gpu/Buffer.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <type_traits>

using namespace FW;
using namespace std;

// Anonymous namespace. This is a C++ way to define things which
// do not need to be visible outside this file.
namespace {

static const float translation_step = 0.05f;
static const float scaling_step = 0.01f;
static const float rotation_step = 1.00f;
F32 elapsed = 0;
static bool ANIMATION_ENABLED = false;

enum TranslationIndices {
	INDEX_X = 0,
	INDEX_Y = 1,
	INDEX_Z = 2,
};

enum VertexShaderAttributeLocations {
	ATTRIB_POSITION = 0,
	ATTRIB_NORMAL = 1,
	ATTRIB_COLOR = 2
};

const Vertex reference_plane_data[] = {
	{ Vec3f(-1, -1, -1), Vec3f(0, 1, 0) },
	{ Vec3f( 1, -1, -1), Vec3f(0, 1, 0) },
	{ Vec3f( 1, -1,  1), Vec3f(0, 1, 0) },
	{ Vec3f(-1, -1, -1), Vec3f(0, 1, 0) },
	{ Vec3f( 1, -1,  1), Vec3f(0, 1, 0) },
	{ Vec3f(-1, -1,  1), Vec3f(0, 1, 0) }
};

vector<Vertex> loadExampleModel() {
	static const Vertex example_data[] = {
		{ Vec3f( 0.0f,  0.5f, 0), Vec3f(0.0f, 0.0f, -1.0f) },
		{ Vec3f(-0.5f, -0.5f, 0), Vec3f(0.0f, 0.0f, -1.0f) },
		{ Vec3f( 0.5f, -0.5f, 0), Vec3f(0.0f, 0.0f, -1.0f) }
	};
	vector<Vertex> vertices;
	for (auto v : example_data)
		vertices.push_back(v);
	return vertices;
}

vector<Vertex> unpackIndexedData(
	const vector<Vec3f>& positions,
	const vector<Vec3f>& normals,
	const vector<array<unsigned, 6>>& faces)
{
	vector<Vertex> vertices;

	// This is a 'range-for' loop which goes through all objects in the container 'faces'.
	// '&' gives us a reference to the object inside the container; if we omitted '&',
	// 'f' would be a copy of the object instead.
	// The compiler already knows the type of objects inside the container, so we can
	// just write 'auto' instead of having to spell out 'array<unsigned, 6>'.
	for (auto& f : faces) {

		// YOUR CODE HERE (R3)
		// Unpack the indexed data into a vertex array. For every face, you have to create three vertices and add them to the vector 'vertices'.

		// f[0] is the index of the position of the first vertex
		// f[1] is the index of the normal of the first vertex
		// f[2] is the index of the position of the second vertex
		// ...		
		Vertex v0 = { positions[f[0]], normals[f[1]] };
		Vertex v1 = { positions[f[2]], normals[f[3]] };
		Vertex v2 = { positions[f[4]], normals[f[5]] };
		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
	}

	return vertices;
};

// This is for testing your unpackIndexedData implementation.
// You should get a tetrahedron like in example.exe.
vector<Vertex> loadIndexedDataModel() {
	
	static const Vec3f point_data[] = {
		Vec3f(0.0f, 0.407f, 0.0f),
		Vec3f(0.0f, -0.3f, -0.5f),
		Vec3f(0.433f, -0.3f, 0.25f),
		Vec3f(-0.433f, -0.3f, 0.25f),
	};
	static const Vec3f normal_data[] = {
		Vec3f(0.8165f, 0.3334f, -0.4714f),
		Vec3f(0.0f, 0.3334f, 0.9428f),
		Vec3f(-0.8165f, 0.3334f, -0.4714f),
		Vec3f(0.0f, -1.0f, 0.0f)
	};
	static const unsigned face_data[][6] = {
		{0, 0, 1, 0, 2, 0},
		{0, 2, 3, 2, 1, 2},
		{0, 1, 2, 1, 3, 1},
		{1, 3, 3, 3, 2, 3}
	};
	vector<Vec3f> points(point_data, point_data + SIZEOF_ARRAY(point_data));
	vector<Vec3f> normals(normal_data, normal_data + SIZEOF_ARRAY(normal_data));
	vector<array<unsigned, 6>> faces;
	for (auto arr : face_data) {
		array<unsigned, 6> f;
		copy(arr, arr+6, f.begin());
		faces.push_back(f);
	}
	return unpackIndexedData(points, normals, faces);
}

// Generate an upright cone with tip at (0, 0, 0), a radius of 0.25 and a height of 1.0.
// You can leave the base of the cone open, like it is in example.exe.
vector<Vertex> loadUserGeneratedModel() {
	static const float radius = 0.25f;
	static const float height = 1.0f;
	static const unsigned faces = 40;
	static const float angle_increment = 2 * FW_PI / faces;

	// Empty array of Vertex structs; every three vertices = one triangle
	vector<Vertex> vertices;
	
	Vertex v0, v1, v2;

	// Generate one face at a time
	for(auto i = 0u; i < faces; ++i)	{
		// YOUR CODE HERE (R2)
		// Figure out the correct positions of the three vertices of this face.
		// v0.position = ...
		// Calculate the normal of the face from the positions and use it for all vertices.
		// v0.normal = v1.normal = v2.normal = ...;
		// 
		// Some hints:
		// - Try just making a triangle in fixed coordinates at first.
		// - "FW::cos(angle_increment * i) * radius" gives you the X-coordinate
		//    of the ith vertex at the base of the cone. Z-coordinate is very similar.
		// - For the normal calculation, you'll want to use the cross() function for
		//   cross product, and Vec3f's .normalized() or .normalize() methods.
		
		// Then we add the vertices to the array.
		// .push_back() grows the size of the vector by one, copies its argument,
		// and places the copy at the back of the vector.
		v0.position =  Vec3f( 0.0f,  0.0f, 0.0f);
		v1.position =  Vec3f(FW::cos(angle_increment * i) * radius, -1.0f, FW::sin(angle_increment * i) * radius);
		v2.position =  Vec3f(FW::cos(angle_increment * (i+1)) * radius, -1.0f, FW::sin(angle_increment * (i+1)) * radius);

		Vec3f edge1 = v1.position - v0.position;
		Vec3f edge2 = v2.position - v0.position;
		Vec3f normal = cross(edge1, edge2);

		v0.normal = v1.normal = v2.normal = normal;
		v0.normal.normalize();
		v1.normal.normalize();
		v2.normal.normalize();
		vertices.push_back(v0); vertices.push_back(v1); vertices.push_back(v2);
	}
	return vertices;
}

}

App::App(void)
:   common_ctrl_			(CommonControls::Feature_Default & ~CommonControls::Feature_RepaintOnF5),
	current_model_			(MODEL_EXAMPLE),
	model_changed_			(true),
	shading_toggle_			(false),
	shading_mode_changed_	(false),
	camera_rotation_angle_	(0.0f)
{
	static_assert(is_standard_layout<Vertex>::value, "struct Vertex must be standard layout to use offsetof");
	initRendering();
	
	common_ctrl_.showFPS(true);
	common_ctrl_.addToggle((S32*)&current_model_, MODEL_EXAMPLE,			FW_KEY_1, "Triangle (1)",				&model_changed_);
	common_ctrl_.addToggle((S32*)&current_model_, MODEL_USER_GENERATED,		FW_KEY_2, "Generated cone (2)",			&model_changed_);
	common_ctrl_.addToggle((S32*)&current_model_, MODEL_FROM_INDEXED_DATA,	FW_KEY_3, "Unpacked tetrahedron (3)",	&model_changed_);
	common_ctrl_.addToggle((S32*)&current_model_, MODEL_FROM_FILE,			FW_KEY_4, "Model loaded from file (4)",	&model_changed_);
	common_ctrl_.addSeparator();
	common_ctrl_.addToggle(&shading_toggle_,								FW_KEY_T, "Toggle shading mode (T)",	&shading_mode_changed_);

	window_.setTitle("Assignment 0");

	window_.addListener(this);
	window_.addListener(&common_ctrl_);

	window_.setSize( Vec2i(800, 800) );
}

bool App::handleEvent(const Window::Event& ev) {
	
	if (ANIMATION_ENABLED) {
		elapsed += timer_.end();
		if (elapsed*1000 >= 20) {
			elapsed= 0;
			camera_rotation_angle_ += 0.01 * FW_PI;
		}
		timer_.start();
	}
	
	if (model_changed_)	{
		model_changed_ = false;
		switch (current_model_)
		{
		case MODEL_EXAMPLE:
			vertices_ = loadExampleModel();
			break;
		case MODEL_FROM_INDEXED_DATA:
			vertices_ = loadIndexedDataModel();
			break;
		case MODEL_USER_GENERATED:
			vertices_ = loadUserGeneratedModel();
			break;
		case MODEL_FROM_FILE:
			{
				auto filename = window_.showFileLoadDialog("Load new mesh");
				if (filename.getLength()) {
					
					String name = filename.getPtr();
					
					Array<String> pieces;
					name.split('.', pieces);

					if (pieces.getSize() < 2) 
					{
						assert(false && "invalid model type");
						current_model_ = MODEL_EXAMPLE;
						model_changed_ = true;
					}
					else {
						if (pieces[1] == "obj") {
							vertices_ = loadObjFileModel(filename.getPtr());
						} else if (pieces[1] == "ply") {
							vertices_ = loadPLYFileModel(filename.getPtr());
						} else {
							assert(false && "invalid model type");
							current_model_ = MODEL_EXAMPLE;
							model_changed_ = true;
						}
					}
				} else {
					current_model_ = MODEL_EXAMPLE;
					model_changed_ = true;
				}
			}
			break;
		default:
			assert(false && "invalid model type");
		}
		// Load the vertex buffer to GPU.
		glBindBuffer(GL_ARRAY_BUFFER, gl_.dynamic_vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	if (shading_mode_changed_) {
		common_ctrl_.message(shading_toggle_ ?
			"Directional light shading using vertex normals; direction to light (0.5, 0.5, -0.6)" :
			"High visibility shading, color from vertex ID");
		shading_mode_changed_ = false;
	}

	if (ev.type == Window::EventType_KeyDown) {
		// YOUR CODE HERE (R1)
		// React to user input and move the model. Look in framework/gui/Keys.hpp for more key codes.
		// Visual Studio tip: you can right-click an identifier like FW_KEY_HOME  and "Go to definition" to jump directly to where the identifier is defined.
		if (ev.key == FW_KEY_HOME)
		{
			camera_rotation_angle_ -= 0.05 * FW_PI;
		}
		else if (ev.key == FW_KEY_END)
		{
			camera_rotation_angle_ += 0.05 * FW_PI;
		}
		else if (ev.key ==  FW_KEY_RIGHT) 
		{
			translation_[INDEX_X] += translation_step;
		}
		else if (ev.key ==  FW_KEY_LEFT) 
		{
			translation_[INDEX_X] -= translation_step;
		}
		else if (ev.key ==  FW_KEY_UP) 
		{
			translation_[INDEX_Y] += translation_step;
		}
		else if (ev.key ==  FW_KEY_DOWN) 
		{
			translation_[INDEX_Y] -= translation_step;
		}
		else if (ev.key ==  FW_KEY_PAGE_UP) 
		{
			translation_[INDEX_Z] += translation_step;
		}
		else if (ev.key ==  FW_KEY_PAGE_DOWN) 
		{
			translation_[INDEX_Z] -= translation_step;
		}
		else if (ev.key ==  FW_KEY_MINUS) 
		{
			scaling_ -=	scaling_step;
		}
		else if (ev.key ==  FW_KEY_PLUS) 
		{
			scaling_ +=	scaling_step;
		}
		else if (ev.key ==  FW_KEY_X) 
		{
			rotation_x_ += rotation_step;
		}
		else if (ev.key ==  FW_KEY_Y) 
		{
			rotation_y_ += rotation_step;
		}
		else if (ev.key ==  FW_KEY_Z) 
		{
			rotation_z_ += rotation_step;
		}
		else if (ev.key ==  FW_KEY_R) 
		{
			ANIMATION_ENABLED =! ANIMATION_ENABLED;
		}
	}
	
	if (ev.type == Window::EventType_KeyUp) {
	}

	if (ev.type == Window::EventType_Mouse) {
		// EXTRA: you can put your mouse controls here.
		// Event::mouseDelta gives the distance the mouse has moved.
		// Event::mouseDragging tells whether some mouse buttons are currently down.
		// If you want to know which ones, you have to keep track of the button down/up events
		// (e.g. FW_KEY_MOUSE_LEFT).
	}

	if (ev.type == Window::EventType_Close) {
		window_.showModalMessage("Exiting...");
		delete this;
		return true;
	}

	window_.setVisible(true);
	if (ev.type == Window::EventType_Paint)
		render();

	window_.repaint();

	return false;
}

void App::initRendering() {
	
	scaling_ = 1.00;
	rotation_x_ = 0.00;
	rotation_y_ = 0.00;
	rotation_z_ = 0.00;
	
	// Ask the Nvidia framework for the GLContext object associated with the window.
	// As a side effect, this initializes the OpenGL context and lets us call GL functions.
	auto ctx = window_.getGL();
	
	// Create vertex attribute objects and buffers for vertex data.
	glGenVertexArrays(1, &gl_.static_vao);
	glGenVertexArrays(1, &gl_.dynamic_vao);
	glGenBuffers(1, &gl_.static_vertex_buffer);
	glGenBuffers(1, &gl_.dynamic_vertex_buffer);
	
	// Set up vertex attribute object for static data.
	glBindVertexArray(gl_.static_vao);
	glBindBuffer(GL_ARRAY_BUFFER, gl_.static_vertex_buffer);
	glEnableVertexAttribArray(ATTRIB_POSITION);
	glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) 0);
	glEnableVertexAttribArray(ATTRIB_NORMAL);
	glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));

	// Load the static data to the GPU; needs to be done only once.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * SIZEOF_ARRAY(reference_plane_data), reference_plane_data, GL_STATIC_DRAW);
	
	// Set up vertex attribute object for dynamic data. We'll load the actual data later, whenever the model changes.
	glBindVertexArray(gl_.dynamic_vao);
	glBindBuffer(GL_ARRAY_BUFFER, gl_.dynamic_vertex_buffer);
	glEnableVertexAttribArray(ATTRIB_POSITION);
	glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) 0);
	glEnableVertexAttribArray(ATTRIB_NORMAL);
	glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	// Compile and link the shader program.
	// We use the Nvidia FW for creating the program; it's not difficult to do manually,
	// but takes about half a page of OpenGL boilerplate code.
	// This shader program will be used to draw everything except the user interface.
	// It consists of one vertex shader and one fragment shader.
	auto shader_program = new GLContext::Program(
		"#version 330\n"
		FW_GL_SHADER_SOURCE(
		layout(location = 0) in vec4 aPosition;
		layout(location = 1) in vec3 aNormal;
		
		out vec4 vColor;
		
		uniform mat4 uModelToWorld;
		uniform mat4 uWorldToClip;
		uniform float uShading;
		
		const vec3 distinctColors[6] = vec3[6](
			vec3(0, 0, 1), vec3(0, 1, 0), vec3(0, 1, 1),
			vec3(1, 0, 0), vec3(1, 0, 1), vec3(1, 1, 0));
		const vec3 directionToLight = normalize(vec3(0.5, 0.5, -0.6));
		
		void main()
		{
			// EXTRA: oops, someone forgot to transform normals here...
			float clampedCosine = clamp(dot(aNormal, directionToLight), 0.0, 1.0);
			vec3 litColor = vec3(clampedCosine);
			vec3 generatedColor = distinctColors[gl_VertexID % 6];
			// gl_Position is a built-in output variable that marks the final position
			// of the vertex in clip space. Vertex shaders must write in it.
			gl_Position = uWorldToClip * uModelToWorld * aPosition;
			vColor = vec4(mix(generatedColor, litColor, uShading), 1);
		}
		),
		"#version 330\n"
		FW_GL_SHADER_SOURCE(
		in vec4 vColor;
		out vec4 fColor;
		void main()
		{
			fColor = vColor;
		}
		));
	// Tell the FW about the program so it gets properly destroyed at exit.
	ctx->setProgram("shaders", shader_program);

	// Get the IDs of the shader program and its uniform input locations from OpenGL.
	gl_.shader_program = shader_program->getHandle();
	gl_.world_to_clip_uniform = glGetUniformLocation(gl_.shader_program, "uWorldToClip");
	gl_.model_to_world_uniform = glGetUniformLocation(gl_.shader_program, "uModelToWorld");
	gl_.shading_toggle_uniform = glGetUniformLocation(gl_.shader_program, "uShading");
}

void App::render() {

	// Simple viewpoint correction when window is resized
	S32 width = window_.getSize()[0]; 
	S32 height = window_.getSize()[1]; 
	S32 max_value = (width >= height) ? height: width;
	glViewport((width-max_value)/2, (height - max_value)/2, max_value, max_value);

	// Clear screen.
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Enable depth testing.
	glEnable(GL_DEPTH_TEST);
	
	// Set up a matrix to transform from world space to clip space.
	// Clip space is a [-1, 1]^3 space where OpenGL expects things to be
	// when it starts drawing them.

	// Our camera is aimed at origin, and orbits around origin at fixed distance.
	static const float camera_distance = 2.1f;	
	Mat4f C;
	Mat3f rot = Mat3f::rotation(Vec3f(0, 1, 0), -camera_rotation_angle_);
	C.setCol(0, Vec4f(rot.getCol(0), 0));
	C.setCol(1, Vec4f(rot.getCol(1), 0));
	C.setCol(2, Vec4f(rot.getCol(2), 0));
	C.setCol(3, Vec4f(0, 0, camera_distance, 1));
	
	// Simple perspective.
	static const float fnear = 0.1f, ffar = 4.0f;
	Mat4f P;
	P.setCol(0, Vec4f(1, 0, 0, 0));
	P.setCol(1, Vec4f(0, 1, 0, 0));
	P.setCol(2, Vec4f(0, 0, (ffar+fnear)/(ffar-fnear), 1));
	P.setCol(3, Vec4f(0, 0, -2*ffar*fnear/(ffar-fnear), 0));

	Mat4f world_to_clip = P * C;
	
	// Set active shader program.
	glUseProgram(gl_.shader_program);
	glUniform1f(gl_.shading_toggle_uniform, shading_toggle_ ? 1.0f : 0.0f);
	glUniformMatrix4fv(gl_.world_to_clip_uniform, 1, GL_FALSE, world_to_clip.getPtr());

	// Draw the reference plane. It is already in world coordinates.
	auto identity = Mat4f();
	glUniformMatrix4fv(gl_.model_to_world_uniform, 1, GL_FALSE, identity.getPtr());
	glBindVertexArray(gl_.static_vao);
	glDrawArrays(GL_TRIANGLES, 0, SIZEOF_ARRAY(reference_plane_data));
	
	static const float angle = 2 * FW_PI / 360;

	/* Performs the scaling FIRST, and THEN the rotation, and THEN the translation */
	Mat4f scaling_m;
	scaling_m.setCol(0, Vec4f(scaling_, 0, 0, 0));
	scaling_m.setCol(1, Vec4f(0, scaling_, 0, 0));
	scaling_m.setCol(2, Vec4f(0, 0, scaling_, 0));
	scaling_m.setCol(3, Vec4f(0, 0, 0, 1));

	Mat4f rotation_xm;
	float phi = angle*rotation_x_;
	rotation_xm.setCol(0, Vec4f(1, 0, 0, 0));
	rotation_xm.setCol(1, Vec4f(0, FW::cos(phi), -FW::sin(phi), 0));
	rotation_xm.setCol(2, Vec4f(0, FW::sin(phi), FW::cos(phi), 0));
	rotation_xm.setCol(3, Vec4f(0, 0, 0, 1));

	Mat4f rotation_ym;
	float theta = angle*rotation_y_;
	rotation_ym.setCol(0, Vec4f(FW::cos(theta), 0, FW::sin(theta), 0));
	rotation_ym.setCol(1, Vec4f(0, 1, 0, 0));
	rotation_ym.setCol(2, Vec4f(FW::sin(theta), 0, -FW::cos(theta), 0));
	rotation_ym.setCol(3, Vec4f(0, 0, 0, 1));

	Mat4f rotation_zm;
	float psi = angle*rotation_z_;
	rotation_zm.setCol(0, Vec4f(FW::cos(psi), -FW::sin(psi), 0, 0));
	rotation_zm.setCol(1, Vec4f(FW::sin(psi), FW::cos(psi), 0, 0));
	rotation_zm.setCol(2, Vec4f(0, 0, 1, 0));
	rotation_zm.setCol(3, Vec4f(0, 0, 0, 1));

	Mat4f rotation_m = rotation_xm * rotation_ym * rotation_zm;

	Mat4f translation_m;
	translation_m.setCol(3, Vec4f(translation_, 1));

	// YOUR CODE HERE (R1)
	// Set the model space -> world space transform to translate the model according to user input.		
	Mat4f modelToWorld; // Creates a new matrix with the value set to the identity transform.	
	//modelToWorld.setCol(3, Vec4f(translation_, 1)); // Notes: Opengl Uses column-major matrix ordering

	modelToWorld = modelToWorld * translation_m;
	modelToWorld = modelToWorld * rotation_m;
	modelToWorld = modelToWorld * scaling_m;
	
	// Draw the model with your model-to-world transformation.
	glUniformMatrix4fv(gl_.model_to_world_uniform, 1, GL_FALSE, modelToWorld.getPtr());
	glBindVertexArray(gl_.dynamic_vao);
	glDrawArrays(GL_TRIANGLES, 0, vertices_.size());

	// Undo our bindings.
	glBindVertexArray(0);
	glUseProgram(0);
	
	// Check for OpenGL errors.
	GLContext::checkErrors();
	
	// Show status messages. You may find it useful to show some debug information in a message.
	common_ctrl_.message(sprintf("Use Home/End to rotate camera."), "instructions");
	common_ctrl_.message(sprintf("Camera is at (%.2f %.2f %.2f) looking towards origin.",
		-FW::sin(camera_rotation_angle_) * camera_distance, 0.0f,
		-FW::cos(camera_rotation_angle_) * camera_distance), "camerainfo");
}

enum PlyPart {
	PLY_HEADER = 0,
	PLY_BODY
};

vector<Vertex> App::loadPLYFileModel(string filename) {
	window_.showModalMessage(sprintf("Loading mesh from '%s'...", filename.c_str()));

	vector<Vec3f> positions, normals;
	vector<array<unsigned, 6>> faces;

	array<unsigned, 6>  f; // Face index array
	Vec3f               v;

	/* Open input file stream for reading. */
	ifstream input(filename, ios::in);

	/* Read the file line by line. */
	string line, s, s1;
	PlyPart part = PLY_HEADER;
	unsigned linenum = 0;
	unsigned verticesNum = 0, facesNum = 0;

	/* Added unused vectors to avoid indexing errors >> Note that in C++ we index things starting from 0, but face indices in OBJ format start from 1.  */
	positions.push_back(Vec3f(0,0,0));
	normals.push_back(Vec3f(0,0,0));

	while(getline(input, line)) {
		linenum++;

		char *line_ptr = (char*) line.c_str();	
		float i0, i1, i2;
		unsigned n, a, b, c, normalNum = 0;

		/* PARSE HEADER */
		istringstream        iss(line);
		iss >> s;

		/* end of parse of header */
		if (s == "end_header") {
			part = PLY_BODY;
		}
		else {
			switch (part) {
				case PLY_HEADER:
					if (s == "element") {
						
						iss >> s1;

						if (s1 == "vertex"){
							sscanf(line_ptr, "element vertex %d", &verticesNum);
						}
						else if (s1 == "face"){
							sscanf(line_ptr, "element face %d", &facesNum);
						}
					}
					break;

				case PLY_BODY:				
					/* vertices */
					if (verticesNum > 0)
					{
						sscanf(line_ptr, "%f %f %f", &i0, &i1, &i2);
						v.set(Vec3f(i0, i1, i2));
						positions.push_back(v);
						verticesNum--;
					}
					/* faces */
					else if (facesNum > 0)
					{
						sscanf(line_ptr, "%d %d %d %d", &n, &a, &b, &c);

						/* use same normal to all faces */						
						v.set(Vec3f(0, 0, 1));
						normals.push_back(v);
					
						f[0] = a;
						f[1] = normalNum; // normal
						f[2] = b;
						f[3] = normalNum; // normal
						f[4] = c;
						f[5] = normalNum; // normal
						faces.push_back(f);

						facesNum--;
						normalNum++;
					}
					break;
			}
		}
	}

	common_ctrl_.message(("Loaded mesh from " + filename).c_str());
	return unpackIndexedData(positions, normals, faces);
}

vector<Vertex> App::loadObjFileModel(string filename) {
	window_.showModalMessage(sprintf("Loading mesh from '%s'...", filename.c_str()));

	vector<Vec3f> positions, normals;
	vector<array<unsigned, 6>> faces;

	// Open input file stream for reading.
	ifstream input(filename, ios::in);
	
	// Read the file line by line.
	string line;
	unsigned linenum = 0;

	/* Added unused vectors to avoid indexing errors >> Note that in C++ we index things starting from 0, but face indices in OBJ format start from 1.  */
	positions.push_back(Vec3f(0,0,0));
	normals.push_back(Vec3f(0,0,0));
	
	while(getline(input, line)) {
		// Replace any '/' characters with spaces ' ' so that all of the
		// values we wish to read are separated with whitespace.

		/* We dont need this one anymore. */
		/*for (auto& c : line) if (c == '/') 	c = ' ';*/
		
		/* For debugging and checking read line */
		linenum++;

		// Temporary objects to read data into.
		array<unsigned, 6>  f; // Face index array
		Vec3f               v;
		string              s;

		// Create a stream from the string to pick out one value at a time.
		istringstream        iss(line);

		// Read the first token from the line into string 's'.
		// It identifies the type of object (vertex or normal or ...)
		iss >> s;

		char *line_ptr = (char*) line.c_str();
		float i0, i1, i2;
		unsigned a_, b_, c_, d_, e_, f_, g_, h_, i_;

		if (s == "v") { // vertex position
			// YOUR CODE HERE (R4)
			// Read the three vertex coordinates (x, y, z) into 'v'.
			// Store a copy of 'v' in 'positions'.
			// See std::vector documentation for push_back.			
			sscanf(line_ptr, "v %f %f %f", &i0, &i1, &i2);
			v.set(Vec3f(i0, i1, i2));
			positions.push_back(v);
		} else if (s == "vn") { // normal
			// YOUR CODE HERE (R4)
			// Similar to above.
			sscanf(line_ptr, "vn %f %f %f", &i0, &i1, &i2);
			v.set(Vec3f(i0, i1, i2));
			normals.push_back(v);
		} else if (s == "f") { // face
			// YOUR CODE HERE (R4)
			// Read the indices representing a face and store it in 'faces'.
			// The data in the file is in the format
			// f v1/vt1/vn1 v2/vt2/vn2 ...
			// where vi = vertex index, vti = texture index, vni = normal index.
			//
			// Remember we already replaced the '/' characters with whitespaces.
			//
			// Since we are not using textures in this exercise, you can ignore
			// the texture indices by reading them into a temporary variable.
			sscanf(line_ptr, "f %d/%d/%d %d/%d/%d %d/%d/%d", &f[0], &b_, &f[1], &f[2], &e_, &f[3], &f[4], &h_, &f[5]);						
			faces.push_back(f);

			unsigned sink; // Temporary variable for reading the unused texture indices.
			// Note that in C++ we index things starting from 0, but face indices in OBJ format start from 1.
			// If you don't adjust for that, you'll index past the range of your vectors and get a crash.

			// It might be a good idea to print the indices to see that they were read correctly.
			// cout << f[0] << " " << f[1] << " " << f[2] << " " << f[3] << " " << f[4] << " " << f[5] << endl;
		}
	}
	common_ctrl_.message(("Loaded mesh from " + filename).c_str());
	return unpackIndexedData(positions, normals, faces);
}

void FW::init(void) {
	new App;
}
