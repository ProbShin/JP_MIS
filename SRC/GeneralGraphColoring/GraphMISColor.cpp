/*************************************************************************
    File Name: SRC/GeneralGraphColoring/GraphMISColor.cpp
    Author: Xin Cheng
    Descriptions: 
    Created Time: Sat 03 Mar 2018 11:33:54 PM EST
*********************************************************************/

#include "GraphMISColor.h"

// ============================================================================
// Construction function
// ----------------------------------------------------------------------------
//  string m_s_VertexOrderingVariant;
//  vector<int> m_vi_OrderedVertices; // m_vi_OrderedVertices.size() = m_vi_Vertices.size() - 1
// ===========================================================================
MISColoring::MISColoring(const string& graph_name, const string&order){
    GraphInputOutput::ReadMatrixMarketAdjacencyGraph(graph_name);
    GraphOrdering::OrderVertices(order);

    default_random_engine generator;
    uniform_real_distribution<double> distribution(0.0,1.0);
    
    const int N = (signed)GraphCore::m_vi_Vertices.size()-1;
    wt_rnd_.reserve(N);
    for(int i=0, iEnd=N; i!=iEnd; i++) {
        wt_rnd_.push_back(distribution(generator));
    }

    vtxColors_.assign(N,-1);
    maxColor_ = -1;  
}//end of function MISColoring;


// ============================================================================
// Jone-Plassmann alg on shared memory
// ----------------------------------------------------------------------------
//
// ============================================================================
int MISColoring::JP_Alg(){
    vector<int> &vi     = GraphCore::m_vi_Vertices;
    vector<int> &vj     = GraphCore::m_vi_Edges; 
    vector<double> &wts = wt_rnd_;
    
    const int N=(int)vi.size()-1;
    const size_t Atip_C = (GraphCore::m_i_MaximumVertexDegree!=-1)?(GraphCore::m_i_MaximumVertexDegree+1):N; 
    
    int &LargestColor=maxColor_;

#pragma omp parallel reduction(max:LargestColor)
    {
        int LargestColor_loc=-1;
        vector<int> Q(Nloc, -1), Qtmp(Nloc,-1);
        size_t  QTail(Nloc), QtmpTail(0);
        
        {// Init Q locally
            int nT  = omp_get_num_threads();
            int tid = omp_get_thread_num();
            const int Nloc = N/nT + ((tid<(N%nT))?1:0);
            const int Disp = tid*(N/nT)+ ((tid<(N%nT))?tid:(N%nT));
            for(int i=0, iEnd=Nloc; i!=iEnd; i++) 
                Q[i]=GraphOrder::m_vi_OrderedVertices[i+Disp];
        }
        
        while(QTail){
            // collect loc domain
            vector<int> loc_doms;
            for(int i=0; i<QTail; i++) {
                int v = Q[i];
                double wt = wts[v];
                bool b_lcdm = true;
                for(int nb=vi[v], nbEnd=vi[v+1]; nb!=nbEnd; nb++) {
                    if(vtxColors_[nb] >= 0 ) continue;  //only uncolored vertex
                    if(wt >= wts[nb]) continue;
                    b_lcdm = false;
                    break;
                }
                if( b_lcdm) loc_doms.push_back(v);
                else Qtmp[QtmpTail++]=v; 
            } // endof for i in range(Qtail)

            // remove loc_doms from Q
            Q.swap(Qtmp);
            QTail=QtmpTail;
            QtmpTail=0;
           
            // color loc_doms
            for(auto v : loc_doms){
                vector<int> cands(Atip_C, -1); 
                for(int nb=vi[i],nbEnd=vi[i+1],nbColor; nb!=nbEnd; nb++) {
                    if( (nbcolor=vtxColors_[nb]) != -1)
                        cands[nbcolor] = nb;
                }
                int c=0; while( cands[c]!=-1 ) c++;
                vtxColors_[v]=c;  // ? false share potential?
                if(LargestColor_loc < c) LargestColor_loc=c;
            }// end of for v in loc_doms
        }// end of while QTail!=0
        LargestColor = LargestColor_loc;
    }// end of omp parallel section
    
    return (_TRUE);
}//end of function JP_Alg


// ============================================================================
// dump
// ============================================================================
void MISColoring::dump(){
}

// ============================================================================
// error
// ============================================================================
void MISColoring::error(const string&s1){
    fprintf(stderr,"ERROR: In class MISColoring with message\" %s\n\"\n", s1.c_str());
}

// ============================================================================
// error
// ============================================================================
void MISColoring::error(const string&s1, const string&s2){
    fprintf(stderr,"ERROR: In class MISColoring with message\" %s \n %s \"\n", s1.c_str(), s2.c_str());
}


