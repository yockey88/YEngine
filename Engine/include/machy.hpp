#ifndef MACHY_HPP
#define MACHY_HPP

#include "state.hpp"
#include "app.hpp"

#define LOG(data) machy::MachY::Instance().Log(data);
#define ERR(data) machy::MachY::Instance().Err(data);

namespace machy {

	class MachY {
		static MachY* instance;
		
		std::unique_ptr<GlobalState> state;

		bool initialized, running;

		MachY() : initialized(false), running(false) {}

		[[nodiscard]] bool init(App *& app);
					
		void mainLoop();

		void update();
		void render();
		
		void shutdown();

		MachY(const MachY&) = delete;
		MachY& operator=(const MachY&) = delete;

		public:
			static MachY& Instance();
			~MachY() { shutdown();  }
			void getInfo();

			void run(App *& app);

			inline void quit() { running = false; }

			inline machy::core::Window& getWindow() { return state->window; }
			inline machy::managers::LogManager& getLog() { return state->log; }
			inline machy::managers::SceneManager& getScenes() { return state->scenes; }

			inline void Log(const std::string& data) { state->log.log(data); }
			inline void Err(const std::string& err) { state->log.error(err); }
	};

}

#endif