#ifndef MACHY_HPP
#define MACHY_HPP

#include "app.hpp"

#include "Core/window.hpp"
#include "Core/assetLibrary.hpp"
#include "Core/deltaTime.hpp"

#include "Managers/logManager.hpp"
#include "Managers/renderManager.hpp"

#include <iostream>
#include <chrono>
#include <memory>

namespace machy {

	class MachY {
		static MachY* instance;
        App* ActiveApp;

		core::Window window;
		core::Timer timer;
		
		managers::LogManager log;
        managers::RenderManager renderer;

		core::AssetLibrary<graphics::VertexArray> VALib;
		core::AssetLibrary<graphics::Shader> ShaderLib;
		core::AssetLibrary<graphics::Texture> TextureLib;
		core::AssetLibrary<graphics::Material> MaterialLib;

		bool initialized, running;

		std::string name;
        std::string version;

		MachY() : ActiveApp(nullptr) , initialized(false), running(false) , name("Machine Y") , version("{1.0.2}") {}

		[[nodiscard]] bool init(App* app);
		void initLibs();

		void update();
		void updateInput();
		void render();
		
		void shutdown();

		MachY(const MachY&) = delete;
		MachY& operator=(const MachY&) = delete;

		public:
			static MachY& Instance();
			~MachY() { shutdown();  }
			void getInfo();

			void run(App* app);

			inline void quit() { running = false; }

			inline App& getApp() { return *ActiveApp; }
			inline core::Window& getWindow() { return window; }
			inline managers::LogManager& getLog() { return log; }
			inline managers::RenderManager& getRM() { return renderer; }
	};

}

#endif