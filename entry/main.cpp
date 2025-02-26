#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <utility>
#include <string>
#include <cmath>

#include "../src/Node.cpp"
#include "../src/Edge.cpp"


/* Convert text files to a friendlier adjancency list */
int list_size;
std::vector<Node*> makeDataSet(std::string file_name_node, std::string file_name_edge){
    std::ifstream node_list, edge_list;
    std::vector<Edge> edges;
    std::vector<Node*> ret;
    
    

    /* open node file */
    node_list.open(file_name_node, std::ios_base::in);
    if(!node_list.is_open()) return std::vector<Node*>();

    /**
     * @brief load node vector from txt file
     * 1. grab node data with getline()
     * 2. get edge vector SEE NEXT @brief
     * 3. construct linked-list from edge vector
     * 4. load head of list into node obj with node data
     * 5. repeat per node
     */

    /* parse node file */
    while(!node_list.eof()){

        /* init node data */
        std::string str_node;
        unsigned node_idx;
        double latitude, longitude;

        /* grab node data */
        std::getline(node_list, str_node);
        std::stringstream s(str_node);
        s >> node_idx >> latitude >> longitude;

        /* mark */
        //std::cout << "Line : " << __LINE__ << std::endl;
        //std::cout << "node_idx : " << node_idx << std::endl;

        /* open edge file */
        edge_list.open(file_name_edge, std::ios_base::in);
        if(!edge_list.is_open()) return std::vector<Node*>();

        /* init edge data */
        std::string str_edge;
        unsigned edge_idx;
        unsigned start_node_idx, end_node_idx;
        double weight;

        /** 
         * @brief load edges vector from txt file 
         * pt1 : grab edges pointing TO target node
         * pt2 : grab edges pointing FROM target node
         */

        /* parse edge file pt1 */
        do{
            /* grab edge data */
            std::getline(edge_list, str_edge);
            std::stringstream s(str_edge);
            s >> edge_idx >> start_node_idx >> end_node_idx >> weight;

            /* if edge TO target --> load edge */
            if(end_node_idx == node_idx){
                edges.push_back(Edge(edge_idx, start_node_idx, end_node_idx, weight, NULL));

                /* mark */
                // std::cout << "Line : " << __LINE__ << std::endl;
                // std::cout << "node_idxes : " << end_node_idx << ", " << start_node_idx << std::endl;
            }

        }while(!edge_list.eof() && start_node_idx < node_idx);

        /* parse edge file pt2 */
        while(start_node_idx == node_idx){

            /* construct Edge */
            edges.push_back(Edge(edge_idx, start_node_idx, end_node_idx, weight, NULL));

            /* mark */
            // std::cout << "Line : " << __LINE__ << std::endl;
            // std::cout << "node_idxes : " << start_node_idx << ", " << end_node_idx << std::endl;

            /* file break */
            if(edge_list.eof()) break;

            /* grab edge data */
            getline(edge_list, str_edge);
            std::stringstream s(str_edge);
            s >> edge_idx >> start_node_idx >> end_node_idx >> weight;
        }
        /* close edge file */
        edge_list.close();

        /* construct edge list from edge vector */
        Edge * edge_list_head = (!edges.empty()) ? new Edge(edges[0]) : NULL, *edge_list_ptr = edge_list_head;
        for(unsigned i = 1; i < edges.size(); i++){
            edge_list_ptr->_next = new Edge(edges[i]);
            edge_list_ptr = edge_list_ptr->_next;
        }

        /* construct Node, push to ret */
        ret.push_back(new Node(node_idx, latitude, longitude, edge_list_head)); //TODO: MEM LEAK HERE

        /* delete edges vector */
        edges.clear();
    }
    /* close node file */
    node_list.close();

    /* Get list size */
    list_size = ret.size();

    /* return */
    return ret;
}

std::pair<unsigned, double> getUserInput(){
    /* get center node # */
    int user_node_get;
    std::cout << "ENTER CENTER NODE # : " << std::endl;
    std::cin >> user_node_get;

    /* Check if valid  user_node_get input */
    while(user_node_get > list_size || user_node_get < 0){
        std::cout << "CENTER NODE OUT OF BOUNDS, RETRY" << std::endl;
        std::cin >> user_node_get;
    }
    //Convert user input to unsigned
    unsigned user_node = (unsigned) user_node_get;


    /* get radius */
    double radius;
    std::cout << "ENTER RADIUS : " << std::endl;
    std::cin >> radius;
    /* Check if valid radius inpput */

    while(radius < 0){
        std::cout << "RADIUS CANT BE NEGATIVE, RETRY : " << std::endl;
        std::cin >> radius;
    }

    /* return */
    return std::pair<unsigned, double>(user_node, radius);


}


void print(std::vector<Node*> set){
    for(auto node : set){
        std::cout << node->_idx << " : " << node->_x << ", " << node->_y << std::endl;
        
        Edge * ptr = node->_edge;
        while(ptr){
            std::cout << ptr->_start_node_idx << ", " << ptr->_end_node_idx << std::endl;
            ptr = ptr->_next;
        }
    }
}

