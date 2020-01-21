#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10000
#define ERROR 404
#define debug 0


typedef struct {
	float distance;
	int s;
}flower;

float dist(float *v1, float *v2){

	int i;
	float d;
	d=0;
	for(i=0; i<4; i++){
		d = d + ((v1[i]-v2[i])*(v1[i]-v2[i]));
	}

	return d;
}

int type(char *v){
	if((strncmp(v,"versicolor", 3))==0)
		return 0;
	else if((strncmp(v,"virginica", 3))==0)
		return 1;
	else if((strncmp(v,"setosa", 3))==0)
		return 2;

	else if(debug) printf("Nome invalido\n");
	return ERROR;
}

void print (int n1, int n2){
	
	char str1[20];
	char str2[20];
	
	if(n1==0)
		strcpy(str1,"versicolor");
	else if(n1==1)
		strcpy(str1,"virginica");
	else if(n1==2)
		strcpy(str1,"setosa");

	if(n2==0)
		strcpy(str2,"versicolor");
	else if(n2==1)
		strcpy(str2,"virginica");
	else if(n2==2)
		strcpy(str2,"setosa");

	printf("%s %s\n", str1, str2);
	
	return;
}


void insertion_sort_int(int *v, int n){

	int i, j, key;
	for(i=1;i<n;i++){
		key = v[i];
		j=i-1;
		while(j>=0 && v[j]>key){
			v[j+1] = v[j];
			j--;
		}
		v[j+1] = key;
	}

}

void insertion_sort_float(flower *v, int n){

	int i, j;
	float key;
	float key2;
	for(i=1;i<n;i++){
		key = v[i].distance;
		key2 = v[i].s;
		j=i-1;
		while(j>=0 && v[j].distance > key){
			v[j+1].distance = v[j].distance;
			v[j+1].s = v[j].s;
			j--;
		}
		v[j+1].distance = key;
		v[j+1].s = key2;
	}

}

int most_frequent(int *v, int n){
	
	int i;
	int counter[4];
	int tmp;
	int ind;
	
	for(i=0;i<4;i++){
		counter[i]=0;
	}
	
	for(i=0;i<n;i++){
		tmp = v[i];
		counter[tmp]++;
	}
	
	int max;
	max = counter[0];
	ind=0;
	
	if(counter[1]>max){
		max=counter[1];
		ind=1;
	}
	if(counter[2]>max){
		max=counter[2];
		ind=2;
	}
	
	for(i=0;i<3;i++){
		if(max==counter[i] && ind!=i)
			return 4;
	}
	
	return ind;
}	

int main (){

int i,j,k;
FILE *ftrain;
FILE *ftest;
float rate;
char filename_train[50];
char filename_test[50];
float train[MAX][6];
float test[MAX][6];
int train_int[MAX];
int test_int[MAX];
char op[15];
int ntrain, ntest, hits;
char ignore[100];
hits=0;
int species;
char c;
char str1[50];
char str2[50];

ntrain=0;
ntest=0;
i=0;


/*Be v a generic vector
v[0] = comprimento sepala
v[1] = largura da sepala
v[2] = comprimento petala
v[3] = largura da petala
0 veriscolor 1 se virginica 2 se setosa*/


scanf("%s", filename_train);
scanf("%s", filename_test);
scanf("%d", &k);


ftrain = fopen(filename_train, "r");
i=0;
fgets(ignore, sizeof(ignore), ftrain); /*skip the first line of the file since it does not contain any valuable information*/
while((fscanf(ftrain, "%f,%f,%f,%f,%c%s", &train[i][0], &train[i][1], &train[i][2], &train[i][3], &c, op))!=EOF){
	ntrain++;
	train_int[i] = type(op);
	i++;
}
fclose(ftrain);


i=0;
ftest = fopen(filename_test, "r");
fgets(ignore, sizeof(ignore), ftest);
while((fscanf(ftest, "%f,%f,%f,%f,%c%s", &test[i][0], &test[i][1], &test[i][2], &test[i][3],&c, op))!=EOF){
	ntest++;
	test_int[i] = type(op);
	i++;
}
fclose(ftest);


if(!(k>=1 && k< ntrain)){
    printf("k is invalid\n");
    return 0;
}

flower v[ntrain];

for(i=0;i<ntest;i++){
	
	for(j=0;j<ntrain;j++){
		v[j].distance = dist(train[j], test[i]);
		v[j].s = train_int[j];
	}
	
	insertion_sort_float(v, ntrain);
	
	int ind;
	int vaux[ntrain];
	for(j=0;j<k;j++){
		vaux[j]=v[j].s;
	}
	
	if(debug){
		for(j=0;j<k;j++){
			printf("%d %f ", vaux[j], v[j].distance);
		}
	}
	
	ind = most_frequent(vaux, k);
	
	if(ind==4){
		species = v[0].s;
	}else{
		species = ind;
	}
	
	if(species == test_int[i]){
		hits++;
	}
	
	print(species, test_int[i]);
}

rate = (float)((float)hits/((float)ntest));

printf("%.4f\n", rate);

return 0;
}
