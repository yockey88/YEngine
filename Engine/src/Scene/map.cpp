#include "machy.hpp"
#include "Scene/map.hpp"
#include <iostream>
#include <queue>

namespace machy {
namespace scene {

    Map::~Map() {
        for (auto it = vertices.begin(); it != vertices.end(); it++)
            delete it->second;
        
        for (auto it = plots.begin(); it != plots.end(); it++)
            delete it->second;
    }

    void Map::IJtoSDL(const int& i , const int& j , MapNode *& node) {
        int scrnH = MachY::Instance().getWindow().getScrnH();
        int scrnW = MachY::Instance().getWindow().getScrnW();

        node->pnt.x = (j * boxw) + ((scrnW - sl) / 2);
        node->pnt.y = (i * boxw) + ((scrnH - sl) / 2);

        return;
    }

    Vec2 Map::SDLtoIJ(MapNode *& node) {
        Vec2 ret;
        int scrnH = MachY::Instance().getWindow().getScrnH();
        int scrnW = MachY::Instance().getWindow().getScrnW();

        ret.x = node->pnt.y - (scrnH - sl) / 2;
        ret.y = node->pnt.x - (scrnW - sl) / 2;

        return ret;
    }

    void Map::IJtoSpos(const int& i , const int& j , MapNode *& node) {
        // node->pos.x = i - (sl / 2);
        // node->pos.y = j - (sl / 2);

        return;
    }

    Vec2 Map::SpostoIJ(MapNode *& node) {
        Vec2 ret;
        // ret.x = node->pos.x + (sl / 2);
        // ret.y = node->pos.y + (sl / 2);

        return ret;
    }

    std::tuple<int , int> Map::gridVertsToDraw(const int& i , const int& j) {
        int keyR  = -1, keyD = -1;
        if (i + 1 < numVertsPerCR)
            keyD = IJtoKey(i + 1 , j);
        if (j + 1 < numVertsPerCR)
            keyR = IJtoKey(i , j + 1);

        return {keyR , keyD};
    }

    void Map::makeBorders(SDL_Rect& top , SDL_Rect& bottom , SDL_Rect& left , SDL_Rect& right) {
        int scrnH = MachY::Instance().getWindow().getScrnH();
        int scrnW = MachY::Instance().getWindow().getScrnW();

        top.w = sl + boxw; bottom.w = sl;
        left.w = boxw / 2; right.w = boxw / 2;

        top.h = boxw / 2; bottom.h = boxw / 2;
        left.h = sl + (boxw / 2); right.h = sl + boxw;

        top.x = ((scrnW - sl) / 2) - (boxw / 2);
        top.y = ((scrnH - sl) / 2) - (boxw / 2);

        bottom.x = (scrnW - sl) / 2;
        bottom.y = (scrnH + sl) / 2;

        left.x = ((scrnW - sl) / 2) - (boxw / 2);
        left.y = (scrnH - sl) / 2;

        right.x = (scrnW + sl) / 2;
        right.y = ((scrnH - sl) / 2) - (boxw / 2);

        return;
    }

    void Map::createVerts() {
       MapNode* vert;

        for (int i = 0; i < numVertsPerCR; i++)
            for (int j = 0; j < numVertsPerCR; j++)
                makeNewVert(i , j , vert);

        return;
    }

    void Map::createEdges() {
        int key;

        for (int i = 0; i < numVertsPerCR; i++) 
            for (int j = 0; j< numVertsPerCR; j++) {
                key = IJtoKey(i , j);

                if (i - 1 >= 0)
                    addEdge(key , IJtoKey(i - 1 , j));
                if (j - 1 >= 0)
                    addEdge(key , IJtoKey(i , j - 1));

                if (i + 1 < numVertsPerCR)
                    addEdge(key , IJtoKey(i + 1 , j));
                if (j + 1 < numVertsPerCR)
                    addEdge(key , IJtoKey(i , j + 1));

                if (i - 1 >= 0 && j - 1 >= 0) 
                    addEdge(key , IJtoKey(i - 1 , j - 1));
                if (i - 1 >= 0 && j + 1 < numVertsPerCR)
                    addEdge(key , IJtoKey(i - 1 , j + 1));
                if (i + 1 < numVertsPerCR && j - 1 >= 0)
                    addEdge(key , IJtoKey(i + 1 , j - 1));
                if (i + 1 < numVertsPerCR && j + 1 < numVertsPerCR)
                    addEdge(key , IJtoKey(i + 1 , j + 1));
            }

        return;
    }

    void Map::createPlots() {
        Plot* p = new Plot;
        int numPlots = 0;

        for (int i = 0; i < numVertsPerCR - 1; i++)
            for (int j = 0; j < numVertsPerCR - 1; j++) {
                p->plotNum = numPlots;
                p->rect.x = vertices[IJtoKey(i , j)]->pnt.x;
                p->rect.y = vertices[IJtoKey(i , j)]->pnt.y;
                p->rect.w = boxw;
                p->rect.h = boxw;

                p->vertices[0] = vertices[IJtoKey(i , j)];
                p->vertices[1] = vertices[IJtoKey(i + 1 , j)];
                p->vertices[2] = vertices[IJtoKey(i , j + 1)];
                p->vertices[3] = vertices[IJtoKey(i + 1, j + 1)];

                p->center.x = p->vertices[0]->pnt.x + (boxw / 2);
                p->center.y = p->vertices[0]->pnt.y + (boxw / 2);

                std::cout << "<" << p->center.x << " , " << p->center.y << ">" << std::endl;

                plots[numPlots] = p;
                numPlots++;
            }

        return;
    }

