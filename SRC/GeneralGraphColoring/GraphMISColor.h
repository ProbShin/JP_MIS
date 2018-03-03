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
    public: virtual void JP_Alg();
    protected:
            vector<double> wt_rnd_; //random weight 
            vector<double> wt_deg_; //degree related weight
    protected:
            vector<int> vtxColors_;  //vtxColors[ vertex id ] <- colors
};

//  string m_s_VertexOrderingVariant;
//  vector<int> m_vi_OrderedVertices; // m_vi_OrderedVertices.size() = m_vi_Vertices.size() - 1
//  
MISColoring::MISColoring(const string& graph_name, const string&order){
    GraphInputOutput::ReadMatrixMarketAdjacencyGraph(graph_name);
    //GraphOrdering::OrderVertices(order);

    default_random_engine generator;
    uniform_real_distribution<double> distribution(0.0,1.0);
    
    const int N = (signed)GraphCore::m_vi_Vertices.size()-1;
    wt_rnd_.reserve(N);
    for(int i=0, iEnd=N; i!=iEnd; i++) {
        wt_rnd_.push_back(distribution(generator));
    }

    vtxColors_.assign(N,-1);

    
    JP_Alg();

}//end of function MISColoring;

class Nodes{
    public: Nodes(int i):id(i),next(nullptr);
    public: ~Nodes();
    int id;
    Nodes *next;
};

void MISColoring::JP_Alg(){
    vector<int> &vi     = GraphCore::m_vi_Vertices;
    vector<int> &vj     = GraphCore::m_vi_Edges; 
    vector<double> &wts = wt_rnd_;
    
    const int N=(int)vi.size()-1;
    const size_t Atip_C = (GraphCore::m_i_MaximumVertexDegree!=-1)?(GraphCore::m_i_MaximumVertexDegree+1):N; 
    
    int LargestColor=-1;

    vector<int> Q(N,-1),Qtmp(N,-1);
    long long int QTail=N, QtmpTail=0;

    //do in parallel
    for(int i=0; i<N; i++){
        Q[i] = GraphOrder::m_vi_OrderedVertices[i];

    while(QTail>0){
        vector<int> loc_doms;
        for(int i=0; i<QTail; i++) {
            int v = Q[i];
            double wt = wts[v];
            bool loc_domain = true;
            for(int nb=vi[v],nbEnd=vi[v+1]; nb!=nbEnd; nb++) {
                if(vtxColors_[nb]! = -1) continue; 
                if(wt< wts[nb]){
                    loc_domain=false;
                    break;
                }
            }
            if(loc_domain) loc_doms.push_back(v);
            else Qtmp[QtmpTail++]=v;
        }
        
        //do in parallel
        for(auto v: loc_doms){
            vector<int> cands(Atip_C, -1); 
            for(int nb=vi[i],nbEnd=vi[i+1]; nb!=nbEnd; nb++) {
                int nbcolor; if( (nbcolor=vtxColors_[nb]) == -1) continue;
                cands[nbcolor] = nb;
            }
            int c=0;
            while( cands[c]!=-1 ) c++;
            vtxColors_[i]=c;
            if(LargestColor < c) LargestColor=c;
            //reduce LargestColor with operand max
            cur=vNpt[v];
        }
    }

        if(loc_domain){
        }
        }
    }
}//end of function JP_Alg


}//end of namespace ColPack
#endif