void print_to_file(std::string file_name, std::vector<Node*> set){
    std::ofstream output(file_name);
    if(!output.is_open()) return;
    
    Edge * ptr;
    for(auto node : set){
        output << node->_idx << " : " << node->_x << ", " << node->_y << std::endl;

        ptr = node->_edge;

        while(ptr){
            output  << ptr->_start_node_idx << ", "
                << ptr->_end_node_idx
                << std::endl;
            
            ptr = ptr->_next;
        }
    }
    output.close();
}

void print_node_file(std::string file_name, std::vector<Node*> set){
    std::ofstream output(file_name);
    if(!output.is_open()) return;
    
    for(auto node : set){
        output << node->_idx << " " << node->_x << " " << node->_y << std::endl;
    }
    output.close();
}

void print_edge_file(std::string file_name, std::vector<Node*> set){
    std::ofstream output(file_name);
    if(!output.is_open()) return;

    for(auto node : set){
        Edge * ptr = node->_edge;

        while(ptr){
            output << ptr->_idx << " "
                << ptr->_start_node_idx << " "
                << ptr->_end_node_idx << " "
                << ptr->_weight
                << std::endl;
            
            ptr = ptr->_next;
        }
    }
    output.close();
}

void compare_file(std::string file_name_out, std::string file_name_test, std::string file_name_orig){
    std::ofstream output(file_name_out);
    if(!output.is_open()) return;

    std::ifstream input_test(file_name_test), input_orig(file_name_orig);
    if(!input_test.is_open() || !input_orig.is_open()) return;

    /* check */
    std::string str_test, str_orig;
    double temp_test, temp_orig;
    while(!input_test.eof() && !input_orig.eof()){
        std::getline(input_test, str_test);
        std::getline(input_orig, str_orig);
        std::stringstream s_t(str_test), s_o(str_orig);

        while(temp_test != '\n' && temp_orig != '\n'){
            s_t >> temp_test;
            s_o >> temp_orig;

            if(temp_test != temp_orig){
                output << str_test << str_orig << std::endl;
            }
        }
    }
    input_test.close();
    input_orig.close();
    output.close();
}

void deleteSet(std::vector<Node*> set){
    for(auto it = set.begin(); it != set.end(); it++){
        (*it)->~Node();
    }
    set.clear();
}


int main() {
    std::cout << "Fuck you Fuck fuck you!!1!" << std::endl;

    //TEST SUITE 1:
    /*Vector String Testcase Files*/
    std::vector<std::pair<std::string, std::string > > file_list;

    /* Load vector with test cases - Comment/Uncomment test cases to select them! */
    file_list.push_back(std::make_pair("../datasets/supersmall_nodes.txt", "../datasets/supersmall_edges.txt"));
    //file_list.push_back({"datasets/verysmall_nodes.txt", "datasets/verysmall_edges.txt"});
    //file_list.push_back({"datasets/small_nodes.txt", "datasets/small_edges.txt"});
    // file_list.push_back({"datasets/california_nodes.txt", "datasets/california_edges.txt"});
    // file_list.push_back({"datasets/san_francisco_nodes.txt", "datasets/san_francisco_edges.txt"});

    //TEST SUITE 2:
    /* Check User Inputs - TRUE = TEST ENABLE, FALSE = TEST DISABLED */
    bool TEST_2 = false;
    if(TEST_2){
        std::cout << "TESTING USER INPUT SYSTEM" << std::endl;
        std::cout << "(Enter out of bounds values to test)" << std::endl;
        std::vector<Node*> test_dataset = makeDataSet(file_list[0].first, file_list[0].second);
        /* Call getUserInput - USE BAD INPUTS TO TEST*/
        std::pair<unsigned, double> user = getUserInput();
        std::cout << "USER INPUT TEST PASS" << std::endl;

    }



    /*Declare vars*/
    std::vector<Node*> subset;  
    std::vector<Node*> dataset;  

    std::string curr_nodes, curr_edges;
    for(int i = 0; i < file_list.size(); i++){
        curr_nodes = file_list[i].first;
        curr_edges = file_list[i].second;

        /* construct adj list from txt files */
        dataset = makeDataSet(curr_nodes, curr_edges);

        /* constuct 'circle' subset */
        //subset = BFS(dataset);

        /* make output file */
        //print_node_file(curr_nodes.substr(curr_nodes.find('/') + 1, curr_nodes.find_last_of('.') - curr_nodes.find('/')) + "output.txt", subset);
        //print_edge_file(curr_edges.substr(curr_edges.find('/') + 1, curr_edges.find_last_of('.') - curr_edges.find('/')) + "output.txt", subset);

        /* print to console */
        print(dataset);

        /* compare to input file */
        //compare_file("compare_node.txt", curr_nodes, curr_nodes.substr(curr_nodes.find('/') + 1, curr_nodes.find_last_of('_') - curr_nodes.find('.')) + "output.txt");
        //compare_file("compare_edge.txt", curr_edges, curr_edges.substr(curr_edges.find('/') + 1, curr_edges.find_last_of('_') - curr_edges.find('.')) + "output.txt");

        /* delete */
        deleteSet(dataset);
        //deleteSet(subset);

    }

    return 0;
}