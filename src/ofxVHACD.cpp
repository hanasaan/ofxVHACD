//
//  ofxVHACD.cpp
//
//  Created by Yuya Hanai on 2016/07/22.
//
//

#include "ofxVHACD.h"
#include "VHACD.h"

using namespace VHACD;

ofxVHACD::ofxVHACD()
{
    interfaceVHACD = CreateVHACD();
}

ofxVHACD::~ofxVHACD()
{
    interfaceVHACD->Clean();
    interfaceVHACD->Release();
}

bool ofxVHACD::createConvexHullMesh(const ofMesh& mesh_in, vector<ofMesh>& mesh_out,
                                    VHACD::IVHACD::Parameters* param)
{
    if (mesh_in.getMode() != OF_PRIMITIVE_TRIANGLES) {
        return false;
    }
    
    IVHACD::Parameters params;
    if (param) {
        params = *param;
    }
    params.m_oclAcceleration = 0;
    
    // const float* const points, const unsigned int stridePoints, const unsigned int nPoints,
    // const int* const triangles, const unsigned int strideTriangles, const unsigned int nTriangles
    
    bool res = interfaceVHACD->Compute(
                (const float* const)&mesh_in.getVertices().front().x, 3,
                (const unsigned int)mesh_in.getVertices().size(),
                (const int* const )mesh_in.getIndexPointer(), 3, (const unsigned int)mesh_in.getIndices().size() / 3, params);
    if (res) {
        // save output
        unsigned int nConvexHulls = interfaceVHACD->GetNConvexHulls();
        ofLog() << "+ Generate output: " << nConvexHulls << " convex-hulls " << endl;
        IVHACD::ConvexHull ch;
        for (unsigned int p = 0; p < nConvexHulls; ++p) {
            interfaceVHACD->GetConvexHull(p, ch);
            ofMesh m;
            m.setMode(OF_PRIMITIVE_TRIANGLES);
            for (int i=0; i<ch.m_nTriangles*3; ++i) {
                m.addIndex(ch.m_triangles[i]);
            }
            for (int i=0; i<ch.m_nPoints; ++i) {
                m.addVertex(ofVec3f(ch.m_points[i*3], ch.m_points[i*3+1], ch.m_points[i*3+2]));
            }
            mesh_out.push_back(m);
//                SaveVRML2(foutCH, ch.m_points, ch.m_triangles, ch.m_nPoints, ch.m_nTriangles, mat, myLogger);
        }
    }
    else {
        ofLogError() << "Decomposition cancelled by user!";
    }

}