    void Map::makeNewVert(const int& i , const int& j , MapNode *& vert) {
        int key = IJtoKey(i , j);
        int scrnH = MachY::Instance().getWindow().getScrnH();
        int scrnW = MachY::Instance().getWindow().getScrnW(); 

        vert = new MapNode;

        vert->key = key;

        vert->visited = false;

        vert->pnt.x = ((scrnW - sl) / 2) + (j * boxw);
        vert->pnt.y = ((scrnH - sl) / 2) + (i * boxw);

        vert->mapPos.x = i;
        vert->mapPos.y = j;

        vert->color.r = 255;
        vert->color.g = 255;
        vert->color.b = 255;
        vert->color.a = 255;

        IJtoSDL(i , j , vert);
        IJtoSpos(i , j , vert);

        vertices[key] = vert;
    
        return;
    }

    void Map::addEdge(int key1 , int key2) {
        AdjMapNode adj;
        adj.mn = vertices[key2];

        vertices[key1]->adj.push_back(adj);

        return;
    }

    void Map::renderEdges(SDL_Renderer *& renderer) {
        SDL_Rect top , bottom;
        SDL_Rect left , right;

        makeBorders(top , bottom , left , right);

        SDL_SetRenderDrawColor(renderer , 100 , 0 , 255 , 100);
        
        SDL_RenderDrawRect(renderer , &top);
        SDL_RenderFillRects(renderer , &top , 1);

        SDL_RenderDrawRect(renderer , &bottom);
        SDL_RenderFillRects(renderer , &bottom , 1);
        
        SDL_RenderDrawRect(renderer , &left);
        SDL_RenderFillRects(renderer , &left , 1);
        
        SDL_RenderDrawRect(renderer , &right);
        SDL_RenderFillRects(renderer , &right , 1);

        return;
    }

    void Map::renderGrid(SDL_Renderer *& renderer) {
        int key, keyR = -1, keyD = -1;
        SDL_SetRenderDrawColor(renderer , 255 , 255 , 255 , 255);
        for (int i = 0; i < numVertsPerCR; i++)
            for (int j = 0; j < numVertsPerCR; j++) {
                key = IJtoKey(i , j);
                auto [keyR , keyD] = gridVertsToDraw(i , j);

                SDL_RenderDrawPoint(renderer , vertices[key]->pnt.x , vertices[key]->pnt.y);
                if (keyR > 0 && keyR < numVerts)
                    SDL_RenderDrawLine(renderer , vertices[key]->pnt.x , vertices[key]->pnt.y , vertices[keyR]->pnt.x , vertices[keyR]->pnt.y);
                if (keyD > 0 && keyD < numVerts)
                    SDL_RenderDrawLine(renderer , vertices[key]->pnt.x , vertices[key]->pnt.y , vertices[keyD]->pnt.x , vertices[keyD]->pnt.y);
            }

        for (int i = 0; i < plots.size(); i++) {
            std::cout << plots[i]->rect.x << " , " << plots[i]->rect.y << std::endl;  
        } 

        return;
    }

    void Map::create(const int& size , const int& gridw) {
        sl = size;
        boxw = gridw;

        if (size % boxw != 0) {
            ERR("SCENE DIMENSIONS MUST BE A MULTIPLE OF PLOT DIMENSIONS | Program Corrupt");
            exit (1); 
        }

        numCR = sl / boxw;
        numVertsPerCR = numCR + 1;
        numVerts = numVertsPerCR * numVertsPerCR;
        numPlots = numCR * numCR;

        std::cout << std::endl;
        LOG("MAP SETTINGS INITIIALIZED");
        LOG("COL/ROWS -> " + std::to_string(numCR));
        LOG("NUM VERTS IN ROW/COL -> " + std::to_string(numVertsPerCR));
        LOG("TOTAL NUM VERTS IN -> " + std::to_string(numVerts));
        LOG("NUMBER OF SCENE PLOTS -> " + std::to_string(numPlots));

        LOG("CREATING VERTICES");
        createVerts();
        LOG("CONNECTING EDGES");
        createEdges();
        LOG("CREATING SCENE PLOTS");
        createPlots();

        std::cout << std::endl;

        return;
    }

    std::vector<std::string> Map::captureEdges() {
        std::vector<std::string> ret;
        std::string hold = "";

        for (int i = 0; i < vertices.size(); i++) {
            hold += "[Vertex: " + std::to_string(vertices[i]->key) + "]--> ";

            for (int j = 0; j < vertices[i]->adj.size(); j++) {
                hold += "[" + std::to_string(vertices[i]->adj[j].mn->key) + "] ";
            }
            ret.push_back(hold);
            hold = "";
        }

        return ret;
    }

    void Map::renderMap(SDL_Renderer *& renderer) {

        renderEdges(renderer);
        renderGrid(renderer);

        return;
    }

}
}