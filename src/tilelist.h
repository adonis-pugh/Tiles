/* TILE LIST
 * Name: Adonis Pugh
 * SL: Ketan Agrawal
 * __________________
 * This file is a layout of the implementation of an abstract data type. The ADT
 * implemented is a linked list for a collection of Tiles. The anticipated
 * utilization of this ADT would be for standard user interactions with multiple
 * windows on a computer display. This class offers a high level of abstraction
 * to allow simplicity for the user. */

#ifndef _tilelist_h
#define _tilelist_h

#include <iostream>
#include <string>
#include "gwindow.h"
#include "tilenode.h"
using namespace std;

class TileList {
public:
    TileList();
    ~TileList();
    void addBack(int x, int y, int width, int height, string color);
    void addFront(int x, int y, int width, int height, string color);
    void clear();
    void debug();
    void drawAll(GWindow& window) const;
    TileNode* getBack() const;
    TileNode* getFront() const;
    bool highlight(int x, int y);
    bool lower(int x, int y);
    void merge(int x, int y);
    bool raise(int x, int y);
    bool remove(int x, int y);
    int removeAll(int x, int y);

private:
    TileNode* front;
    TileNode* back;
    TileNode* findTile(int x, int y);
    void detachTile(TileNode* unwanted);
    void attachFront(TileNode* newFront);
    void attachBack(TileNode* newBack);

};

#endif // _tilelist_h
