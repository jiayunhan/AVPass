/*
 * Copyright (c) 2016 PolySync
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <cmath>
#include <iostream>
#include <unistd.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "GridMap.hpp"

using namespace cv;
using namespace std;

/**
 * \example GridMap.cpp
 *
 * PolySync Path Planner Example.
 *
 * Render a map with goal and _gol locations.  Check goals and legal moves by
 * comparing pixel color at a given cell.  Requires openCV
 *
 * @file GridMap.cpp
 * @brief GridMap Source
 *
 */
GridMap::GridMap( )
{

    srand(time(NULL));

}

GridMap::~GridMap( ) {

}

/**
 * @brief generateMap
 *
 * Overloaded function to read in an image file and generate a working map. Goal
 * generated randomly.
 *
 */
void GridMap::generateMap( )
{

    map = imread( _mapID, CV_LOAD_IMAGE_COLOR );

    resize( map, map, Size( nRows, nCols ) );

    threshold( map, map, 254.9, 255, THRESH_BINARY );

    if( !map.data )
    {
        printf( "Error loading src \n" );

        return;
    }

    generateGoal( );

    _staticMap = map.clone();
    _pathMap = map.clone();

    generateRobot( );

}

/**
 * @brief generateMap
 *
 * Overloaded function to read in an image file and generate a working map. Goal
 * generated at a specific location.
 *
 * @param int, int - x and y coordinates of desired goal location.
 */
void GridMap::generateMap( int x, int y )
{

    map = imread( _mapID, CV_LOAD_IMAGE_COLOR );

    resize( map, map, Size( nRows, nCols ) );

    threshold( map, map, 254.9, 255, THRESH_BINARY );

    if( !map.data )
    {
        printf( "Error loading src \n" );

        return;
    }

    generateGoal( x, y );

    _staticMap = map.clone();
    _pathMap = map.clone();

    generateRobot( );

    showMap( );

}

/**
 * @brief generateRobot
 *
 * Read in an image file and generate a working robot instance.
 *
 */
void GridMap::generateRobot( )
{

    _robot = imread( _robID, CV_LOAD_IMAGE_COLOR );

    resize( _robot, _robot, Size( robSize, robSize ) );

    robLoc[ 0 ][ 0 ] = nCols - robSize - 1;
    robLoc[ 0 ][ 1 ] = nRows - robSize - 1;

    fillQuad( robLoc, robSize );

    _robot.copyTo( map( cv::Rect(   robLoc[ 0 ][ 0 ],
                                    robLoc[ 0 ][ 1 ],
                                    robSize,
                                    robSize ) ) );

}

/**
 * @brief generateGoal
 *
 * Overloaded function to generate a working goal state at a pseudo-random
 * position.
 *
 */
void GridMap::generateGoal( )
{

    _gol = imread( _golID, CV_LOAD_IMAGE_COLOR );

    resize( _gol, _gol, Size( robSize, robSize ) );

    golLoc[ 0 ][ 0 ] = rand() % 50 + 150;
    golLoc[ 0 ][ 1 ] = rand() % 50 + 1;

    fillQuad( golLoc, robSize );

    _gol.copyTo( map( cv::Rect( golLoc[ 0 ][ 0 ],
                                golLoc[ 0 ][ 1 ],
                                robSize,
                                robSize ) ) );

}

/**
 * @brief generateGoal
 *
 * Overloaded function to generate a working goal state at a specific location.
 *
 * @param int, int - x and y coordinates of desired goal location.
 */
void GridMap::generateGoal( int x, int y )
{

    _gol = imread( _golID, CV_LOAD_IMAGE_COLOR );

    resize( _gol, _gol, Size( robSize, robSize ) );

    golLoc[ 0 ][ 0 ] = x;
    golLoc[ 0 ][ 1 ] = y;

    fillQuad( golLoc, robSize );

    _gol.copyTo( map( cv::Rect( golLoc[ 0 ][ 0 ],
                                golLoc[ 0 ][ 1 ],
                                robSize,
                                robSize ) ) );

}

/**
 * @brief moveRobot
 *
 * Move robot icon and location array to new location
 *
 * @param int, int - x and y coordinates of desired robot location.
 */
void GridMap::moveRobot( int x, int y )
{

    bool hit = checkHit( x, y, robSize );

    if ( hit == false )
    {

        robLoc[ 0 ][ 0 ] = x;
        robLoc[ 0 ][ 1 ] = y;

        fillQuad( robLoc, robSize );

        updateMap( );

    }

    else
    {
        return;
    }

    checkGoal( getIndexFromState( robLoc[ 0 ][ 0 ], robLoc[ 0 ][ 1 ] ) );

}

