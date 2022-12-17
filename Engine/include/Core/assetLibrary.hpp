#ifndef ASSET_LIBRARY_HPP
#define ASSET_LIBRARY_HPP

#include "log.hpp"

#include <string>
#include <memory>
#include <unordered_map>

namespace machy {
namespace core {

    template <typename T>
    class AssetLibrary {
        std::unordered_map<std::string , std::shared_ptr<T>> assets;
        public:
            AssetLibrary() = default;
            ~AssetLibrary() = default;

            const std::unordered_map<std::string , std::shared_ptr<T>>& getAllAssets() { return assets; }

            inline bool exists(const std::string& name) { return (assets.find(name) != assets.end()); }

            void deleteAsset(const std::string& name) {
                if (exists(name)) {
                    auto itr = assets.find(name);
                    auto useCount = itr->second.use_count();
                    if (useCount > 1) {
                        MACHY_INFO("AssetLibrary::Delete() -> remaining handles for asset {} -> {}" , name , useCount);
                    }
                }
            }

            void load(const std::string& name , std::shared_ptr<T> asset) {
                assets[name] = asset;
            }

            std::shared_ptr<T> get(const std::string& name) {
                if (exists(name)) {
                    return assets[name];
                } else {
                    MACHY_WARN("AssetLibrary::get({}) failed, {} not found" , name , name);
                    return nullptr;
                }

            }
    };

}
}

#endif