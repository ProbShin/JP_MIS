#include "ColPackHeaders.h"
#include <cstring>

using namespace ColPack;
void usage();

int main(int argc, char* argv[]) {
    string fname;
    string order("LARGEST_FIRST");
    string methd("DISTANCE_ONE");
    bool   bBiColor(false);
    bool   bVerbose(false);
    bool   bD1_OMP(false);

    vector<int> nTs;
    
    for(int i=1; i<argc; i++){
        if(argv[i][0]!='-') continue;
        if(     !strcmp(argv[i], "-f")) fname = argv[++i];
        else if(!strcmp(argv[i], "-o")) order = argv[++i];
        else if(!strcmp(argv[i], "-m")) methd = argv[++i];
        else if(!strcmp(argv[i], "-b") || !strcmp(argv[i],"--bc")) bBiColor=true;
        else if(!strcmp(argv[i], "-p")) bD1_OMP  = true; 
        else if(!strcmp(argv[i], "--nT")) {
            nTs.clear(); 
            for(int j=i+1; j<argc; j++, i++){
                if(argv[j][0]=='-') break;
                nTs.push_back( atoi(argv[j]));
            }
        }
        else if(!strcmp(argv[i], "-v")) bVerbose = true;
    }   

    if(fname.empty()) {usage(); exit(0); }

    if(bD1_OMP){

        GraphColoringInterface *g = new GraphColoringInterface(SRC_FILE, fname.c_str(), "AUTO_DETECTED");

	double t1 = -omp_get_wtime();
        g->OrderVertices(order.c_str());
        t1+=omp_get_wtime();
        printf("# Order\t%s\t%f\n",order.c_str(), t1);  
        
        if(bVerbose) printf("//\tnT\tvalid\tColor\tT_toal\tT1\tT2\tconflicts\trounds\n");  
        for(auto nT:nTs){
            omp_set_num_threads(nT);
            g->Coloring(order.c_str(), "DISTANCE_ONE_OMP");
        }
        
        if(nTs.empty()) g->Coloring(order.c_str(), "DISTANCE_ONE_OMP");
        return 0;
    }
    if(bBiColor){
        if(bVerbose) fprintf(stdout,"\ngraph:%s\norder:%s\ntype:%s\n-p:%s\n%s",fname.c_str(), order.c_str(), methd.c_str(),bD1_OMP?"true":"false", bBiColor?"BiColor":"GeneralColor");
        BipartiteGraphBicoloringInterface *p = new BipartiteGraphBicoloringInterface(0, fname.c_str(), "MM");
        p->Bicoloring(order.c_str(), methd.c_str());
        fprintf(stdout, "%d\n", p->GetVertexColorCount());
        delete p;
    }
    else{
        if(bVerbose) fprintf(stdout,"\ngraph:%s\norder:%s\ntype:%s\n-p:%s\n%s",fname.c_str(), order.c_str(), methd.c_str(),bD1_OMP?"true":"false", bBiColor?"BiColor":"GeneralColor");
        GraphColoringInterface *g = new GraphColoringInterface(SRC_FILE, fname.c_str(), "AUTO_DETECTED");
        g->Coloring(order.c_str(), methd.c_str());
        fprintf(stdout, "@%s %s color %d \n",order.c_str(), methd.c_str(), g->GetVertexColorCount());
        delete g;
    }
    return 0;
}

void usage(){
    fprintf(stderr, "\nusage: ./ColPack -f <gname> -o <ordering> -m <methods> [--bc/-b] [-v]\n"
            "-f <gname>  :  Input file name\n"
            "-o <order>  :  LARGEST_FIRST\n"
            "               SMALLEST_LAST,\n"
            "               DYNAMIC_LARGEST_FIRST,\n"
            "               INCIDENCE_DEGREE,\n"
            "               NATURAL,\n"
            "               RANDOM\n"
            "-m <methods>:  DISTANCE_ONE\n"
            "               ACYCLIC\n"
            "               ACYCLIC_FOR_INDIRECT_RECOVERY\n"
            "               STAR\n"
            "               RESTRICTED_STAR\n"
            "               --------------------\n"
            "               DISTANCE_ONE_OMP    (stderr:nThreads,num_colors,timall,tim1,tim2,loops)\n "
            "               --------------------\n"
            "               IMPLICIT_COVERING__STAR_BICOLORING\n"
            "               EXPLICIT_COVERING__STAR_BICOLORING\n"
            "               EXPLICIT_COVERING__MODIFIED_STAR_BICOLORING\n"
            "               IMPLICIT_COVERING__GREEDY_STAR_BICOLORING\n"
            "-b/--bc     :  BiColroing [default is general coloring]\n"
            "-v          :  verbose for debug infomation\n"
            "\n"
            "-p            :  cx's special task for comparing OMP threads\n"
            "--nT [nproc..]:  cx's special task for comparing OMP threads\n"
            "\n"
           ); 
}


