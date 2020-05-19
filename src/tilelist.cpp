/* TILE LIST
 * Name: Adonis Pugh
 * ------------------------
 * This file functionalizes the TileList class. The basic function of a TileList is to dynamically
 * alter and maintain appropriate ordering of objects. The class is specified to work with Tiles,
 * but the code can easily be generalized to order any collection of objects in the manner described
 * in this file. This class operates with an internal linked list and maintains just two global
 * pointers to the front and the back of the list. Traversal of nodes and redefinition of the two
 * pointers allows for a range of functionality of this ADT. */

#include "tilelist.h"
#include <algorithm>
#include "strlib.h"
using namespace std;

/* O(1) The linked list is initially constructed with a front pointer and back pointer. */
TileList::TileList() {
    front = nullptr;
    back = nullptr;
}

/* O(N) The destructor calls the clear function and deletes the front and back pointers. */
TileList::~TileList() {
    clear();
    delete front;
    delete back;
}

/* O(N) This function traverses the list and returns the tile node closest to the front
 * that contains the input x and y coordinates. */
TileNode* TileList::findTile(int x, int y) {
    for(TileNode* current = front; current != nullptr; current = current->next) {
        if(current->contains(x,y)) {
            return current;
        }
    }
    return nullptr;
}

/* O(1) A TileNode is detached from the list by redefining the previous and next pointers
 * of adjacent TileNodes. */
void TileList::detachTile(TileNode* tile) {
    if(tile != front) {
        tile->prev->next = tile->next;
    } else {
        front = tile->next; // moves front pointer one forward if node is first
    }
    if(tile != back) {
        tile->next->prev = tile->prev;
    } else {
        back = tile->prev;  // moves back pointer one backward if node is last
    }
}

/* O(1) A TileNode is taken as an input and is attached to the front of the list. */
void TileList::attachFront(TileNode* newFront) {
    newFront->prev = nullptr;
    if(front == nullptr) { // when list is empty
        front = newFront;
        back = front;
    } else {
        newFront->next = front;
        front->prev = newFront;
        front = newFront;
    }
}

/* O(1) This function calls attachFront after creating a new TileNode. */
void TileList::addFront(int x, int y, int width, int height, string color) {
    attachFront(new TileNode(x, y, width, height, color, nullptr, nullptr));
}

/* O(1) A TileNode is taken as an input and is attached to the back of the list. */
void TileList::attachBack(TileNode* newBack) {
    newBack->next = nullptr;
    if(front == nullptr) { // when list is empty
        front = newBack;
        back = front;
    } else {
        newBack->prev = back;
        back->next = newBack;
        back = newBack;
    }
}

/* O(1) This function calls attachBack after creating a new TileNode. */
void TileList::addBack(int x, int y, int width, int height, string color) {
    attachBack(new TileNode(x, y, width, height, color, nullptr, nullptr));
}

/* O(N) The list is emptied by defining a temporary pointer to the first TileNode
 * in the list, moving the front pointer one forward in the list, deleting the
 * old pointer, and so forth until the front pointer and back pointer equal. */
void TileList::clear() {
    while(front != back) {
        TileNode* current = front;
        front = front->next;
        delete current;
    }
    front = nullptr;
    back = nullptr;
}

/* O(N) This function traverses the list and prints out information about each TileNode. */
void TileList::debug() {
    for(TileNode* current = back; current != nullptr; current = current->prev) {
        cout << "Node " << *current << endl;
    }
}

/* O(N) The tiles are added to the window starting from the back TileNode to ensure earlier
 * tiles are added on top of later ones in the list. */
void TileList::drawAll(GWindow& window) const {
    for(TileNode* current = back; current != nullptr; current = current->prev) {
        current->draw(window);
    }
}

/* O(1) The front pointer is returned. */
TileNode* TileList::getFront() const {
    return front;
}

/* O(1) The back pointer is returned. */
TileNode* TileList::getBack() const {
    return back;
}

/* O(N) The first tile in the list that contains the input x and y coordinates
 * is located by calling findTile, after which the color of the tile is changed
 * to yellow to indicate highlighting. A boolean is returned indicating success. */
bool TileList::highlight(int x, int y) {
    TileNode* tile = findTile(x,y);
    if(tile != nullptr) {
        tile->color = "yellow";
        return true;
    }
    return false;
}

/* O(N) To raise a tile, the appropriate tile is first located. Once the tile
 * is found, it is detached from its current position in the list and attached
 * to the front. A boolean is returned indicating success. */
bool TileList::raise(int x, int y) {
    TileNode* tile = findTile(x, y);
    if(tile != nullptr && tile != front) {
        detachTile(tile);
        attachFront(tile);
        return true;
    }
    return false;
}

/* O(N) To lower a tile, the appropriate tile is first located. Once the tile
 * is found, it is detached from its current position in the list and attached
 * to the back. A boolean is returned indicating success. */
bool TileList::lower(int x, int y) {
    TileNode* tile = findTile(x, y);
    if(tile != nullptr && tile != back) {
        detachTile(tile);
        attachBack(tile);
        return true;
    }
    return false;
}

/* O(N) A tile is removed by first detaching it from the list followed by
 * its deletion. A boolean is returned indicating success. */
bool TileList::remove(int x, int y) {
    TileNode* tile = findTile(x, y);
    if(tile != nullptr) {
        detachTile(tile);
        delete tile;
        return true;
    }
    return false;
}

/* O(N^2) All tiles in the list are removed calling findTile, detaching the
 * TileNode returned, deleting the TileNode, and repeating until findTile
 * fails to locate TileNode at the given x and y coordinates. An integer is
 * returned indicating the number of tiles removed. */
int TileList::removeAll(int x, int y) {
    int removed = 0;
    for(TileNode* tile = findTile(x,y); tile != nullptr; tile = findTile(x,y)) {
        detachTile(tile);
        delete tile;
        removed++;
    }
    return removed;
}

/* O(N^2) The merge function works in a similar manner to removeAll, except that
 * the first located TileNode is initialized separately from the iteration so that
 * its respective x and y can used to initialize several variables used to keep
 * track of the smallest and largest x and y coordinates by comparing these values
 * to those of other TileNodes and modifying them if needed. This method results in
 * the smallest possible bounding box to contain all tiles located. At the end, a
 * single tile with the bounding box area and color of the first TileNode located
 * is attached to the front of the list. */
void TileList::merge(int x, int y) {
    int minX, minY, maxX, maxY;
    TileNode* tile = findTile(x, y);
    if(tile == nullptr) return;
    string color = tile->color;
    minX = tile->x;
    maxX = minX + tile->width;
    minY = tile->y;
    maxY = minY + tile->height;
    for(tile; tile != nullptr; tile = findTile(x,y)) {
        detachTile(tile);
        if (tile->x < minX) {
            minX = tile->x;
        }
        if(tile->x + tile->width > maxX) {
            maxX = tile->x + tile->width;
        }
        if(tile->y < minY) {
            minY = tile->y;
        }
        if(tile->y + tile->height > maxY) {
            maxY = tile->y + tile->height;
        }
        delete tile;
    }
    addFront(minX, minY, maxX - minX, maxY - minY, color);
}
