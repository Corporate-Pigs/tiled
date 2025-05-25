#pragma once

#include <fstream>
#include <istream>
#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <vector>

static void loadJsonFromFile(std::string path, nlohmann::json& json)
{
    std::ifstream file(path);
    assert(file.is_open());
    file >> json;
    file.close();
}

struct TiledTileSheetTileAnimationFrame
{
    uint32_t tileId;
    double duration;
};

struct TiledTileSheetTileObject
{
    float id, x, y, height, width;
    std::string type;
};

struct TiledProperty
{
    std::string name;
    std::variant<bool, std::string> value;
};

struct TiledTileSheetTileObjectGroup
{
    std::vector<TiledTileSheetTileObject> objects;
};

struct TiledTileSheetTile
{
    std::string type;
    uint32_t id;
    TiledTileSheetTileObjectGroup objectGroup;
    std::vector<TiledProperty> properties;
    std::vector<TiledTileSheetTileAnimationFrame> keyframes;
};

struct TiledTileMapLayerObject
{
    uint32_t id, x, y;
    std::string name, type;
    std::vector<TiledProperty> properties;
};

struct TiledTileMapLayerTile
{
    uint32_t id;
    bool flippedHorizontally;
    bool flippedVertically;
    bool flippedDiagonally;
};

struct TiledTileMapLayer
{
    uint32_t id;
    std::string name;
    std::vector<TiledTileMapLayerObject> objects;
    std::vector<TiledTileMapLayerTile> data;
    std::vector<TiledTileMapLayer> layers;
};

struct TiledTileSheet
{
    std::string image;
    uint32_t id, columns, rows, tileWidth, tileHeight, imageWidth, imageHeight;
    std::unordered_map<uint32_t, TiledTileSheetTile> tiles;
    static void fromJson(const std::string path, TiledTileSheet& p);
};

struct TiledTileMapTileSet
{
    TiledTileSheet sheet;
    uint32_t firstId;
};

struct TiledTileMap
{
    uint32_t width, height, tileWidth, tileHeight;
    std::vector<TiledTileMapTileSet> tileSets;
    std::vector<TiledTileMapLayer> layers;

    static void fromJson(const std::string path, TiledTileMap& p);
    const TiledTileMapTileSet* getTileSetPtrForTileId(uint32_t tileId) const;
};

void from_json(const nlohmann::json& j, TiledTileSheetTileObjectGroup& p);
void from_json(const nlohmann::json& j, TiledTileSheetTileObject& p);
void from_json(const nlohmann::json& j, TiledTileSheetTileAnimationFrame& p);
void from_json(const nlohmann::json& j, TiledProperty& p);
void from_json(const nlohmann::json& j, TiledTileSheetTile& p);
void from_json(const nlohmann::json& j, TiledTileSheet& p);
void from_json(const nlohmann::json& j, TiledTileMapLayerObject& p);
void from_json(const nlohmann::json& j, TiledTileMapLayer& p);
void from_json(const nlohmann::json& j, TiledTileMapLayerTile& p);
void from_json(const nlohmann::json& j, TiledTileMapTileSet& p);
void from_json(const nlohmann::json& j, TiledTileMap& p);
