#include "tiled.h"

static constexpr unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
static constexpr unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;
static constexpr unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;
static constexpr unsigned ROTATED_HEXAGONAL_120_FLAG = 0x10000000;

extern std::string tiledResourcesPath;

void from_json(const nlohmann::json& j, TiledTileSheetTileObjectGroup& p) { j.at("objects").get_to(p.objects); }

void from_json(const nlohmann::json& j, TiledTileSheetTileObject& p)
{
    j.at("id").get_to(p.id);
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
    j.at("height").get_to(p.height);
    j.at("width").get_to(p.width);
    j.at("type").get_to(p.type);
}

void from_json(const nlohmann::json& j, TiledTileSheetTileAnimationFrame& p)
{
    j.at("duration").get_to(p.duration);
    p.duration /= 1000;
    j.at("tileid").get_to(p.tileId);
}

void from_json(const nlohmann::json& j, TiledProperty& p)
{
    j.at("name").get_to(p.name);
    auto value = j.at("value");

    if (value.is_boolean())
    {
        p.value = value.get<bool>();
        return;
    }

    if (value.is_string())
    {
        p.value = value.get<std::string>();
        return;
    }
}

void from_json(const nlohmann::json& j, TiledTileSheetTile& p)
{
    j.at("id").get_to(p.id);
    if (j.contains("type"))
    {
        j.at("type").get_to(p.type);
    }
    if (j.contains("animation"))
    {
        j.at("animation").get_to(p.keyframes);
    }
    if (j.contains("properties"))
    {
        j.at("properties").get_to(p.properties);
    }
    if (j.contains("objectgroup"))
    {
        j.at("objectgroup").get_to(p.objectGroup);
    }
}

void from_json(const nlohmann::json& j, TiledTileSheet& p)
{
    j.at("columns").get_to(p.columns);
    j.at("image").get_to(p.image);
    j.at("imageheight").get_to(p.imageHeight);
    j.at("imagewidth").get_to(p.imageWidth);
    j.at("tileheight").get_to(p.tileHeight);
    j.at("tilewidth").get_to(p.tileWidth);
    p.rows = p.imageHeight / p.tileHeight;
    if (j.contains("tiles"))
    {
        std::vector<TiledTileSheetTile> tiles;
        j.at("tiles").get_to(tiles);
        for (const auto& tile : tiles)
        {
            p.tiles[tile.id] = tile;
        }
    }
}

void from_json(const nlohmann::json& j, TiledTileMapLayerObject& p)
{
    j.at("id").get_to(p.id);
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
    j.at("name").get_to(p.name);
    j.at("type").get_to(p.type);
    if (j.contains("properties"))
    {
        j.at("properties").get_to(p.properties);
    }
}

void from_json(const nlohmann::json& j, TiledTileMapLayer& p)
{
    j.at("id").get_to(p.id);
    j.at("name").get_to(p.name);
    if (j.contains("objects"))
    {
        j.at("objects").get_to(p.objects);
    }
    if (j.contains("data"))
    {
        j.at("data").get_to(p.data);
    }
    if (j.contains("layers"))
    {
        j.at("layers").get_to(p.layers);
    }
}

void from_json(const nlohmann::json& j, TiledTileMapLayerTile& p)
{
    uint32_t globalTileId = j;
    p.flippedHorizontally = (globalTileId & FLIPPED_HORIZONTALLY_FLAG);
    p.flippedVertically = (globalTileId & FLIPPED_VERTICALLY_FLAG);
    p.flippedDiagonally = (globalTileId & FLIPPED_DIAGONALLY_FLAG);

    // bool rotatedHex120 = (globalTileId & ROTATED_HEXAGONAL_120_FLAG);

    // Clear all four flags
    globalTileId &=
        ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG | ROTATED_HEXAGONAL_120_FLAG);
    p.id = globalTileId;
}

void from_json(const nlohmann::json& j, TiledTileMapTileSet& p)
{
    std::string source;
    j.at("source").get_to(source);
    TiledTileSheet::fromJson(tiledResourcesPath + source, p.sheet);

    j.at("firstgid").get_to(p.firstId);
}

void from_json(const nlohmann::json& j, TiledTileMap& p)
{
    j.at("tilewidth").get_to(p.tileWidth);
    j.at("width").get_to(p.width);
    j.at("tileheight").get_to(p.tileHeight);
    j.at("height").get_to(p.height);
    j.at("tilesets").get_to(p.tileSets);
    j.at("layers").get_to(p.layers);
}

const TiledTileMapTileSet* TiledTileMap::getTileSetPtrForTileId(uint32_t tileId) const
{
    for (int32_t i = tileSets.size() - 1; i > -1; i--)
    {
        auto& tileSet = tileSets[i];
        if (tileSet.firstId <= tileId)
        {
            return &tileSets[i];
        }
    }
    return nullptr;
}

void TiledTileMap::fromJson(const std::string path, TiledTileMap& p)
{
    nlohmann::json json;
    loadJsonFromFile(path, json);
    p = json.get<TiledTileMap>();
}

void TiledTileSheet::fromJson(const std::string path, TiledTileSheet& p)
{
    nlohmann::json json;
    loadJsonFromFile(path, json);
    p = json.get<TiledTileSheet>();
}