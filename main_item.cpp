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
    int user;
    int movie;

    if(matrix.empty()){
        cerr<<"The matrix is empty"<<endl;
        return matrix_reshape;
    }

    // Resize matrix_reshape to handle the reshaping
    //1000 movies, 200 users
    matrix_reshape.resize(1000, vector<int>(201));
    for(int i=0; i<matrix_reshape.size(); i++){
        matrix_reshape[i][0] = i+1;
    }

    //reshape the matrix to be user by movie
    for(int i=0; i<matrix.size(); i++){
        //need to mark the true 0s that want to fill out
        movie = matrix[i][1];
        if(matrix[i][0]<201){
            user = matrix[i][0];
        } else {
            if(matrix[i][0]%100==0){
                user = matrix[i-1][0]%100+1;
            } else {
                user = matrix[i][0]%100;
            }
        }
        matrix_reshape[movie-1][user] = matrix[i][2];
        
    }
    return matrix_reshape;
}

vector<vector<int> > mark_unknown(vector<vector<int> > &matrix_test5){
    int movie;
    int user;
    vector<vector<int> > result;
    result.resize(1000, vector<int>(101));
    for(int i=0; i<result.size(); i++){
        result[i][0] = i+1;
    }
    for(int i=0; i<matrix_test5.size(); i++){
        if(matrix_test5[i][2]==0){
            //cout<<"find 0 at "<<matrix_test5[i][0]<<" "<<matrix_test5[i][1]<<endl;
            movie = matrix_test5[i][1];
            //cout<<"movie = "<<matrix_test5[i][1]<<endl;
            if(matrix_test5[i][0]%100==0){
                user = 100;
                //cout<<"user = "<<user<<endl;
            } else {
                user = matrix_test5[i][0]%100;
                //cout<<"user = "<<user<<endl;
            }
            result[movie-1][user] = 1;
            //cout<<"put 1 at movie "<<movie<<" user "<<user<<endl;
        } else {
            continue;
        }
    }
    return result;    
}

vector<int> average(vector<vector<int> > &matrix){
    //calculate user average in train
    vector<int> user_avg;
    user_avg.resize(201);
    for(int i=1; i<matrix[0].size(); i++){
        double sum = 0.0;
        double c = 0.0;
        for(int j=0; j<matrix.size(); j++){
            if(matrix[j][i]==0){
                continue;
            }
            sum = sum + matrix[j][i];
            c++;
        }
        if(c==0){
            user_avg[i] = 0;
        } else{
            user_avg[i] = sum/c;
        }
    } 
    return user_avg;  
}


//void colla_filter(vector<vector<int> >& matrix_test5, vector<vector<int> >& matrix_reshape_train, vector<vector<int> >& matrix_reshape_test5, int movieID, vector<vector<int> > &unknown, vector<int> &train_user_avg, vector<int> &test_user_avg)
//{

       
//}


