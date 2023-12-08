#include <iostream>
#include <map>
#include <string> 
#include <fstream>
#include <vector>
#include <unordered_map>
#include <climits>
using namespace std;
double minArea=100000000;

unordered_map< string , vector < pair < double , double > > >delays;
unordered_map<string,double> timeNodes;
vector<string>  should_be_called;
unordered_map<string,vector<pair<string,string> > > nextNode;
unordered_map<string,vector<pair<string,string> > > parentNode;
unordered_map<string,int> node_to_priority;
unordered_map<string,int> prev_node_to_priority;
unordered_map<string,string> resultnodes;
unordered_map<string,string> intermediateNodes;
unordered_map<string,string> inputnodes;
unordered_map<string,int> differentGates;
unordered_map<string,double> timeThreshHold;

int how_much_of_string=1;

void display_info(){
    std::cout<<"-----nodes to priority (priority should be zero if called at the end)------"<<endl;
    for(auto i:node_to_priority){
        std::cout<<i.first<<" - > "<<i.second<<endl;
    }
    std::cout<<"-----prev nodes to priority (priority should be zero if called at the end)------"<<endl;
    for(auto i:prev_node_to_priority){
        std::cout<<i.first<<" - > "<<i.second<<endl;
    }
    std::cout<<"-----should be called (if the display_info() is called at end the output should be empty)------"<<endl;
    for(auto i:should_be_called){
        std::cout<<i<<" ";
    }
    std::cout<<endl;

    std::cout<<"-----result nodes------"<<endl;
    for(auto i:resultnodes){
        std::cout<<i.first<<" ";
    }
    std::cout<<endl;
    std::cout<<"----- parent node ------"<<endl;
    for(auto i:parentNode){
        std::cout<<i.first<<" --> ";
        for(auto j:i.second){
            std::cout<<"("<<j.first<<","<<j.second<<") ";
        }
        std::cout<<endl;
    }
    std::cout<<"----- next node ------"<<endl;
    for(auto i:nextNode){
        std::cout<<i.first<<" --> ";
        for(auto j:i.second){
            std::cout<<"("<<j.first<<","<<j.second<<") ";
        }
        std::cout<<endl;
    }
    std::cout<<"-----transistor delay------"<<endl;
    for( auto i: delays){
        std::cout<<i.first<<"0 "<<i.second[0].first<<" "<<i.second[0].second<<endl;
        std::cout<<i.first<<"1 "<<i.second[1].first<<" "<<i.second[1].second<<endl;
        std::cout<<i.first<<"2 "<<i.second[2].first<<" "<<i.second[2].second<<endl;
    }
    std::cout<<"-----nodes delay------"<<endl;
    for( auto i:timeNodes){
        std::cout<<i.first<<" "<<i.second<<endl;
    }
    std::cout<<"-----threshold delay------"<<endl;
    for( auto i:timeThreshHold){
        std::cout<<i.first<<" "<<i.second<<endl;
    }
    
    return;

}
 
using namespace std;

