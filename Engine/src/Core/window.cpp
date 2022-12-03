#include "machy.hpp"
#include "util.hpp"
#include "Core/window.hpp"
#include "Graphics/framebuffer.hpp"
#include "Graphics/vertex.hpp"
#include "Graphics/shader.hpp"
#include "Graphics/helper.hpp"
#include "Input/mouse.hpp"
#include "Input/keyboard.hpp"
#include "Input/joystick.hpp"

#include "glad/glad.h"

#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

namespace machy::core {

	WindowProperties::WindowProperties() {
		x = SDL_WINDOWPOS_CENTERED; y = x;
		w = 1920; h = 1080;
		wMin = 1024; hMin = 576;
		flags = SDL_WINDOW_OPENGL;
		aspectRatio = 16.f / 9.f;
		cc.x = static_cast<float>(0x64) / static_cast<float>(0xFF);
		cc.y = static_cast<float>(0x95) / static_cast<float>(0xFF);
		cc.z = static_cast<float>(0xED) / static_cast<float>(0xFF);
		title = "[Machine Y Appless Engine v{1.0.1}]";
	}

	void Window::initializeScrnRender() {

		vertArray = std::make_shared<graphics::VertexArray>();
		{
			MACHY_CREATE_VERTEX_BUFFER(vb , short);
			vb->pushVertex({  1 ,  1  ,  1 , 1 });
			vb->pushVertex({  1 , -1  ,  1 , 0 });
			vb->pushVertex({ -1 , -1  ,  0 , 0 });
			vb->pushVertex({ -1 ,  1  ,  0 , 1 });
			vb->setLayout({ 2 , 2 });
			vertArray->pushBuffer(std::move(vb));
		}
		vertArray->setElements({ 0 , 3 , 1 , 1 , 3 , 2 }); 
		vertArray->upload();

		vShader = util::readShaderFile("resources/shaders/basic_shader.vert");
		fShader = util::readShaderFile("resources/shaders/basic_shader.frag");
		shader = std::make_shared<graphics::Shader>(vShader.c_str() , fShader.c_str());

		return;
	}

	/* handleEvents() 
		@params--> nothing
		@return--> void
		
		-> filters all SDL and ImGui events into Engine
	*/
	void Window::handleEvents(SDL_Event &e) {
		switch (e.type) {
			case SDL_KEYDOWN: 
				switch (e.key.keysym.sym) {
					case SDLK_ESCAPE: MachY::Instance().quit(); break;
					default: break;
				}
			break;
			case SDL_WINDOWEVENT: 
				switch (e.window.event) {
					case SDL_WINDOWEVENT_CLOSE: MachY::Instance().quit(); break;
					case SDL_WINDOWEVENT_RESIZED: handleResize(e.window.data1 , e.window.data2); break;
					default: break;
				}
			break;
			case SDL_CONTROLLERDEVICEADDED: input::joystick::onJoystickConnected(e.cdevice); break;
			case SDL_CONTROLLERDEVICEREMOVED: input::joystick::onJoystickConnected(e.cdevice); break;
			default: break;
		}

		return;
	}

	void Window::renderToScreen() {
		MACHY_ASSERT(vertArray->isValid() , "Attempting to render with invalid VertexArray -> Did you forget to call upload()?");
		if (vertArray->isValid()) {
			glClearColor(0 , 0 , 0 , 1);
			vertArray->bind();
			glBindTexture(GL_TEXTURE_2D , frameBuffer->getTextureID()); MACHY_CHECK_GL_ERROR;
			shader->bind();

			glm::vec2 scale = fBufferSize / (glm::vec2)getSize();
			glm::mat4 model(1.0);
			model = glm::scale(model , {scale.x , scale.y , 1.f });
			shader->setUniformMat4("model" , model);
			glDrawElements(GL_TRIANGLES , vertArray->getEltCount() , GL_UNSIGNED_INT , 0); MACHY_CHECK_GL_ERROR;

			shader->unbind();
			glBindTexture(GL_TEXTURE_2D , 0); MACHY_CHECK_GL_ERROR;
			vertArray->unbind();
		}

		return;
	}

