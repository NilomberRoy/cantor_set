#include<bits/stdc++.h>
#include<chrono>

using namespace std;
using namespace std::chrono;

std::random_device r;
std::seed_seq seed{r(), r(), r(), r(), r()};
std::mt19937 gen(seed);

double p =0.0; //probability
int t = 0; // Number of iterations
int en = 0; // Number of ensemble runs
double wid = 0.0; //width of interval
int f=0;//file number

string cantor_collapse;
string cantor_loglog;

void seed_generator() {
    auto now = system_clock::now();
    auto duration = now.time_since_epoch();
    auto seed_value = duration.count();
    gen.seed(seed_value);
}

int main(int argc, char *argv[]) {
	//using command line arguement
    p =atof(argv[1]);
    t =atoi(argv[2]);
    en =atoi(argv[3]);
    wid =atof(argv[4]);
    f =atoi(argv[5]);
    
    cantor_collapse="cantor_collap_p"+to_string(p)+"_iter"+to_string(t)+"_en"+to_string(en)+"_wid"+to_string(wid)+"_fnum"+to_string(f)+".dat";//name of dat file
    cantor_loglog="cantor_loglog_p"+to_string(p)+"_iter"+to_string(t)+"_en"+to_string(en)+"_wid"+to_string(wid)+"_fnum"+to_string(f)+".dat";//name of dat file
    ofstream file1(cantor_collapse);
    ofstream file2(cantor_loglog);
    
    vector<vector<pair<long double, int>>> all_histograms(en);//for store histograms of each ensemble
     seed_generator();
    for (int ensemble = 0; ensemble < en; ensemble++) {
        
       
        vector<pair<long double, long double>> cantor_set={{0.0, 1.0}};// Reset the cantor_set for each ensemble //pair 1D vector
        
        uniform_real_distribution<long double> dis1(0.0, 1.0);//generating random number from 0 to 1
            	
            	 long double rand_num_ini1 = dis1(gen);
             	 long double middle_point_ini = rand_num_ini1 ;
            	// cout<<"rand1= "<<middle_point<<endl;
            	 long double rand_num_ini2 = dis1(gen);
            	 
            	
                //cantor_set.erase(cantor_set.begin());// erasing initial pair
                //cantor_set.clear();
                //cantor_set.push_back(make_pair(0.0 , middle_point_ini));//Adding a pair to the vector
                cantor_set.at(0)= {0.0, middle_point_ini};
                
                if (rand_num_ini2 <= p){
                	cantor_set.push_back(make_pair(middle_point_ini , 1.0));//Adding a pair to the vector
				} 
        
        
            for (size_t j = 0; j < t; j++) { //loop of time
            	
            	
            	uniform_real_distribution<long double> dis(0.0, 1.0);//generating random number from 0 to 1
            	
            	 long double rand_num1 = dis(gen);
             	 long double middle_point =  rand_num1 ;
            	// cout<<"rand1= "<<middle_point<<endl;
            	 long double rand_num2 = dis(gen);
            
                for (size_t i =0 ; i < cantor_set.size(); i++) {
                	
                   
                   double interval_start = cantor_set[i].first;
                  // cout<<"start= "<< interval_start<<endl;
                   double interval_end = cantor_set[i].second;
                  // cout<<"end ="<<interval_end<<endl;
                   
                   //cout<<"ok1"<<endl;
                
                   if (interval_start <= middle_point && middle_point < interval_end) {
                      //  cout<<"rand2= "<<rand_num2<<endl;
                        //cantor_set.erase(cantor_set.begin()+ i);
                        //cantor_set.push_back(make_pair(interval_start, middle_point));
                        cantor_set.at(i)= {interval_start, middle_point};
                        
                        if (rand_num2 <= p) {
                      
                        cantor_set.push_back({middle_point, interval_end});
                       
                         } 
					
						//cout<<"ok3"<<endl;
                       
                        break;
                   }
            //cout<<"ok4"<<endl;
                    
                }
            
              //  cout<<"time "<<j<<" done"<<endl;
                //cout<<endl;
       }
        
        for (const auto& interval : cantor_set) {
       // cout << "{" << interval.first << ", " << interval.second << "}" <<endl; //output of cantor set
    }

        vector<pair<long double, int>> histogram;
        

        for (double x = 0.0; x < 1- wid; x += wid) {
            int count = 0;

            for (const auto& interval : cantor_set) { //using range-based loop 
                double interval_start = interval.first;
                double interval_end = interval.second;
                double length= interval_end - interval_start;
                
                if (length >= x && length < (x + wid)) {
                count++;
                }
            }
            double midpoint = (x + x + wid) / 2.0;
            
            //if(count != 0){
			
            histogram.push_back({midpoint, count});
       // }
        }

        all_histograms[ensemble] = histogram;
        
        vector<pair<long double, long double>>().swap(cantor_set);//clearing cantor_set
        vector<pair<long double, int>>().swap(histogram);//clearing histogram vector 
        
        cout<<"ensemble "<<ensemble<<" ok"<<endl;
    }

    // Finding the maximum histogram size
    size_t max_histogram_size = 0;
    for (const auto& histogram : all_histograms) {
        max_histogram_size = max(max_histogram_size, histogram.size());//using max function
    }

    // Initializing the average histogram
    vector<pair<long double,long double>> average_histogram(max_histogram_size, {0.0, 0.0});//to average to histograms

    // Accumulating counts from all ensembles
    for (const auto& histogram : all_histograms) {
    	
        for (size_t i = 0; i < histogram.size(); i++) {
        	if (histogram[i].second != 0) {
			
            average_histogram[i].first = histogram[i].first;
            average_histogram[i].second +=(long double) histogram[i].second / en;
        }
        }
    }
    vector<vector<pair<long double, int>>>().swap(all_histograms);
    // Calculating average by dividing counts by number of ensembles
   // for (auto& point : average_histogram) {
     //   point.second = (long double) (point.second / en);
    //}
    
    // Printing average histogram
   // cout << "Average Histogram:" << endl;
    for (const auto& point : average_histogram) {
    	
    	if(point.second != 0.0){
    	
        cout << "x: " << point.first << ", Avg C(x,t): " << point.second / wid << endl;
        
        file1<<fixed<<setprecision(10)<<(long double)(point.first*t)<<"  "<<setprecision(10)<<(long double)(point.second / (wid*(pow(t,(1+p)))))<<endl;
        file2<<fixed<<setprecision(10)<<(long double)(point.first*t)<<"  "<<setprecision(10)<<(long double)log((double)(point.second / (pow(t,(1+p)) * wid)))<<endl;
        
        
		/*file<<fixed<<setprecision(10)<<(long double) point.first<<"   "
		<<setprecision(10)<<(long double)(point.first*t)<<"  "
		<<setprecision(10)<<(long double)(point.second / wid)<<"  "
		<<setprecision(10)<<(long double)log((double)(point.second / wid))<<"  "
		<<setprecision(10)<<(long double)(point.second / (wid*(pow(t,(1+p)))))<<"  "
		<<setprecision(10)<<(long double)log((double)(point.second / (pow(t,(1+p)) * wid)))<<endl;*/
    }
}
    
    vector<pair<long double,long double>>().swap(average_histogram);
    return 0;
}

