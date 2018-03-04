#ifndef GRAPHMISCOLOR_H
#define GRAPHMISCOLOR_H
/*************************************************************************
    File Name: GraphMISColor.h
    Author: Xin Cheng
    Descriptions: 
    Created Time: Tue 27 Feb 2018 10:18:27 AM EST
*********************************************************************/

#include <iostream>
#include <random>
using namespace std;
namespace ColPack {
//=============================================================================
//GraphColoring using Jhon Plasiman's Alg
//=============================================================================
class MISColoring : public GraphOrdering{
    public: MISColoring(const string& graph_name, const string&order);
    public: virtual ~MISColoring(){};
    public: virtual void dump();
    public: virtual void error(const string &s1);
    public: virtual void error(const string &s1, const string &s2);
    public: virtual int  JP_Alg();
    public: int get_num_colors()const {return maxColor_+1; }
    protected:
            vector<double> wt_rnd_; //random weight 
            vector<double> wt_deg_; //degree related weight
    protected:
            vector<int> vtxColors_;  //vtxColors[ vertex id ] <- colors
            int maxColor_;  //the largest color, base 0
};



}//end of namespace ColPack
#endif
