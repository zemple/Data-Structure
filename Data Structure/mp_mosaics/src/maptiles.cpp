/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    map<Point<3>, TileImage*> m;
    vector<Point<3>> points;
    int x = theSource.getRows();
    int y = theSource.getColumns();
    MosaicCanvas* output = new MosaicCanvas(x, y);
    for (auto iter = theTiles.begin(); iter != theTiles.end(); iter++) {
        Point<3> tmp = convertToXYZ(iter->getAverageColor());
        points.push_back(tmp);
        m[tmp] = &*iter;
    }
    KDTree<3> tree(points);
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            output->setTile(i, j, m[tree.findNearestNeighbor(convertToXYZ(theSource.getRegionColor
            (i, j)))]);
        }
    }
    return output;
}