void randomise_search(int y){
    double area=0;
    unordered_map<string,int> type_of_delay;

    
    unordered_map<string,int> ::iterator itt;
    
    for(itt=differentGates.begin();itt!=differentGates.end();++itt){
        type_of_delay[itt->first]=rand()%3;
    }
    
    map<string,int> Tnode_to_priority;
    for(auto i:node_to_priority){
        Tnode_to_priority[i.first]=i.second;
    }
    vector<string> Tshould_be_called;
    for(auto i:should_be_called){
        Tshould_be_called.push_back(i);
    }
    unordered_map<string,int> visited_gates;
    double answer=0;
    while(Tshould_be_called.size()!=0){
        vector<string> temp;
        
        for(int i=0;i<Tshould_be_called.size();i++){
            for(int j=0;j<nextNode[Tshould_be_called[i]].size();j++){
                //std::cout<<nextNode[Tshould_be_called[i]][j].first<<endl;
                timeNodes[nextNode[Tshould_be_called[i]][j].first]=max(timeNodes[nextNode[Tshould_be_called[i]][j].first],timeNodes[Tshould_be_called[i]]+delays[nextNode[Tshould_be_called[i]][j].second][type_of_delay[nextNode[Tshould_be_called[i]][j].first]].first);
                //cout<<Tshould_be_called[i]<<" ->"<<nextNode[Tshould_be_called[i]][j].first<<" = "<<delays[nextNode[Tshould_be_called[i]][j].second][type_of_delay[nextNode[Tshould_be_called[i]][j].first]].first<<endl;
                if(visited_gates.find(nextNode[Tshould_be_called[i]][j].first)==visited_gates.end()){
                    area+=delays[nextNode[Tshould_be_called[i]][j].second][type_of_delay[nextNode[Tshould_be_called[i]][j].first]].second;
                    visited_gates[nextNode[Tshould_be_called[i]][j].first]=1;
                }
                if(resultnodes.find(nextNode[Tshould_be_called[i]][j].first)!=resultnodes.end()){
                    answer=max(answer,timeNodes[nextNode[Tshould_be_called[i]][j].first]);
                }
                //cout<<area<<endl;
                //cout<<timeNodes[nextNode[Tshould_be_called[i]][j].first]<<endl;
                //cout<<delays[nextNode[should_be_called[i]][j].second][type_of_delay[nextNode[should_be_called[i]][j].first]].second<<endl;
                Tnode_to_priority[nextNode[Tshould_be_called[i]][j].first]-=1;
                if(Tnode_to_priority[nextNode[Tshould_be_called[i]][j].first]==0){
                    temp.push_back(nextNode[Tshould_be_called[i]][j].first);
                }
            }
            timeNodes[Tshould_be_called[i]]=0;
        }
        Tshould_be_called.clear();
        for(int i=0;i<temp.size();i++){
            //area+=delays[nextNode[should_be_called[i]][j].second][type_of_delay[nextNode[should_be_called[i]][j].first]].second;
            Tshould_be_called.push_back(temp[i]);
            //cout<<temp[i]<<" "<<timeNodes[temp[i]]<<endl;
        }
        //cout<<endl;
    }
    unordered_map<string,string> ::iterator it;
    
    
    
    for(it=resultnodes.begin();it!=resultnodes.end();++it){
        //cout<<it->first<<" - "<<timeNodes[it->first]<<" "<<flush;
        
        timeNodes[it->first]=0;

    }
    //cout<<endl;
    
    
    if(answer<=y){
        /* for(auto i :type_of_delay){
            cout<<i.first<<" "<<i.second<<" - "<<flush;
        }
        cout<<area<<"<>"<<answer<<endl; */
        minArea=min(area,minArea);
    }
    
}
void up(string circuitFile,string delayFile,string delayConstraint){
        //store tmie delay for each logic gate
    
    string circuit_line;
    ifstream circuit(circuitFile);
    if(circuit.is_open()){
        while(getline(circuit,circuit_line)){
            if(circuit_line.length()>=2){
                if(int(circuit_line[circuit_line.length()-1])!=13){
                    //cout<<circuit_line.length()<<"  "<<circuit_line<<endl;
                    how_much_of_string=0;
                }
                else{
                    how_much_of_string=1;
                }
                if(circuit_line[0]!='/' && circuit_line[1]!='/'){
                    vector<string> cmd;
                    string s="";
                    for(int i=0;i<circuit_line.length()-how_much_of_string;i++){
                        if(circuit_line[i]==' '){
                            cmd.push_back(s);
                            s="";
                        }
                        else {
                            s+=circuit_line[i];
                        }
                    }
                    cmd.push_back(s);
                    if(cmd[0]=="PRIMARY_INPUTS"){

                        for(int i=1;i<cmd.size();i++){
                            should_be_called.push_back(cmd[i]);
                            timeNodes[cmd[i]]=0;
                            node_to_priority[cmd[i]]=0;
                        }

                    }
                    else if(cmd[0]=="PRIMARY_OUTPUTS"){
                        for(int i=1;i<cmd.size();i++){
                            resultnodes[cmd[i]]=cmd[i];
                        }

                    }
                    else if(cmd[0]=="INTERNAL_SIGNALS"){
                        //meh
                    }
                    else if(cmd[0]=="DFF"){
                        node_to_priority[cmd[2]]=0;
                        should_be_called.push_back(cmd[2]);
                        resultnodes[cmd[1]]=cmd[1];
                        

                    }
                    else {

                        string last=cmd[cmd.size()-1];
                        for(int i=1;i<cmd.size()-1;i++){
                            nextNode[cmd[i]].push_back(make_pair(last,cmd[0]));
                        }
                        if(node_to_priority.find(last)!=node_to_priority.end()){
                            node_to_priority[last]+=cmd.size()-2;
                        }
                        else{
                            node_to_priority[last]=cmd.size()-2;

                        }

                    }

                }

            }
        }
        circuit.close();
    }
    else{
        std::cout<<"circuit.txt file not available"<<endl;
        std::cout<<"Please ensure correct path of the file"<<endl;
    }
    string gate_line;
    ifstream gate_delays(delayFile);
    if(gate_delays.is_open()){
        while(getline(gate_delays,gate_line)){
            if(gate_line.length()>=2){
                if(gate_line[0]!='/' && gate_line[1]!='/'){
                    vector<string> cmd;
                    string com="";
                    //cout<<gate_line.length()<<" "<<gate_line<<endl;
                    for(int i=0;i<gate_line.length()-how_much_of_string;i++){
                        
                        if(gate_line[i]==' '){
                            cmd.push_back(com);
                            com="";
                        }
                        else{
                            com+=gate_line[i];

                        }

                    }
                    cmd.push_back(com);

                    
                    
                    
                    delays[cmd[1]].push_back(make_pair(stod(cmd[2]),stod(cmd[3])));
                    sort(delays[cmd[1]].begin(),delays[cmd[1]].end());
                    
                    
                                        
                }
            }          
        }
        gate_delays.close();
    }
    else{
        std::cout<<"gate_delays.txt file not available"<<endl;
        std::cout<<"Please ensure correct path of the file"<<endl;
    }
    //actual algorithm to solve
    // time complexity o(n) where n is the number of wires in the pictorical systemᵔᴥᵔ
    //display_info();
    while(should_be_called.size()!=0){
        vector<string> temp;
        for(int i=0;i<should_be_called.size();i++){
            for(int j=0;j<nextNode[should_be_called[i]].size();j++){
                timeNodes[nextNode[should_be_called[i]][j].first]=max(timeNodes[nextNode[should_be_called[i]][j].first],timeNodes[should_be_called[i]]+delays[nextNode[should_be_called[i]][j].second][0].first);
                node_to_priority[nextNode[should_be_called[i]][j].first]-=1;
                if(node_to_priority[nextNode[should_be_called[i]][j].first]==0){
                    temp.push_back(nextNode[should_be_called[i]][j].first);
                }
            }
        }
        should_be_called.clear();
        for(int i=0;i<temp.size();i++){
            should_be_called.push_back(temp[i]);
        }
    };
    //display_info();
    ofstream longest_delays("longest_delay.txt");
    unordered_map<string,string> ::iterator it;
    double answer=0;
    for(it=resultnodes.begin();it!=resultnodes.end();++it){
        answer=max(answer,timeNodes[it->first]);

        
    }
    longest_delays<<to_string(answer);
    longest_delays.close();
}
void up_but_area_opt(){
    
    double area=0;
    for(auto i:inputnodes){
        should_be_called.push_back(i.first);
    }
    unordered_map<string,vector<pair<int,string > > > visited_gates;

    unordered_map<string,double > Tnode_to_priority;
    for(auto i:node_to_priority){
        Tnode_to_priority[i.first]=i.second;
    }

    while(should_be_called.size()!=0){
        vector<string> temp;
        for(int i=0;i<should_be_called.size();i++){
            for(int j=0;j<nextNode[should_be_called[i]].size();j++){
                bool notdone=true;
                int posn=2;
                while(notdone){
                    //cout<<should_be_called[i]<<" -- > "<<nextNode[should_be_called[i]][j].first<<" "<<timeNodes[should_be_called[i]]<<" "<<delays[nextNode[should_be_called[i]][j].second][posn].first<<" "<<timeThreshHold[nextNode[should_be_called[i]][j].first]<<endl;
                    if(timeNodes[should_be_called[i]]+delays[nextNode[should_be_called[i]][j].second][posn].first<=timeThreshHold[nextNode[should_be_called[i]][j].first]){
                        if(timeNodes[nextNode[should_be_called[i]][j].first]<timeNodes[should_be_called[i]]+delays[nextNode[should_be_called[i]][j].second][posn].first){
                            timeNodes[nextNode[should_be_called[i]][j].first]=timeNodes[should_be_called[i]]+delays[nextNode[should_be_called[i]][j].second][posn].first;
                            
                        }
                        notdone=false;
                        break;
                    }
                    else{
                        posn--;
                    }
                    
                    if(posn==-1){
                        //cout<<timeNodes[should_be_called[i]]<<" "<<delays[nextNode[should_be_called[i]][j].second][0].first<<" "<<timeThreshHold[nextNode[should_be_called[i]][j].first]<<endl;
                        //cout<<"something is not right"<<endl;
                        return;
                        break;

                    }


                }
                //cout<<posn<<endl;
                
                    //area+=delays[nextNode[should_be_called[i]][j].second][posn].second;
                visited_gates[nextNode[should_be_called[i]][j].first].push_back(make_pair(posn,nextNode[should_be_called[i]][j].second) );
                
                
                //timeNodes[nextNode[should_be_called[i]][j].first]=max(timeNodes[nextNode[should_be_called[i]][j].first],timeNodes[should_be_called[i]]+delays[nextNode[should_be_called[i]][j].second][0].first);
                Tnode_to_priority[nextNode[should_be_called[i]][j].first]-=1;
                if(Tnode_to_priority[nextNode[should_be_called[i]][j].first]==0){
                    temp.push_back(nextNode[should_be_called[i]][j].first);
                }
            }
        }
        should_be_called.clear();
        for(int i=0;i<temp.size();i++){
            should_be_called.push_back(temp[i]);
        }
    }
    
    for(auto i:visited_gates){
        int min_posn=3;
        string gate_tt="";
        for(int j=0;j<i.second.size();j++){
            //cout<<i.second[j]<<" "<<flush;
            if(min_posn>i.second[j].first){
                min_posn=i.second[j].first;
                gate_tt=i.second[j].second;
            }
            //min_posn=min(min_posn,);
            //cout<<min_posn<<endl;

        }
        //cout<<i.first<<endl;
        area+=delays[gate_tt][min_posn].second;

    }
    minArea=min(minArea,area);

    
}
void get_Threshold(string circuitFile,double delayVlaue){
    should_be_called.clear();
    unordered_map<string,double>::iterator it;
    for(it=timeNodes.begin();it!=timeNodes.end();++it){
        it->second=10000000;
    }
    for(auto i:resultnodes){
        should_be_called.push_back(i.first);
        timeNodes[i.first]=delayVlaue;
    }
    
    unordered_map<string,double> Tprev_node_to_priority;
    for(auto i:prev_node_to_priority){
        Tprev_node_to_priority[i.first]=i.second;
    }
    //display_info();
    //actual algorithm to solve
    // time complexity o(n) where n is the number of wires in the pictorical system ᵔᴥᵔ
    while(should_be_called.size()!=0){
        vector<string> temp;
        for(int i=0;i<should_be_called.size();i++){
            int posn=rand()%3;
            for(int j=0;j<parentNode[should_be_called[i]].size();j++){
                //cout<<timeNodes["C"]<<endl;
                timeNodes[parentNode[should_be_called[i]][j].first]=min(timeNodes[parentNode[should_be_called[i]][j].first],timeNodes[should_be_called[i]]-delays[parentNode[should_be_called[i]][j].second][posn].first);
                //cout<<parentNode[should_be_called[i]][j].first<<" -> "<<timeNodes[parentNode[should_be_called[i]][j].first]<<endl;
                Tprev_node_to_priority[parentNode[should_be_called[i]][j].first]-=1;
                if(Tprev_node_to_priority[parentNode[should_be_called[i]][j].first]==0){
                    temp.push_back(parentNode[should_be_called[i]][j].first);

                }

            }

        }
        should_be_called.clear();
        for(int i=0;i<temp.size();i++){
            should_be_called.push_back(temp[i]);
        }

    }
     
    for(auto i: timeNodes){
        timeThreshHold[i.first]=i.second;
        i.second=0;
    }       
    for(auto i:inputnodes){
        timeThreshHold[i.first]=0;
    }
    for(auto i:timeNodes){
        timeNodes[i.first]=0;
    }

    //display_info();
    


    
    
}
void FIXEDget_Threshold1(string circuitFile,double delayVlaue,int pp){
    should_be_called.clear();
    unordered_map<string,double>::iterator it;
    for(it=timeNodes.begin();it!=timeNodes.end();++it){
        it->second=10000000;
    }
    for(auto i:resultnodes){
        should_be_called.push_back(i.first);
        timeNodes[i.first]=delayVlaue;
    }
    
    unordered_map<string,double> Tprev_node_to_priority;
    for(auto i:prev_node_to_priority){
        Tprev_node_to_priority[i.first]=i.second;
    }
    //display_info();
    //actual algorithm to solve
    // time complexity o(n) where n is the number of wires in the pictorical system ᵔᴥᵔ
    while(should_be_called.size()!=0){
        vector<string> temp;
        for(int i=0;i<should_be_called.size();i++){
            int posn=pp;
            for(int j=0;j<parentNode[should_be_called[i]].size();j++){
                //cout<<timeNodes["C"]<<endl;
                timeNodes[parentNode[should_be_called[i]][j].first]=min(timeNodes[parentNode[should_be_called[i]][j].first],timeNodes[should_be_called[i]]-delays[parentNode[should_be_called[i]][j].second][posn].first);
                //cout<<parentNode[should_be_called[i]][j].first<<" -> "<<timeNodes[parentNode[should_be_called[i]][j].first]<<endl;
                Tprev_node_to_priority[parentNode[should_be_called[i]][j].first]-=1;
                if(Tprev_node_to_priority[parentNode[should_be_called[i]][j].first]==0){
                    temp.push_back(parentNode[should_be_called[i]][j].first);

                }

            }

        }
        should_be_called.clear();
        for(int i=0;i<temp.size();i++){
            should_be_called.push_back(temp[i]);
        }

    }
     
    for(auto i: timeNodes){
        timeThreshHold[i.first]=i.second;
        i.second=0;
    }       
    for(auto i:inputnodes){
        timeThreshHold[i.first]=0;
    }
    for(auto i:timeNodes){
        timeNodes[i.first]=0;
    }
}
double exp(int x,double y){
    double area=0;
    unordered_map<string,int> type_of_delay;
    unordered_map<string,int> ::iterator itt;
    for(itt=differentGates.begin();itt!=differentGates.end();++itt){
        type_of_delay[itt->first]=x%3;
        x=x/3;
    }
    map<string,int> Tnode_to_priority;
    for(auto i:node_to_priority){
        Tnode_to_priority[i.first]=i.second;
    }
    vector<string> Tshould_be_called;
    for(auto i:should_be_called){
        Tshould_be_called.push_back(i);
    }
    unordered_map<string,int> visited_gates;
    double answer=0;

    while(Tshould_be_called.size()!=0){
        vector<string> temp;
        
        for(int i=0;i<Tshould_be_called.size();i++){
            for(int j=0;j<nextNode[Tshould_be_called[i]].size();j++){
                //std::cout<<nextNode[Tshould_be_called[i]][j].first<<endl;
                timeNodes[nextNode[Tshould_be_called[i]][j].first]=max(timeNodes[nextNode[Tshould_be_called[i]][j].first],timeNodes[Tshould_be_called[i]]+delays[nextNode[Tshould_be_called[i]][j].second][type_of_delay[nextNode[Tshould_be_called[i]][j].first]].first);
                //cout<<Tshould_be_called[i]<<" ->"<<nextNode[Tshould_be_called[i]][j].first<<" = "<<delays[nextNode[Tshould_be_called[i]][j].second][type_of_delay[nextNode[Tshould_be_called[i]][j].first]].first<<endl;
                if(visited_gates.find(nextNode[Tshould_be_called[i]][j].first)==visited_gates.end()){
                    area+=delays[nextNode[Tshould_be_called[i]][j].second][type_of_delay[nextNode[Tshould_be_called[i]][j].first]].second;
                    visited_gates[nextNode[Tshould_be_called[i]][j].first]=1;
                }
                if(resultnodes.find(nextNode[Tshould_be_called[i]][j].first)!=resultnodes.end()){
                    answer=max(answer,timeNodes[nextNode[Tshould_be_called[i]][j].first]);
                }
                //cout<<area<<endl;
                //cout<<timeNodes[nextNode[Tshould_be_called[i]][j].first]<<endl;
                //cout<<delays[nextNode[should_be_called[i]][j].second][type_of_delay[nextNode[should_be_called[i]][j].first]].second<<endl;
                Tnode_to_priority[nextNode[Tshould_be_called[i]][j].first]-=1;
                if(Tnode_to_priority[nextNode[Tshould_be_called[i]][j].first]==0){
                    temp.push_back(nextNode[Tshould_be_called[i]][j].first);
                }
            }
            timeNodes[Tshould_be_called[i]]=0;
        }
        Tshould_be_called.clear();
        for(int i=0;i<temp.size();i++){
            //area+=delays[nextNode[should_be_called[i]][j].second][type_of_delay[nextNode[should_be_called[i]][j].first]].second;
            Tshould_be_called.push_back(temp[i]);
            //cout<<temp[i]<<" "<<timeNodes[temp[i]]<<endl;
        }
        //cout<<endl;
    }
    
    unordered_map<string,string> ::iterator it;
    for(it=resultnodes.begin();it!=resultnodes.end();++it){
        //cout<<it->first<<" - "<<timeNodes[it->first]<<" "<<flush;
        timeNodes[it->first]=0;     
    }  
    if(answer<=y){
        /* for(auto i :type_of_delay){
            cout<<i.first<<" "<<i.second<<" - "<<flush;
        }
        cout<<area<<"<>"<<answer<<endl; */
        return area;
    }
    else {
        return -1;
    }
   

    
    
}
void solve(string circuitFile,string delayFile,string delayConstraint){
    string circuit_line;
    ifstream circuit(circuitFile);
    
    
    if(circuit.is_open()){
        while(getline(circuit,circuit_line)){
            
            if(circuit_line.length()>=2){
                if(int(circuit_line[circuit_line.length()-1])!=13){
                    //cout<<circuit_line.length()<<"  "<<circuit_line<<endl;
                    how_much_of_string=0;
                }
                else{
                    how_much_of_string=1;
                }
                if(circuit_line[0]!='/' && circuit_line[1]!='/'){
                    vector<string> cmd;
                    string s="";
                    for(int i=0;i<circuit_line.length()-how_much_of_string;i++){
                        if(circuit_line[i]==' '){
                            cmd.push_back(s);
                            s="";
                        }
                        else {
                            s+=circuit_line[i];
                        }
                    }
                    cmd.push_back(s);
                    if(cmd[0]=="PRIMARY_INPUTS"){

                        for(int i=1;i<cmd.size();i++){
                            inputnodes[cmd[i]]=cmd[i];
                            should_be_called.push_back(cmd[i]);
                            timeNodes[cmd[i]]=0;
                            node_to_priority[cmd[i]]=0;
                        }

                    }
                    else if(cmd[0]=="PRIMARY_OUTPUTS"){
                        
                        for(int i=1;i<cmd.size();i++){
                            differentGates[cmd[i]]=1;
                            resultnodes[cmd[i]]=cmd[i];
                            prev_node_to_priority[cmd[i]]=0;
                            timeNodes[cmd[i]]=0;
                        }

                    }
                    else if(cmd[0]=="INTERNAL_SIGNALS"){
                        
                        for(int i=1;i<cmd.size();i++){
                            differentGates[cmd[i]]=1;
                            intermediateNodes[cmd[i]]=cmd[i];
                            //resultnodes[cmd[i]]=cmd[i];
                            timeNodes[cmd[i]]=0;
                        }
                    }
                    else if(cmd[0]=="DFF"){
                        differentGates[cmd[1]]=1;
                        if(differentGates.find(cmd[2])!=differentGates.end()){
                            differentGates.erase(cmd[2]);
                        }
                        inputnodes[cmd[2]]=cmd[2];
                        timeNodes[cmd[2]]=0;
                        node_to_priority[cmd[2]]=0;
                        prev_node_to_priority[cmd[1]]=0;
                        should_be_called.push_back(cmd[2]);
                        resultnodes[cmd[1]]=cmd[1];
                        

                    }
                    else {

                        string last=cmd[cmd.size()-1];
                        for(int i=1;i<cmd.size()-1;i++){
                            
                            nextNode[cmd[i]].push_back(make_pair(last,cmd[0]));
                            parentNode[last].push_back(make_pair(cmd[i],cmd[0]));
                            if(prev_node_to_priority.find(cmd[i])!=prev_node_to_priority.end()){
                                prev_node_to_priority[cmd[i]]+=1;
                            }
                            else{
                                prev_node_to_priority[cmd[i]]=1;
                            }
                            
                        }
                        if(node_to_priority.find(last)!=node_to_priority.end()){
                            node_to_priority[last]+=cmd.size()-2;
                        }
                        else{
                            node_to_priority[last]=cmd.size()-2;

                        }




                    }

                }

            }
        }
        circuit.close();
    }
    else{
        std::cout<<"circuit.txt file not available"<<endl;
        std::cout<<"Please ensure correct path of the file"<<endl;
    }
    string gate_line;
    ifstream gate_delays(delayFile);
    if(gate_delays.is_open()){
        while(getline(gate_delays,gate_line)){
            if(gate_line.length()>=2){
                if(gate_line[0]!='/' && gate_line[1]!='/'){
                    vector<string> cmd;
                    string com="";
                    //cout<<gate_line.length()<<" "<<gate_line<<endl;
                    for(int i=0;i<gate_line.length()-how_much_of_string;i++){
                        
                        if(gate_line[i]==' '){
                            cmd.push_back(com);
                            com="";
                        }
                        else{
                            com+=gate_line[i];

                        }

                    }
                    cmd.push_back(com);

                    
                    
                    
                    delays[cmd[1]].push_back(make_pair(stod(cmd[2]),stod(cmd[3])));
                    sort(delays[cmd[1]].begin(),delays[cmd[1]].end());
                    
                    
                                        
                }
            }          
        }
        gate_delays.close();
    }
    else{
        std::cout<<"gate_delays.txt file not available"<<endl;
        std::cout<<"Please ensure correct path of the file"<<endl;
    }
    //actual algorithm to solve
    // time complexity o(n) where n is the number of wires in the pictorical systemᵔᴥᵔ
    //
    string delay_constraint_line;
    ifstream delay_constraint(delayConstraint);
    double delayCon;
    if(delay_constraint.is_open()){
        while(getline(delay_constraint,delay_constraint_line)){
            delayCon=stod(delay_constraint_line);
        }
    }
    else{
        cout<<"file not opened correctly!!"<<endl;
    }
    //cout<<"delay con = "<<delayCon<<endl;
   
    //display_info();
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;

    //bool testing=true;

    /* for(auto i:differentGates){
        cout<<i.first<<endl;
    } */
    if(differentGates.size()<20){
        
        long long threshold=10000000;
        long long gg=1;
        for(int i=0;i<differentGates.size();i++){
            gg*=3;
        }
        unordered_map<string,int> ::iterator it;
        int zz=0;
        //cout<<gg<<endl;
        for(it=node_to_priority.begin();it!=node_to_priority.end();++it){
            zz+=it->second;
        }
        if(gg*zz<threshold){
            //cout<<"yes"<<endl;
            cout<<gg<<" "<<zz<<endl;
            minArea=-1;
            int last_computation=0;
            for(int i=0;i<gg;i++){
                minArea=exp(i,delayCon);
                if(i%40000==0){
                    cout<<"█"<<flush;
                }
                if(minArea!=-1){
                    last_computation=i;
                    break;
                }
            }
            for(int i=last_computation+1;i<gg;i++){
                double TminArea=exp(i,delayCon);
                //cout<<TminArea<<endl;
                if(i%40000==0){
                    cout<<"█"<<flush;
                }
                if(TminArea!=-1 && minArea>TminArea){
                    minArea=TminArea;                
                }
            }
            ofstream minimum_area("minimum_area.txt");
            minimum_area<<to_string(minArea);
            minimum_area.close();
            return;
        }
    }
    //display_info();
    unsigned long gg=1;
    for(int i=0;i<=1000000/differentGates.size();i++){
        if(i%max(20000/differentGates.size(),gg)==0){
            cout<<"█"<<flush;
        }
        randomise_search(delayCon);
        //cout<<minArea<<" "<<flush;
    } 
    //cout<<minArea<<endl;
   //cout<<"uwu"<<endl;
    for(int i=0;i<=1000000/differentGates.size();i++){
        if(i%max(20000/differentGates.size(),gg)==0){
            cout<<"█"<<flush;
        }
        timeThreshHold.clear();
        get_Threshold(circuitFile,delayCon);
        //display_info();
        for(auto i:timeThreshHold){
            //cout<<i.first<<" "<<i.second<<endl;
        }
        up_but_area_opt();
        for(auto i: timeNodes){
            timeNodes[i.first]=0;
        } 


    }
    timeThreshHold.clear();
    FIXEDget_Threshold1(circuitFile,delayCon,0);
    //display_info();
    for(auto i:timeThreshHold){
        //cout<<i.first<<" "<<i.second<<endl;
    }
    up_but_area_opt();
    for(auto i: timeNodes){
        timeNodes[i.first]=0;
    } 
    timeThreshHold.clear();
    FIXEDget_Threshold1(circuitFile,delayCon,1);
    //display_info();
    for(auto i:timeThreshHold){
        //cout<<i.first<<" "<<i.second<<endl;
    }
    up_but_area_opt();
    for(auto i: timeNodes){
        timeNodes[i.first]=0;
    }
    timeThreshHold.clear();
    FIXEDget_Threshold1(circuitFile,delayCon,2);
    //display_info();
    for(auto i:timeThreshHold){
        //cout<<i.first<<" "<<i.second<<endl;
    }
    up_but_area_opt();
    for(auto i: timeNodes){
        timeNodes[i.first]=0;
    }
    
    //display_info();
    
    
    
    
    

    
    ofstream minimum_area("minimum_area.txt");
    if(minArea==100000000){
        minimum_area<<"delay constraint not properly set";
    }
    else{
        minimum_area<<to_string(minArea);
    }
    
    minimum_area.close();
            
    return;
    
}

int main(int argc,char* files[]){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    srand(time(0));
    string part=files[1];
    if(part=="A"){
        
        cout<<"started"<<endl;
       
        up(files[2],files[3],files[4]);
        
        cout<<"finished"<<endl;
    }
    else{
        int start=time(0);
        

        solve(files[2],files[3],files[4]);
        cout<<endl;
        cout<<"Time Taken :"<<time(0)-start<<" sec"<<endl;
        
   
    }
}
