
#ifndef TMXMAP_H
#define TMXMAP_H

#include <iostream> 
#include <vector> 
#include <string> 
#include <fstream>
#include <sstream>
#include <assert.h>
#include "..\RapidXML\rapidxml.hpp"

using namespace std;
using namespace rapidxml;

struct MOBjectProperty
{//Object property
	string name;
	string value;
};

struct MTileProperty : MOBjectProperty
{//Tile property
};

struct MTile
{
	MTileProperty prop; //Could possibly be a vector
	int id;
};

struct MTileset
{//Map Tileset
	string name_;
	string fileName_;
	int tileWidth_;
	int tileHeight_;
	int tileCount_;
	int firstgid_;
	vector<MTile>tile;
	string getTilePropertyName(int tileID)
	{//Will return a tile's property based on it's ID
		string t("");
		bool found(false);
		int counter(0);
		while (counter < static_cast<int> (tile.size()) && !found)
		{
			if (tile[counter].id == tileID)
			{
				found = true;
				t = tile[counter].prop.name;
			}
			++counter;
		}
		return(t);

	}
	string getTilePropertyValue(int tileID)
	{
		string t("");
		bool found(false);
		int counter(0);
		while (counter < static_cast<int> ( tile.size())  && !found)
		{
			if (tile[counter].id == tileID)
			{
				found = true;
				t = tile[counter].prop.value;
			}
			++counter;
		}
		return(t);
	}
};

struct MLayer
{
	string name;
	int width;
	int height;
	vector<vector<int>> data;
};

struct MObject
{
	MObject(int id, int x, int y, int width, int height) :
		id(id), x(x), y(y), width(width), height(height)
	{

	}
	int id;
	int x;
	int y;
	int width;
	int height;
	std::vector<MOBjectProperty>properties;
};

struct MObjectGroup
{//Object layer
	string name;
	vector<MObject> objects;
};


class MTileMap
{
public:
	~MTileMap();
	const vector<MTileset*>& getTilesets() const;
	bool loadMap(const std::string&);
	int getWidth() const { return width_; }
	int getHeight() const { return height_; }
	int getTileWidth() const { return tileWidth_; }
	int getTileHeight() const{ return tileHeight_; }
	int getLayerCount() const { return layers_.size(); }
	int getTilesetCount() const { return tilesets_.size(); }
	//Returns the total number of object groups (object layers) in the tiled map
	int getObjectGroupCount() const { return objectGroups_.size(); }
	const vector<MLayer*> getLayer() const { return(layers_); }
	const vector< MTileset*> getTileSet() const { return(tilesets_); }
	MObjectGroup getObjectGroup(int) const; //Param: id of the objectgroup 
private:
	string tmxToString(const std::string&);
	void setupTileMap();
	void setupTilesets(xml_node<>*);
	void setupLayer(xml_node<>*);
	void setupObjectGroups(xml_node<>*);
	int width_;  //Map width
	int height_; //Map height
	int tileWidth_;
	int tileHeight_;
	vector <MTileset*> tilesets_;
	vector <MLayer*> layers_;
	vector<MObjectGroup> objectGroups_;
	xml_document<> doc_;
};
#endif
