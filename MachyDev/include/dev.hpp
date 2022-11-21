#ifndef MACHY_DEV_HPP
#define MACHY_DEV_HPP

#include "app.hpp"

namespace machy {

    class Dev : public App {
        AppData initData;

        public:
            Dev() {}
            virtual ~Dev() override {}

            virtual void InitNoArgs() override;

            virtual inline AppData* getData() override { return &initData; };
    };

}

#endif