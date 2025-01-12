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

vector<vector<int> >reshape(vector<vector<int> >& matrix){
    vector<vector<int> > matrix_reshape;
    int first;
    int movie;

    if(matrix.empty()){
        cerr<<"The matrix is empty"<<endl;
        return matrix_reshape;
    }

    // Resize matrix_reshape to handle the reshaping
    matrix_reshape.resize(200, vector<int>(1001));

    int count = 0; //trace if need to create a row for new user
    first = matrix[0][0];//user
    matrix_reshape[0][0] = first;
    //reshape the matrix to be user by movie
    for(int i=0; i<matrix.size(); i++){
        //need to mark the true 0s that want to fill out
            if(first==matrix[i][0]){//if this row is also start with the same user
                movie = matrix[i][1]; //get the movie number
                matrix_reshape[count][movie] = matrix[i][2]; //put the rate in corresponding movie number(j,column)
            } else{
                count++; //next line, new user
                //put the first rating of new user into reshape matrix
                first = matrix[i][0]; 
                matrix_reshape[count][0] = first;
                movie = matrix[i][1];
                matrix_reshape[count][movie] = matrix[i][2];
            }
        
    }
    return matrix_reshape;
}

void colla_filter(vector<vector<int> >& matrix_test5, vector<vector<int> >& matrix_reshape_train, vector<vector<int> >& matrix_reshape_test5, int userID)
{

    vector<int> movie;
    vector<int> score;
    vector<int> movie_un;
    vector<int> rate_un;
    vector<int> movie_k;
    vector<int> rate_k;

    for(int i=0; i<matrix_test5.size(); i++){
        //if the userid = user in test5, put movie number in movie, rate in score
        if(matrix_test5[i][0]==userID){
            movie.push_back(matrix_test5[i][1]);
            score.push_back(matrix_test5[i][2]);
        }
        
        //load the user's known and unkown rating&movie into array
        if(matrix_test5[i][2]!=0 && userID == matrix_test5[i][0]){
            movie_k.push_back(matrix_test5[i][1]);
            rate_k.push_back(matrix_test5[i][2]);
            
        } else if(matrix_test5[i][2]==0 && userID ==matrix_test5[i][0]){
            movie_un.push_back(matrix_test5[i][1]);
            rate_un.push_back(matrix_test5[i][2]);
        } 
    }

    //sort user's movie based on movie number
    vector<int> indices(movie.size());
    for(int i=0; i<indices.size(); i++){
        indices[i] = i;
    }
    sort(indices.begin(), indices.end(),[&movie](int a, int b){
        return movie[a] < movie[b];
    });
    vector<int> sorted_movie(indices.size());
    vector<int> sorted_score(indices.size());
    for(int i=0; i<indices.size(); i++){
        sorted_movie[i] = movie[indices[i]];
        sorted_score[i] = score[indices[i]];
    }

    double m_train = matrix_reshape_train.size();
    double m_test = matrix_reshape_test5.size();
    
    //calculate the cosine similarity
    double dot = 0.0;
    double mag_a = 0.0;
    double mag_b = 0.0;
    vector<double> cos_sim;
    vector<int> cos_sim_user;
    
    for(int i=0; i<matrix_reshape_train.size(); i++){
        int counting = 0;
        for(int j=0; j<movie_k.size(); j++){
            double time;
            double t;
            double mj_test = m_test/time;
            double mj_train = m_train/t;
            if(matrix_reshape_train[i][movie_k[j]]==0){
                continue;
            }
            
            counting ++;
            dot = dot + rate_k[j]*matrix_reshape_train[i][movie_k[j]];
            mag_a = mag_a + pow(rate_k[j],2);
            mag_b = mag_b + pow(matrix_reshape_train[i][movie_k[j]], 2);
        }
        double cosine;
        double final;
        if(counting == 1||mag_b==0.0){
            cosine = 0.0;
        } else {
            double mag_a_sqrt = sqrt(mag_a);
            double mag_b_sqrt = sqrt(mag_b);
            cosine = (dot/(mag_a_sqrt*mag_b_sqrt))*(counting/counting + movie_k.size());
            final = cosine * pow(abs(cosine),1.5);
        }
        
        cos_sim_user.push_back(matrix_reshape_train[i][0]);
        cos_sim.push_back(final);
    
        dot = 0.0;
        mag_a = 0.0;
        mag_b = 0.0;
    }
    //rank the cosine similarity
    vector<double> indices_cos(cos_sim.size());
    for(int i=0; i<indices_cos.size(); i++){
        indices_cos[i] = i;
    }
    sort(indices_cos.begin(), indices_cos.end(),[&cos_sim](double a, double b){
        return cos_sim[a] > cos_sim[b];
    });
    vector<double> sorted_cos_sim(indices_cos.size());
    vector<int> sorted_cos_sim_user(indices_cos.size());
    for(int i=0; i<indices_cos.size(); i++){
        sorted_cos_sim[i] = cos_sim[indices_cos[i]];
        sorted_cos_sim_user[i] = cos_sim_user[indices_cos[i]];
    }
    
    int top = 0;
    for(int i=0; i<sorted_cos_sim_user.size(); i++){
        if(sorted_cos_sim[i]==0){
            break;
        }
        top ++;
    }
    //cosine similarity:pick top k similar user 
    int k = top;
    vector<int> result_movie;
    vector<int> result_score;
    double result = 0.0;
    for(int i=0; i<sorted_movie.size(); i++){
         if(sorted_score[i]==0){
            double weighted_sum = 0.0;
            double weight_sum = 0.0;
            for(int j=0; j<k; j++){
                int user_index = sorted_cos_sim_user[j]-1;
                int movie_index = sorted_movie[i];
                double weight = sorted_cos_sim[j];
                double rating = matrix_reshape_train[user_index][movie_index];
                if(rating == 0){
                    continue;
                }
                weighted_sum += weight * rating;
                weight_sum += weight;
            }
            double p = (weight_sum!=0) ? (weighted_sum / weight_sum) : 0;
            result = p;
            result_movie.push_back(sorted_movie[i]);
            if(result > 0 && result <= 0.5||result ==0){
                result = 1;
            }
            result_score.push_back(round(result));
        }
    }
    

    ofstream file("result20_ci.txt", ios::app);
    if(file.is_open()){
        for(int i=0; i<result_movie.size(); i++){
            file << userID << " " << result_movie[i] << " " << result_score[i]<<endl;
        }
    }
        
}

int main(){
    vector<vector<int> > matrix_train;
    vector<vector<int> > matrix_reshape_train;

    vector<vector<int> > matrix_test5;
    vector<vector<int> > matrix_reshape_test5;

    string filename_train;
    string filename_test5;

    filename_train = "train.txt";
    filename_test5 = "test20.txt";
    //get a matrix by reading the txt file
    matrix_train = readFile(filename_train);
    matrix_reshape_train = reshape(matrix_train);

    matrix_test5 = readFile(filename_test5);
    matrix_reshape_test5 = reshape(matrix_test5);
    matrix_reshape_test5.resize(100,vector<int>(1001));

   //loop to calucalte each user's unkown rating
   for(int i=0; i<matrix_reshape_test5.size(); i++){
    colla_filter(matrix_test5, matrix_reshape_train,matrix_reshape_test5, matrix_reshape_test5[i][0]);
   }
   
    return 0;
}