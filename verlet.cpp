
/* THIS IS NOT THE COMPLETE RESULT, I STILL NEED TO MODIFY THESE CODES. */


#include <vector>
#include <iostream>
#define DIMENTION 2

using namespace std;
double dt=0.05; 
int num_of_bodies=1;
struct Body{  
	double mass=1;
	double* pos=new double[DIMENTION]();
	double* velocity=new double[DIMENTION]();
	double* acceleration=new double[DIMENTION]();
	double* force=new double[DIMENTION](); //sum of all forces (f in the equation)

	void next_moment(){
	
		//step 1: update pos ( find r(t+dt) )
		for(int i=0;i<DIMENTION;i++){
			double change_in_dimention_i=velocity[i]*dt+acceleration[i]*dt*dt/2;
			pos[i]+=change_in_dimention_i;
		}
		
		//step 2: update half-step velocity ( find v(t+dt/2) )
		for(int i=0;i<DIMENTION;i++){
			velocity[i]+=1/2*acceleration[i]*dt;
		}
		
		//step 3: update acceleration ( find a(t+dt) )
		//assume we know f(r(t+dt))=F
		
		for(int i=0;i<DIMENTION;i++){
			acceleration[i]+=force[i]/mass;
		}
		
		//step 4: update another half-step velocity
		for(int i=0;i<DIMENTION;i++){
			velocity[i]+=velocity[i]+1/2*acceleration[i]*dt;
		}
		
	}	
};


int main(){
	struct Body* Bodies =new struct Body[num_of_bodies]();

	int n=10; //total number of steps
	for (int i=0;i<n;i++){
		
		for(int b=0;b<num_of_bodies;b++){
			Bodies[b].next_moment();
		} 

	}
	for(int b=0;b<num_of_bodies;b++){
		printf("final position is");
		for(int i=0;i<DIMENTION;i++){
			printf(" %f",Bodies[b].pos[i]);
		}
		printf("\n");
		delete[] Bodies[b].pos;
		delete[] Bodies[b].velocity;
		delete[] Bodies[b].acceleration;
		delete[] Bodies[b].force;
	}
	return 0;
}

