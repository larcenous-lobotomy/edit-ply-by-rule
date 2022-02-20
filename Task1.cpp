#include "Task1.h"

Task1::Task1(string name) {
    filename = name;
}

void Task1::read(){ 
    ifstream input(filename);
    string temp;

    while(getline(input, temp)) {
        input_read.push_back(temp);
    }
    input.close();
}

//prune and simplify the given .ply file according to specifications
void Task1::operate(){

    faces = 0; vertices = 0;

    //remove all comments
    for(int i = 0; i < input_read.size(); i++) {
        if(input_read[i].find("comment") != string::npos) {
            input_read.erase(input_read.begin()+i);
        }
    } 

    //////////////////////////////////////////////////
    //find number of faces and vertices, and erase the rest 
    vector<string> temp;
    for(int i = 0; i < input_read.size(); i++) {
        //exit the loop if faces and vertices have been assigned
        if(faces != 0 && vertices != 0) break;

        if(input_read[i].find("face") != string::npos) {
            temp.clear();
            temp = split_str(input_read[i]);
            faces = stoi(temp[2]);
        }
        else if(input_read[i].find("vertex") != string::npos) {
            temp.clear();
            temp = split_str(input_read[i]);
            vertices = stoi(temp[2]);
        }
    }

    cout << "original number of vertices = " << vertices << endl;
    cout << "original number of faces = " << faces << endl;

    input_read.erase(input_read.begin(), input_read.end() - faces - vertices);

    //////////////////////////////////////////////////
    //ACQUIRE DATA IN VECTOR FORM

    vector<vector<float>> vertex_list; //2d vector for vertex -> (x,y,z)
    vector<vector<int>> face_list; //2d vector -> (indices of vertices of face)
    vector<float> temp1;
    vector<int> temp2;

    for(int i = 0; i < vertices; i++) {
        temp.clear();
        temp = split_str(input_read[i]);
        for(string s: temp) {
            temp1.push_back(stof(s));
        }
        vertex_list.push_back(temp1);
        temp1.clear();
    }

    for(int i = vertices; i < vertices + faces; i++) {
        temp.clear();
        temp = split_str(input_read[i]);
        for(auto s: temp) {
            temp2.push_back(stoi(s));
        }
        face_list.push_back(temp2);
        face_list[face_list.size()-1].erase(face_list[face_list.size()-1].begin()); //remove number of elements at beginning
        temp2.clear();
    } //2d array with only indices and not their count

    /////////////////////////////////////////////////////
    //TRIM N-GONS TO TRIANGLES AND PRUNE LOWER 10 % 

    //iterate only for the original elements; the additions are all confirmed triangles
    int face_list_unmodified = face_list.size(), n_gons = 0;
    //convert n>3-gons to triangles
    for(int i = 0; face_list_unmodified > 0 && i < face_list_unmodified; i++) {
        if(face_list[i].size() > 3) {
            n_gons++;
            //choose first element as pivot 
            for(int j = 1; j < face_list[i].size() - 1; j++) {
                temp2.clear();
                temp2.push_back(face_list[i][0]);
                temp2.push_back(face_list[i][j]);
                temp2.push_back(face_list[i][j+1]);
                face_list.push_back(temp2);
            }
            face_list.erase(face_list.begin() + i); //remove n>3-gon
            i--; //avoid skipping an element in the iteration
            face_list_unmodified--; //since one element has been deleted
        }
    }

    cout << "number of faces with n > 3 = " << n_gons << endl;
    cout << "new count of faces after converting all such faces to collection of triangles = " << face_list.size() << endl;

    //build area-list
    float x1,y1,z1,x2,y2,z2,x3,y3,z3,s,a,b,c,area;
    vector<float> area_list;

    for(auto f: face_list) {
        x1 = vertex_list[f[0]][0];
        y1 = vertex_list[f[0]][1];
        z1 = vertex_list[f[0]][2];
        x2 = vertex_list[f[1]][0];
        y2 = vertex_list[f[1]][1];
        z2 = vertex_list[f[1]][2];
        x3 = vertex_list[f[2]][0];
        y3 = vertex_list[f[2]][1];
        z3 = vertex_list[f[2]][2];
        a = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2));
        b = sqrt((x2 - x3)*(x2 - x3) + (y2 - y3)*(y2 - y3) + (z2 - z3)*(z2 - z3));
        c = sqrt((x1 - x3)*(x1 - x3) + (y1 - y3)*(y1 - y3) + (z1 - z3)*(z1 - z3));
        s = (a+b+c) / 2;
        area = sqrt(s*(s-a)*(s-b)*(s-c));
        area_list.push_back(area); 
    }

    //delete faces in lower 10% of sorted

    vector<float> area_list_copy = area_list;
    sort(area_list.begin(), area_list.end());
    int lower_ten_perc_index = static_cast<int>((static_cast<float>(area_list.size())*0.1) - 1); 
    float area_limit = area_list[lower_ten_perc_index];
    int counter = 0;
    //keep removing low area triangles until the lower ten percent have been removed
    for(int i = 0; counter < lower_ten_perc_index + 1 && i < area_list.size(); i++) {
        if(area_list[i] < area_limit) {
            face_list.erase(face_list.begin() + i - counter); //because <counter> elements behind it have been deleted
            counter++;
        }
    }

    cout << "number of faces in the lower 10% of sorted areas = " << counter << endl;
    cout << "new count of faces after pruning = " << face_list.size() << endl;

    //vertices not part of any face 
    bool is_vertex[vertices];
    int extra_vertices = 0; 
    fill(is_vertex, is_vertex + vertices - 1,false);  
    for(auto F : face_list) {
        for(int i = 0; i < F.size(); i++) {
            is_vertex[F[i]] = true;
        }
    }
    for(int i = 0; i < vertices; i++) {
        if(!is_vertex[i]) {
            for(int j = 0; j < face_list.size(); j++) {
                for(int k = 0; k < 3; k++) {
                    if(face_list[j][k] > i - extra_vertices) {
                        face_list[j][k]-=1;
                    }
                }
            }
            vertex_list.erase(vertex_list.begin() + i - extra_vertices);
            extra_vertices++;
        }
    }  

    cout << "vertices that are not part of any faces after pruning = " << extra_vertices << endl;
    cout << "new count for vertices after removing these extraneous vertices = " << vertex_list.size() << endl;

    ///////////////////////////////////////////////////

    vertices = vertex_list.size();
    faces = face_list.size();

    //copy in right format to output
    for(auto v: vertex_list) {
        output_write.push_back(custom_trunc(to_string(v[0])) + " " + custom_trunc(to_string(v[1])) + " " + custom_trunc(to_string(v[2])));
    }
    string str = "";
    for(auto f: face_list) {
       str.clear();
       str += to_string(f.size());
       for(int i = 0; i < f.size(); i++) {
           str += " " + to_string(f[i]);
       }
       output_write.push_back(str);      
    }
}

void Task1::write(){
    ofstream output("output.ply"); //overwrites by default

    output << "ply" << endl << "format ascii 1.0" << endl;

    output << "element vertex " << vertices << endl;
    output << "property float32 x" << endl << "property float32 y" << endl << "property float32 z" << endl;
    output << "element face " << faces << endl;
    output << "property list uint8 int32 vertex_indices" << endl << "end_header" << endl;

    for(auto s: output_write) {
        output << s << endl;
    }
    output.close();
}

vector<string> Task1::split_str(string str) {
    istringstream ss(str);
    vector<string> output;
    string word; 
    while (ss >> word) 
    {
        output.push_back(word);
    }
    return output;
}

string Task1::custom_trunc(string f) {
    for(int i = 0; i < f.length(); i++) {
        if(f[i] == '.') {
            f = f.substr(0,i + 5);
            break;
        }
    }
    while(f[f.length() - 1] == '0') {
        f = f.substr(0,f.length() - 1);
        if(f[f.length() - 1] == '.') {f = f.substr(0,f.length() - 1); break;}
    }
    return f;
}