	void Window::handleResize(int w , int h) {

		int fBufferWidth = (int)(h * winProps.aspectRatio);
		int fBufferHeight = (int)(w * (1.f / winProps.aspectRatio));

		if (h >= fBufferHeight) {
			fBufferHeight = w;
		} else {
			fBufferHeight = h;
		}

		fBufferSize = { fBufferWidth , fBufferHeight };

		return;
	}

	/* create() 
		@params--> MachyWindowProps struct
		@return--> bool
			-- [successful window creation | true]
			-- [failure to create window | false]

			-> initializes SDL 
			-> creates OpenGL context
			-> sets window properties from App
		
		-> filters all SDL and ImGui events into Engine
	*/
	bool Window::create(const WindowProperties& props) {
		winProps = props;

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			MACHY_FATAL("SDL Initialization Failure -> {}" , SDL_GetError());
			return false;
		}

		window = SDL_CreateWindow(props.title.c_str() , props.x , props.y , props.w, props.h , props.flags);

		if (window == nullptr) {
			MACHY_FATAL("RENDERER CREATION FAILURE <SDL ERROR>-> {}" , SDL_GetError());
			return false;
		}

#ifdef MACHY_PLATFORM_MAC
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS , SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK , SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION , 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION , 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER , 1);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE , 8);

		SDL_SetWindowMinimumSize(window , 200 , 200);

		glContext = SDL_GL_CreateContext(window);
		if (glContext == nullptr) {
			MACHY_ERROR("OpenGl context is null | See SDL Error -> {}" , SDL_GetError());
			return false;
		}
		gladLoadGLLoader(SDL_GL_GetProcAddress);

		MACHY_TRACE("Creating GUI");
		gui.create(props.guiProps);
			
		open = true;

		frameBuffer = std::make_shared<graphics::Framebuffer>(props.w , props.h);
		glm::vec4 cc = { props.cc.r , props.cc.g , props.cc.b , 1.f };
		frameBuffer->setClearColor(cc);

		initializeScrnRender();
		handleResize(props.w , props.h);

		return true;
	}

	/* beginRender()
		@params--> nothing
		@return--> void

		-> clears screen
		-> begins queueing render commands
	*/
	void Window::beginRender() {
		auto& rm = MachY::Instance().getRM();
		rm.clear();
		rm.submit(MACHY_SUBMIT_RENDER_CMND(PushFramebuffer , frameBuffer));
		return;
	}

	/* flushEvents()
		@params--> nothing
		@returns-> void

		-> pools all events and filter them into system 
			-- see core::Window::handlEvents() for more	
	*/
	void Window::flushEvents() {
		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			handleEvents(e);

			gui.handleSDLEvent(e);

			if (!gui.wantMouse()) 
				input::mouse::update();

			if (!gui.wantKeyboard()) 
				input::keyboard::update();

			input::joystick::update();
		}

		return;
	}

	/* endRender()
		@params--> nothing
		@return--> void

		-> render gui
		-> renders app gui

		-> swaps buffer
	*/
	void Window::endRender() {

		auto& rm = MachY::Instance().getRM();
		rm.submit(MACHY_SUBMIT_RENDER_CMND(PopFramebuffer));
		rm.flush();

		if (renderToScrn) 
			renderToScreen();

		gui.beginRender();
		MachY::Instance().getApp().ImGuiRender();
		gui.endRender();

		SDL_GL_SwapWindow(window);

		return;
	}

	/* shutdown()
		@params--> nothing
		@returns--> void

		-> handles all necessary cleanup from window	
	*/
	void Window::shutdown() {
		
		gui.shutdown();
		if (window != nullptr) SDL_DestroyWindow(window);
		SDL_Quit();

		return;
	}

	/* getSize()
		@params-> integer ref width , integer ref height
		@returns-> void

		-> changes variables to extract SDL window width and height
	*/
	glm::ivec2 Window::getSize() {
		int w , h;
		SDL_GetWindowSize(window , &w , &h);
		return glm::ivec2{w , h};
	}

	glm::ivec2 Window::getCorrectAspectRatioSize(int w , int h) {

		int fBufferWidth = (int)(h * winProps.aspectRatio);
		int fBufferHeight = (int)(w * (1.f / winProps.aspectRatio));

		if (h >= fBufferHeight) {
			fBufferHeight = w;
		} else {
			fBufferHeight = h;
		}

		return { fBufferWidth , fBufferHeight };

	}

}
