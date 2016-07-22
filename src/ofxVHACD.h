//
//  ofxVHACD.h
//
//  Created by Yuya Hanai on 2016/07/22.
//
//
#pragma once

#include "ofMain.h"
#include "VHACD.h"

class ofxVHACD
{
protected:
    VHACD::IVHACD* interfaceVHACD = nullptr;
public:
    ofxVHACD();
    ~ofxVHACD();
    
    bool createConvexHullMesh(const ofMesh& mesh_in,
                              vector<ofMesh>& mesh_out,
                              VHACD::IVHACD::Parameters* param = NULL);
};