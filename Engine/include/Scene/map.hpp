#ifndef MAP_HPP
#define MAP_HPP

#include <SDL.h>
#include <tuple>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

namespace machy {
namespace scene {

    struct Vec2 {
        int x , y;
    };

    struct RGB {
        int r , g , b , a;
    };

    struct MapNode;
    struct AdjMapNode {
        MapNode* mn;
    };

    struct MapNode {
        std::vector<AdjMapNode> adj;
        Vec2 mapPos;
        Vec2 scenePos;
        Vec2 SDLpos;
        RGB color;
        SDL_Point pnt;
        int key;
        bool visited;
    };

    struct Plot {
        MapNode* vertices[4];
        SDL_Rect rect;
        Vec2 center;
        int plotNum;
    };

    class Map {
        std::map<int , MapNode*> vertices;
        std::map<int , Plot*> plots;
        int sl , boxw;
        int numVertsPerCR , numVerts;
        int numCR;
        int numPlots;

        inline int IJtoKey(const int& x , const int &y) { return y + numVertsPerCR * x; }
        void IJtoSDL(const int& i , const int& j , MapNode *& node);
        Vec2 SDLtoIJ(MapNode *& node);
        void IJtoSpos(const int& i , const int& j , MapNode *& node);
        Vec2 SpostoIJ(MapNode *& node);

        inline int getRightKey(const int& key) { return ((key + 1) > numVertsPerCR) ? -1 : key + 1; }
        inline int getLeftKey(const int& key) { return ((key - 1) < 0) ? -1 : key - 1; }
        inline int getDownKey(const int& key) { return ((key + numVertsPerCR) > numVerts) ? -1 : key + numVertsPerCR; }
        inline int getUpKey(const int& key) { return ((key - numVertsPerCR) < 0) ? -1 : key - numVertsPerCR; }
        std::tuple<int , int> gridVertsToDraw(const int& i , const int& j);

        void makeBorders(SDL_Rect& rectt , SDL_Rect& rectb , SDL_Rect& rectl , SDL_Rect& rectr);

        /////////////// 
        void createVerts();
        void createEdges();
        void createPlots();

        void makeNewVert(const int& i , const int& j , MapNode *& node);

        void addEdge(int key1 , int key2);

        void renderGrid(SDL_Renderer *& renderer);

        Map(const Map&) = delete;
		Map& operator=(const Map&) = delete;

        public:
            Map() : sl(-1) , boxw(-1) , numCR(-1) , numPlots(-1) {}
            ~Map();

            void create(const int& size , const int& gridw);

            std::vector<std::string> captureEdges();
            std::vector<Plot**> capturePlots();

            void renderEdges(SDL_Renderer *& renderer);
            void renderMap(SDL_Renderer *& renderer);

            inline int getWidth() const { return sl; }
            inline int getGridDim() const { return boxw; }
            inline int getNumCR() const { return numCR; }
            inline int getNumPlots() const { return numPlots; }

            void destroy();
    };

}
}

#endif 