int main(){
    vector<vector<int> > matrix_train;
    vector<vector<int> > matrix_reshape_train;

    vector<vector<int> > matrix_test5;
    vector<vector<int> > matrix_reshape_test5;

    vector<vector<int> > unknown;
    vector<int> train_user_avg;
    vector<int> test_user_avg;

    string filename_train;
    string filename_test5;

    filename_train = "train.txt";
    filename_test5 = "test5.txt";
    //get a matrix by reading the txt file
    matrix_train = readFile(filename_train);
    matrix_reshape_train = reshape(matrix_train);

    matrix_test5 = readFile(filename_test5);
    matrix_reshape_test5 = reshape(matrix_test5);
    matrix_reshape_test5.resize(1000,vector<int>(101));

    unknown = mark_unknown(matrix_test5);
    train_user_avg = average(matrix_reshape_train);
    test_user_avg = average(matrix_reshape_test5);
    test_user_avg.resize(101);

    vector<vector<int> > result_matrix;
    result_matrix.resize(1000,vector<int>(101));
    for(int i=0; i<result_matrix.size(); i++){
        result_matrix[i][0] = i+1;
    }
    

    /*
    for(int i=0; i<unknown.size(); i++){
        for(int j=0; j<unknown[0].size(); j++){
            cout<<unknown[i][j]<<" ";
        }
        cout<<endl;
    }
    
     for(int i=0; i<test_user_avg.size(); i++){
        cout<<"user "<<i<<" avg "<<test_user_avg[i]<<endl;
    }
    */

   //loop to calucalte each movie's unkown rating
   for(int i=0; i<matrix_reshape_test5.size(); i++){
    int movieID = matrix_reshape_test5[i][0];
    //colla_filter(matrix_test5, matrix_reshape_train, matrix_reshape_test5, matrix_reshape_test5[i][0], unknown, train_user_avg, test_user_avg);
    vector<int> movie;
    vector<int> score;
    vector<int> movie_un;
    vector<int> rate_un;
    vector<int> movie_k;
    vector<int> rate_k;

    vector<int> user_k;
    vector<int> user_cal;
    
   
    //need to use reshape test5(movie) to compute with reshape train
    //need to collect unknow movie rate to calculate, e.g. how many unknow in movie 1 need to calculate

    for(int i=0; i<matrix_test5.size(); i++){
        //if the userid = user in test5, put movie number in movie, rate in score
        if(matrix_test5[i][0]==movieID){
            movie.push_back(matrix_test5[i][1]);
            score.push_back(matrix_test5[i][2]);
        }
        
        //load the user's known and unkown rating&movie into array
        if(matrix_test5[i][2]!=0 && movieID == matrix_test5[i][0]){
            movie_k.push_back(matrix_test5[i][1]);
            rate_k.push_back(matrix_test5[i][2]);
            
        } else if(matrix_test5[i][2]==0 && movieID ==matrix_test5[i][0]){
            movie_un.push_back(matrix_test5[i][1]);
            rate_un.push_back(matrix_test5[i][2]);
        } 
    }

    //sort user's movie based on movie number
    //can ignore
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

    //collect which users rate that movie
    for(int i=1; i<matrix_reshape_test5[0].size(); i++){
        if(matrix_reshape_test5[movieID-1][i]!=0){
            user_k.push_back(i);
        }
    }
    //collect which users rate need to calculate on that movie
    for(int i=1; i<unknown[0].size(); i++){
        if(unknown[movieID-1][i]==1){
            user_cal.push_back(i);
        }
    }

    //calculate the cosine similarity
    vector<double> cos_sim;
    vector<int> cos_sim_movie;
    for(int i=0; i<matrix_reshape_train.size(); i++){
        int counting = 0;
        double nu = 0.0;
        double de_i = 0.0;
        double de_j = 0.0;
        for(int j=0; j<user_k.size(); j++){
            if(matrix_reshape_train[i][user_k[j]]==0){
                continue;
            }
            nu = nu+(matrix_reshape_test5[movieID-1][user_k[j]]-test_user_avg[user_k[j]]) * (matrix_reshape_train[i][user_k[j]]-train_user_avg[user_k[j]]);
            de_i = de_i + pow(matrix_reshape_test5[movieID-1][user_k[j]]-test_user_avg[user_k[j]],2);
            de_j = de_j + pow(matrix_reshape_train[i][user_k[j]]-train_user_avg[user_k[j]],2);
        }
        double de = sqrt(de_i)*sqrt(de_j);
        double cosine;
        if(de==0.0){
            cosine = 0.0;
        } else {
            cosine = nu/de;
        }
        
        cos_sim_movie.push_back(matrix_reshape_train[i][0]);
        cos_sim.push_back(cosine);
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
    vector<int> sorted_cos_sim_movie(indices_cos.size());
    for(int i=0; i<indices_cos.size(); i++){
        sorted_cos_sim[i] = cos_sim[indices_cos[i]];
        sorted_cos_sim_movie[i] = cos_sim_movie[indices_cos[i]];
    }
 
    int top;
    for(int i=0; i<sorted_cos_sim_movie.size(); i++){
        if(sorted_cos_sim[i]!=0){
            top = 200;
            break;
        } else {
            top = 0;
        }
    }
    //cosine similarity:pick top k similar user 
    
    int k = top;
    double result = 0.0;
    double sum = 0.0;
    double weight_sum = 0.0; 
    for(int i=0; i<user_cal.size(); i++){
        if(k==0){
            if(test_user_avg[user_cal[i]]==0){
                result = 3;
            } else {
                result = test_user_avg[user_cal[i]];
            }
        } else {
            for(int j=0; j<k; j++){
                
                if(matrix_reshape_test5[sorted_cos_sim_movie[j]-1][user_cal[i]]==0){
                    continue;
                }
                sum = sum + (matrix_reshape_test5[sorted_cos_sim_movie[j]-1][user_cal[i]] - test_user_avg[user_cal[i]])*sorted_cos_sim[j];
                weight_sum = weight_sum + abs(sorted_cos_sim[j]);
            }
            if(weight_sum == 0){
                result = test_user_avg[user_cal[i]];
            } else {
                result = test_user_avg[user_cal[i]] + sum / weight_sum;
                if(result<0.5&&result >0){
                    result = 1;
                } else {
                    result = round(result);
                }
            }
        }
        result_matrix[movieID-1][user_cal[i]] = result;
        sum = 0.0;
        weight_sum = 0.0;
    }
   
   }
   vector<vector<int> > result;
   result.resize(100,vector<int>(1001));

    ofstream file("result5_i.txt", ios::app);
    if(file.is_open()){
        for(int i=1; i<result_matrix[0].size(); i++){
            for(int j=0; j<result_matrix.size(); j++){
                if(result_matrix[j][i]!=0){
                    file << (matrix_test5[0][0]-1)+i<<" "<<result_matrix[j][0]<<" "<<result_matrix[j][i]<<endl;
                }
            }
        }
    }
    
    
    return 0;
}