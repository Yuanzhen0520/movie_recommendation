#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <typeinfo>

using namespace std;

vector<vector<int> >readFile(string& filename){
    vector<vector<int> > matrix;
    string line;
    int num;
    vector<int> row; 

    ifstream file(filename);
    if(!file){
        cerr<<"Cannot open the file."<<endl;
        return matrix;
    }
    //save number in file into a matrix
    while(getline(file,line)){
        istringstream stream(line);
        while(stream >> num){
            row.push_back(num);
        }
        matrix.push_back(row);
        row.clear();
    }
    file.close();
    return matrix;
}

int main(){
    vector<vector<int> > cosine;
    vector<vector<int> > pearson;
    vector<vector<int> > result;

    string result_cosine;
    string result_pearson;

    result_cosine = "result20_ci.txt";
    result_pearson = "result20_p.txt";

    cosine = readFile(result_cosine);
    pearson = readFile(result_pearson);
    result.resize(cosine.size(), vector<int>(3));

    double cosine_mae = 0.784107;
    double pearson_mae = 0.82527;

    double cos_weight = (1/cosine_mae)/((1/cosine_mae)+(1/pearson_mae));
    double pea_weight = (1/pearson_mae)/((1/cosine_mae)+(1/pearson_mae));

   double cos_w;
   double pea_w;
   double sum;
    for(int i=0; i<cosine.size(); i++){
        result[i][0] = cosine[i][0];
        result[i][1] = cosine[i][1];
        cos_w = cosine[i][2]*cos_weight;
        pea_w = pearson[i][2]*pea_weight;
        sum = round(cos_w+pea_w);
        result[i][2] = sum;
    }

    ofstream file("result20_f.txt", ios::app);
    if(file.is_open()){
        for(int i=0; i<result.size(); i++){
            file << result[i][0] << " " << result[i][1] << " " << result[i][2]<<endl;
        }
    }
}