/**
 * @brief fillQuad
 *
 * Populate corners of icon using its size as delimiter.  This is used to check
 * hit, goal, or error conditions.
 *
 * @param int[4][2], int - location of upper left corner, icon size
 */
void GridMap::fillQuad( int ( &location )[ 4 ][ 2 ], int size )
{

    location[ 1 ][ 0 ] = location[ 0 ][ 0 ] + size;

    location[ 1 ][ 1 ] = location[ 0 ][ 1 ];

    location[ 2 ][ 0 ] = location[ 0 ][ 0 ];

    location[ 2 ][ 1 ] = location[ 0 ][ 1 ] + size;

    location[ 3 ][ 0 ] = location[ 1 ][ 0 ];

    location[ 3 ][ 1 ] = location[ 2 ][ 1 ];

}

/**
 * @brief showMap
 *
 * Clear current map render, populate current robot, query location, show image
 *
 */
void GridMap::showMap( )
{

    map = _staticMap.clone( );

    _robot.copyTo( map( cv::Rect(   robLoc[ 0 ][ 0 ],
                                    robLoc[ 0 ][ 1 ],
                                    robSize,
                                    robSize ) ) );

    imshow( "PolySync Path Planner Example", map );

    waitKey( 50 );

}

/**
 * @brief updateMap
 *
 * Clear current map render, populate current robot, query location, add the
 * current location as a grey dot (write a path), show image
 *
 */
void GridMap::updateMap( )
{

    int xCenter = int( floor( robLoc[ 0 ][ 0 ] + ( robSize / 2 ) ) );
    int yCenter = int( floor( robLoc[ 0 ][ 1 ] + ( robSize / 2 ) ) );

    map = _pathMap.clone();

    _pathMap.at< uchar >( Point( 3 * xCenter - 1, yCenter ) ) = 0;

    _robot.copyTo( map( cv::Rect(   robLoc[ 0 ][ 0 ],
                                    robLoc[ 0 ][ 1 ],
                                    robSize,
                                    robSize ) ) );

    imshow( "PolySync Path Planner Example", map );

    waitKey( 1 );

}

/**
 * @brief checkHit
 *
 * check corners of icon location for out of bounds, or illegal hit
 *
 * @param int, int, int - x, y coordinate of upper left corner, icon size
 */
bool GridMap::checkHit( int x, int y, int size )
{

    int tempLoc[ 4 ][ 2 ]{ x, y, 0, 0, 0, 0, 0, 0 };

    fillQuad( tempLoc, size );

    for ( int i = 0; i < 4; i++ )
    {
        if (map.at< uchar >( Point( 3 * tempLoc[ i ][ 0 ],
                                    tempLoc[ i ][ 1 ] ) ) == 0 )
        {

            return true;

        }

        else if (   tempLoc[ i ][ 0 ] < 0 || tempLoc[ i ][ 1 ] < 0 ||
                    tempLoc[ i ][ 0 ] >= nCols || tempLoc[ i ][ 1 ] >= nRows )
        {

            return true;

        }

    }

    return false;

}

/**
 * @brief checkGoal
 *
 * check whether index is at the goal state
 *
 * @param int - index of current position
 */
bool GridMap::checkGoal( int index )
{

    getStateFromIndex( index );

    if( abs( checkedMoveIndX - golLoc[ 0 ][ 0 ] ) <= robSize - 1 &&
        abs( checkedMoveIndY - golLoc[ 0 ][ 1 ] ) <= robSize - 1 )
    {

        return true;

    }

    return false;

}

/**
 * @brief checkMove
 *
 * Easier access to checkHit by passing index
 *
 * @param int, int - index of current position, icon size
 */
bool GridMap::checkMove( int index, int size )
{

    getStateFromIndex( index );

    if ( !checkHit( checkedMoveIndX, checkedMoveIndY, size ) )
    {

        return true;

    }

    return false;

}

/**
 * @brief getIndexFromState
 *
 * return index from x and y coordinates
 *
 * @param int, int - x and y map coordinates
 * @return int - index at x and y coordinates
 */
int GridMap::getIndexFromState( int x, int y )
{

    return int( y * nRows + x );

}

/**
 * @brief getStateFromIndex
 *
 * return x and y coordinates from index.  Accessed by public variable.
 *
 * @param int - index at x and y coordinates
 * @return int, int - x and y map coordinates
 */
void GridMap::getStateFromIndex( int index )
{

    checkedMoveIndX = index % nRows;
    checkedMoveIndY = index / nCols;

    return;

}
